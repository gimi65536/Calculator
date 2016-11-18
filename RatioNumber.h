#define _DYNAMIC_ 1
#include "BigNumber.h"

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

class RatioNumber{
private:
	static int precision;
	static int endure_precision;
	static int mode;
	bnint numerator;
	bnint denominator;
	bool positive;
	void reduce();
	void reduce() const;
	template <typename T>
	typename enable_if<is_floating_point<T>::value, void>::type PASS_BY_IEEE(T r, int byte, int exp);
	template <typename T>
	typename enable_if<is_floating_point<T>::value, void>::type PASS_BY_80_bits(T ld);
	void common_pass_float(const string& str, RatioNumber& base, int& EXP);
	void PASS_BY_STRING(string str);
	void PASS_BY_STRING_with_notation(string str);
public:
	friend ostream& operator << (ostream& os, const RatioNumber r);
	RatioNumber();
	RatioNumber(const bnint& num, const bnint& den);
	RatioNumber(const RatioNumber& r);
	RatioNumber(float f);
	RatioNumber(double d);
	RatioNumber(long double ld);
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
	const RatioNumber abs() const;
	const RatioNumber& operator += (const RatioNumber& r);
	const RatioNumber operator + (const RatioNumber& r) const;
	const RatioNumber& operator -= (const RatioNumber& r);
	const RatioNumber operator - (const RatioNumber& r) const;
	const RatioNumber& operator *= (const RatioNumber& r);
	const RatioNumber operator * (const RatioNumber& r) const;
	const RatioNumber& operator /= (const RatioNumber& r);
	const RatioNumber operator / (const RatioNumber& r) const;
	const RatioNumber& operator ^= (const bnint& n);
	const RatioNumber operator ^ (const bnint& n) const;
	void print() const;
	const bnint getNumerator() const{return numerator;}
	const bnint gerDenominator() const{return denominator;}
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
};

int RatioNumber::precision = -1;
int RatioNumber::endure_precision = DEFAULT_endure_precision;
int RatioNumber::mode = 0;

bnint gcd(bnint a, bnint b){
	bnint tmp;
	while(a != 0 && b != 0){
		tmp = a % b;
		a = b;
		b = tmp;
	}
	if(b == 0){
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
const bnint gerDenominator(const RatioNumber& r){return r.gerDenominator();}

void Setprecision(){RatioNumber::Setprecision();}
void Setprecision(int n){RatioNumber::Setprecision(n);}
void unSetprecision(){RatioNumber::unSetprecision();}
void Set_autoprecision(int n){RatioNumber::Set_autoprecision();}
void Set_autoprecision(){RatioNumber::Set_autoprecision();}
void unSet_autoprecision(){RatioNumber::unSet_autoprecision();}
void SetRound(){RatioNumber::SetRound();}
void SetRoundDown(){RatioNumber::SetRoundDown();}
void SetRoundUp(){RatioNumber::SetRoundUp();}
void unSetRound(){RatioNumber::unSetRound();}

void RatioNumber::reduce(){
	if(numerator < 0){
		positive = !positive;
		numerator *= -1;
	}
	if(denominator < 0){
		positive = !positive;
		denominator *= -1;
	}
	bnint tmp = gcd(numerator, denominator);
	if(tmp == 0){ //NaN
		positive = true;
		return;
	}else{
		numerator /= tmp;
		denominator /= tmp;
		if(numerator == 0){
			positive = true;
		}
	}
}

void RatioNumber::reduce() const{}

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

RatioNumber::RatioNumber(){
	numerator = 0;
	denominator = 1;
	positive = true;
}

RatioNumber::RatioNumber(const bnint& num, const bnint& den){
	numerator = num;
	denominator = den;
	positive = true;
	reduce();
}

RatioNumber::RatioNumber(const RatioNumber& r){
	(*this) = r;
}

RatioNumber::RatioNumber(float f){
	PASS_BY_IEEE(f, 4, 8);
}

RatioNumber::RatioNumber(double d){
	PASS_BY_IEEE(d, 8, 11);
}

RatioNumber::RatioNumber(long double ld){
	if(ld_type == UNKNOWN_LD){
		judge_ldtype();
	}
	if(ld_type == is_IEEE){
		PASS_BY_IEEE(ld, ld_byte, ld_exp);
	}else{
		PASS_BY_80_bits(ld);
	}
}

RatioNumber::RatioNumber(string str){
	PASS_BY_STRING(str);
}

bool RatioNumber::is_INF() const{
	if(numerator != 0 && denominator == 0){
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
	if(numerator == 0 && denominator == 0){
		return true;
	}
	return false;
}

bool is_NaN(const RatioNumber& r){
	return r.is_NaN();
}

bool RatioNumber::is_fraction() const{
	return !is_INF() && !is_NaN();
}

bool is_fraction(const RatioNumber& r){
	return r.is_fraction();
}

void RatioNumber::to_INF(){
	numerator = 1;
	denominator = 0;
}

void RatioNumber::to_positive_INF(){
	numerator = 1;
	denominator = 0;
	positive = true;
}

RatioNumber positive_INF_generator(){
	RatioNumber tmp(1, 0);
	return tmp;
}

void RatioNumber::to_negative_INF(){
	numerator = 1;
	denominator = 0;
	positive = false;
}

RatioNumber negative_INF_generator(){
	RatioNumber tmp(-1, 0);
	return tmp;
}

void RatioNumber::to_NaN(){
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

const RatioNumber& RatioNumber::operator = (const RatioNumber& r){
	numerator = r.numerator;
	denominator = r.denominator;
	positive = r.positive;
}
const RatioNumber RatioNumber::abs() const{
	if(is_NaN()){
		return (*this);
	}
	RatioNumber tmp = (*this);
	tmp.positive = true;
	return tmp;
}
const RatioNumber& RatioNumber::operator += (const RatioNumber& r){
	if(is_NaN() || r.is_NaN()){
		positive = true;
		denominator = 0;
		numerator = 0;
		return (*this);
	}else if(is_INF() || r.is_INF()){
		if(is_positive_INF() && r.is_positive_INF()){
			return (*this);
		}else if(is_negative_INF() && r.is_negative_INF()){
			return (*this);
		}else{
			positive = true;
			denominator = 0;
			numerator = 0;
			return (*this);
		}
	}
	bnint Lcm = lcm(denominator, r.denominator);
	if(!positive){
		numerator *= -1;
		positive = true;
	}
	numerator *= Lcm / denominator;
	bnint be_add = r.numerator * (Lcm / r.denominator);
	if(!r.positive){
		be_add *= -1;
	}
	numerator += be_add;
	denominator = Lcm;
	reduce();
	return (*this);
}
const RatioNumber RatioNumber::operator + (const RatioNumber& r) const{
	RatioNumber tmp = (*this);
	tmp += r;
	return tmp;
}
const RatioNumber& RatioNumber::operator -= (const RatioNumber& r){
	if(is_NaN() || r.is_NaN()){
		positive = true;
		denominator = 0;
		numerator = 0;
		return (*this);
	}else if(is_INF() || r.is_INF()){
		if(is_positive_INF() && r.is_negative_INF()){
			return (*this);
		}else if(is_negative_INF() && r.is_positive_INF()){
			return (*this);
		}else{
			positive = true;
			denominator = 0;
			numerator = 0;
			return (*this);
		}
	}
	bnint Lcm = lcm(denominator, r.denominator);
	if(!positive){
		numerator *= -1;
		positive = true;
	}
	numerator *= Lcm / denominator;
	bnint be_add = r.numerator * (Lcm / r.denominator);
	if(!r.positive){
		be_add *= -1;
	}
	numerator -= be_add;
	denominator = Lcm;
	reduce();
	return (*this);
}
const RatioNumber RatioNumber::operator - (const RatioNumber& r) const{
	RatioNumber tmp = (*this);
	tmp -= r;
	return tmp;
}
const RatioNumber& RatioNumber::operator *= (const RatioNumber& r){
	numerator *= r.numerator;
	denominator *= r.denominator;
	if(!r.positive){
		positive = !positive;
	}
	reduce();
}
const RatioNumber RatioNumber::operator * (const RatioNumber& r) const{
	RatioNumber tmp = (*this);
	tmp *= r;
	return tmp;
}
const RatioNumber& RatioNumber::operator /= (const RatioNumber& r){
	numerator *= r.denominator;
	denominator *= r.numerator;
	if(!r.positive){
		positive = !positive;
	}
	reduce();
}
const RatioNumber RatioNumber::operator / (const RatioNumber& r) const{
	RatioNumber tmp = (*this);
	tmp /= r;
	return tmp;
}
const RatioNumber& RatioNumber::operator ^= (const bnint& n){
	if(n == 0){
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
	}
	bnint times = n;
	if(times < 0){
		bnint tmp = numerator;
		numerator = denominator;
		denominator = tmp;
		times = times.abs();
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

void RatioNumber::print() const{
	cout << (*this);
}

ostream& operator << (ostream& os, const RatioNumber r){
	if(r.is_positive_INF()){
		os << "Inf";
	}else if(r.is_negative_INF()){
		os << "-Inf";
	}else if(r.is_NaN()){
		os << "NaN";
	}else{
		if(!r.positive){
			os << '-';
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
				if(num % r.denominator == 0){
					precision = i - space;
					break;
				}
			}
			num = r.numerator * (base ^ precision);
			while(precision < RatioNumber::endure_precision){
				if(num % r.denominator == 0){
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
		if(precision == 0){
			os << num;
		}else{
			base = base ^ precision;
			os << num / base;
			bnint rem = num % base;
			if(RatioNumber::precision > 0 || rem != 0){
				os << '.';
				string str = rem.str();
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

#endif