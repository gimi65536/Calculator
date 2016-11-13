#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <fstream>
#include <chrono>
#include <ctime>
using namespace std;

const int BASIC_SIZE = 4;

ofstream fout("calculations_log.txt", ios::app);

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
	bool operator < (const int& n) const{
		resize();
		BigNumber temp = n;
		return (*this) < temp;
	}
	bool operator <= (const BigNumber& n) const{
		return (*this) < n || (*this) == n;
	}
	bool operator <= (const int& n) const{
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
	bool operator > (const int& n) const{
		resize();
		BigNumber temp = n;
		return (*this) > temp;
	}
	bool operator >= (const BigNumber& n) const{
		return (*this) > n || (*this) == n;
	}
	bool operator >= (const int& n) const{
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
	const BigNumber& operator = (const int& n){
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
	const BigNumber& operator += (const int& temp){
		resize();
		BigNumber n(temp);
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
	const BigNumber& operator -= (const int& temp){
		resize();
		BigNumber n(temp);
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
	const BigNumber operator ^ (const BigNumber& n) const;
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
const BigNumber BigNumber::operator ^ (const BigNumber& n) const{
	coresize(n);
	BigNumber temp;
	if(n < 0){
		temp = 0;
	}else{
		temp = 1;
		HI = 0, LO = 0;
		for(BigNumber i = 1;i <= n;i++){
			temp *= (*this);
		}
	}
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

const string CPPinputOPERATOR = "+-*/%()=";
const string inputOPERATOR = CPPinputOPERATOR + "^";
const string additional_assignment_OPERATOR = "`!@\\&";
const string OPERATOR = inputOPERATOR + "~#" + additional_assignment_OPERATOR;
const string additional_assignment[5] = {"+=", "-=", "*=", "/=", "%="};
const string variable_namespace = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_$";
const string scalar = "1234567890";
const string digit_separator = "\'"; //c++14
const string space = " ";
int put_length = 15; //should > 0

class Node{
friend class Variable_ADT;
private:
	string name;
	Node* next;
	Node(){
		name = "\0";
		num = 0;
		next = nullptr;
	}
	Node(string nama){
		name = nama;
		num = 0;
		next = nullptr;
	}
	Node(string nama, BigNumber n){
		name = nama;
		num = n;
		next = nullptr;
	}
	~Node(){
		delete next;
	}
public:
	BigNumber num;
	//set con/destructor private so that user cannot create Node casually
	//but set Var_ADT friend so that it can create Node objects
	//because Var_ADT is friend class, so we don't have to prepare getter or setter
};

class Variable_ADT{
private:
	Node* head;
	BigNumber count;
	Node Error_node;
public:
	Variable_ADT(){
		head = nullptr;
		count = 0;
	}
	Variable_ADT(bool temp){
		head = nullptr;
		count = 0;
		if(temp){
			BigNumber zero = 0;
			for(int i = 0;i < 52;i++){
				string nama;
				nama += variable_namespace[i];
				new_var(nama, zero);
			}
		}
	}
	~Variable_ADT(){
		delete head;
	}
	BigNumber& operator [] (BigNumber i){
		if(i < 0 || i >= count){
			return Error_node.num;
		}
		/*Node* tmp = head;
		for(BigNumber j = 0;j < i;j += 1){
			tmp = head -> next;
		}
		return tmp -> num;*/ //this is wrong but I don't know why
		Node* tmp = head;
		for(BigNumber j = 0;j < count;j += 1){
			if(j == i){
				return tmp -> num;
			}
			tmp = tmp -> next;
		}
	}
	bool new_var(string nama, const BigNumber& n){
		if(count == 0){
			head = new Node(nama, n);
			count += 1;
			return true;
		}
		Node* tmp = head;
		while(tmp -> next != nullptr){
			tmp = tmp -> next;
		}
		tmp -> next = new Node(nama, n);
		count += 1;
		return true;
	}
	bool check_name(string nama){
		Node* tmp = head;
		while(tmp != nullptr){
			if(tmp -> name == nama){
				return true;
			}
			tmp = tmp -> next;
		}
		return false;
	}
	BigNumber getIndex(string nama){
		Node* tmp = head;
		for(BigNumber i = 0;i < count;i += 1){
			if(tmp -> name == nama){
				return i;
			}
			tmp = tmp -> next;
		}
		return -1;
	}
	const BigNumber& getCount(){
		return count;
	}
};

stack<char> operand; //will be used to save operator
stack<BigNumber> number; //will be used to save number
stack<bool> number_is_var;
Variable_ADT variable(true);

bool ope = true; //use for unary minus
bool CPPTYPE = true; //whether to use C++ format arithmetic expression

int getPrior(char& n, bool ope = ::ope){
	if(n == '~' || n == '#' || n == '^'){
		return 50;
	}else if(n == '*' || n == '/' || n == '%'){
		return 40;
	}else if(!ope && (n == '+' || n == '-')){
		return 30;
	}else if(n == '=' || n == '`' || n == '!' || n == '@' || n == '\\' || n == '&'){
		return 20;
	}else if(ope && n == '-'){
		n = '~'; //turn unary negative to '~'
		return 50;
	}else if(ope && n == '+'){
		n = '#'; //turn unary positive to '#'
		return 50;
	}
	return 10; //preserved for ()
}

bool left_to_right(const char& n){
	if(n == '=' || n == '~' || n == '#' || n == '^' || n == '`' || n == '!' || n == '@' || n == '\\' || n == '&'){
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
			fout << ' ';
		}else{
			cout << str[i];
			fout << str[i];
		}
	}
	cout << endl;
	fout << endl;
	for(int i = 0;i < put_length;i++){
		if(i >= put_length / 2 - lef && i <= put_length / 2 + righ){
			cout << '^';
			fout << '^';
		}else{
			cout << ' ';
			fout << ' ';
		}
	}
	cout << endl << error << endl;
	fout << endl << error << endl;
}

bool Cppcheck(string str){
	string delim = CPPinputOPERATOR + variable_namespace + scalar + space + digit_separator;
	string& allow = delim, deny = delim;
	if(str.find_first_not_of(delim) != string::npos){
		cout << "You should not input any invalid symbol!" << endl;
		fout << "You should not input any invalid symbol!" << endl;
		return false;
	} //complete B01
	for(int i = 0;i < 5;i++){
		size_t pos = str.find(additional_assignment[i]);
		while(pos != string::npos){
			str.erase(pos, 1);
			pos = str.find(additional_assignment[i], pos + 1);
		}
	}
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
		fout << "The number of ( does not equal to the number of ) !" << endl;
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
	bool variablesignal = false, scalarsignal = false;
	for(int i = 0;i < len;i++){
		if(is(str[i], inputOPERATOR)){
			variablesignal = false;
			scalarsignal = false;
		}else if(is(str[i], scalar)){
			if(!variablesignal){
				scalarsignal = true;
			}
		}else if(is(str[i], variable_namespace)){
			if(scalarsignal){
				cout << "Variable name should initial with not integer" << endl;
				fout << "Variable name should initial with not integer" << endl;
				return false;
			}//complete B06
			variablesignal = true;
		}else if(str[i] == '\''){
			if(variablesignal){
				cout << "Quote should be between two digits!" << endl;
				fout << "Quote should be between two digits!" << endl;
				return false;
			}else if(scalarsignal){
				if(i == str.length() - 1 || !is(str[i + 1], scalar)){
					cout << "Quote should be between two digits!" << endl;
					fout << "Quote should be between two digits!" << endl;
					return false;
				}
			}
		}
	}
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
		}else if(is(prev, scalar + variable_namespace)){
			if(now == '('){
				alert(i, 1, 0, str, "Cannot put ( right after number !");
				return false;
			}
		}
		prev = now;
	}
	delim = "+-*/%()"; //reuse
	pos = str.find_first_of(delim); //reuse
	while(pos != string::npos){
		str[pos] = ' ';
		pos = str.find_first_of(delim);
	}
	pos = str.find('='); //reuse
	if(pos != string::npos){
		string substr = str.substr(pos + 1, len - pos - 1);
		pos = substr.find('='); //reuse
		while(pos != string::npos){
			substr[pos] = ' ';
		}
		stringstream ss;
		ss << substr;
		string buf;
		while(ss >> buf){
			if(is(buf[0], variable_namespace) && !variable.check_name(buf)){
				cout << "Disallow to initialize a variable after the first assignment." << endl;
				fout << "Disallow to initialize a variable after the first assignment." << endl;
				return false;
			} //complete B13
		}
	}
	return true;
}

bool check(const string& String){
	string str = String, specialassignmentstr = String, delim = inputOPERATOR + variable_namespace + scalar + space;
	string& allow = delim, deny = delim;
	for(int i = 0;i < 5;i++){
		size_t pos = str.find(additional_assignment[i]);
		while(pos != string::npos){
			str.erase(pos, 1);
			specialassignmentstr.erase(pos, 1);
			specialassignmentstr[pos] = additional_assignment_OPERATOR[i];
			pos = str.find(additional_assignment[i], pos + 1);
		}
	}
	size_t pos = str.find_first_not_of(delim);
	while(pos != string::npos){
		str.erase(pos, 1);
		specialassignmentstr.erase(pos, 1);
		pos = str.find_first_not_of(delim);
	} //A01 complete
	delim = variable_namespace + scalar; //reuse
	pos = str.find(' '); //reuse
	while(pos != string::npos){
		if(pos == 0 || pos == str.length() - 1){
			str.erase(pos, 1);
			specialassignmentstr.erase(pos, 1);
			pos = str.find(' ', pos);
		}else{
			if(!is(str[pos - 1], delim) || !is(str[pos + 1], delim)){
				str.erase(pos, 1);
				specialassignmentstr.erase(pos, 1);
				pos = str.find(' ', pos);
			}else{
				pos = str.find(' ', pos + 1);
			}
		}
	} //A00 complete
	if(str.length() == 0){
		cout << "You should input any valid symbol!" << endl;
		fout << "You should input any valid symbol!" << endl;
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
				if(now == '*' || now == '/' || now == '%' || now == '=' || now == ')' || now == '^'){
					alert(i, 1, 0, str, "Cannot put \"*/%)=^\" right after operator \"+-\" !");
					return false;
				} //complete A04
			}else if(prev == '*' || prev == '/' || prev == '%' || prev == '^'){
				if(now == '*' || now == '/' || now == '%' || now == '=' || now == ')' || now == '^'){
					alert(i, 1, 0, str, "Cannot put \"*/%)=^\" right after operator \"*/%^\" !");
					return false;
				} //complete A03
			}else if(prev == '('){
				if(now == '*' || now == '/' || now == '%' || now == ')' || now == '=' || now == '^'){
					alert(i, 1, 0, str, "Cannot put \"*/%)=^\" right after operator \"(\" !");
					return false;
				} //complete A05 A06
			}else if(prev == ')'){
				if(now == '='){
					alert(i, 1, 0, str, "Cannot put \"=\" right after operator \")\" !");
					return false;
				}
			}else if(prev == '='){
				if(now == '*' || now == '/' || now == '%' || now == ')' || now == '=' || now == '^'){
					alert(i, 1, 0, str, "Cannot put \"*/%)=\" right after operator \"=\" !");
					return false;
				} //complete A07 A08 A09 A14 A15 A16
			}
		}
		prev = now;
	}
	if(now == '+' || now == '-' || now == '*' || now == '/' || now == '%' || now == '(' || now == '=' || now == '^'){
		alert(len - 1, 0, 0, str, "Should not put this at this position!");
		return false;
	} //complete A13 A17
	pos = str.find_last_of('='); //reuse
	if(pos != string::npos){
		deny = "+-*/%)^";
		bool variablesignal = false;
		for(int i = 0;i < pos;i++){
			if(is(str[i], deny)){
				alert(i, 0, 0, str, "This operator should NOT appear BEFORE any assignment!");
				return false;
			}else if(is(str[i], scalar) && !variablesignal){
				alert(i, 0, 0, str, "Scalar should NOT appear BEFORE any assignment!");
				return false;
			}else if(is(str[i], inputOPERATOR)){
				variablesignal = false;
			}else if(is(str[i], variable_namespace)){
				variablesignal = true;
			}else if(str[i] == ' '){
				variablesignal = false;
			}
		} //complete A11
		pos = str.find_first_of('=');
		string substr = str.substr(0, pos);
		if(substr.find(" ") != string::npos){
			cout << "Before an assignment, ONLY 1 VARIABLE is allowed!" << endl;
			fout << "Before an assignment, ONLY 1 VARIABLE is allowed!" << endl;
			return false;
		}
		size_t apos = str.find_first_of('=', pos + 1);
		while(apos != string::npos){
			substr = str.substr(pos, apos - pos - 1);
			if(substr.find(' ') != string::npos){
				cout << "Before an assignment, ONLY 1 VARIABLE is allowed!" << endl;
				fout << "Before an assignment, ONLY 1 VARIABLE is allowed!" << endl;
				return false;
			}
			pos = apos;
			apos = str.find_first_of('=', pos + 1);
		} //complete A10
	}
	delim = "+-*/%()^"; //reuse
	pos = str.find_first_of(delim); //reuse
	while(pos != string::npos){
		str[pos] = ' ';
		specialassignmentstr[pos] = ' ';
		pos = str.find_first_of(delim);
	}
	bool variablesignal = false;
	for(int i = 0;i < str.length();i++){
		if(str[i] == ' ' || is(str[i], inputOPERATOR)){
			variablesignal = false;
		}else if(is(str[i], variable_namespace)){
			variablesignal = true;
		}else if(is(str[i], scalar)){
			if(!variablesignal){
				str[i] = ' ';
				specialassignmentstr[i] = ' ';
			}
		}
	}
	pos = str.find_last_of("="); //reuse
	string substr;
	if(pos != string::npos){
		substr = str.substr(pos + 1, str.length() - pos - 1);
	}else{
		substr = str;
	}
	stringstream ss;
	ss << substr;
	string buf;
	while(ss >> buf){
		if(is(buf[0], variable_namespace) && !variable.check_name(buf)){
			cout << "Cannot use undefined variable in computing" << endl;
			fout << "Cannot use undefined variable in computing" << endl;
			return false;
		} //complete A19
	}
	pos = 0; //reuse
	size_t apos = specialassignmentstr.find_first_of(additional_assignment_OPERATOR + "=");
	while(apos != string::npos){
		bool is_special = is(specialassignmentstr[apos], additional_assignment_OPERATOR);
		substr = specialassignmentstr.substr(pos, apos - pos); //reuse
		ss.clear();
		ss.str("");
		ss << substr;
		while(ss >> buf){
			if(is_special && !variable.check_name(buf)){
				cout << "Don't put undefined variable before additional assignment operator." << endl;
				fout << "Don't put undefined variable before additional assignment operator." << endl;
				return false;
			}
		}
		pos = apos + 1;
		apos = specialassignmentstr.find_first_of(additional_assignment_OPERATOR + "=", pos);
	} //complete A18
	return !CPPTYPE || Cppcheck(String);
}

void var_resize(){
	int max = 0;
	for(BigNumber i = 0;i < variable.getCount();++i){
		int tmp = variable[i].getSize();
		if(tmp > max){max = tmp;}
	}
	if(max < BASIC_SIZE){max = BASIC_SIZE;}
	BigNumber::SIZE = max;
	for(BigNumber i = 0;i < variable.getCount();++i){
		variable[i].resize();
	}
}

int main(){
	ifstream fin("setup.ini");
	fin >> CPPTYPE >> put_length;
	fin.close();
	string str, buf, postfix, delim = inputOPERATOR + variable_namespace + scalar + space;
	stringstream ss;
	auto nowtime = chrono::system_clock::to_time_t(chrono::system_clock::now());
	fout << endl << endl << ctime(&nowtime) << "Start to calculate..." << endl << endl;
	while(cout << ">>> ", getline(cin, str)){
		fout << ">>> " << str << endl;
		if(str.length() == 0){
			continue;
		}
		if(!check(str)){
			cout << "Please try to input again..." << endl;
			fout << "Please try to input again..." << endl;
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
		for(int i = 0;i < 5;i++){
			pos = str.find(additional_assignment[i]);
			while(pos != string::npos){
				str.erase(pos, 1);
				str[pos] = additional_assignment_OPERATOR[i];
				pos = str.find(additional_assignment[i]);
			}
		}
		while(number.size() > 0){
			number.pop();
		}
		while(number_is_var.size() > 0){
			number_is_var.pop();
		}
		bool variablesignal = false, scalarsignal = false;
		for(int i = 0;i < str.length();i++){
			if(is(str[i], inputOPERATOR)){
				variablesignal = false;
				scalarsignal = false;
				str.insert(i + 1, " ");
				str.insert(i, " ");
				i++;
			}else if(is(str[i], scalar)){
				if(!variablesignal){
					scalarsignal = true;
				}
			}else if(is(str[i], variable_namespace)){
				if(scalarsignal){
					scalarsignal = false;
					str.insert(i, " ");
					i++;
				}
				variablesignal = true;
			}
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
				if(is(buf[0], variable_namespace)){ //if it is a var
					if(!variable.check_name(buf)){
						variable.new_var(buf, 0);
					}
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
					number.push(variable.getIndex(buf));
					number_is_var.push(true);
				}
			}else{
				char now_char = buf[0];
				BigNumber ll = 0, rl = 0;
				if(now_char == '~' || now_char == '#'){
					if(number.size() < 1){
						cout << "An unknown error happened!" << endl;
						fout << "An unknown error happened!" << endl;
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
						fout << "An unknown error happened!" << endl;
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
							fout << "Cannot divided by 0!" << endl;
							error = true;
							break;
						}
						number.push(ll / rl);
					}else if(now_char == '%'){
						if(rl == 0){
							cout << "Cannot divided by 0!" << endl;
							fout << "Cannot divided by 0!" << endl;
							error = true;
							break;
						}
						number.push(ll % rl);
					}else if(now_char == '^'){
						if(ll == 0 && rl < 0){
							cout << "Cannot compute 0 to the power of negative number!" << endl;
							fout << "Cannot compute 0 to the power of negative number!" << endl;
							error = true;
							break;
						}
						number.push(ll ^ rl);
					}else if(now_char == '`'){
						number.push(variable[temp_index_for_assign] += rl);
					}else if(now_char == '!'){
						number.push(variable[temp_index_for_assign] -= rl);
					}else if(now_char == '@'){
						number.push(variable[temp_index_for_assign] *= rl);
					}else if(now_char == '\\'){
						if(rl == 0){
							cout << "Cannot divided by 0!" << endl;
							fout << "Cannot divided by 0!" << endl;
							error = true;
							break;
						}
						number.push(variable[temp_index_for_assign] /= rl);
					}else if(now_char == '&'){
						if(rl == 0){
							cout << "Cannot divided by 0!" << endl;
							fout << "Cannot divided by 0!" << endl;
							error = true;
							break;
						}
						number.push(variable[temp_index_for_assign] %= rl);
					}else{
						number.push(variable[temp_index_for_assign] = rl);
					}
					number_is_var.push(false);
				}
			}
		}
		if(error){
			var_resize();
			continue;
		}
		if(number.size() != 1){
			cout << "An unknown error happened!" << endl;
			fout << "An unknown error happened!" << endl;
			var_resize();
			continue;
		}
		if(number_is_var.top()){
			cout << variable[number.top()] << endl;
			fout << variable[number.top()] << endl;
		}else{
			cout << number.top() << endl;
			fout << number.top() << endl;
		}
		number.pop();
		number_is_var.pop();
		var_resize();
	}
	return 0;
}