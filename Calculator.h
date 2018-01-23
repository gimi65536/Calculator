#ifndef _CALCULATOR_
#define _CALCULATOR_
#include "BigNumber.h"
#include "variable.h"
#include <stack>
#include <fstream>
#include <chrono>
#include <ctime>
#include <regex>
using namespace std;

ofstream fout("calculations_log.txt", ios::app);
bool file_out = false;

class wstream{};
class Wendl{};
template <typename T>
const wstream& operator << (const wstream& wout, const T& n){
	cout << n;
	if(file_out){
		fout << n;
	}
	return wout;
}
const wstream& operator << (const wstream& wout, const Wendl& wendl){
	cout << endl;
	if(file_out){
		fout << endl;
	}
	return wout;
}
constexpr wstream wout;
constexpr Wendl wendl;

template<typename T, size_t N>
constexpr size_t SIZE(T(&a)[N]){return N;}
string char_to_string(char c){
	return string(1, c);
}

string sign_to_regex(const string& str){
	static const regex r("([^\\w])");
	return regex_replace(str, r, "\\$1");
}

const vector<char> baseplusope{'+', '-'};
const vector<char> basemultiope{'*', '/', '%'};
const vector<char> baseexpope{'^'};
const vector<char> baseleftparentope{'('};
const vector<char> baserightope{')'};
const vector<char> baseassignope{'='};

const vector<char> cppbaseplusope{'+', '-'};
const vector<char> cppbasemultiope{'*', '/', '%'};
const vector<char> cppbaseexpope{};
const vector<char> cppbaseleftparentope{'('};
const vector<char> cppbaserightope{')'};
const vector<char> cppbaseassignope{'='};



const map<string, char> pairplusope{};
const map<string, char> pairmultiope{};
const map<string, char> pairexpope{};
const map<string, char> pairleftparentope{};
const map<string, char> pairrightope{};
const map<string, char> pairassignope{{"+=", '`'}, {"-=", '!'}, {"*=", '@'}, {"/=", '\\'}, {"%=", '&'}};

const map<string, char> cpppairplusope{};
const map<string, char> cpppairmultiope{};
const map<string, char> cpppairexpope{};
const map<string, char> cpppairleftparentope{};
const map<string, char> cpppairrightope{};
const map<string, char> cpppairassignope{{"+=", '`'}, {"-=", '!'}, {"*=", '@'}, {"/=", '\\'}, {"%=", '&'}};

const map<char, char> operator_mapping{{'+', '#'}, {'-', '~'}};

const string CPPinputOPERATOR = join("", concatsv(cppbaseplusope, cppbasemultiope, cppbaseexpope, cppbaseleftparentope, cppbaserightope, cppbaseassignope));
const string inputOPERATOR = join("", concatsv(baseplusope, basemultiope, baseexpope, baseleftparentope, baserightope, baseassignope));
const string additional_assignment_OPERATOR = join("", values(pairassignope));
const string OPERATOR = inputOPERATOR + additional_assignment_OPERATOR + join("", values(operator_mapping));
const vector<string> additional_assignment = keys(pairassignope);
const string scalar = "1234567890";
const string digit_separator = "\'"; //c++14
const string space = " ";
//regex cppcheck01(R"***(^\s+|\s+$|([\w\$]\s)\s+((?=[\w\$]))|([\w\$])\s+((?=[+\-*/%()^`!@\\&~#=]))|([+\-*/%()^`!@\\&~#=])\s+((?=[\w\$]))|([+\-*/%()^`!@\\&~#=])\s+((?=[+\-*/%()^`!@\\&~#=])))***");
int put_length = 15; //should > 0

stack<char> operand; //will be used to save operator
stack<BigNumber> number; //will be used to save number
stack<bool> number_is_var;

bool ope = true; //use for unary minus
bool CPPTYPE = true; //whether to use C++ format arithmetic expression
bool is_regex = false;

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

void alert(const int now, const int lef, const int righ, const string& str, const string& error){
	int len = str.length();
	for(int i = now - put_length / 2;i < now + (put_length + 1) / 2;i++){
		if(i >= len || i < 0){
			wout << ' ';
		}else{
			wout << str[i];
		}
	}
	wout << wendl;
	for(int i = 0;i < put_length;i++){
		if(i >= put_length / 2 - lef && i <= put_length / 2 + righ){
			wout << '^';
		}else{
			wout << ' ';
		}
	}
	wout << wendl << error << wendl;
}

bool Cppcheck(string str, bool is_regex){
	if(is_regex){
		static const vector<char> baseope         = concatsv(cppbaseplusope, cppbasemultiope, cppbaseexpope, cppbaseleftparentope, cppbaserightope, cppbaseassignope);
		static const map<string, char> pairope    = concatsm(cpppairplusope, cpppairmultiope, cpppairexpope, cpppairleftparentope, cpppairrightope, cpppairassignope);	
		static const vector<char> plusope         = concatsv(cppbaseplusope, values(cpppairplusope));
		static const vector<char> multiope        = concatsv(cppbasemultiope, values(cpppairmultiope));
		static const vector<char> expope          = concatsv(cppbaseexpope, values(cpppairexpope));
		static const vector<char> leftparentope   = concatsv(cppbaseleftparentope, values(cpppairleftparentope));
		static const vector<char> rightparentope  = concatsv(cppbaserightope, values(cpppairrightope));
		static const vector<char> assignope       = concatsv(cppbaseassignope, values(cpppairassignope));
		static const vector<char> ope             = concatsv(plusope, multiope, expope, leftparentope, rightparentope, assignope);
		static const string baseope_regex         = sign_to_regex(join("", baseope));
		static const string plusope_regex         = sign_to_regex(join("", plusope));
		static const string multiope_regex        = sign_to_regex(join("", multiope));
		static const string expope_regex          = sign_to_regex(join("", expope));
		static const string leftparentope_regex   = sign_to_regex(join("", leftparentope));
		static const string rightparentope_regex  = sign_to_regex(join("", rightparentope));
		static const string assignope_regex       = sign_to_regex(join("", assignope));
		static const string ope_regex             = sign_to_regex(join("", ope));
		static const vector<char> separator       = {'\''};
		static const string separator_regex       = sign_to_regex(join("", separator));
		static const regex B00_1("[^\\d" + vartotal_regex + baseope_regex + separator_regex + "\\s]");
		static const regex B00_3("([" + ope_regex + "])");
		static const regex B00_5("(?:^|\\s)\\d[\\d" + separator_regex + "]*([^\\s\\d" + separator_regex + "])");
		static const regex B00_6("(?:^|\\s)[" + varinit_regex + "][" + varcontent_regex + "]*([^\\s" + varcontent_regex + "])");
		static const regex B00_7("^\\s+|\\s+$|([^\\s]\\s)\\s+(?=[^\\s])");
		static const regex B01_1("(?:^|\\s)((?=[" + separator_regex + "]))(?=[^" + varinit_regex + "])");
		static const regex B01_2("(?:^|\\s)\\d[\\d" + separator_regex + "]*?([" + separator_regex + "])(?=[^\\d]|$)");
		static const regex B02("[^" + ope_regex + "]\\s[" + leftparentope_regex + "]");
		static const regex B03("[" + rightparentope_regex + "]\\s[^" + ope_regex + "]");
		static const regex B04("[^" + ope_regex + "]\\s[^" + ope_regex + "]");
		static const regex B05("[" + leftparentope_regex + "]\\s[" + rightparentope_regex + "]|[" + rightparentope_regex + "]\\s[" + leftparentope_regex + "]");
		static const regex is_leftparent("[" + leftparentope_regex + "]");
		static const regex is_rightparent("[" + rightparentope_regex + "]");
		static const regex is_assign("[" + assignope_regex + "]");
		if(smatch m; regex_search(str, m, B00_1)){
			alert(m.position(), 0, 0, str, "You should not input any invalid symbol!");
			return false;
		}
		for(const auto& [P, rep] : pairope){ //B00-2
			str = regex_replace(str, regex(sign_to_regex(P)), char_to_string(rep));
		}
		str = regex_replace(str, B00_3, " $1 ");
		//B00-4 is deprecated
		if(smatch m; regex_search(str, m, B00_5)){
			alert(m.position(1), 0, 0, str, "Cannot put invalid symbol in a number!");
			return false;
		}
		if(smatch m; regex_search(str, m, B00_6)){
			alert(m.position(1), 0, 0, str, "Cannot put invalid symbol in a variable!");
			return false;
		}
		str = regex_replace(str, B00_7, "$1$2");
		if(smatch m; regex_search(str, m, B01_1)){
			alert(m.position(1), 0, 0, str, "Cannot put quote at beginning position!");
			return false;
		}
		if(smatch m; regex_search(str, m, B01_2)){
			alert(m.position(1), 1, 1, str, "Quote should be between two digits!");
			return false;
		}
		if(smatch m; regex_search(str, m, B02)){
			alert(m.position() + 2, 0, 0, str, "Cannot put ( right after number or variable!");
			return false;
		}
		if(smatch m; regex_search(str, m, B03)){
			alert(m.position(), 0, 0, str, "Cannot put number or variable right after ) !");
			return false;
		}
		if(smatch m; regex_search(str, m, B04)){
			alert(m.position() + 1, 1, 1, str, "Cannot put space between two numbers or variables!");
			return false;
		}
		if(smatch m; regex_search(str, m, B05)){
			alert(m.position() + 1, 1, 1, str, "Cannot put ( right after ) !");
			return false;
		}
		stringstream ss(str);
		int count = 0;
		string buf;
		while(ss >> buf){
			if(regex_match(buf, is_leftparent)){
				count++;
			}else if(regex_match(buf, is_rightparent)){
				count--;
			}
			if(count < 0){
				alert(size_t(ss.tellg()) - 1, 0, 0, str, "Do NOT put right parenthesis when no left parentesis is waiting!");
				return false;
			}
		}
		if(count != 0){
			wout << "The number of ( does not equal to the number of ) !" << wendl;
			return false;
		}
		if(smatch m;regex_search(str, m, is_assign)){
			ss.clear();
			ss.str("");
			ss << m.suffix();
			while(ss >> buf){
				if(is(buf[0], variable_namespace) && !variable.check_name(buf)){
					alert(m.position() + 1 + ss.tellg() - buf.size(), 0, buf.size() - 1, str, "Disallow to initialize a variable after the first assignment.");
					return false;
				} //complete B13
			}
		}
	}else{
		string delim = CPPinputOPERATOR + variable_namespace + scalar + space + digit_separator;
		string& allow = delim, deny = delim;
		if(str.find_first_not_of(delim) != string::npos){
			wout << "You should not input any invalid symbol!" << wendl;
			return false;
		} //complete B01
		for(const auto& add_assign : additional_assignment){
			size_t pos = str.find(add_assign);
			while(pos != string::npos){
				str.erase(pos, 1);
				pos = str.find(add_assign, pos + 1);
			}
		}
		delim = variable_namespace + scalar + digit_separator;
		size_t pos;
		pos = str.find(' ');
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
		}
		//str = regex_replace(str, cppcheck01, "$1$2$3$4$5$6$7$8");
		 //complete B00
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
			wout << "The number of ( does not equal to the number of ) !" << wendl;
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
					wout << "Variable name should initial with not integer" << wendl;
					return false;
				}//complete B06
				variablesignal = true;
			}else if(str[i] == '\''){
				if(variablesignal){
					wout << "Quote should be between two digits!" << wendl;
					return false;
				}else if(scalarsignal){
					if(i == str.length() - 1 || !is(str[i + 1], scalar)){
						wout << "Quote should be between two digits!" << wendl;
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
					wout << "Disallow to initialize a variable after the first assignment." << wendl;
					return false;
				} //complete B13
			}
		}
	}
	return true;
}

bool check(const string& String, bool is_regex){
	if(is_regex){
		static const vector<char> baseope         = concatsv(baseplusope, basemultiope, baseexpope, baseleftparentope, baserightope, baseassignope);
		static const map<string, char> pairope    = concatsm(pairplusope, pairmultiope, pairexpope, pairleftparentope, pairrightope, pairassignope);	
		static const vector<char> plusope         = concatsv(baseplusope, values(pairplusope));
		static const vector<char> multiope        = concatsv(basemultiope, values(pairmultiope));
		static const vector<char> expope          = concatsv(baseexpope, values(pairexpope));
		static const vector<char> leftparentope   = concatsv(baseleftparentope, values(pairleftparentope));
		static const vector<char> rightparentope  = concatsv(baserightope, values(pairrightope));
		static const vector<char> assignope       = concatsv(baseassignope, values(pairassignope));
		static const vector<char> ope             = concatsv(plusope, multiope, expope, leftparentope, rightparentope, assignope);
		static const string baseope_regex         = sign_to_regex(join("", baseope));
		static const string plusope_regex         = sign_to_regex(join("", plusope));
		static const string multiope_regex        = sign_to_regex(join("", multiope));
		static const string expope_regex          = sign_to_regex(join("", expope));
		static const string leftparentope_regex   = sign_to_regex(join("", leftparentope));
		static const string rightparentope_regex  = sign_to_regex(join("", rightparentope));
		static const string assignope_regex       = sign_to_regex(join("", assignope));
		static const string ope_regex             = sign_to_regex(join("", ope));
		static const regex A00_1("[^\\d" + varinit_regex + varcontent_regex + baseope_regex + "\\s]");
		static const regex A00_3("([" + ope_regex + "])");
		static const regex A00_4("([" + varinit_regex + "][" + varcontent_regex + "]*)");
		//static const regex A00_5("(?:^|\\s)((?=[" + varcontonly_regex + "]))(?=[^\\d])|(?:^|\\s)\\d[^\\s]*?(?=[^\\d])((?=[" + varcontonly_regex + "]))");
		static const regex A00_6("^\\s+|\\s+$|([^\\s]\\s)\\s+(?=[^\\s])");
		static const regex A01("[^\\s]");
		static const regex A02("^[^" + vartotal_regex + plusope_regex + leftparentope_regex + "]");
		static const regex A03("[^" + vartotal_regex + rightparentope_regex + "]$");
		static const regex A04("[" + plusope_regex + multiope_regex + expope_regex + leftparentope_regex + assignope_regex + "]\\s[" + multiope_regex + expope_regex + "]");
		static const regex A05("[^" + vartotal_regex + rightparentope_regex + "]\\s[" + rightparentope_regex + "]");
		static const regex A06("[^" + vartotal_regex + "]\\s[" + assignope_regex + "]");
		static const regex A07("[^" + vartotal_regex + leftparentope_regex + assignope_regex + "\\s][^" + leftparentope_regex + "]*?[" + assignope_regex + "]");
		static const regex A08_1("(?:^|\\s)(\\d)[^" + leftparentope_regex + "]*?[" + assignope_regex + "]");
		static const regex A08_2("(?:^|[" + leftparentope_regex + assignope_regex + "]\\s)[" + varinit_regex + "][" + vartotal_regex + "\\s]*?\\s([" + varinit_regex + "])[" + vartotal_regex + "\\s]*?[" + assignope_regex + "]");
		static const regex A09("([" + varinit_regex + "][" + varcontent_regex + "]*?)(?=\\s(.)|$)");
		static const regex is_baseassign("[" + baseope_regex + "]");
		string str = regex_replace(String, A00_1, "");
		for(const auto& [P, rep] : pairope){ //A00-2
			str = regex_replace(str, regex(sign_to_regex(P)), char_to_string(rep));
		}
		str = regex_replace(str, A00_3, " $1 ");
		str = regex_replace(str, A00_4, " $1 ");
		/*if(smatch m;regex_search(str, m, A00_5)){
			size_t pos = 0;
			if(m[1].matched){
				pos = m.position(1);
			}else{
				pos = m.position(2);
			}
			alert(pos, 0, 0, str, "Should not put this at this position!");
			return false;
		}*/
		str = regex_replace(str, A00_6, "$1$2");
		if(!regex_search(str, A01)){
			wout << "You should input any valid symbol!" << wendl;
			return false;
		}
		if(regex_search(str, A02)){
			alert(0, 0, 0, str, "Should not put this at this position!");
			return false;
		}
		if(regex_search(str, A03)){
			alert(str.length() - 1, 0, 0, str, "Should not put this at this position!");
			return false;
		}
		if(smatch m;regex_search(str, m, A04)){
			alert(m.position() + 1, 1, 1, str, "Cannot put \"+-*/%^(=\" right after operator \"*/%^\" !");
			return false;
		}
		if(smatch m;regex_search(str, m, A05)){
			alert(m.position() + 1, 1, 1, str, "Cannot put this before \")\" !");
			return false;
		}
		if(smatch m;regex_search(str, m, A06)){
			alert(m.position() + 1, 1, 1, str, "Operator should NOT appear right before any assignment!");
			return false;
		}
		if(smatch m;regex_search(str, m, A07)){
			alert(m.position(), 0, 0, str, "This operator illegally appear BEFORE an assignment!");
			return false;
		}
		if(smatch m;regex_search(str, m, A08_1)){
			alert(m.position(1), 0, 0, str, "This number illegally appear BEFORE an assignment!");
			return false;
		}
		if(smatch m;regex_search(str, m, A08_2)){
			alert(m.position(1), 0, 0, str, "No second variable BEFORE an assignment!");
			return false;
		}
		smatch m;
		while(regex_search(str, m, A09)){
			string var = m.str(1);
			if(!variable.check_name(var)){
				if(!m[2].matched || !regex_match(m.str(2), is_baseassign)){
					alert(m.position(1), 0, var.size() - 1, str, "Don't put undefined variable before additional assignment operator.");
					return false;
				}
			}
			str = m.suffix();
		}
	}else{
		string str = String, specialassignmentstr = String, delim = inputOPERATOR + variable_namespace + scalar + space;
		string& allow = delim, deny = delim;
		for(int i = 0;i < additional_assignment.size();i++){
			size_t pos = str.find(additional_assignment[i]);
			while(pos != string::npos){
				str.erase(pos, 1);
				specialassignmentstr.erase(pos, 1);
				specialassignmentstr[pos] = additional_assignment_OPERATOR[i];
				pos = str.find(additional_assignment[i], pos + 1);
			}
			/*regex reg(additional_assignment[i]);
			str = regex_replace(str, reg, "=");
			specialassignmentstr = regex_replace(str, reg, string(1, additional_assignment_OPERATOR[i]));*/
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
			wout << "You should input any valid symbol!" << wendl;
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
				wout << "Before an assignment, ONLY 1 VARIABLE is allowed!" << wendl;
				return false;
			}
			size_t apos = str.find_first_of('=', pos + 1);
			while(apos != string::npos){
				substr = str.substr(pos, apos - pos - 1);
				if(substr.find(' ') != string::npos){
					wout << "Before an assignment, ONLY 1 VARIABLE is allowed!" << wendl;
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
			substr = str.substr(pos + 1);
		}else{
			substr = str;
		}
		stringstream ss;
		ss << substr;
		string buf;
		while(ss >> buf){
			if(is(buf[0], variable_namespace) && !variable.check_name(buf)){
				wout << "Cannot use undefined variable in computing" << wendl;
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
					wout << "Don't put undefined variable before additional assignment operator." << wendl;
					return false;
				}
			}
			pos = apos + 1;
			apos = specialassignmentstr.find_first_of(additional_assignment_OPERATOR + "=", pos);
		} //complete A18
	}
	return !CPPTYPE || Cppcheck(String, is_regex);
}

int main(){
	ifstream fin("setup.ini");
	fin >> CPPTYPE >> put_length >> file_out >> is_regex;
	fin.close();
	string str, buf, postfix, delim = inputOPERATOR + variable_namespace + scalar + space;
	stringstream ss;
	if(file_out){
		auto nowtime = chrono::system_clock::to_time_t(chrono::system_clock::now());
		fout << endl << endl << ctime(&nowtime) << "Start to calculate..." << endl << endl;
	}
	while(cout << ">>> ", getline(cin, str)){
		if(str.length() == 0){
			continue;
		}
		if(file_out){
			fout << ">>> " << str << endl;
		}
		if(!check(str, is_regex)){
			wout << "Please try to input again..." << wendl;
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
		for(int i = 0;i < additional_assignment.size();i++){
			pos = str.find(additional_assignment[i]);
			while(pos != string::npos){
				str.erase(pos, 1);
				str[pos] = additional_assignment_OPERATOR[i];
				pos = str.find(additional_assignment[i]);
			}
			/*regex reg(additional_assignment[i]);
			str = regex_replace(str, reg, string(1, additional_assignment_OPERATOR[i]));*/
		}
		while(number.size() > 0){
			number.pop();
		}
		while(number_is_var.size() > 0){
			number_is_var.pop();
		}
		/*bool variablesignal = false, scalarsignal = false;
		for(int i = 0;i < str.length();i++){
			if(is(str[i], OPERATOR)){
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
		}*/
		str = regex_replace(str, regex(R"***(([+\-*/%()^`!@\\&~#=]))***"), " $1 ");
		str = regex_replace(str, regex(R"***(([A-Za-z_\$][\w\$]*))***"), " $1 ");
		wout << str << wendl;
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
		//cout << postfix << wendl; //turn original to postfix successfully
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
						wout << "An unknown error happened!" << wendl;
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
						wout << "An unknown error happened!" << wendl;
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
					BtoI temp_index_for_assign = 0;
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
							wout << "Cannot divided by 0!" << wendl;
							error = true;
							break;
						}
						number.push(ll / rl);
					}else if(now_char == '%'){
						if(rl == 0){
							wout << "Cannot divided by 0!" << wendl;
							error = true;
							break;
						}
						number.push(ll % rl);
					}else if(now_char == '^'){
						if(ll == 0 && rl < 0){
							wout << "Cannot compute 0 to the power of negative number!" << wendl;
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
							wout << "Cannot divided by 0!" << wendl;
							error = true;
							break;
						}
						number.push(variable[temp_index_for_assign] /= rl);
					}else if(now_char == '&'){
						if(rl == 0){
							wout << "Cannot divided by 0!" << wendl;
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
			continue;
		}
		if(number.size() != 1){
			wout << "An unknown error happened!" << wendl;
			continue;
		}
		if(number_is_var.top()){
			wout << variable[number.top()] << wendl;
		}else{
			wout << number.top() << wendl;
		}
		number.pop();
		number_is_var.pop();
	}
	return 0;
}
#endif