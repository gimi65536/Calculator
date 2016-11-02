#include <iostream>
#include <cassert>
#include <stack>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

const int BASIC_SIZE = 4;
class BigNumber;
class BigBigNumber;

class BigNumber{
friend class BigBigNumber;
private:
	static const int SIZE = BASIC_SIZE;
	int a[SIZE];
	bool positive;
	const BigNumber& PURE_assignment(const BigNumber& n){
		for(int i = 0;i < SIZE;i++){
			a[i] = n.a[i];
		}
	}
public:
	const BigNumber& operator = (const BigNumber& n){
		PURE_assignment(n);
		positive = n.positive;
		return (*this);
	}
private:
	const BigNumber& PURE_ADD_assignment(const BigNumber& n){
		for(int i = 0;i < SIZE;i++){
			a[i] += n.a[i];
			if(a[i] >= 1000000000 && i < SIZE - 1){
				a[i + 1] ++;
				a[i] -= 1000000000;
			}
		}
		if(a[SIZE - 1] >= 1000000000){
			//OVERFLOW
		}
		return (*this);
	}
	const BigNumber& PURE_MINUS_assignment(const BigNumber& n){
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
		stringstream ss;
		ss << (*this) << 0;
		string str = ss.str();
		BigNumber temp(str);
		(*this) = temp;
		return (*this);
	}
	const BigNumber PURE_PSEUDODIVIDE_assignment(){ //pseudodivide
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
		positive = true;
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
		assert(str.length() <= 9 * SIZE);
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
		(*this) = n;
	}
	BigNumber(const BigNumber& n){
		for(int i = 0;i < SIZE;i++){
			a[i] = n.a[i];
		}
		positive = n.positive;
	}
	size_t digit() const{
		stringstream ss;
		ss << (*this);
		string str = ss.str();
		if(str[0] == '-'){
			str.erase(0, 1);
		}
		return str.length();
	}
	int getDigit(int n) const{
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
		}
	}
	bool operator == (const BigNumber& n) const{
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
	bool operator <= (const BigNumber& n) const{
		return (*this) < n || (*this) == n;
	}
	bool operator > (const BigNumber& n) const{
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
	bool operator >= (const BigNumber& n) const{
		return (*this) > n || (*this) == n;
	}
	const BigNumber operator + () const{
		return (*this);
	}
	const BigNumber operator - () const{
		BigNumber temp = (*this);
		temp.positive = !temp.positive;
		return temp;
	}
	const BigNumber& operator = (const int& n){
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
		BigNumber temp = (*this);
		temp.positive = true;
		return temp;
	}
	const BigNumber& operator += (const BigNumber& n){
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
	const BigNumber operator + (const BigNumber& n) const{
		BigNumber temp = (*this);
		temp += n;
		return temp;
	}
	const BigNumber& operator -= (const BigNumber& n){
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
	const BigNumber operator - (const BigNumber& n) const{
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
};

BigNumber HI(0), LO(0); //using for mult and div

class BigBigNumber{ //only use in overflow, user cannot access it directly
friend class BigNumber;
private:
	static const int SIZE = 2 * BASIC_SIZE;
	int a[SIZE];
	bool positive;
	friend ostream& operator << (ostream& os, const BigBigNumber& n);
	BigBigNumber(string str){
		if(str[0] == '-'){
			positive = false;
			str.erase(0, 1);
		}else{
			positive = true;
		}
		assert(str.length() <= 9 * SIZE);
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
	BigBigNumber(const BigNumber& n){
		for(int i = 0;i < SIZE / 2;i++){
			a[i] = n.a[i];
		}
		for(int i = SIZE / 2;i < SIZE;i++){
			a[i] = 0;
		}
		positive = n.positive;
	}
	BigBigNumber(const BigBigNumber& n){
		for(int i = 0;i < SIZE;i++){
			a[i] = n.a[i];
		}
		positive = n.positive;
	}
	const BigBigNumber& PURE_assignment(const BigBigNumber& n){
		for(int i = 0;i < SIZE;i++){
			a[i] = n.a[i];
		}
	}
	const BigBigNumber& operator = (const BigBigNumber& n){
		PURE_assignment(n);
		positive = n.positive;
		return (*this);
	}
	const BigBigNumber& operator = (const int& n){
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
	const BigBigNumber& PURE_ADD_assignment(const BigBigNumber& n){
		for(int i = 0;i < SIZE;i++){
			a[i] += n.a[i];
			if(a[i] >= 1000000000 && i < SIZE - 1){
				a[i + 1] ++;
				a[i] -= 1000000000;
			}
		}
		return (*this);
	}
	const BigBigNumber PURE_PSEUDOMULTIPLE_assignment(){ //pseudomultiple
		stringstream ss;
		ss << (*this) << 0;
		string str = ss.str();
		BigBigNumber temp(str);
		(*this) = temp;
		return (*this);
	}
	const BigBigNumber PURE_PSEUDODIVIDE_assignment(){ //pseudodivide
		stringstream ss;
		ss << (*this);
		string str = ss.str();
		str.pop_back();
		BigBigNumber temp(str);
		(*this) = temp;
		return (*this);
	}
	void proliferate(){
		HI.positive = positive;
		LO.positive = positive;
		for(int i = 0;i < SIZE / 2;i++){
			LO.a[i] = a[i];
		}
		for(int i = 0;i < SIZE / 2;i++){
			HI.a[i] = a[BigNumber::SIZE + i];
		}
	}
};

static BigNumber be_divided(0);
static BigNumber divide(0);

const BigNumber& BigNumber::operator *= (const BigNumber& n){
	be_divided = 0;
	divide = 0;
	bool solution_positive = (positive == n.positive);
	if(digit() + n.digit() > 9 * SIZE){
		BigBigNumber base = (*this);
		BigBigNumber sol("0");
		int t = n.digit();
		for(int i = 2;i <= t;i++){
			base.PURE_PSEUDOMULTIPLE_assignment();
		}
		for(int i = t;i >= 1;i--){
			int time = n.getDigit(i);
			for(int j = 1;j <= time;j++){
				sol.PURE_ADD_assignment(base);
			}
			base.PURE_PSEUDODIVIDE_assignment();
		}
		sol.positive = solution_positive;
		sol.proliferate();
		if(HI != 0){} //overflow alerk
		(*this) = LO;
	}else{
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
	}
	return (*this);
}
const BigNumber BigNumber::operator * (const BigNumber& n) const{
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
	if(divide == n && be_divided == (*this)){
		(*this) = LO;
		return (*this);
	}
	COMMON_DIVIDE(n);
	(*this) = LO;
	return (*this);
}
const BigNumber BigNumber::operator / (const BigNumber& n) const{
	BigNumber temp = (*this);
	temp /= n;
	return temp;
}
const BigNumber& BigNumber::operator %= (const BigNumber& n){
	if(divide == n && be_divided == (*this)){
		(*this) = HI;
		return (*this);
	}
	COMMON_DIVIDE(n);
	(*this) = HI;
	return (*this);
}
const BigNumber BigNumber::operator % (const BigNumber& n) const{
	BigNumber temp = (*this);
	temp %= n;
	return temp;
}

ostream& operator << (ostream& os, const BigNumber& n){
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

ostream& operator << (ostream& os, const BigBigNumber& n){
	for(int i = BigBigNumber::SIZE - 1;i >= 0;i--){
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

const string inputOPERATOR = "+-*/%()=";
const string OPERATOR = inputOPERATOR + "~#";
const string variable_namespace = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const string scalar = "1234567890";
const string digit_separator = "\'"; //c++14
const string space = " ";
int put_length = 15; //should > 0

stack<char> operand; //will be used to save operator
stack<BigNumber> number; //will be used to save number
stack<bool> number_is_var;
BigNumber variable[52] = {0};

bool ope = true; //use for unary minus
bool CPPTYPE = true; //whether to use C++ format arithmetic expression

int getPrior(char& n, bool ope = ::ope){
	if(n == '~' || n == '#'){
		return 5;
	}else if(n == '*' || n == '/' || n == '%'){
		return 4;
	}else if(!ope && (n == '+' || n == '-')){
		return 3;
	}else if(n == '='){
		return 2;
	}else if(ope && n == '-'){
		n = '~'; //turn unary negative to '~'
		return 5;
	}else if(ope && n == '+'){
		n = '#'; //turn unary positive to '#'
		return 5;
	}
	return 1; //preserved for ()
}

BigNumber getIndex(const char& n){
	size_t pos = variable_namespace.find(n);
	if(pos != string::npos){
		BigNumber temp = pos;
		return temp;
	}
	BigNumber temp = -1;
	return temp; //error
}

bool left_to_right(const char& n){
	if(n == '='){
		return false;
	}
	return true;
}

void put(char& n, string& postfix){
	int priority = getPrior(n);
	bool ltr = left_to_right(n);
	while(operand.size() > 0){
		int popriorty = getPrior(operand.top(), false);
		if(popriorty > priority || (ltr && popriorty == priority)){
			postfix += operand.top();
			postfix += " ";
			operand.pop();
		}else{
			break;
		}
	}
	operand.push(n);
}

bool is(const char n, const string& str){
	if(str.find(n) != string::npos){
		return true;
	}
	return false;
}

void alert(const int now, const int lef, const int righ, const string& str, const string error){
	int len = str.length();
	for(int i = now - put_length / 2;i < now + (put_length + 1) / 2;i++){
		if(i >= len || i < 0){
			cout << ' ';
		}else{
			cout << str[i];
		}
	}
	cout << endl;
	for(int i = 0;i < put_length;i++){
		if(i >= put_length / 2 - lef && i <= put_length / 2 + righ){
			cout << '^';
		}else{
			cout << ' ';
		}
	}
	cout << endl << error << endl;
}

bool Cppcheack(string str){
	string delim = inputOPERATOR + variable_namespace + scalar + space + digit_separator;
	string& allow = delim, deny = delim;
	if(str.find_first_not_of(delim) != string::npos){
		cout << "You should not input any invalid symbol!" << endl;
		return false;
	} //complete B01
	delim = variable_namespace + scalar + digit_separator;
	size_t pos = str.find(' ');
	while(pos != string::npos){
		if(pos == 0 || pos == str.length() - 1){
			str.erase(pos, 1);
			pos = str.find(' ', pos);
		}else{
			if(!is(str[pos - 1], delim) || !is(str[pos + 1], delim)){
				str.erase(pos, 1);
				pos = str.find(' ', pos);
			}else{
				pos = str.find(' ', pos + 1);
			}
		}
	} //complete B00
	if(str.find(' ') != string::npos){
		alert(str.find(' '), 0, 0, str, "Cannot put space between two numbers or variables!");
		return false;
	} //complete B07
	int len = str.length();
	int count = 0;
	for(int i = 0;i < len;i++){
		if(str[i] == '('){
			count ++;
		}else if(str[i] == ')'){
			if(count <= 0){
				alert(i, 0, 0, str, "Do NOT put right parenthesis when no left parentesis is waiting!");
				return false;
			}
			count --;
		}
	} //complete B05
	if(count != 0){
		cout << "The number of ( does not equal to the number of ) !" << endl;
		return false;
	} //complete B04
	allow = scalar; //reuse
	pos = str.find('\''); //reuse
	while(pos != string::npos){
		if(pos == 0 || pos == len - 1){
			alert(pos, 0, 0, str, "Cannot put quote at beginning or ending position!");
			return false;
		}else if(!is(str[pos - 1], allow) || !is(str[pos + 1], allow)){
			alert(pos, 1, 1, str, "Quote should be between two digits!");
			return false;
		}
		pos = str.find('\'', pos + 1);
	} //complete B09 B10 B11
	if(str[len - 1] == '('){
		alert(len - 1, 0, 0, str, "Do NOT put left parenthesis at ending position!");
		return false;
	} //complete B12
	pos = str.find('\'');
	while(pos != string::npos){
		str.erase(pos, 1);
		pos = str.find('\'');
	}
	len = str.length();
	char prev = str[0], now = 0;
	deny = scalar + variable_namespace;
	for(int i = 1;i < len;i++){
		now = str[i];
		if(prev == ')'){
			if(now == '('){
				alert(i, 1, 0, str, "Cannot put ( or variable right after ) !");
				return false;
			}else if(is(now, deny)){
				alert(i, 1, 0, str, "Cannot put number or variable right after ) !");
				return false;
			} //complete B03 B08
		}else if(is(prev, scalar)){
			if(now == '('){
				alert(i, 1, 0, str, "Cannot put ( right after number !");
				return false;
			}else if(is(now, variable_namespace)){
				alert(i, 1, 0, str, "Cannot put variable right after number !");
				return false;
			}
		}else if(is(prev, variable_namespace)){
			if(now == '('){
				alert(i, 1, 0, str, "Cannot put ( right after variable !");
				return false;
			}else if(is(now, variable_namespace)){
				alert(i, 1, 0, str, "Cannot put number or variable right after variable !");
				return false;
			} //complete B02 B06
		}
		prev = now;
	}
	return true;
}

bool check(const string& String){
	string str = String, delim = inputOPERATOR + variable_namespace + scalar + space;
	string& allow = delim, deny = delim;
	size_t pos = str.find_first_not_of(delim);
	while(pos != string::npos){
		str.erase(pos, 1);
		pos = str.find_first_not_of(delim);
	} //A01 complete
	delim = variable_namespace + scalar; //reuse
	pos = str.find(' '); //reuse
	while(pos != string::npos){
		if(pos == 0 || pos == str.length() - 1){
			str.erase(pos, 1);
			pos = str.find(' ', pos);
		}else{
			if(!is(str[pos - 1], delim) || !is(str[pos + 1], delim)){
				str.erase(pos, 1);
				pos = str.find(' ', pos);
			}else{
				pos = str.find(' ', pos + 1);
			}
		}
	} //A00 complete
	if(str.length() == 0){
		cout << "You should input any valid symbol!" << endl;
		return false;
	} //A12 complete
	allow = variable_namespace + scalar + "+-("; //reuse
	int len = str.length();
	if(!is(str[0], allow)){
		alert(0, 0, 0, str, "Should not put this at this position!");
		return false;
	} //A02 complete
	char prev = str[0], now = 0;
	for(int i = 1;i < len;i++){
		now = str[i];
		if(is(prev, inputOPERATOR) && is(now, inputOPERATOR)){
			if(prev == '+' || prev == '-'){
				if(now == '*' || now == '/' || now == '%' || now == '=' || now == ')'){
					alert(i, 1, 0, str, "Cannot put \"*/%)=\" right after operator \"+-\" !");
					return false;
				} //complete A04
			}else if(prev == '*' || prev == '/' || prev == '%'){
				if(now == '*' || now == '/' || now == '%' || now == '=' || now == ')'){
					alert(i, 1, 0, str, "Cannot put \"*/%)=\" right after operator \"*/%\" !");
					return false;
				} //complete A03
			}else if(prev == '('){
				if(now == '*' || now == '/' || now == '%' || now == ')' || now == '='){
					alert(i, 1, 0, str, "Cannot put \"*/%)=\" right after operator \"(\" !");
					return false;
				} //complete A05 A06
			}else if(prev == ')'){
				if(now == '='){
					alert(i, 1, 0, str, "Cannot put \"=\" right after operator \")\" !");
					return false;
				}
			}else if(prev == '='){
				if(now == '*' || now == '/' || now == '%' || now == ')' || now == '='){
					alert(i, 1, 0, str, "Cannot put \"*/%)=\" right after operator \"=\" !");
					return false;
				} //complete A07 A08 A09 A14
			}
		}
		prev = now;
	}
	if(now == '+' || now == '-' || now == '*' || now == '/' || now == '%' || now == '='){
		alert(len - 1, 0, 0, str, "Should not put this at this position!");
		return false;
	} //complete A13
	pos = str.find_last_of('='); //reuse
	if(pos != string::npos){
		deny = "+-*/%)";
		for(int i = 0;i < pos;i++){
			if(is(str[i], deny)){
				alert(i, 0, 0, str, "This operator should NOT appear BEFORE any assignment!");
				return false;
			}else if(is(str[i], scalar)){
				alert(i, 0, 0, str, "Scalar should NOT appear BEFORE any assignment!");
				return false;
			}
		} //complete A11
		pos = str.find_first_of('=');
		string substr = str.substr(0, pos);
		if(substr.find_first_of(variable_namespace) != substr.find_last_of(variable_namespace)){
			alert(pos, 0, 0, str, "Before this assignment, ONLY 1 VARIABLE is allowed!");
			return false;
		}
		size_t apos = str.find_first_of('=', pos + 1);
		while(apos != string::npos){
			substr = str.substr(pos, apos - pos - 1);
			if(substr.find_first_of(variable_namespace) != substr.find_last_of(variable_namespace)){
				alert(apos, 0, 0, str, "Before this assignment, ONLY 1 VARIABLE is allowed!");
				return false;
			}
			pos = apos;
			apos = str.find_first_of('=', pos + 1);
		} //complete A10
	}
	return !CPPTYPE || Cppcheack(String);
}

int main(){
	ifstream fin("setup.ini");
	fin >> CPPTYPE >> put_length;
	fin.close();
	string str, buf, postfix, delim = inputOPERATOR + variable_namespace + scalar + space;
	stringstream ss;
	while(cout << ">>", getline(cin, str)){
		if(str.length() == 0){
			continue;
		}
		if(!check(str)){
			cout << "Please try to input again..." << endl;
			continue;
		}
		postfix = "";
		ope = true;
		bool error = false;
		size_t pos = str.find_first_not_of(delim);
		while(pos != string::npos){
			str.erase(pos, 1);
			pos = str.find_first_not_of(delim);
		}
		while(number.size() > 0){
			number.pop();
		}
		while(number_is_var.size() > 0){
			number_is_var.pop();
		}
		pos = str.find_first_of(OPERATOR + variable_namespace);
		while(pos != string::npos){
			str.insert(pos + 1, " ");
			str.insert(pos, " ");
			pos = str.find_first_of(OPERATOR + variable_namespace, pos + 2);
		}
		//cout << str << endl;
		ss.clear();
		ss.str("");
		ss << str;
		while(ss >> buf){
			if(buf.find_first_of(OPERATOR) == string::npos){
				if(!ope){ //put * automatically
					char now_char = '*';
					put(now_char, postfix);
				}
				postfix += buf;
				postfix += " ";
				ope = false;
			}else{
				char now_char = buf[0];
				if(now_char == '('){
					if(!ope){ //put * automatically
						char now_char = '*';
						put(now_char, postfix);
					}
					operand.push(now_char);
					ope = true; //to put unary operator
				}else if(now_char == ')'){
					while(operand.size() > 0 && operand.top() != '('){
						postfix += operand.top();
						postfix += " ";
						operand.pop();
					}
					if(operand.size() != 0){
						operand.pop(); //pop '('
					}
					ope = false; //not to put unary operator
				}else{
					put(now_char, postfix);
					ope = true;
				}
			}
		}
		while(operand.size() > 0){
			if(operand.top() != '('){
				postfix += operand.top();
				postfix += " ";
			}
			operand.pop();
		}
		//cout << postfix << endl; //turn original to postfix successfully
		ss.clear();
		ss.str("");
		ss << postfix;
		while(ss >> buf){
			if(buf.find_first_of(OPERATOR) == string::npos){
				if(is(buf[0], scalar)){
					BigNumber temp(buf);
					number.push(temp);
					number_is_var.push(false);
				}else{
					number.push(getIndex(buf[0]));
					number_is_var.push(true);
				}
			}else{
				char now_char = buf[0];
				BigNumber ll = 0, rl = 0;
				if(now_char == '~' || now_char == '#'){
					if(number.size() < 1){
						cout << "An unknown error happened!" << endl;
						error = true;
						break;
					}
					if(number_is_var.top()){
						rl = variable[static_cast<int>(number.top())];
					}else{
						rl = number.top();
					}
					number.pop();
					number_is_var.pop();
					if(now_char == '~'){
						number.push(-rl);
					}else{
						number.push(+rl);
					}
					number_is_var.push(false);
				}else{
					if(number.size() < 2){
						cout << "An unknown error happened!" << endl;
						error = true;
						break;
					}
					if(number_is_var.top()){
						rl = variable[static_cast<int>(number.top())];
					}else{
						rl = number.top();
					}
					number.pop();
					number_is_var.pop();
					int temp_index_for_assign = 0;
					if(number_is_var.top()){
						ll = variable[static_cast<int>(number.top())];
						temp_index_for_assign = static_cast<int>(number.top());
					}else{
						ll = number.top();
					}
					number.pop();
					number_is_var.pop();
					if(now_char == '+'){
						number.push(ll + rl);
					}else if(now_char == '-'){
						number.push(ll - rl);
					}else if(now_char == '*'){
						number.push(ll * rl);
					}else if(now_char == '/'){
						if(rl == 0){
							cout << "Cannot divided by 0!" << endl;
							error = true;
							break;
						}
						number.push(ll / rl);
					}else if(now_char == '%'){
						if(rl == 0){
							cout << "Cannot divided by 0!" << endl;
							error = true;
							break;
						}
						number.push(ll % rl);
					}else{
						variable[temp_index_for_assign] = rl;
						number.push(rl);
					}
					number_is_var.push(false);
				}
			}
		}
		if(error){
			continue;
		}
		if(number.size() != 1){
			cout << "An unknown error happened!" << endl;
			continue;
		}
		if(number_is_var.top()){
			cout << variable[number.top()] << endl;
		}else{
			cout << number.top() << endl;
		}
		number.pop();
		number_is_var.pop();
	}
	return 0;
}
