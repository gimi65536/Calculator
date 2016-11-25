#define _DYNAMIC_ 1
#include "BigNumber.h"
#include <tuple>
#include <vector>

#ifndef _RATIO_NUMBER_
#define _RATIO_NUMBER_

bnint gcd(bnint a, bnint b);
bnint lcm(bnint a, bnint b);
const int DEFAULT_endure_precision = 100;

enum Endian{UNKNOWN, big_endian, little_endian};
enum LONG_DOUBLE{UNKNOWN_LD, is_80bits, is_IEEE};
Endian endian = UNKNOWN;
LONG_DOUBLE ld_type = UNKNOWN_LD;
int ld_byte = 0;
int ld_exp = 0;
void judge_endian();
void judge_ldtype();
bool fast_zero_signal = false;

class RatioNumber;
typedef tuple<string, RatioNumber&, bnint, int, int, int, vector<bnint> > Package_thread;
typedef RatioNumber numeric;

class RatioNumber{
private:
	static int precision;
	static int endure_precision;
	static int mode;
	static vector<Package_thread> fast_thread;
	static int now_thread;
	enum _INDICATE{NAME, SOLUTION, PUT_IN, SOL_PRECISION, PRO_PRECISION, ROUND_MODE, MIDWAY_BN_VECTOR};
	bnint numerator;
	bnint denominator;
	bool positive;
	mutable bool lock;
	template <typename T>
	typename enable_if<is_floating_point<T>::value, void>::type PASS_BY_IEEE(T r, int byte, int exp);
	template <typename T>
	typename enable_if<is_floating_point<T>::value, void>::type PASS_BY_80_bits(T ld);
	void common_pass_float(const string& str, RatioNumber& base, int& EXP);
	void PASS_BY_STRING(string str);
	void PASS_BY_STRING_with_notation(string str);
	template <typename T>
	void WIDE_CHAR_PASS(const T& ch);
	template <typename T>
	void WIDE_CHARARRAY_PASS(const T* C_STR);
public:
	friend ostream& operator << (ostream& os, const RatioNumber& r);
	void reduce();
	void reduce() const;
	void correct_positive();
	RatioNumber();
	RatioNumber(const RatioNumber& r);
	RatioNumber(const bnint& num, const bnint& den);
	template <class T>
	RatioNumber(const T& n);
	RatioNumber(string str);
	bool is_INF() const;
	bool is_positive_INF() const;
	bool is_negative_INF() const;
	bool is_NaN() const;
	bool is_fraction() const;
	void to_INF();
	void to_positive_INF();
	void to_negative_INF();
	void to_NaN();
	bnint get_numerator() const{return numerator * (positive ? 1 : -1);}
	bnint get_denominator() const{return denominator;}
	bool operator == (const RatioNumber& r) const;
	bool operator != (const RatioNumber& r) const;
	bool operator < (const RatioNumber& r) const;
	bool operator <= (const RatioNumber& r) const;
	bool operator > (const RatioNumber& r) const;
	bool operator >= (const RatioNumber& r) const;
	const RatioNumber operator + () const;
	const RatioNumber operator - () const;
	const RatioNumber& operator = (const RatioNumber& r);
	const RatioNumber& operator = (const bnint& n);
	const RatioNumber& operator = (const string& str);
	template <typename T>
	const RatioNumber& operator = (const basic_string<T>& STR);
	const RatioNumber& operator = (const char& ch);
	const RatioNumber& operator = (char* const n);
	const RatioNumber& operator = (const char* const n);
	const RatioNumber& operator = (const wchar_t& ch);
	const RatioNumber& operator = (wchar_t* const n);
	const RatioNumber& operator = (const wchar_t* const n);
	const RatioNumber& operator = (const char16_t& ch);
	const RatioNumber& operator = (char16_t* const n);
	const RatioNumber& operator = (const char16_t* const n);
	const RatioNumber& operator = (const char32_t& ch);
	const RatioNumber& operator = (char32_t* const n);
	const RatioNumber& operator = (const char32_t* const n);
	template <typename T>
	typename enable_if<is_floating_point<T>::value, const RatioNumber&>::type operator = (T d);
	template <typename T>
	typename enable_if<is_integral<T>::value, const RatioNumber&>::type operator = (T n);
	template <typename T>
	const RatioNumber& operator = (T* const n);
	void assign_without_reduction(RatioNumber r);
	const RatioNumber abs() const;
	const RatioNumber abs_inverse() const;
	const RatioNumber& operator += (const RatioNumber& r);
	template <typename T>
	const RatioNumber& operator += (const T& n);
	const RatioNumber operator + (const RatioNumber& r) const;
	template<typename T>
	const RatioNumber operator + (const T& n) const;
	void add_without_reduction(RatioNumber r);
	const RatioNumber& operator -= (const RatioNumber& r);
	template <typename T>
	const RatioNumber& operator -= (const T& n);
	const RatioNumber operator - (const RatioNumber& r) const;
	template<typename T>
	const RatioNumber operator - (const T& n) const;
	void minus_without_reduction(RatioNumber r);
	const RatioNumber& operator *= (const RatioNumber& r);
	template <typename T>
	const RatioNumber& operator *= (const T& n);
	const RatioNumber operator * (const RatioNumber& r) const;
	template<typename T>
	const RatioNumber operator * (const T& n) const;
	void multiply_without_reduction(RatioNumber r);
	const RatioNumber& operator /= (const RatioNumber& r);
	template <typename T>
	const RatioNumber& operator /= (const T& n);
	const RatioNumber operator / (const RatioNumber& r) const;
	template<typename T>
	const RatioNumber operator / (const T& n) const;
	void divide_without_reduction(RatioNumber r);
	const RatioNumber& operator ^= (const bnint& n);
	const RatioNumber operator ^ (const bnint& n) const;
	void print();
	void print() const;
	string str() const;
	void negate(){if(!is_NaN()){positive = !positive;}}
	RatioNumber reciprocal() const;
	const bnint getNumerator() const{return numerator;}
	const bnint getDenominator() const{return denominator;}
	static void Setprecision(){precision = -1;}
	static void Setprecision(int n){if(n >= 0){precision = n;}else{precision = -1;}}
	static void unSetprecision(){precision = -1;}
	static void Set_autoprecision(int n){if(n < 0){endure_precision = DEFAULT_endure_precision;}else{endure_precision = n;}}
	static void Set_autoprecision(){endure_precision = DEFAULT_endure_precision;}
	static void unSet_autoprecision(){endure_precision = DEFAULT_endure_precision;}
	static void SetRound(){mode = 0;}
	static void SetRoundDown(){mode = -1;}
	static void SetRoundUp(){mode = 1;}
	static void unSetRound(){mode = 0;}
	static bool fast_start(string str, RatioNumber& sol, int sol_precis, int process_precis = -1, int round = 0);
	static bool fast_switch(string str);
	static bool fast_switch(const RatioNumber& sol);
	static void fast_add(const RatioNumber& r);
	static void fast_add(const bnint& num, const bnint& den);
	static void fast_minus(const RatioNumber& r);
	static void fast_minus(const bnint& num, const bnint& den);
	static void fast_putin_temp();
	static bool fast_end();
	static bool fast_end(string str);
	static bool fast_end(RatioNumber& sol);
	friend RatioNumber sin(const RatioNumber& r, int pre);
	friend RatioNumber arctan(const RatioNumber& r, int pre);
};

bool fast_start(string str, RatioNumber& sol, int sol_precis, int process_precis = -1, int round = 0){
	return RatioNumber::fast_start(str, sol, sol_precis, process_precis, round);
}
bool fast_switch(string str){
	return RatioNumber::fast_switch(str);
}
bool fast_switch(const RatioNumber& sol){
	return RatioNumber::fast_switch(sol);
}
void fast_add(const RatioNumber& r){
	RatioNumber::fast_add(r);
}
void fast_add(const bnint& num, const bnint& den){
	RatioNumber::fast_add(num, den);
}
void fast_minus(const RatioNumber& r){
	RatioNumber::fast_minus(r);
}
void fast_minus(const bnint& num, const bnint& den){
	RatioNumber::fast_minus(num, den);
}
void fast_putin_temp(){
	RatioNumber::fast_putin_temp();
}
bool fast_end(){
	return RatioNumber::fast_end();
}
bool fast_end(string str){
	return RatioNumber::fast_end(str);
}
bool fast_end(RatioNumber& sol){
	return RatioNumber::fast_end(sol);
}

int RatioNumber::precision = -1;
int RatioNumber::endure_precision = DEFAULT_endure_precision;
int RatioNumber::mode = 0;
vector<Package_thread> RatioNumber::fast_thread;
int RatioNumber::now_thread = -1;

bnint gcd(bnint a, bnint b){
	if(!a.get_positive()){
		a.negate();
	}
	if(!b.get_positive()){
		b.negate();
	}
	bnint tmp;
	while(!a.is_zero() && !b.is_zero()){
		tmp = a % b;
		a = b;
		b = tmp;
	}
	if(b.is_zero()){
		return a;
	}else{
		return b;
	}
}

bnint lcm(bnint a, bnint b){
	return a * b / gcd(a, b);
}

void judge_endian(){
	int i = 1;
	char* ptr = reinterpret_cast<char*>(&i);
	if(ptr[0] == 1){
		endian = little_endian;
	}else{
		endian = big_endian;
	}
}

void judge_ldtype(){
	long double t = 1.0;
	char* j = reinterpret_cast<char*>(&t);
	int size = sizeof(t);
	if(endian == UNKNOWN){
		judge_endian();
	}
	int i = 0, di = 0;
	if(endian == little_endian){
		i = size - 1;
		di = -1;
	}else{
		i = 0;
		di = 1;
	}
	string str;
	for(int ii = 0;ii < size;ii++, i += di){
		int tmp = static_cast<int>(j[i]);
		if(tmp < 0){
			tmp += 256;
		}
		string buf = static_cast<bnint>(tmp).str(2);
		buf.erase(0, 2);
		while(buf.length() < 8){
			buf.insert(0, 1, '0');
		}
		str += buf;
	}
	if(str.length() >= 80){
		if(str.substr(str.length() - 80).find("001111111111111110") == 0){
			ld_type = is_80bits;
			return;
		}
	}
	ld_type = is_IEEE;
	size_t pos = str.find_last_of('1');
	size_t apos = str.find_last_of('0', pos);
	ld_exp = pos - apos + 1;
	ld_byte = (str.length() - apos + 1) / 8;
}

const bnint getNumerator(const RatioNumber& r){return r.getNumerator();}
const bnint getDenominator(const RatioNumber& r){return r.getDenominator();}

void Setprecision(){RatioNumber::Setprecision();}
void Setprecision(int n){RatioNumber::Setprecision(n);}
void unSetprecision(){RatioNumber::unSetprecision();}
void Set_autoprecision(int n){RatioNumber::Set_autoprecision(n);}
void Set_autoprecision(){RatioNumber::Set_autoprecision();}
void unSet_autoprecision(){RatioNumber::unSet_autoprecision();}
void SetRound(){RatioNumber::SetRound();}
void SetRoundDown(){RatioNumber::SetRoundDown();}
void SetRoundUp(){RatioNumber::SetRoundUp();}
void unSetRound(){RatioNumber::unSetRound();}

void RatioNumber::reduce(){
	if(!numerator.get_positive()){
		positive = !positive;
		numerator.negate();
	}
	if(!denominator.get_positive()){
		positive = !positive;
		denominator.negate();
	}
	bnint tmp = gcd(numerator, denominator);
	if(tmp.is_zero()){ //NaN
		positive = true;
		return;
	}else{
		numerator /= tmp;
		denominator /= tmp;
		if(numerator.is_zero()){
			positive = true;
		}
	}
}

void RatioNumber::reduce() const{}

void RatioNumber::correct_positive(){
	if(!numerator.get_positive() && !numerator.is_zero()){
		positive = !positive;
		numerator.negate();
	}
	if(!denominator.get_positive() && !denominator.is_zero()){
		positive = !positive;
		denominator.negate();
	}
}

template <typename T>
typename enable_if<is_floating_point<T>::value, void>::type RatioNumber::PASS_BY_IEEE(T r, int byte, int exp){
	int hensa = 1;
	for(int i = 1;i < exp;i++){
		hensa *= 2;
	}
	hensa -= 1;
	char* j = reinterpret_cast<char*>(&r);
	int size = sizeof(r);
	if(endian == UNKNOWN){
		judge_endian();
	}
	int i = 0, di = 0;
	if(endian == little_endian){
		i = byte - 1;
		di = -1;
	}else{
		i = size - byte;
		di = 1;
	}
	string str;
	for(int ii = 0;ii < byte;ii++, i += di){
		int tmp = static_cast<int>(j[i]);
		if(tmp < 0){
			tmp += 256;
		}
		string buf = static_cast<bnint>(tmp).str(2);
		buf.erase(0, 2);
		while(buf.length() < 8){
			buf.insert(0, 1, '0');
		}
		str += buf;
	}
	str = str.substr(str.length() - 8 * byte);
	//positive
	string buf = str.substr(0, 1);
	str.erase(0, 1);
	if(buf == "0"){
		positive = true;
	}else{
		positive = false;
	}
	//exp
	buf = str.substr(0, exp);
	str.erase(0, exp);
	buf.insert(0, "0b");
	bnint EXP_mid = buf;
	int EXP = static_cast<BtoI>(EXP_mid);
	RatioNumber base(0, 1);
	if(EXP == 0){
		if(str.find_first_not_of("0") == string::npos){
			numerator = 0;
			denominator = 1;
			positive = true;
			return;
		}
		EXP = -hensa + 1;
	}else if(EXP <= 2 * hensa){
		base.numerator = 1;
		EXP -= hensa;
	}else{
		if(str.find('1') == string::npos){
			numerator = 1;
			denominator = 0;
		}else{
			numerator = 0;
			denominator = 0;
			positive = true;
		}
		return;
	}
	//fraction
	common_pass_float(str, base, EXP);
}

template <typename T>
typename enable_if<is_floating_point<T>::value, void>::type RatioNumber::PASS_BY_80_bits(T ld){
	int hensa = 16383, exp = 15;
	char* j = reinterpret_cast<char*>(&ld);
	int size = sizeof(ld);
	if(endian == UNKNOWN){
		judge_endian();
	}
	int i = 0, di = 0;
	if(endian == little_endian){
		i = size - 1;
		di = -1;
	}else{
		i = 0;
		di = 1;
	}
	string str;
	for(int ii = 0;ii < size;ii++, i += di){
		int tmp = static_cast<int>(j[i]);
		if(tmp < 0){
			tmp += 256;
		}
		string buf = static_cast<bnint>(tmp).str(2);
		buf.erase(0, 2);
		while(buf.length() < 8){
			buf.insert(0, 1, '0');
		}
		str += buf;
	}
	str = str.substr(str.length() - 80);
	//positive
	string buf = str.substr(0, 1);
	str.erase(0, 1);
	if(buf == "0"){
		positive = true;
	}else{
		positive = false;
	}
	//exp
	buf = str.substr(0, exp);
	str.erase(0, exp);
	buf.insert(0, "0b");
	bnint EXP_mid = buf;
	int EXP = static_cast<BtoI>(EXP_mid);
	RatioNumber base(0, 1);
	if(EXP == 0){
		if(str.find_first_not_of("0") == string::npos){
			numerator = 0;
			denominator = 1;
			positive = true;
			return;
		}
		EXP = -hensa + 1;
	}else if(EXP <= 2 * hensa){
		EXP -= hensa;
	}else{
		if(str[0] != str[1]){
			str.erase(0, 2);
			if(str.find_first_not_of("0") == string::npos){
				numerator = 1;
				denominator = 0;
			}else{
				numerator = 0;
				denominator = 0;
				positive = true;
			}
		}else{
			numerator = 0;
			denominator = 0;
			positive = true;
		}
		return;
	}
	//fraction
	if(str[0] == '1'){
		base.numerator = 1;
	}
	str.erase(0, 1);
	common_pass_float(str, base, EXP);
}

void RatioNumber::common_pass_float(const string& str, RatioNumber& base, int& EXP){
	RatioNumber exp_base(1, 2);
	for(int i = 0;i < str.length();i++){
		if(str[i] == '1'){
			base += exp_base;
		}
		exp_base.denominator *= 2;
	}
	if(EXP != 0){
		bnint two_base_n = 2, two_base_d = 1;
		if(EXP < 0){
			two_base_n = 1;
			two_base_d = 2;
			EXP *= -1;
		}
		base.numerator *= two_base_n ^ EXP;
		base.denominator *= two_base_d ^ EXP;
	}
	numerator = base.numerator;
	denominator = base.denominator;
	reduce();
}

void RatioNumber::PASS_BY_STRING(string str){
	//check notation
	string test;
	if(str.length() >= 3 && (str[0] == '+' || str[0] == '-')){
		test = str.substr(1, 2);
	}else if(str.length() >= 2){
		test = str.substr(0, 2);
	}
	if(test.length() == 2){
		if(test[0] == '0' && test.find_first_of("BbCcDdEeFfGgHhIiJjKkLlMmNnOoXx") != string::npos){
			PASS_BY_STRING_with_notation(str);
			return;
		}
	}
	//end
	size_t pos = str.find_first_not_of("0123456789+-.");
	while(pos != string::npos){
		str.erase(pos, 1);
		pos = str.find_first_not_of("0123456789+-.");
	}
	if(str.length() > 0 && (str[0] == '-' || str[0] == '+')){
		str.insert(1, "0J");
	}else{
		str.insert(0, "0J");
	}
	PASS_BY_STRING_with_notation(str);
}

void RatioNumber::PASS_BY_STRING_with_notation(string str){
	if(str[0] == '+'){
		positive = true;
		str.erase(0, 1);
	}else if(str[0] == '-'){
		positive = false;
		str.erase(0, 1);
	}else{
		positive = true;
	}
	string Nota = str.substr(0, 2);
	str.erase(0, 1);
	for(int i = 0;i < str.length();i++){
		str[i] = toupper(str[i]);
		if(!isalpha(str[i]) && !isdigit(str[i]) && str[i] != '.'){
			str.erase(i, 1);
			i--;
		}
	}
	int notation_t = (str[0] != 'X') ? (str[0] - 'A' + 1) : (16);
	bnint notation = notation_t;
	str.erase(0, 1);
	size_t pos = str.find('.');
	if(pos != string::npos){
		bnint integer_part = Nota + str.substr(0, pos);
		str.erase(0, pos + 1);
		pos = str.find('.');
		while(pos != string::npos){
			str.erase(pos, 1);
			pos = str.find('.');
		}
		while(str.length() > 0 && str[str.length() - 1] == '0'){
			str.erase(str.length() - 1, 1);
		}
		int times = str.length();
		bnint fraction_part = Nota + str;
		denominator = notation ^ times;
		numerator = integer_part * denominator + fraction_part;
		reduce();
	}else{
		numerator = Nota + str;
		denominator = 1;
	}
}

template <typename T>
void RatioNumber::WIDE_CHAR_PASS(const T& ch){
	basic_string<T> STR;
	STR += ch;
	PASS_BY_STRING(cvt_string(STR));
}

template <typename T>
void RatioNumber::WIDE_CHARARRAY_PASS(const T* C_STR){
	basic_string<T> STR = C_STR;
	PASS_BY_STRING(cvt_string(STR));
}

RatioNumber::RatioNumber(){
	numerator = 0;
	denominator = 1;
	positive = true;
	lock = false;
}

RatioNumber::RatioNumber(const RatioNumber& r){
	numerator = r.numerator;
	denominator = r.denominator;
	positive = r.positive;
	lock = false;
	reduce();
}

RatioNumber::RatioNumber(const bnint& num, const bnint& den){
	numerator = num;
	denominator = den;
	positive = true;
	lock = false;
	reduce();
}

template <class T>
RatioNumber::RatioNumber(const T& n){
	lock = false;
	(*this) = n;
}

RatioNumber::RatioNumber(string str){
	lock = false;
	PASS_BY_STRING(str);
}

bool RatioNumber::is_INF() const{
	if(!numerator.is_zero() && denominator.is_zero()){
		return true;
	}
	return false;
}

bool is_INF(const RatioNumber& r){
	return r.is_INF();
}

bool RatioNumber::is_positive_INF() const{
	if(is_INF() && positive){
		return true;
	}
	return false;
}

bool is_positive_INF(const RatioNumber& r){
	return r.is_positive_INF();
}

bool RatioNumber::is_negative_INF() const{
	if(is_INF() && !positive){
		return true;
	}
	return false;
}

bool is_negative_INF(const RatioNumber& r){
	return r.is_negative_INF();
}

bool RatioNumber::is_NaN() const{
	if(numerator.is_zero() && denominator.is_zero()){
		return true;
	}
	return false;
}

bool is_NaN(const RatioNumber& r){
	return r.is_NaN();
}

bool RatioNumber::is_fraction() const{
	return !denominator.is_zero();
}

bool is_fraction(const RatioNumber& r){
	return r.is_fraction();
}

void RatioNumber::to_INF(){
	if(lock){
		return;
	}
	numerator = 1;
	denominator = 0;
}

void RatioNumber::to_positive_INF(){
	if(lock){
		return;
	}
	numerator = 1;
	denominator = 0;
	positive = true;
}

RatioNumber positive_INF_generator(){
	RatioNumber tmp(1, 0);
	return tmp;
}

void RatioNumber::to_negative_INF(){
	if(lock){
		return;
	}
	numerator = 1;
	denominator = 0;
	positive = false;
}

RatioNumber negative_INF_generator(){
	RatioNumber tmp(-1, 0);
	return tmp;
}

void RatioNumber::to_NaN(){
	if(lock){
		return;
	}
	numerator = 0;
	denominator = 0;
	positive = true;
}

RatioNumber NaN_generator(){
	RatioNumber tmp(0, 0);
	return tmp;
}

bool RatioNumber::operator == (const RatioNumber& r) const{
	if(is_NaN() || r.is_NaN()){
		return false;
	}else if(is_INF() || r.is_INF()){
		if(is_positive_INF() && r.is_positive_INF()){
			return true;
		}else if(is_negative_INF() && r.is_negative_INF()){
			return true;
		}else{
			return false;
		}
	}
	bnint Lcm = lcm(denominator, r.denominator);
	bnint t1 = numerator * (Lcm / denominator), t2 = r.numerator * (Lcm / r.denominator);
	if(!positive){t1 *= -1;}
	if(!r.positive){t2 *= -1;}
	if(t1 == t2){
		return true;
	}
	return false;
}
bool RatioNumber::operator != (const RatioNumber& r) const{
	return !((*this) == r);
}
bool RatioNumber::operator < (const RatioNumber& r) const{
	if(is_NaN() || r.is_NaN()){
		return false;
	}else if(is_positive_INF()){
		return false;
	}else if(is_negative_INF()){
		return !r.is_negative_INF();
	}else if(r.is_positive_INF()){
		return true; //!is_positive_INF()
	}else if(r.is_negative_INF()){
		return false;
	}
	bnint Lcm = lcm(denominator, r.denominator);
	bnint t1 = numerator * (Lcm / denominator), t2 = r.numerator * (Lcm / r.denominator);
	if(!positive){t1 *= -1;}
	if(!r.positive){t2 *= -1;}
	if(t1 < t2){
		return true;
	}
	return false;
}
bool RatioNumber::operator <= (const RatioNumber& r) const{
	return (*this) < r || (*this) == r;
}
bool RatioNumber::operator > (const RatioNumber& r) const{
	if(is_NaN() || r.is_NaN()){
		return false;
	}else if(is_positive_INF()){
		return !r.is_positive_INF();
	}else if(is_negative_INF()){
		return false;
	}else if(r.is_positive_INF()){
		return false;
	}else if(r.is_negative_INF()){
		return true; //!is_negative_INF()
	}
	bnint Lcm = lcm(denominator, r.denominator);
	bnint t1 = numerator * (Lcm / denominator), t2 = r.numerator * (Lcm / r.denominator);
	if(!positive){t1 *= -1;}
	if(!r.positive){t2 *= -1;}
	if(t1 > t2){
		return true;
	}
	return false;
}
bool RatioNumber::operator >= (const RatioNumber& r) const{
	return (*this) > r || (*this) == r;
}
const RatioNumber RatioNumber::operator + () const{
	return (*this);
}
const RatioNumber RatioNumber::operator - () const{
	if(is_NaN()){
		return (*this);
	}else{
		RatioNumber tmp = (*this);
		if(numerator != 0){
			tmp.positive = false;
		}
		return tmp;
	}
}
void RatioNumber::assign_without_reduction(RatioNumber r){
	if(lock){
		return;
	}
	numerator = r.numerator;
	denominator = r.denominator;
	positive = r.positive;
	lock = false;
}
const RatioNumber& RatioNumber::operator = (const RatioNumber& r){
	if(lock){
		return (*this);
	}
	assign_without_reduction(r);
	reduce();
	return (*this);
}
const RatioNumber& RatioNumber::operator = (const bnint& n){
	if(lock){
		return (*this);
	}
	numerator = n;
	denominator = 1;
	if(!numerator.get_positive()){
		positive = false;
		numerator.negate();
	}else{
		positive = true;
	}
}
const RatioNumber& RatioNumber::operator = (const string& str){
	if(lock){
		return (*this);
	}
	PASS_BY_STRING(str);
	reduce();
	return (*this);
}
template <typename T>
const RatioNumber& RatioNumber::operator = (const basic_string<T>& STR){
	if(lock){
		return (*this);
	}
	PASS_BY_STRING(cvt_string(STR));
	reduce();
	return (*this);
}
const RatioNumber& RatioNumber::operator = (const char& ch){
	if(lock){
		return (*this);
	}
	string str;
	str += ch;
	PASS_BY_STRING(str);
	reduce();
	return (*this);
}
const RatioNumber& RatioNumber::operator = (char* const n){
	if(lock){
		return (*this);
	}
	string str = n;
	PASS_BY_STRING(str);
	reduce();
	return (*this);
}
const RatioNumber& RatioNumber::operator = (const char* const n){
	if(lock){
		return (*this);
	}
	string str = n;
	PASS_BY_STRING(str);
	reduce();
	return (*this);
}
const RatioNumber& RatioNumber::operator = (const wchar_t& ch){
	if(lock){
		return (*this);
	}
	WIDE_CHAR_PASS(ch);
	reduce();
	return (*this);
}
const RatioNumber& RatioNumber::operator = (wchar_t* const n){
	if(lock){
		return (*this);
	}
}
const RatioNumber& RatioNumber::operator = (const wchar_t* const n){
	if(lock){
		return (*this);
	}
}
const RatioNumber& RatioNumber::operator = (const char16_t& ch){
	if(lock){
		return (*this);
	}
	WIDE_CHAR_PASS(ch);
	reduce();
	return (*this);
}
const RatioNumber& RatioNumber::operator = (char16_t* const n){
	if(lock){
		return (*this);
	}
	WIDE_CHARARRAY_PASS(n);
	reduce();
	return (*this);
}
const RatioNumber& RatioNumber::operator = (const char16_t* const n){
	if(lock){
		return (*this);
	}
	WIDE_CHARARRAY_PASS(n);
	reduce();
	return (*this);
}
const RatioNumber& RatioNumber::operator = (const char32_t& ch){
	if(lock){
		return (*this);
	}
	WIDE_CHAR_PASS(ch);
	reduce();
	return (*this);
}
const RatioNumber& RatioNumber::operator = (char32_t* const n){
	if(lock){
		return (*this);
	}
	WIDE_CHARARRAY_PASS(n);
	reduce();
	return (*this);
}
const RatioNumber& RatioNumber::operator = (const char32_t* const n){
	if(lock){
		return (*this);
	}
	WIDE_CHARARRAY_PASS(n);
	reduce();
	return (*this);
}
template <typename T>
typename enable_if<is_floating_point<T>::value, const RatioNumber&>::type RatioNumber::operator = (T d){
	if(lock){
		return (*this);
	}
	if(is_same<T, float>::value){
		PASS_BY_IEEE(d, 4, 8);
	}else if(is_same<T, double>::value){
		PASS_BY_IEEE(d, 8, 11);
	}else if(is_same<T, long double>::value){
		if(ld_type == UNKNOWN_LD){
			judge_ldtype();
		}
		if(ld_type == is_IEEE){
			PASS_BY_IEEE(d, ld_byte, ld_exp);
		}else{
			PASS_BY_80_bits(d);
		}
	}
}
template <typename T>
typename enable_if<is_integral<T>::value, const RatioNumber&>::type RatioNumber::operator = (T n){
	if(lock){
		return (*this);
	}
	numerator = n;
	denominator = 1;
	if(!numerator.get_positive()){
		numerator.negate();
		positive = false;
	}else{
		positive = true;
	}
}
template <typename T>
const RatioNumber& RatioNumber::operator = (T* const n){
	if(lock){
		return (*this);
	}
	numerator = n;
	denominator = 1;
	positive = true;
}
const RatioNumber RatioNumber::abs() const{
	if(is_NaN()){
		return (*this);
	}
	RatioNumber tmp = (*this);
	if(!tmp.numerator.get_positive()){
		tmp.numerator.negate();
	}
	if(!tmp.denominator.get_positive()){
		tmp.denominator.negate();
	}
	tmp.positive = true;
	return tmp;
}
const RatioNumber RatioNumber::abs_inverse() const{
	if(is_NaN()){
		return (*this);
	}
	RatioNumber tmp = (*this);
	if(!tmp.numerator.get_positive()){
		tmp.numerator.negate();
	}
	if(!tmp.denominator.get_positive()){
		tmp.denominator.negate();
	}
	tmp.positive = false;
	return tmp;
}
const RatioNumber& RatioNumber::operator += (const RatioNumber& r){
	if(lock){
		return (*this);
	}
	add_without_reduction(r);
	reduce();
	return (*this);
}
template <typename T>
const RatioNumber& RatioNumber::operator += (const T& n){
	RatioNumber temp = n;
	(*this) += temp;
	return (*this);
}
const RatioNumber RatioNumber::operator + (const RatioNumber& r) const{
	RatioNumber tmp = (*this);
	tmp += r;
	return tmp;
}
template<typename T>
const RatioNumber RatioNumber::operator + (const T& n) const{
	RatioNumber temp = (*this), tmp = n;
	temp += tmp;
	return temp;
}
void RatioNumber::add_without_reduction(RatioNumber r){
	if(lock){
		return;
	}
	if(is_NaN() || r.is_NaN()){
		positive = true;
		denominator = 0;
		numerator = 0;
		return;
	}else if(is_INF() || r.is_INF()){
		if(is_positive_INF() && r.is_positive_INF()){
			return;
		}else if(is_negative_INF() && r.is_negative_INF()){
			return;
		}else{
			positive = true;
			denominator = 0;
			numerator = 0;
			return;
		}
	}
	bnint Lcm = lcm(denominator, r.denominator);
	if(!positive){
		numerator.negate();
		positive = true;
	}
	numerator *= Lcm / denominator;
	bnint be_add = r.numerator * (Lcm / r.denominator);
	if(!r.positive){
		be_add.negate();
	}
	numerator += be_add;
	denominator = Lcm;
}
const RatioNumber& RatioNumber::operator -= (const RatioNumber& r){
	if(lock){
		return (*this);
	}
	minus_without_reduction(r);
	reduce();
	return (*this);
}
template <typename T>
const RatioNumber& RatioNumber::operator -= (const T& n){
	RatioNumber temp = n;
	(*this) -= temp;
	return (*this);
}
const RatioNumber RatioNumber::operator - (const RatioNumber& r) const{
	RatioNumber tmp = (*this);
	tmp -= r;
	return tmp;
}
template<typename T>
const RatioNumber RatioNumber::operator - (const T& n) const{
	RatioNumber temp = (*this), tmp = n;
	temp -= tmp;
	return temp;
}
void RatioNumber::minus_without_reduction(RatioNumber r){
	if(lock){
		return;
	}
	if(is_NaN() || r.is_NaN()){
		positive = true;
		denominator = 0;
		numerator = 0;
		return;
	}else if(is_INF() || r.is_INF()){
		if(is_positive_INF() && r.is_negative_INF()){
			return;
		}else if(is_negative_INF() && r.is_positive_INF()){
			return;
		}else{
			positive = true;
			denominator = 0;
			numerator = 0;
			return;
		}
	}
	bnint Lcm = lcm(denominator, r.denominator);
	if(!positive){
		numerator.negate();
		positive = true;
	}
	numerator *= Lcm / denominator;
	bnint be_add = r.numerator * (Lcm / r.denominator);
	if(!r.positive){
		be_add.negate();
	}
	numerator -= be_add;
	denominator = Lcm;
}
const RatioNumber& RatioNumber::operator *= (const RatioNumber& r){
	if(lock){
		return (*this);
	}
	multiply_without_reduction(r);
	reduce();
}
template <typename T>
const RatioNumber& RatioNumber::operator *= (const T& n){
	RatioNumber temp = n;
	(*this) *= temp;
	return (*this);
}
const RatioNumber RatioNumber::operator * (const RatioNumber& r) const{
	RatioNumber tmp = (*this);
	tmp *= r;
	return tmp;
}
template<typename T>
const RatioNumber RatioNumber::operator * (const T& n) const{
	RatioNumber temp = (*this), tmp = n;
	temp *= tmp;
	return temp;
}
void RatioNumber::multiply_without_reduction(RatioNumber r){
	if(lock){
		return;
	}
	numerator *= r.numerator;
	denominator *= r.denominator;
	if(!r.positive){
		positive = !positive;
	}
}
const RatioNumber& RatioNumber::operator /= (const RatioNumber& r){
	if(lock){
		return (*this);
	}
	divide_without_reduction(r);
	reduce();
}
template <typename T>
const RatioNumber& RatioNumber::operator /= (const T& n){
	RatioNumber temp = n;
	(*this) /= temp;
	return (*this);
}
const RatioNumber RatioNumber::operator / (const RatioNumber& r) const{
	RatioNumber tmp = (*this);
	tmp /= r;
	return tmp;
}
template<typename T>
const RatioNumber RatioNumber::operator / (const T& n) const{
	RatioNumber temp = (*this), tmp = n;
	temp /= tmp;
	return temp;
}
void RatioNumber::divide_without_reduction(RatioNumber r){
	if(lock){
		return;
	}
	numerator *= r.denominator;
	denominator *= r.numerator;
	if(!r.positive){
		positive = !positive;
	}
}
const RatioNumber& RatioNumber::operator ^= (const bnint& n){
	if(lock){
		return (*this);
	}
	if(n.is_zero()){
		numerator = 1;
		denominator = 1;
		positive = true;
		return (*this);
	}else if(is_NaN()){
		return (*this);
	}else if(is_INF()){
		if(n < 0){
			numerator = 0;
			denominator = 1;
			positive = true;
			return (*this);
		}
		if(is_positive_INF()){
			return (*this);
		}
		if(n % 2 != 0){
			return (*this);
		}else{
			positive = true;
			return (*this);
		}
	}else if(numerator.is_zero()){
		return (*this);
	}
	bnint times = n;
	if(times < 0){
		bnint tmp = numerator;
		numerator = denominator;
		denominator = tmp;
		times.negate();
	}
	if(!positive){
		numerator *= -1;
		positive = true;
	}
	numerator = numerator ^ times;
	denominator = denominator ^ times;
	reduce();
	return (*this);
}
const RatioNumber RatioNumber::operator ^ (const bnint& n) const{
	RatioNumber temp = (*this);
	temp ^= n;
	return temp;
}

void RatioNumber::print(){
	correct_positive();
	cout << (*this);
}

void RatioNumber::print() const{
	cout << (*this);
}

string RatioNumber::str() const{
	stringstream ss;
	ss << (*this);
	return ss.str();
}

RatioNumber RatioNumber::reciprocal() const{
	RatioNumber tmp(denominator, numerator);
	if(!positive){
		tmp.positive = !tmp.positive;
	}
	return tmp;
}

ostream& operator << (ostream& os, const RatioNumber& r){
	if(r.is_positive_INF()){
		os << "Inf";
	}else if(r.is_negative_INF()){
		os << "-Inf";
	}else if(r.is_NaN()){
		os << "NaN";
	}else{
		int n = 0;
		if(!r.positive){
			n++;
		}
		if(r.numerator.is_zero()){
			n = 0;
		}else{
			if(!r.numerator.get_positive()){
				n++;
			}
			if(!r.denominator.get_positive()){
				n++;
			}
		}
		bnint base = 10;
		bnint num = r.numerator;
		int precision = 0;
		if(RatioNumber::precision >= 0){
			precision = RatioNumber::precision;
			num *= base ^ (RatioNumber::precision + 1);
		}else{
			static int space = 1000; //to optimize the searching process
			for(int i = space;i < RatioNumber::endure_precision;i += space){
				num *= base ^ space;
				if((num % r.denominator).is_zero()){
					precision = i - space;
					break;
				}
			}
			num = r.numerator * (base ^ precision);
			while(precision < RatioNumber::endure_precision){
				if((num % r.denominator).is_zero()){
					num *= base;
					break;
				}
				num *= base;
				precision ++;
			}
			if(precision == RatioNumber::endure_precision){
				num *= base;
			}
		}
		num /= r.denominator;
		auto wait = num.getDigit(1);
		if(wait > 0 && RatioNumber::mode == 1){
			if(r.positive){
				num += base;
			}
		}else if(wait > 0 && RatioNumber::mode == -1){
			if(!r.positive){
				num += base;
			}
		}else if(wait >= 5){
			if(wait > 5){
				num += base;
			}else{
				if(r.positive){
					num += base;
				}
			}
		}
		num /= base;
		if(n % 2 == 1){
			os << '-';
		}
		if(precision == 0){
			os << num;
		}else{
			base = base ^ precision;
			os << abs(num / base);
			bnint rem = num % base;
			if(RatioNumber::precision > 0 || !rem.is_zero()){
				os << '.';
				string str = rem.abs().str();
				while(str.length() != precision){
					str.insert(0, 1, '0');
				}
				os << str;
			}
		}
	}
	return os;
}

void print(const RatioNumber& r){
	r.print();
}

bool RatioNumber::fast_start(string str, RatioNumber& sol, int sol_precis, int process_precis, int round){
	if(round < -1){
		round = 0;
	}else if(round > 1){
		round = 0;
	}
	if(process_precis < sol_precis){
		process_precis = sol_precis;
	}
	if(sol.lock){
		return false;
	}
	for(int i = 0;i < fast_thread.size();i++){
		if(get<NAME>(fast_thread[i]) == str){
			return false;
		}
	}
	int ori_pre = RatioNumber::precision, ori_mode = RatioNumber::mode;
	RatioNumber::precision = process_precis, RatioNumber::mode = round;
	now_thread = fast_thread.size();
	vector<bnint> vec;
	Package_thread tmp(str, sol, sol.str(), sol_precis, process_precis, round, vec);
	fast_thread.push_back(tmp);
	sol.lock = true;
	RatioNumber::precision = ori_pre, RatioNumber::mode = ori_mode;
	return true;
}
bool RatioNumber::fast_switch(string str){
	for(int i = 0;i < fast_thread.size();i++){
		if(get<NAME>(fast_thread[i]) == str){
			now_thread = i;
			return true;
		}
	}
	return false;
}
bool RatioNumber::fast_switch(const RatioNumber& sol){
	if(!sol.lock){
		return false;
	}
	for(int i = 0;i < fast_thread.size();i++){
		if(&get<SOLUTION>(fast_thread[i]) == &sol){
			now_thread = i;
			return true;
		}
	}
	return false;
}
void RatioNumber::fast_add(const RatioNumber& r){
	if(now_thread < 0 || now_thread >= fast_thread.size()){
		return;
	}
	int ori_pre = RatioNumber::precision, ori_mode = RatioNumber::mode;
	RatioNumber::precision = get<PRO_PRECISION>(fast_thread[now_thread]);
	RatioNumber::mode = get<ROUND_MODE>(fast_thread[now_thread]);
	bnint tmp = r.str();
	if(tmp.is_zero()){
		fast_zero_signal = true;
	}else{
		get<PUT_IN>(fast_thread[now_thread]) += tmp;
	}
	RatioNumber::precision = ori_pre, RatioNumber::mode = ori_mode;
}
void RatioNumber::fast_add(const bnint& num, const bnint& den){
	RatioNumber tmp(1, 1);
	tmp.numerator = num;
	tmp.denominator = den;
	fast_add(tmp);
}
void RatioNumber::fast_minus(const RatioNumber& r){
	RatioNumber tmp(1, 1);
	tmp.numerator = r.numerator;
	tmp.denominator = r.denominator;
	tmp.positive = !r.positive;
	fast_add(tmp);
}
void RatioNumber::fast_minus(const bnint& num, const bnint& den){
	RatioNumber tmp(1, 1);
	tmp.numerator = num;
	tmp.denominator = den;
	tmp.positive = false;
	fast_add(tmp);
}
void RatioNumber::fast_putin_temp(){
	if(now_thread < 0 || now_thread >= fast_thread.size()){
		return;
	}
	get<MIDWAY_BN_VECTOR>(fast_thread[now_thread]).push_back(get<PUT_IN>(fast_thread[now_thread]));
}
bool RatioNumber::fast_end(){
	if(now_thread < 0 || now_thread >= fast_thread.size()){
		return false;
	}
	bnint& sol = get<PUT_IN>(fast_thread[now_thread]);
	RatioNumber& ratio_sol = get<SOLUTION>(fast_thread[now_thread]);
	vector<bnint>& midway = get<MIDWAY_BN_VECTOR>(fast_thread[now_thread]);
	int sol_precis = get<SOL_PRECISION>(fast_thread[now_thread]);
	int process_precis = get<PRO_PRECISION>(fast_thread[now_thread]);
	int rounding_mode = get<ROUND_MODE>(fast_thread[now_thread]);
	ratio_sol.lock = false;
	for(int i = 0;i < midway.size();i++){
		sol += midway[i];
	}
	sol /= midway.size() + 1;
	if(process_precis != sol_precis){
		bnint base = 10_b ^ (process_precis - sol_precis - 1);
		sol /= base;
		if(rounding_mode == 1 && sol.get_positive() && sol.getDigit(1) > 0){
			sol += 10;
		}else if(rounding_mode == -1 && !sol.get_positive() && sol.getDigit(1) > 0){
			sol += 10;
		}else if(rounding_mode == 0 && sol.getDigit(1) > 5){
			if(sol.get_positive()){
				sol += 10;
			}else{
				sol -= 10;
			}
		}else if(rounding_mode == 0 && sol.get_positive() && sol.getDigit(1) == 5){
			sol += 10;
		}
		sol /= 10;
	}
	string str = sol.str();
	bool remain = false;
	if(str[0] == '-'){
		str.erase(0, 1);
		remain = true;
	}
	while(str.length() < sol_precis + 1){
		str.insert(0, "0");
	}
	if(remain){
		str.insert(0, "-");
	}
	str.insert(str.length() - sol_precis, ".");
	ratio_sol = str;
	fast_thread.erase(fast_thread.begin() + now_thread);
	now_thread = fast_thread.size() - 1;
}
bool RatioNumber::fast_end(string str){
	string temp = get<NAME>(fast_thread[now_thread]);
	if(fast_switch(str)){
		fast_end();
		fast_switch(temp);
		return true;
	}
	return false;
}
bool RatioNumber::fast_end(RatioNumber& sol){
	if(!sol.lock){
		return false;
	}
	RatioNumber& temp = get<SOLUTION>(fast_thread[now_thread]);
	if(fast_switch(sol)){
		fast_end();
		fast_switch(temp);
		return true;
	}
	return false;
}

const RatioNumber operator "" _n(const char literal){
	RatioNumber temp = literal;
	return temp;
}
const RatioNumber operator "" _n(const char* literal_string){
	RatioNumber temp = literal_string;
	return temp;
}
const RatioNumber operator "" _n(const char* string_values, size_t num_chars){
	RatioNumber temp = string_values;
	return temp;
}
const RatioNumber operator "" _n(const wchar_t literal){
	RatioNumber temp = literal;
	return temp;
}
const RatioNumber operator "" _n(const wchar_t* string_values, size_t num_chars){
	RatioNumber temp = string_values;
	return temp;
}
const RatioNumber operator "" _n(const char16_t literal){
	RatioNumber temp = literal;
	return temp;
}
const RatioNumber operator "" _n(const char16_t* string_values, size_t num_chars){
	RatioNumber temp = string_values;
	return temp;
}
const RatioNumber operator "" _n(const char32_t literal){
	RatioNumber temp = literal;
	return temp;
}
const RatioNumber operator "" _n(const char32_t* string_values, size_t num_chars){
	RatioNumber temp = string_values;
	return temp;
}
const RatioNumber operator "" _numeric(const char literal){
	RatioNumber temp = literal;
	return temp;
}
const RatioNumber operator "" _numeric(const char* literal_string){
	RatioNumber temp = literal_string;
	return temp;
}
const RatioNumber operator "" _numeric(const char* string_values, size_t num_chars){
	RatioNumber temp = string_values;
	return temp;
}
const RatioNumber operator "" _numeric(const wchar_t literal){
	RatioNumber temp = literal;
	return temp;
}
const RatioNumber operator "" _numeric(const wchar_t* string_values, size_t num_chars){
	RatioNumber temp = string_values;
	return temp;
}
const RatioNumber operator "" _numeric(const char16_t literal){
	RatioNumber temp = literal;
	return temp;
}
const RatioNumber operator "" _numeric(const char16_t* string_values, size_t num_chars){
	RatioNumber temp = string_values;
	return temp;
}
const RatioNumber operator "" _numeric(const char32_t literal){
	RatioNumber temp = literal;
	return temp;
}
const RatioNumber operator "" _numeric(const char32_t* string_values, size_t num_chars){
	RatioNumber temp = string_values;
	return temp;
}

RatioNumber sin(const RatioNumber& r, int time){
	RatioNumber sol, sol1;
	sol = sol1 = r;
	const bnint base_n = r.numerator ^ 2, base_d = r.denominator ^ 2;
	bnint base = 1, now_n = r.numerator, now_d = r.denominator;
	if(!r.positive){
		now_n.negate();
	}
	for(int i = 1;i < time;i++){
		RatioNumber added;
		base *= (2 * i + 1) * 2 * i;
		now_n *= base_n;
		now_d *= base_d;
		added.numerator = now_n;
		added.denominator = base * now_d;
		if(i % 2 != 0){
			added.positive = false;
		}
		sol.add_without_reduction(added);
		if(i == time - 2){
			sol1 = sol;
		}
	}
	sol.add_without_reduction(sol1);
	sol.denominator *= 2;
	sol.reduce();
	return sol;
}

RatioNumber arctan(const RatioNumber& r, int time){
	RatioNumber sol, sol1;
	const bnint base_n = r.numerator ^ 2, base_d = r.denominator ^ 2;
	bnint now_n = r.numerator, now_d = r.denominator;
	if(!r.positive){
		now_n.negate();
	}
	for(int i = 0, j = 1;i < time;i++, j += 2, now_n *= base_n, now_d *= base_d){
		RatioNumber added;
		added.numerator = now_n;
		added.denominator = now_d * j;
		if(i % 2 != 0){
			added.positive = false;
		}
		sol.add_without_reduction(added);
		//sol += added;
		if(i == time - 2){
			sol1 = sol;
		}
	}
	sol.add_without_reduction(sol1);
	sol.denominator *= 2;
	sol.reduce();
	return sol;
}

#endif