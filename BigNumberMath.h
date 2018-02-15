#define _DYNAMIC_ 1
#include "BigNumber.h"
#include <cmath>
#include <optional>
#include <random>

#ifndef _BNMATH_
#define _BNMATH_

namespace BigIntMath{
bnint sqrt(const bnint& n){
	if(n.is_zero()){
		return 0;
	}
	if(!n.get_positive()){
		throw domain_error("sqrt(" + n.str() + ") is invalid.");
	}
	size_t nsize = n.getRealSize();
	size_t rootsize = (nsize + 1) / 2;
	bnint sol = bnint(IMax) ^ (rootsize); //init, over-estimated
	bnint old;
	do{
		old = sol;
		sol += n / old;
		sol.divide2();
		if(sol > old){
			return old;
		}
		//cout << old << ' ' << sol << endl;
	}while(old != sol);
	return sol;
}
bnint log2(const bnint& n){
	if(n.is_zero() || !n.get_positive()){
		throw domain_error("log2(" + n.str() + ") is invalid.");
	}
	static const/*expr*/ auto log2_IMax = std::log2(IMax);
	size_t s = n.getRealSize();
	auto tmp = log2_IMax * (s - 1) + std::log2(n.get(s - 1));
	uintmax_t sol = round(tmp);
	if(sol == uintmax_t(tmp)){
		sol++;
	}
	if(sol == 0 || (2_b ^ sol) <= n){
		return sol;
	}else{
		return sol - 1;
	}
	/*bnint base = n, sol2 = 0;
	base.divide2();
	while(!base.is_zero()){
		base.divide2();
		sol2++;
	}
	return sol2;*/
}
bnint log(const bnint& n, unsigned int base = 10){
	if(n.is_zero() || !n.get_positive()){
		throw domain_error("log(" + n.str() + ") is invalid.");
	}
	auto log_IMax = std::log(IMax) / std::log(base);
	size_t s = n.getRealSize();
	auto tmp = log_IMax * (s - 1) + std::log(n.get(s - 1)) / std::log(base);
	uintmax_t sol = round(tmp);
	if(sol == uintmax_t(tmp)){
		sol++;
	}
	if(sol == 0 || (static_cast<bnint>(base) ^ sol) <= n){
		return sol;
	}else{
		return sol - 1;
	}
}
bool mod3(const bnint& n){
	size_t s = n.getRealSize();
	int a = 0;
	for(int i = 0;i < s;i++){
		a += n.get(i);
		a %= 3;
	}
	return a;
}
/*bool isPrimeAKS(const bnint& n){ //not to be implemented
	if(n.is_even()){
		return false;
	}
	if(mod3(n) == 0){
		return false;
	}
	if(bnint s = sqrt(n);s * s == n){
		return false;
	}
	bnint tmp = log2(n);
	tmp.divide2();
	tmp++;
	for(bnint d = 3;d <= tmp;d += 2){
		bnint l = log(n) / d + 1;
		bnint r = log(l);
		bnint g = 1, s = 1;
		for(bnint m = 1;m < r;m++){
			bnint old_g = g, old_s = s;
			g = (old_s * (old_g - (old_g ^ d) - n)) % (2_b ^ (2_b ^ m));
			s = (2 * old_s - (d - 1) * (old_g ^ (d - 1)) * (old_s ^ 2)) % (2_b ^ (2_b ^ m));
		}
		g = (g - s * ((g ^ d) - n)) % (2_b ^ l);
		if((g ^ d) == n){
			return false;
		}
	}
}*/
}
namespace std{
//C++ allows template specialization in std
template<>
class uniform_int_distribution<bnint>{
//Code modified from bits/uniform_int_dist.h
public:
	using result_type = bnint;
	struct param_type{
		using distribution_type = uniform_int_distribution<bnint>;
		explicit param_type(optional<bnint> __a = nullopt, optional<bnint> __b = nullopt, bool reverse = false) : _M_a(reverse ? __b : __a), _M_b(reverse ? __a : __b){
			if(__a.has_value() && __b.has_value() && *__a > *__b){
				swap(_M_a, _M_b);
			}
		}
		result_type a() const{
			return _M_a.value_or(0);
		}
		result_type b() const{
			return _M_b.value_or(0);
		}
		optional<bnint> a_real() const{
			return _M_a;
		}
		optional<bnint> b_real() const{
			return _M_b;
		}
		friend bool operator == (const param_type& __p1, const param_type& __p2){
			return __p1._M_a == __p2._M_a && __p1._M_b == __p2._M_b;
		}
		friend bool operator != (const param_type& __p1, const param_type& __p2){
			return !(__p1 == __p2);
		}
	private:
		optional<bnint> _M_a;
		optional<bnint> _M_b;
	};
	explicit uniform_int_distribution(optional<bnint> __a = nullopt, optional<bnint> __b = nullopt, bool reverse = false) : _M_param(__a, __b, reverse){}
	explicit uniform_int_distribution(const param_type& __p) : _M_param(__p){}
	void reset(){}
	result_type a() const{
		return _M_param.a();
	}
	result_type b() const{
		return _M_param.b();
	}
	optional<bnint> a_real() const{
		return _M_param.a_real();
	}
	optional<bnint> b_real() const{
		return _M_param.b_real();
	}
	param_type param() const{
		return _M_param;
	}
	void param(const param_type& __param){
		_M_param = __param;
	}
	result_type min() const{
		return a();
	}
	result_type max() const{
		return b();
	}
	template<typename _UniformRandomNumberGenerator>
	result_type operator () (_UniformRandomNumberGenerator& __urng, size_t inf_size = 1024){
		return operator () (__urng, _M_param, inf_size);
	}
	template<typename _UniformRandomNumberGenerator>
	result_type operator () (_UniformRandomNumberGenerator& __urng, const param_type& __p, size_t inf_size = 1024){
		static bnint base = IMax;
		static optional<size_t> inf_size_saved;
		static bnint saved;
		bnint low, high;
		if(!a_real().has_value() || !b_real().has_value()){
			if(inf_size_saved != inf_size){
				if(!inf_size_saved.has_value()){
					saved = base ^ inf_size;
				}else if(inf_size > inf_size_saved){
					saved *= base ^ (inf_size - *inf_size_saved);
				}else{
					if(*inf_size_saved - inf_size < inf_size){
						saved /= base ^ (*inf_size_saved - inf_size);
					}else{
						saved = base ^ inf_size;
					}
				}
				inf_size_saved = inf_size;
			}
		}
		if(!__p.a_real().has_value()){
			low = saved;
			low.negate();
		}else{
			low = __p.a();
		}
		if(!__p.b_real().has_value()){
			high = saved;
		}else{
			high = __p.b();
		}
		if(low > high){
			swap(low, high);
		}
		bnint range = high - low;
		size_t size = range.getRealSize();
		uniform_int_distribution<typename bnint::base_type> init_dis(0, range.get(size - 1));
		static uniform_int_distribution<typename bnint::base_type> rem_dis(0, bnint::base_max);
		bnint sol = 0;
		sol.reserve(size);
		//no matter what IMax is or how large/special the number is, the expected value of times of attempt is less than 2.
		while(true){
			cout << '!';
			bnint::base_type init = init_dis(__urng);
			bool signal = (init == range.get(size - 1));
			sol += init;
			for(int i = 1;i < size;i++){
				sol << 1;
				sol += rem_dis(__urng);
			}
			if(init == range.get(size - 1) && sol > range){
				sol = 0;
				sol.reserve(size);
				continue;
			}
			sol.fit();
			sol += low;
			return sol;
		}
	}
	/*template<typename _ForwardIterator, typename _UniformRandomNumberGenerator>
	void __generate(_ForwardIterator __f, _ForwardIterator __t, _UniformRandomNumberGenerator& __urng){
		__generate(__f, __t, __urng, _M_param);
	}
	template<typename _ForwardIterator, typename _UniformRandomNumberGenerator>
	void __generate(_ForwardIterator __f, _ForwardIterator __t, _UniformRandomNumberGenerator& __urng, const param_type& __p){
		__generate_impl(__f, __t, __urng, __p);
	}
	template<typename _UniformRandomNumberGenerator>
	void __generate(result_type* __f, result_type* __t, _UniformRandomNumberGenerator& __urng, const param_type& __p){
		__generate_impl(__f, __t, __urng, __p);
	}*/
	friend bool operator == (const uniform_int_distribution& __d1, const uniform_int_distribution& __d2){
		return __d1._M_param == __d2._M_param;
	}
private:
	param_type _M_param;
	//template<typename _ForwardIterator, typename _UniformRandomNumberGenerator>
	//void __generate_impl(_ForwardIterator __f, _ForwardIterator __t, _UniformRandomNumberGenerator& __urng, const param_type& __p);
};
}

#endif