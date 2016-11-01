#include <iostream>
#include <cassert>
#include <stack>
#include <string>
#include <sstream>
using namespace std;

const string OPERATOR = "+-*/%()=~#";

stack<char> operand; //will be used to save operator
stack<int> number; //will be used to save number

int HI = 0, LO = 0; //using for mult and div
bool ope = true; //use for unary minus

int getPrior(char& n, bool ope = ::ope){
	if(n == '~'){
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

int main(){
	string str, buf, postfix;
	stringstream ss;
	getline(cin, str);
	size_t pos = str.find_first_of(OPERATOR);
	while(pos != string::npos){
		str.insert(pos + 1, " ");
		str.insert(pos, " ");
		pos = str.find_first_of(OPERATOR, pos + 2);
	}
	cout << str << endl;
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
	cout << postfix; //turn original to postfix successfully
	ss.clear();
	ss.str("");
	ss << postfix << endl;
	while(ss >> buf){
		if(buf.find_first_of(OPERATOR) == string::npos){
			number.push(stoi(buf));
		}else{
			char now_char = buf[0];
			int ll = 0, rl = 0; //////not yet to fit variable
			if(now_char == '~' || now_char == '#'){
				assert(number.size() >= 1);
				rl = number.top();
				number.pop();
				if(now_char == '~'){
					number.push(-rl);
				}else{
					number.push(+rl);
				}
			}else{
				assert(number.size() >= 2);
				rl = number.top();
				number.pop();
				ll = number.top();
				number.pop();
				if(now_char == '+'){
					number.push(ll + rl);
				}else if(now_char == '-'){
					number.push(ll - rl);
				}else if(now_char == '*'){
					number.push(ll * rl);
				}else if(now_char == '/'){
					number.push(ll / rl);
				}else if(now_char == '%'){
					number.push(ll % rl);
				}
			}
		}
	}
	assert(number.size() == 1);
	cout << number.top();
	number.pop();
	return 0;
}