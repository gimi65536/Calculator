#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#ifndef _BIG_NUMBER_DYNAMIC_
#ifndef _BIG_NUMBER_STATIC_
#define _BIG_NUMBER_DYNAMIC_

const int BASIC_SIZE = 4;

class BigNumber{
private:
	static int SIZE;
	mutable int size;
	mutable int* a;
	bool positive;
	void resize() const{
		if(size != SIZE){
			int* temp = new int[SIZE];
			for(int i = 0;i < size && i < SIZE;i++){
				temp[i] = a[i];
			}
			for(int i = size;i < SIZE;i++){
				temp[i] = 0;
			}
			delete[] a;
			a = temp;
			size = SIZE;
		}
	}
	void coresize(const BigNumber& n) const{
		resize();
		n.resize();
	}
	int getSize(){
		for(int i = size - 1;i >= 0;i--){
			if(a[i] != 0){
				return i + 1;
			}
		}
		return 1;
	}
	const BigNumber& PURE_assignment(const BigNumber& n){
		coresize(n);
		for(int i = 0;i < SIZE;i++){
			a[i] = n.a[i];
		}
	}
public:
	const BigNumber& operator = (const BigNumber& n){
		coresize(n);
		PURE_assignment(n);
		positive = n.positive;
		return (*this);
	}
private:
	const BigNumber& PURE_ADD_assignment(const BigNumber& n){
		coresize(n);
		for(int i = 0;i < SIZE;i++){
			a[i] += n.a[i];
			if(a[i] >= 1000000000 && i < SIZE - 1){
				a[i + 1] ++;
				a[i] -= 1000000000;
			}
		}
		if(a[SIZE - 1] >= 1000000000){
			//cout << "Overflow when adding a number!" << endl << "save the last " << 9 * SIZE << " digit only." << endl;
			int temp = SIZE;
			SIZE *= 2;
			coresize(n);
			a[temp - 1] -= 1000000000;
			a[temp] ++;
		}
		return (*this);
	}
	const BigNumber& PURE_MINUS_assignment(const BigNumber& n){
		coresize(n);
		for(int i = 0;i < SIZE;i++){
			a[i] -= n.a[i];
			if(a[i] < 0 && i < SIZE - 1){
				a[i + 1] --;
				a[i] += 1000000000;
			}
		}
		return (*this);
	}
	const BigNumber PURE_PSEUDOMULTIPLE_assignment(){ //pseudomultiple
		resize();
		stringstream ss;
		ss << (*this) << 0;
		string str = ss.str();
		BigNumber temp(str);
		(*this) = temp;
		return (*this);
	}
	const BigNumber PURE_PSEUDODIVIDE_assignment(){ //pseudodivide
		resize();
		stringstream ss;
		ss << (*this);
		string str = ss.str();
		str.pop_back();
		BigNumber temp(str);
		(*this) = temp;
		return (*this);
	}
	void COMMON_DIVIDE(const BigNumber& n) const;
public:
	friend ostream& operator << (ostream& os, const BigNumber& n);
	BigNumber(){
		size = SIZE;
		positive = true;
		a = new int[SIZE];
		for(int i = 0;i < SIZE;i++){
			a[i] = 0;
		}
	}
	BigNumber(string str){
		if(str.length() > 0 && str[0] == '-'){
			positive = false;
			str.erase(0, 1);
		}else{
			positive = true;
		}
		while(str.length() > 0 && str[0] == '0'){
			str.erase(0, 1);
		}
		while(str.length() > 9 * SIZE){
			//cout << "Overflow when creating a number with too more digits!" << endl << "save the last " << 9 * SIZE << " digit only." << endl;
			SIZE *= 2;
		}
		size = SIZE;
		a = new int[SIZE];
		for(int i = 0;i < SIZE;i++){
			a[i] = 0;
		}
		if(str.length() == 0 || str.find_first_of("0123456789") == string::npos){
			return;
		}
		stringstream ss;
		int t = (str.length() - 1) / 9;
		for(int i = 0;i < t;i++){
			string sub = str.substr(str.length() - 9, 9);
			ss << sub;
			ss >> a[i];
			ss.clear();
			ss.str("");
			str = str.erase(str.length() - 9, 9);
		}
		ss << str;
		ss >> a[t];
	}
	BigNumber(int n){
		size = SIZE;
		a = new int[SIZE];
		(*this) = n;
	}
	BigNumber(const BigNumber& n){
		n.resize();
		size = SIZE;
		a = new int[SIZE];
		for(int i = 0;i < SIZE;i++){
			a[i] = n.a[i];
		}
		positive = n.positive;
	}
	~BigNumber(){
		delete[] a;
	}
	size_t digit() const{
		resize();
		stringstream ss;
		ss << (*this);
		string str = ss.str();
		if(str[0] == '-'){
			str.erase(0, 1);
		}
		return str.length();
	}
	int getDigit(int n) const{
		resize();
		if(n <= 0 || n > 9 * SIZE){
			return -1;
		}
		int index = (n - 1) / 9;
		int target = a[index];
		n = (n - 1) % 9;
		for(int i = 1;i <= n;i++){
			target /= 10;
		}
		return target % 10;
	}
	operator int(){
		return a[0] * (positive ? 1 : -1);
	}
	bool operator == (int n) const{
		resize();
		for(int i = SIZE - 1;i > 1;i--){
			if(a[i] != 0){
				return false;
			}
		}
		if(a[1] == 0){
			int temp = a[0];
			if(!positive){temp *= -1;}
			return temp == n;
		}else if(a[1] == 1){
			int temp = 1000000000 + a[0];
			if(!positive){temp *= -1;}
			return temp == n;
		}else if(a[1] == 2){
			if(a[0] <= 147483467){
				int temp = 2000000000 + a[0];
				if(!positive){temp *= -1;}
				return temp == n;
			}else if(a[0] > 147483648){
				return false;
			}else{
				if(positive){return false;}
				else{return n == -2147483648;}
			}
		}else{
			return false;
		}
	}
	bool operator == (const BigNumber& n) const{
		coresize(n);
		bool zero = true;
		for(int i = 0;i < SIZE;i++){
			if(a[i] != n.a[i]){
				return false;
			}
			if(a[i] != 0){
				zero = false;
			}
		}
		return zero || (positive == n.positive); //if 0, don't ask +-
	}
	bool operator != (int n) const{
		return !(*this == n);
	}
	bool operator != (const BigNumber& n) const{
		return !(*this == n);
	}
	bool operator < (const BigNumber& n) const{
		coresize(n);
		bool same_number = true, pure_number_smaller = false, zero = true;
		for(int i = SIZE - 1;i >= 0;i--){
			if(zero && (a[i] != 0 || n.a[i] != 0)){
				zero = false;
			}
			if(a[i] > n.a[i]){
				pure_number_smaller = false;
				same_number = false;
				break;
			}else if(a[i] < n.a[i]){
				pure_number_smaller = true;
				same_number = false;
				break;
			}
		}
		if(!same_number){ //impossibly be 0
			if(positive && !n.positive){return false;}
			else if(!positive && n.positive){return true;}
			else if(positive){return pure_number_smaller;}
			else{return !pure_number_smaller;}
		}else{ //possibly be 0
			if(zero){return false;}
			else if(!positive && n.positive){return true;}
			else{return false;}
		}
	}
	bool operator < (int n) const{
		resize();
		BigNumber temp = n;
		return (*this) < temp;
	}
	bool operator <= (const BigNumber& n) const{
		return (*this) < n || (*this) == n;
	}
	bool operator <= (int n) const{
		return (*this) < n || (*this) == n;
	}
	bool operator > (const BigNumber& n) const{
		coresize(n);
		bool same_number = true, pure_number_larger = false, zero = true;
		for(int i = SIZE - 1;i >= 0;i--){
			if(zero && (a[i] != 0 || n.a[i] != 0)){
				zero = false;
			}
			if(a[i] > n.a[i]){
				pure_number_larger = true;
				same_number = false;
				break;
			}else if(a[i] < n.a[i]){
				pure_number_larger = false;
				same_number = false;
				break;
			}
		}
		if(!same_number){ //impossibly be 0
			if(positive && !n.positive){return true;}
			else if(!positive && n.positive){return false;}
			else if(positive){return pure_number_larger;}
			else{return !pure_number_larger;}
		}else{ //possibly be 0
			if(zero){return false;}
			else if(positive && !n.positive){return true;}
			else{return false;}
		}
	}
	bool operator > (int n) const{
		resize();
		BigNumber temp = n;
		return (*this) > temp;
	}
	bool operator >= (const BigNumber& n) const{
		return (*this) > n || (*this) == n;
	}
	bool operator >= (int n) const{
		return (*this) > n || (*this) == n;
	}
	const BigNumber operator + () const{
		resize();
		return (*this);
	}
	const BigNumber operator - () const{
		resize();
		BigNumber temp = (*this);
		temp.positive = !temp.positive;
		return temp;
	}
	const BigNumber& operator = (int n){
		resize();
		for(int i = 1;i < SIZE;i++){
			a[i] = 0;
		}
		if(n < 0){a[0] = -n;}
		else{a[0] = n;}
		while(a[0] >= 1000000000){
			a[0] -= 1000000000;
			a[1] ++;
		}
		positive = (n >= 0);
		return (*this);
	}
	const BigNumber abs() const{
		resize();
		BigNumber temp = (*this);
		temp.positive = true;
		return temp;
	}
	const BigNumber& operator += (const BigNumber& n){
		coresize(n);
		if(positive == n.positive){ //straightly add
			PURE_ADD_assignment(n);
		}else if(abs() >= n.abs()){
			PURE_MINUS_assignment(n);
		}else{
			const BigNumber temp = (*this);
			(*this) = n;
			PURE_MINUS_assignment(temp);
		}
		return (*this);
	}
	const BigNumber& operator += (int temp){
		resize();
		BigNumber n(temp);
		(*this) += n;
		return (*this);
	}
	const BigNumber& operator ++ (){
		(*this) += 1;
		return (*this);
	}
	const BigNumber operator ++ (int null){
		BigNumber tmp = (*this);
		++(*this);
		return tmp;
	}
	const BigNumber operator + (const BigNumber& n) const{
		resize();
		BigNumber temp = (*this);
		temp += n;
		return temp;
	}
	const BigNumber& operator -= (const BigNumber& n){
		coresize(n);
		if(positive != n.positive){ //straightly add
			PURE_ADD_assignment(n);
		}else if(abs() >= n.abs()){
			PURE_MINUS_assignment(n);
		}else{
			const BigNumber temp = (*this);
			(*this) = -n;
			PURE_MINUS_assignment(temp);
		}
		return (*this);
	}
	const BigNumber& operator -= (int temp){
		resize();
		BigNumber n(temp);
		(*this) -= n;
		return (*this);
	}
	const BigNumber& operator -- (){
		(*this) -= 1;
		return (*this);
	}
	const BigNumber operator -- (int null){
		BigNumber tmp = (*this);
		--(*this);
		return tmp;
	}
	const BigNumber operator - (const BigNumber& n) const{
		resize();
		BigNumber temp = (*this);
		temp -= n;
		return temp;
	}
	const BigNumber& operator *= (const BigNumber& n);
	const BigNumber operator * (const BigNumber& n) const;
	const BigNumber& operator /= (const BigNumber& n);
	const BigNumber operator / (const BigNumber& n) const;
	const BigNumber& operator %= (const BigNumber& n);
	const BigNumber operator % (const BigNumber& n) const;
	friend void var_resize();
};

int BigNumber::SIZE = BASIC_SIZE;

BigNumber HI(0), LO(0);
static BigNumber be_divided(0);
static BigNumber divide(0);

const BigNumber& BigNumber::operator *= (const BigNumber& n){
	coresize(n);
	be_divided = 0;
	divide = 0;
	bool solution_positive = (positive == n.positive);
	if(digit() + n.digit() > 9 * SIZE){
		SIZE *= 2;
		coresize(n);
	}
	BigNumber base = (*this);
	(*this) = 0;
	int t = n.digit();
	for(int i = 2;i <= t;i++){
		base.PURE_PSEUDOMULTIPLE_assignment();
	}
	for(int i = t;i >= 1;i--){
		int time = n.getDigit(i);
		for(int j = 1;j <= time;j++){
			PURE_ADD_assignment(base);
		}
		base.PURE_PSEUDODIVIDE_assignment();
	}
		positive = solution_positive;
	return (*this);
}
const BigNumber BigNumber::operator * (const BigNumber& n) const{
	resize();
	BigNumber temp = (*this);
	temp *= n;
	return temp;
}
void BigNumber::COMMON_DIVIDE(const BigNumber& n) const{
	be_divided = (*this);
	divide = n;
	BigNumber module = (*this), minus = n, quo(0), plus(1);
	bool solution_positive = (positive == n.positive);
	int t = digit() - n.digit();
	if(t < 0){
		HI = (*this);
		LO = 0;
		return;
	}
	for(int i = 1;i <= t;i++){
		minus.PURE_PSEUDOMULTIPLE_assignment();
		plus.PURE_PSEUDOMULTIPLE_assignment();
	}
	for(int i = t;i >= 0;i--){
		while(module.abs() >= minus.abs()){
			module.PURE_MINUS_assignment(minus);
			quo.PURE_ADD_assignment(plus);
		}
		minus.PURE_PSEUDODIVIDE_assignment();
		plus.PURE_PSEUDODIVIDE_assignment();
	}
	quo.positive = solution_positive;
	HI = module;
	LO = quo;
}
const BigNumber& BigNumber::operator /= (const BigNumber& n){
	coresize(n);
	if(divide == n && be_divided == (*this)){
		(*this) = LO;
		return (*this);
	}
	COMMON_DIVIDE(n);
	(*this) = LO;
	return (*this);
}
const BigNumber BigNumber::operator / (const BigNumber& n) const{
	resize();
	BigNumber temp = (*this);
	temp /= n;
	return temp;
}
const BigNumber& BigNumber::operator %= (const BigNumber& n){
	coresize(n);
	if(divide == n && be_divided == (*this)){
		(*this) = HI;
		return (*this);
	}
	COMMON_DIVIDE(n);
	(*this) = HI;
	return (*this);
}
const BigNumber BigNumber::operator % (const BigNumber& n) const{
	resize();
	BigNumber temp = (*this);
	temp %= n;
	return temp;
}

ostream& operator << (ostream& os, const BigNumber& n){
	n.resize();
	for(int i = BigNumber::SIZE - 1;i >= 0;i--){
		if(n.a[i] != 0){
			if(!n.positive){
				os << '-';
			}
			os << n.a[i];
			for(int j = i - 1;j >= 0;j--){
				os.width(9);
				os.fill('0');
				os << n.a[j];
			}
			break;
		}else if(i == 0){
			os << 0;
		}
	}
	return os;
}

istream& operator >> (istream& is, BigNumber& n){
	n.resize();
	string str;
	is >> str;
	BigNumber temp(str);
	n = temp;
	return is;
}

#endif
#endif