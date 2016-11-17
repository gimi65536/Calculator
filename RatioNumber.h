#define _DYNAMIC_ 1
#include "BigNumber.h"

#ifndef _RATIO_NUMBER_
#define _RATIO_NUMBER_

bnint gcd(bnint a, bnint b);
bnint lcm(bnint a, bnint b);
const int DEFAULT_endure_precision = 100;

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
public:
	friend ostream& operator << (ostream& os, const RatioNumber r);
	RatioNumber();
	RatioNumber(const bnint& num, const bnint& den);
	RatioNumber(const RatioNumber& r);
	bool is_INF() const;
	bool is_positive_INF() const;
	bool is_negative_INF() const;
	bool is_NaN() const;
	bool is_fraction() const;
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

bool RatioNumber::is_INF() const{
	if(numerator != 0 && denominator == 0){
		return true;
	}
	return false;
}

bool RatioNumber::is_positive_INF() const{
	if(is_INF() && positive){
		return true;
	}
	return false;
}

bool RatioNumber::is_negative_INF() const{
	if(is_INF() && !positive){
		return true;
	}
	return false;
}

bool RatioNumber::is_NaN() const{
	if(numerator == 0 && denominator == 0){
		return true;
	}
	return false;
}

bool RatioNumber::is_fraction() const{
	return !is_INF() && !is_NaN();
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
}/*
const RatioNumber& RatioNumber::operator *= (const RatioNumber& r);
const RatioNumber RatioNumber::operator * (const RatioNumber& r) const;
const RatioNumber& RatioNumber::operator /= (const RatioNumber& r);
const RatioNumber RatioNumber::operator / (const RatioNumber& r) const;*/

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