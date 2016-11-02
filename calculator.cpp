#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

const string inputOPERATOR = "+-*/%()=";
const string OPERATOR = inputOPERATOR + "~#";
const string variable_namespace = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const string scalar = "1234567890";
const string digit_separator = "\'"; //c++14
const string space = " ";
int put_length = 15; //should > 0

stack<char> operand; //will be used to save operator
stack<int> number; //will be used to save number
stack<bool> number_is_var;
int variable[52] = {0};

int HI = 0, LO = 0; //using for mult and div
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

int getIndex(const char& n){
	size_t pos = variable_namespace.find(n);
	if(pos != string::npos){
		return pos;
	}
	return -1; //error
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
					number.push(stoi(buf));
					number_is_var.push(false);
				}else{
					number.push(getIndex(buf[0]));
					number_is_var.push(true);
				}
			}else{
				char now_char = buf[0];
				int ll = 0, rl = 0; //////not yet to fit variable
				if(now_char == '~' || now_char == '#'){
					if(number.size() < 1){
						cout << "An unknown error happened!" << endl;
						error = true;
						break;
					}
					if(number_is_var.top()){
						rl = variable[number.top()];
					}else{
						rl = number.top();
					}
					number.pop();
					number_is_var.pop();
					if(now_char == '~'){
						number.push(-rl);
#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

const string inputOPERATOR = "+-*/%()=";
const string OPERATOR = inputOPERATOR + "~#";
const string variable_namespace = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const string scalar = "1234567890";
const string digit_separator = "\'"; //c++14
const string space = " ";
int put_length = 15; //should > 0

stack<char> operand; //will be used to save operator
stack<int> number; //will be used to save number
stack<bool> number_is_var;
int variable[52] = {0};

int HI = 0, LO = 0; //using for mult and div
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

int getIndex(const char& n){
	size_t pos = variable_namespace.find(n);
	if(pos != string::npos){
		return pos;
	}
	return -1; //error
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
					number.push(stoi(buf));
					number_is_var.push(false);
				}else{
					number.push(getIndex(buf[0]));
					number_is_var.push(true);
				}
			}else{
				char now_char = buf[0];
				int ll = 0, rl = 0; //////not yet to fit variable
				if(now_char == '~' || now_char == '#'){
					if(number.size() < 1){
						cout << "An unknown error happened!" << endl;
						error = true;
						break;
					}
					if(number_is_var.top()){
						rl = variable[number.top()];
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
						rl = variable[number.top()];
					}else{
						rl = number.top();
					}
					number.pop();
					number_is_var.pop();
					int temp_index_for_assign = 0;
					if(number_is_var.top()){
						ll = variable[number.top()];
						temp_index_for_assign = number.top();
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
