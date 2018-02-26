#define _DYNAMIC_ 1
#include "BigNumber.h"
#include <cmath>
#include <optional>
#include <random>
#include <array>
#include <algorithm>
//#include <execution>
#include <functional>
#include <thread>

#ifndef _BNMATH_
#define _BNMATH_

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
		if(!__p.a_real().has_value() || !__p.b_real().has_value()){
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
	friend bool operator != (const uniform_int_distribution& __d1, const uniform_int_distribution& __d2){
		return !(__d1._M_param == __d2._M_param);
	}
private:
	param_type _M_param;
	//template<typename _ForwardIterator, typename _UniformRandomNumberGenerator>
	//void __generate_impl(_ForwardIterator __f, _ForwardIterator __t, _UniformRandomNumberGenerator& __urng, const param_type& __p);
};
template<class CharT, class Traits>
basic_ostream<CharT, Traits>& operator << (basic_ostream<CharT, Traits>& os, const uniform_int_distribution<bnint>& __d){
	if(__d.a_real().has_value()){
		os << __d.a();
	}else{
		os << "-inf";
	}
	os << ' ';
	if(__d.b_real().has_value()){
		os << __d.b();
	}else{
		os << "inf";
	}
	return os;
}
} //end of std

namespace BigIntMath{
using Math::easygcd;
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
namespace ext_euc{
template<size_t N>
struct gauss_row{
	bnint a[N + 1];
	static constexpr size_t member_size = N;
	bool operator == (const gauss_row& g) const{return a[0] == g.a[0];}
	bool operator != (const gauss_row& g) const{return a[0] != g.a[0];}
	bool operator <= (const gauss_row& g) const{return a[0] <= g.a[0];}
	bool operator >= (const gauss_row& g) const{return a[0] >= g.a[0];}
	bool operator < (const gauss_row& g) const{return a[0] < g.a[0];}
	bool operator > (const gauss_row& g) const{return a[0] > g.a[0];}
	bool operator == (const bnint& n) const{return a[0] == n;}
	bool operator != (const bnint& n) const{return a[0] != n;}
	bool operator <= (const bnint& n) const{return a[0] <= n;}
	bool operator >= (const bnint& n) const{return a[0] >= n;}
	bool operator < (const bnint& n) const{return a[0] < n;}
	bool operator > (const bnint& n) const{return a[0] > n;}
	const gauss_row& operator = (const gauss_row& g){
		if(this == &g){return *this;}
		for(int i = 0;i <= N;i++){
			a[i] = g.a[i];
		}
		return *this;
	}
	const gauss_row& operator += (const gauss_row& g){
		for(int i = 0;i <= N;i++){
			a[i] += g.a[i];
		}
		return *this;
	}
	gauss_row operator + (const gauss_row& g){
		gauss_row tmp = *this;
		tmp += g;
		return tmp;
	}
	const gauss_row& operator -= (const gauss_row& g){
		for(int i = 0;i <= N;i++){
			a[i] -= g.a[i];
		}
		return *this;
	}
	gauss_row operator - (const gauss_row& g){
		gauss_row tmp = *this;
		tmp -= g;
		return tmp;
	}
	const gauss_row& operator *= (const bnint& n){
		for(int i = 0;i <= N;i++){
			a[i] *= n;
		}
		return *this;
	}
	gauss_row operator + (const bnint& n){
		gauss_row tmp = *this;
		tmp *= n;
		return tmp;
	}
	const gauss_row& operator %= (const gauss_row& g){
		bnint path = (a[0] - (a[0] % g.a[0])) / g.a[0];
		if(path == 0){return *this;}
		for(int i = 0;i <= N;i++){
			a[i] -= g.a[i] * path;
		}
		return *this;
	}
	gauss_row operator % (const gauss_row& g){
		gauss_row tmp = *this;
		tmp %= g;
		return tmp;
	}
	gauss_row operator + () const{
		return *this;
	}
	gauss_row operator - () const{
		gauss_row g = *this;
		for(auto& i : g.a){
			i = -i;
		}
		return *this;
	}
	//operator T () const{return a[0];}
	friend ostream& operator << (ostream& os, const gauss_row& g){
		os << g.a[0];
		for(int i = 1;i <= g.member_size;i++){
			os << ' ' << g.a[i];
		}
		return os;
	}
};
} //end of ext_euc
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
bnint powmod(bnint base, bnint power, const bnint& mod){
	//not understood so much, to be researched
	bnint sol = 1;
	while(power.get_positive() && !power.is_zero()){
		if(power.is_odd()){
			sol = (sol * base) % mod;
		}
		base ^= 2;
		base %= mod;
		power.divide2();
	}
	return sol;
}
optional<bnint> modinv(bnint a, const bnint& n){
	if(gcd(a, n) != 1){ //no modular multiplicative inverse
		return nullopt;
	}
	a %= n;
	if(!a.get_positive()){ //it is impossible for a to be 0 with gcd(a, n) == 1
		a += n;
	}
	auto tmp = easygcd(ext_euc::gauss_row<2>{a, 1, 0}, ext_euc::gauss_row<2>{n, 0, 1});
	bnint sol = tmp.a[1];
	sol %= n;
	if(!sol.get_positive()){
		sol += n;
	}
	return sol;
}
pair<bool, optional<vector<int>>> MillerRabin(const bnint& n, int addition = 0/*, bool parallel = false, size_t th = 1*/){
	constexpr static array must_com = {2, 3, 5, 7, 11, 233};
	constexpr static array chos_com = {13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
		53, 59, 61, 67, 71, 73, 79, 83, 89, 97,
		101, 103, 107, 109, 113, 127, 131, 137, 139, 149,
		151, 157, 163, 167, 173, 179, 181, 191, 193, 197,
		199, 211, 223, 227, 229, 239, 241, 251, 257, 263,
		269, 271, 277, 281, 283, 293, 307, 311, 313, 317,
		331, 337, 347, 349, 353, 359, 367, 373, 379, 383,
		389, 397, 401, 409, 419, 421, 431, 433, 439, 443,
		449, 457, 461, 463, 467, 479, 487, 491, 499, 503,
		509, 521, 523, 541, 547, 557, 563, 569, 571, 577,
		587, 593, 599, 601, 607, 613, 617, 619, 631, 641,
		643, 647, 653, 659, 661, 673, 677, 683, 691, 701,
		709, 719, 727, 733, 739, 743, 751, 757, 761, 769,
		773, 787, 797, 809, 811, 821, 823, 827, 829, 839,
		853, 857, 859, 863, 877, 881, 883, 887, 907, 911,
		919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997};
	//static uniform_int_distribution<> g(0, chos_com.size() - 1);
	static mt19937_64 rd(random_device{}());
	if(n.digit() < 4){
		auto f = bind(equal_to<bnint>(), n, placeholders::_1);
		return make_pair(any_of(must_com.begin(), must_com.end(), f)
		              || any_of(chos_com.begin(), chos_com.end(), f), nullopt);
		/*return make_pair(any_of(execution::par_unseq, must_com.begin(), must_com.end(), f))
		    || any_of(execution::par_unseq, chos_com.begin(), chos_com.end(), f), nullopt);*/
	}
	if(n.is_even()){
		return make_pair(false, nullopt);
	}
	if(n.get(0) % 5 == 0){
		return make_pair(false, nullopt);
	}
	if(addition < 0){addition = 0;}
	else if(addition + 1 > chos_com.size()){
		addition = chos_com.size() - 1;
	}
	vector<int> witness(must_com.size() + 1 + addition);
	copy(must_com.begin(), must_com.end(), witness.begin());
	sample(chos_com.begin(), chos_com.end(), witness.begin() + must_com.size(), 1 + addition, rd);
	//*witness.rbegin() = chos_com[g(rd)];
	bnint d, s = 0, minusone;
	d = minusone = n - 1;
	while(d.is_even()){
		d.divide2();
		s++;
	}
	//if(!parallel){
	for(auto a : witness){
		//cout << a << ' ' << s << endl;
		bnint base = powmod(a, d, n); //a ^ d mod n
		if(base == 1 || base == minusone){
			continue;
		}
		bool success = false;
		for(bnint i = 1;i < s;i++){
			base ^= 2;
			base %= n;
			if(base == minusone){
				success = true;
				break;
			}else if(base == 1){
				//always 1 (!= n - 1) so it is composite
				break;
			}
		}
		if(!success){
			return make_pair(false, witness);
		}
	}
	return make_pair(true, witness);
	/*}else{ //bad
		bool guess = true;
		auto f = [&](int a){
			bnint base = powmod(a, d, n); //a ^ d mod n
			if(base == 1 || base == minusone){
				return;
			}
			bool success = false;
			for(bnint i = 1;i < s;i++){
				if(!guess){cout << a;return;}
				base ^= 2;
				base %= n;
				if(base == minusone){
					success = true;
					break;
				}else if(base == 1){
					//always 1 (!= n - 1) so it is composite
					break;
				}
			}
			if(!success){
				//return make_pair(false, witness);
				guess = false;
			}
		};
		vector<thread> vth;
		vth.reserve(th);
		for(int i = 1;i <= witness.size();i++){
			vth.push_back(thread(f, witness[i - 1]));
			if(i % th == 0){
				for(auto& t : vth){
					t.join();
				}
				vth.clear();
			}
		}
		for(auto& t : vth){
			t.join();
		}
		return make_pair(guess, witness);
	}*/
}
auto RSAkeygenerator(){
	static const size_t bits = 2048;
	static independent_bits_engine<mt19937_64, 64, uint64_t> ge(mt19937_64(chrono::system_clock::now().time_since_epoch().count()));
	static uniform_int_distribution<bnint> rd(1, (2_b ^ (bits / 2)) - 1);
	static bnint aaa = 2_b ^ (11 + bits / 4);
	bnint p, q;
	do{
		//cout << 1;
		p = rd(ge);
	}while(!MillerRabin(p).first);
	while(true){
		//cout << 2;
		q = rd(ge);
		if(abs(p - q) <= aaa){
			continue;
		}
		if(!MillerRabin(q).first){
			continue;
		}
		if(gcd(p, q) != 1){ //false prime
			//cout << "?";
			continue;
		}
		bnint N = p * q;
		bnint N14 = sqrt(sqrt(N) + 1) + 1;
		bnint r = (p - 1) * (q - 1) / gcd(p - 1, q - 1);
		uniform_int_distribution<bnint> rd2(65537, r - 1);
		bnint e, d;
		do{
			do{
				//cout << 4;
				e = rd2(ge);
			}while(gcd(r, e) != 1);
			d = *modinv(e, r);
		}while(d <= N14);
		auto sol = make_tuple(N, e, d);
		p = 0, q = 0;
		return sol;
	}
}
template<typename T>
bnint howpow(const T& a, const T& b){
	return _howpow<bnint>(a, b);
}
} //end of BigIntMath


#endif