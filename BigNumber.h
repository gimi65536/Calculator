#ifndef _BIG_NUMBER_
#define _BIG_NUMBER_
#include <iostream>
#include <string>
#include <sstream>
#include <type_traits>
#include <climits>
#include <cctype>
using namespace std;


#ifndef _DYNAMIC_
#define _DYNAMIC_ 1
#endif

#if _DYNAMIC_ == 0
#define _BIG_NUMBER_STATIC_
#else
#define _BIG_NUMBER_DYNAMIC_
#endif

const int BASIC_SIZE = 10;
static_assert(BASIC_SIZE >= 2, "BASIC_SIZE should >= 2.");

typedef long long int BtoI;
bool string_overflow = false;

class BigNumber;
class BigBigNumber;

class BigNumber{
friend class BigBigNumber;
private:

	#ifdef _BIG_NUMBER_DYNAMIC_
	static int SIZE;
	mutable int size;
	mutable int* a;

	#else
	static const int SIZE = BASIC_SIZE;
	int a[SIZE];
	
	#endif
	bool positive;
	void resize() const;
	void coresize(const BigNumber& n) const;
	int getSize() const;
	const BigNumber& PURE_assignment(const BigNumber& n);
	void PASS_BY_STRING(string str);
	void PASS_BY_STRING_with_notation(string str);
	const BigNumber& PURE_ADD_assignment(const BigNumber& n);
	const BigNumber& PURE_MINUS_assignment(const BigNumber& n);
	const BigNumber& PURE_PSEUDOMULTIPLE_assignment();
	const BigNumber& PURE_PSEUDODIVIDE_assignment();
	void COMMON_DIVIDE(const BigNumber& n) const;
public:
	friend ostream& operator << (ostream& os, const BigNumber& n);
	friend istream& operator >> (istream& is, BigNumber& n);
	BigNumber();
	BigNumber(const string& str);
	BigNumber(const char& ch);
	BigNumber(char* const n);
	BigNumber(const char* const n);
	template <typename T>
	BigNumber(const T& n);
	BigNumber(const BigNumber& n);
	~BigNumber();
	size_t digit() const;
	int getDigit(int n) const;
	template <typename T>
	bool operator == (const T& n) const;
	bool operator == (const BigNumber& n) const;
	template <typename T>
	bool operator != (const T& n) const;
	bool operator != (const BigNumber& n) const;
	bool operator < (const BigNumber& n) const;
	template <typename T>
	bool operator < (const T& n) const;
	bool operator <= (const BigNumber& n) const;
	template <typename T>
	bool operator <= (const T& n) const;
	bool operator > (const BigNumber& n) const;
	template <typename T>
	bool operator > (const T& n) const;
	bool operator >= (const BigNumber& n) const;
	template <typename T>
	bool operator >= (const T& n) const;
	const BigNumber operator + () const;
	const BigNumber operator - () const;
	const BigNumber& operator = (const BigNumber& n);
	const BigNumber& operator = (const string& str);
	const BigNumber& operator = (const char& ch);
	const BigNumber& operator = (char* const n);
	const BigNumber& operator = (const char* const n);
	template <typename T>
	const BigNumber& operator = (const T& n);
	const BigNumber abs() const;
	const BigNumber& operator += (const BigNumber& n);
	template <typename T>
	const BigNumber& operator += (const T& temp);
	const BigNumber& operator ++ ();
	const BigNumber operator ++ (int null);
	const BigNumber operator + (const BigNumber& n) const;
	template<typename T>
	const BigNumber operator + (const T& tmp) const;
	const BigNumber& operator -= (const BigNumber& n);
	template <typename T>
	const BigNumber& operator -= (const T& temp);
	const BigNumber& operator -- ();
	const BigNumber operator -- (int null);
	const BigNumber operator - (const BigNumber& n) const;
	template <typename T>
	const BigNumber operator - (const T& tmp) const;
	const BigNumber& operator *= (const BigNumber& n);
	template <typename T>
	const BigNumber& operator *= (const T& n);
	const BigNumber operator * (const BigNumber& n) const;
	template <typename T>
	const BigNumber operator * (const T& n) const;
	const BigNumber& operator /= (const BigNumber& n);
	template <typename T>
	const BigNumber& operator /= (const T& n);
	const BigNumber operator / (const BigNumber& n) const;
	template <typename T>
	const BigNumber operator / (const T& n) const;
	const BigNumber& operator %= (const BigNumber& n);
	template <typename T>
	const BigNumber& operator %= (const T& n);
	const BigNumber operator % (const BigNumber& n) const;
	template <typename T>
	const BigNumber operator % (const T& n) const;
	const BigNumber operator ^ (const BigNumber& n) const;
	template <typename T>
	const BigNumber operator ^ (const T& n) const;
	friend void var_resize();
	const string str(int notation) const;
	operator long long int() const;
	operator unsigned long long int() const;
	operator long double() const;
};

#ifdef _BIG_NUMBER_DYNAMIC_
int BigNumber::SIZE = BASIC_SIZE;
#endif

BigNumber HI = 0, LO = 0;
static BigNumber be_divided = 0;
static BigNumber divide = 0;

class BigBigNumber{ //only use in overflow, user cannot access it directly
friend class BigNumber;
private:

#ifdef _BIG_NUMBER_STATIC_
	static const int SIZE = 2 * BASIC_SIZE;
	int a[SIZE];
	bool positive;
	void PASS_BY_STRING(string str);
	friend ostream& operator << (ostream& os, const BigBigNumber& n);
	BigBigNumber(string str);
	template <typename T>
	BigBigNumber(const T& n);
	BigBigNumber(const BigNumber& n);
	BigBigNumber(const BigBigNumber& n);
	const BigBigNumber& PURE_assignment(const BigBigNumber& n);
	const BigBigNumber& operator = (const BigBigNumber& n);
	template <typename T>
	const BigBigNumber& operator = (const T& n);
	const BigBigNumber& PURE_ADD_assignment(const BigBigNumber& n);
	const BigBigNumber& PURE_PSEUDOMULTIPLE_assignment();
	const BigBigNumber& PURE_PSEUDODIVIDE_assignment();
	void proliferate();

#else
	BigBigNumber(){}

#endif
};



void BigNumber::resize() const{
	#ifdef _BIG_NUMBER_DYNAMIC_
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

	#endif
}
void BigNumber::coresize(const BigNumber& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();
	n.resize();

	#endif
}

int BigNumber::getSize() const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	for(int i = size - 1;i >= 0;i--){

	#else
	for(int i = SIZE - 1;i >= 0;i--){

	#endif
		if(a[i] != 0){
			return i + 1;
		}
	}
	return 1;
}
const BigNumber& BigNumber::PURE_assignment(const BigNumber& n){

	#ifdef _BIG_NUMBER_DYNAMIC_
	coresize(n);

	#endif
	for(int i = 0;i < SIZE;i++){
		a[i] = n.a[i];
	}
	return (*this);
}
void BigNumber::PASS_BY_STRING(string str){
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
	size_t pos = str.find_first_not_of("0123456789+-");
	while(pos != string::npos){
		str.erase(pos, 1);
		pos = str.find_first_not_of("0123456789+-");
	}
	if(str.length() > 0 && str[0] == '-'){
		positive = false;
		str.erase(0, 1);
	}else if(str.length() > 0 && str[0] == '+'){
		positive = true;
		str.erase(0, 1);
	}else{
		positive = true;
	}
	pos = str.find_first_of("+-");
	while(pos != string::npos){
		str.erase(pos, 1);
		pos = str.find_first_of("+-");
	}
	while(str.length() > 0 && str[0] == '0'){
		str.erase(0, 1);
	}

	#ifdef _BIG_NUMBER_DYNAMIC_
	while(str.length() > 9 * SIZE){
		SIZE *= 2;
	}
	size = SIZE;
	delete[] a;
	a = new int[SIZE];

	#else
	if(str.length() > 9 * SIZE){
		while(str.length() > 9 * SIZE){
			str.erase(0, 1);
		}
		cout << "Overflow when creating a number with too more digits!" << endl << "save the last " << 9 * SIZE << " digit only." << endl;
	}

	#endif
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
void BigNumber::PASS_BY_STRING_with_notation(string str){
	string_overflow = false;
	bool remain_positive = false;
	if(str[0] == '+'){
		remain_positive = true;
		str.erase(0, 1);
	}else if(str[0] == '-'){
		remain_positive = false;
		str.erase(0, 1);
	}
	str.erase(0, 1);
	for(int i = 0;i < str.length();i++){
		str[i] = toupper(str[i]);
		if(!isalpha(str[i]) && !isdigit(str[i])){
			str.erase(i, 1);
			i--;
		}
	}
	int notation_t = (str[0] != 'X') ? (str[0] - 'A' + 1) : (16);
	BigNumber notation = notation_t;
	str.erase(0, 1);

	#ifdef _BIG_NUMBER_DYNAMIC_
	size = SIZE;
	delete[] a;
	a = new int[SIZE];

	#endif
	for(int i = 0;i < SIZE;i++){
		a[i] = 0;
	}
	for(int i = 0;i < str.length();i++){
		(*this) = static_cast<BigNumber>(str[i]) + ((*this) * notation);
		if(string_overflow){
			return;
		}
	}
	positive = remain_positive;
}
const BigNumber& BigNumber::PURE_ADD_assignment(const BigNumber& n){

	#ifdef _BIG_NUMBER_DYNAMIC_
	coresize(n);

	#endif
	for(int i = 0;i < SIZE;i++){
		a[i] += n.a[i];
		if(a[i] >= 1000000000 && i < SIZE - 1){
			a[i + 1] ++;
			a[i] -= 1000000000;
		}
	}
	if(a[SIZE - 1] >= 1000000000){

		#ifdef _BIG_NUMBER_DYNAMIC_
		int temp = SIZE;
		SIZE *= 2;
		coresize(n);
		a[temp - 1] -= 1000000000;
		a[temp] ++;

		#else
		cout << "Overflow when adding a number!" << endl << "save the last " << 9 * SIZE << " digit only." << endl;
		a[SIZE - 1] -= 1000000000;
		string_overflow = true;

		#endif
	}
	return (*this);
}
const BigNumber& BigNumber::PURE_MINUS_assignment(const BigNumber& n){

	#ifdef _BIG_NUMBER_DYNAMIC_
	coresize(n);

	#endif
	for(int i = 0;i < SIZE;i++){
		a[i] -= n.a[i];
		if(a[i] < 0 && i < SIZE - 1){
			a[i + 1] --;
			a[i] += 1000000000;
		}
	}
	return (*this);
}
const BigNumber& BigNumber::PURE_PSEUDOMULTIPLE_assignment(){ //pseudomultiple

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	stringstream ss;
	ss << (*this) << 0;
	string str = ss.str();
	BigNumber temp(str);
	(*this) = temp;
	return (*this);
}
const BigNumber& BigNumber::PURE_PSEUDODIVIDE_assignment(){ //pseudodivide

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();
	
	#endif
	stringstream ss;
	ss << (*this);
	string str = ss.str();
	str.pop_back();
	BigNumber temp(str);
	(*this) = temp;
	return (*this);
}
BigNumber::BigNumber(){

	#ifdef _BIG_NUMBER_DYNAMIC_
	size = SIZE;
	a = new int[SIZE];

	#endif
	positive = true;
	for(int i = 0;i < SIZE;i++){
		a[i] = 0;
	}
}
BigNumber::BigNumber(const string& str){

	#ifdef _BIG_NUMBER_DYNAMIC_
	a = nullptr;

	#endif
	PASS_BY_STRING(str);
	(*this) = str;
}
BigNumber::BigNumber(const char& ch){

	#ifdef _BIG_NUMBER_DYNAMIC_
	a = nullptr;

	#endif
	(*this) = ch;
}
BigNumber::BigNumber(char* const n){

	#ifdef _BIG_NUMBER_DYNAMIC_
	a = nullptr;

	#endif
	(*this) = n;
}
BigNumber::BigNumber(const char* const n){

	#ifdef _BIG_NUMBER_DYNAMIC_
	a = nullptr;

	#endif
	(*this) = n;
}
template <typename T>
BigNumber::BigNumber(const T& n){

	#ifdef _BIG_NUMBER_DYNAMIC_
	size = SIZE;
	a = new int[SIZE];

	#endif
	(*this) = n;
}
BigNumber::BigNumber(const BigNumber& n){

	#ifdef _BIG_NUMBER_DYNAMIC_
	n.resize();
	size = SIZE;
	a = new int[SIZE];

	#endif
	for(int i = 0;i < SIZE;i++){
		a[i] = n.a[i];
	}
	positive = n.positive;
}

BigNumber::~BigNumber(){

	#ifdef _BIG_NUMBER_DYNAMIC_
	delete[] a;

	#endif
}

size_t BigNumber::digit() const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	stringstream ss;
	ss << (*this);
	string str = ss.str();
	if(str[0] == '-'){
		str.erase(0, 1);
	}
	return str.length();
}
int BigNumber::getDigit(int n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
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
template <typename T>
bool BigNumber::operator == (const T& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = n;
	return (*this) == temp;
}
bool BigNumber::operator == (const BigNumber& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	coresize(n);

	#endif
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
template <typename T>
bool BigNumber::operator != (const T& n) const{
	return !(*this == n);
}
bool BigNumber::operator != (const BigNumber& n) const{
	return !(*this == n);
}
bool BigNumber::operator < (const BigNumber& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	coresize(n);

	#endif
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
template <typename T>
bool BigNumber::operator < (const T& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = n;
	return (*this) < temp;
}
bool BigNumber::operator <= (const BigNumber& n) const{
	return (*this) < n || (*this) == n;
}
template <typename T>
bool BigNumber::operator <= (const T& n) const{
	return (*this) < n || (*this) == n;
}
bool BigNumber::operator > (const BigNumber& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	coresize(n);

	#endif
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
template <typename T>
bool BigNumber::operator > (const T& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = n;
	return (*this) > temp;
}
bool BigNumber::operator >= (const BigNumber& n) const{
	return (*this) > n || (*this) == n;
}
template <typename T>
bool BigNumber::operator >= (const T& n) const{
	return (*this) > n || (*this) == n;
}
const BigNumber BigNumber::operator + () const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	return (*this);
}
const BigNumber BigNumber::operator - () const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = (*this);
	temp.positive = !temp.positive;
	return temp;
}
const BigNumber& BigNumber::operator = (const BigNumber& n){

	#ifdef _BIG_NUMBER_DYNAMIC_
	coresize(n);

	#endif
	PURE_assignment(n);
	positive = n.positive;
	return (*this);
}
const BigNumber& BigNumber::operator = (const string& str){
	PASS_BY_STRING(str);
}
const BigNumber& BigNumber::operator = (const char& ch){
	string str;
	str += ch;
	PASS_BY_STRING(str);
}
const BigNumber& BigNumber::operator = (char* const n){
	string str = n;
	PASS_BY_STRING(str);
}
const BigNumber& BigNumber::operator = (const char* const n){
	string str = n;
	PASS_BY_STRING(str);
}
template <typename T>
const BigNumber& BigNumber::operator = (const T& n){

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	if(is_scalar<T>::value){ //faster
		if(is_signed<T>::value){
			long long int temp = static_cast<long long int>(n);
			positive = (n >= 0);
			for(int i = SIZE - 1;i > 2;i--){
				a[i] = 0;
			}
			a[2] = static_cast<int>((temp / 1000000000000000000) * (temp < 0 ? -1 : 1));
			a[1] = static_cast<int>((temp % 1000000000000000000 / 1000000000) * (temp < 0 ? -1 : 1));
			a[0] = static_cast<int>((temp % 1000000000) * (temp < 0 ? -1 : 1));
		}else{
			unsigned long long int temp = static_cast<long long int>(n);
			positive = true;
			for(int i = SIZE - 1;i > 2;i--){
				a[i] = 0;
			}
			a[2] = static_cast<int>(temp / 1000000000000000000);
			a[1] = static_cast<int>(temp % 1000000000000000000 / 1000000000);
			a[0] = static_cast<int>(temp % 1000000000);
		}
	}else{
		stringstream ss;
		ss << n;
		string str = ss.str();
		PASS_BY_STRING(str);
	}
	return (*this);
}
const BigNumber BigNumber::abs() const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = (*this);
	temp.positive = true;
	return temp;
}
const BigNumber& BigNumber::operator += (const BigNumber& n){

	#ifdef _BIG_NUMBER_DYNAMIC_
	coresize(n);

	#endif
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
template <typename T>
const BigNumber& BigNumber::operator += (const T& temp){

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber n = temp;
	(*this) += n;
	return (*this);
}
const BigNumber& BigNumber::operator ++ (){
	(*this) += 1;
	return (*this);
}
const BigNumber BigNumber::operator ++ (int null){
	BigNumber tmp = (*this);
	++(*this);
	return tmp;
}
const BigNumber BigNumber::operator + (const BigNumber& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = (*this);
	temp += n;
	return temp;
}
template<typename T>
const BigNumber BigNumber::operator + (const T& tmp) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = (*this), n = tmp;
	temp += n;
	return temp;
}
const BigNumber& BigNumber::operator -= (const BigNumber& n){

	#ifdef _BIG_NUMBER_DYNAMIC_
	coresize(n);

	#endif
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
template <typename T>
const BigNumber& BigNumber::operator -= (const T& temp){

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber n = temp;
	(*this) -= n;
	return (*this);
}
const BigNumber& BigNumber::operator -- (){
	(*this) -= 1;
	return (*this);
}
const BigNumber BigNumber::operator -- (int null){
	BigNumber tmp = (*this);
	--(*this);
	return tmp;
}
const BigNumber BigNumber::operator - (const BigNumber& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = (*this);
	temp -= n;
	return temp;
}
template <typename T>
const BigNumber BigNumber::operator - (const T& tmp) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = (*this), n = tmp;
	temp -= n;
	return temp;
}
const BigNumber& BigNumber::operator *= (const BigNumber& n){
	if(getSize() < n.getSize()){
		BigNumber tmp = n;
		tmp *= (*this);
		(*this) = tmp;
		return (*this);
	}

	#ifdef _BIG_NUMBER_DYNAMIC_
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

	#else
	be_divided = 0;
	divide = 0;
	bool solution_positive = (positive == n.positive);
	if(digit() + n.digit() > 9 * SIZE){
		BigBigNumber base = (*this);
		BigBigNumber sol = 0;
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
		if(HI != 0){
			cout << "Overflow when multiplying!" << endl << "save the last " << 9 * SIZE << " digit only." << endl;
			string_overflow = true;
		} //overflow alerk
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

	#endif
	return (*this);
}
template <typename T>
const BigNumber& BigNumber::operator *= (const T& n){

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = n;
	(*this) *= temp;
	return (*this);
}
const BigNumber BigNumber::operator * (const BigNumber& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = (*this);
	temp *= n;
	return temp;
}
template <typename T>
const BigNumber BigNumber::operator * (const T& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = n, tmp = (*this);
	tmp *= temp;
	return tmp;
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

	#ifdef _BIG_NUMBER_DYNAMIC_
	coresize(n);

	#endif
	if(divide == n && be_divided == (*this)){
		(*this) = LO;
		return (*this);
	}
	COMMON_DIVIDE(n);
	(*this) = LO;
	return (*this);
}
template <typename T>
const BigNumber& BigNumber::operator /= (const T& n){

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = n;
	(*this) /= temp;
	return (*this);
}
const BigNumber BigNumber::operator / (const BigNumber& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = (*this);
	temp /= n;
	return temp;
}
template <typename T>
const BigNumber BigNumber::operator / (const T& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = n, tmp = (*this);
	tmp /= temp;
	return tmp;
}
const BigNumber& BigNumber::operator %= (const BigNumber& n){

	#ifdef _BIG_NUMBER_DYNAMIC_
	coresize(n);

	#endif
	if(divide == n && be_divided == (*this)){
		(*this) = HI;
		return (*this);
	}
	COMMON_DIVIDE(n);
	(*this) = HI;
	return (*this);
}
template <typename T>
const BigNumber& BigNumber::operator %= (const T& n){

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = n;
	(*this) %= temp;
	return (*this);
}
const BigNumber BigNumber::operator % (const BigNumber& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = (*this);
	temp %= n;
	return temp;
}
template <typename T>
const BigNumber BigNumber::operator % (const T& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = n, tmp = (*this);
	tmp %= temp;
	return tmp;
}
const BigNumber BigNumber::operator ^ (const BigNumber& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	coresize(n);

	#endif
	BigNumber temp;
	if(n < 0){
		temp = 0;
	}else{
		temp = 1;
		HI = 0, LO = 0;
		for(BigNumber i = 1;i <= n;i++){
			temp *= (*this);
			if(HI != 0){
				//overflow
				cout << "Stop the power calculation." << endl;
				break;
			}
		}
	}
	return temp;
}
template <typename T>
const BigNumber BigNumber::operator ^ (const T& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = n;
	return (*this) ^ n;
}
BigNumber::operator long long int() const{
	long long int tmp = 0;
	if((*this) >= LLONG_MAX){
		tmp = LLONG_MAX;
	}else if((*this) <= LLONG_MIN){
		tmp = LLONG_MIN;
	}else{
		tmp += a[0];
		tmp += a[1] * static_cast<long long int>(1000000000);
		tmp += a[2] * 1000000000000000000;
		tmp *= (positive ? 1 : -1);
	}
	return tmp;
}
BigNumber::operator unsigned long long int() const{
	unsigned long long int tmp = 0;
	if((*this) >= ULLONG_MAX){
		tmp = ULLONG_MAX;
	}else if((*this) <= 0){
		tmp = 0;
	}else{
		tmp += a[0];
		tmp += a[1] * static_cast<unsigned long long int>(1000000000);
		tmp += a[2] * 1000000000000000000;
	}
	return tmp;
}
BigNumber::operator long double() const{
	long double tmp = static_cast<long double>(static_cast<long long int>((*this)));
	return tmp;
}

template <typename T>
bool operator == (const T& n, const BigNumber& N){
	return N == n;
}
template <typename T>
bool operator != (const T& n, const BigNumber& N){
	return N != n;
}
template <typename T>
bool operator < (const T& n, const BigNumber& N){
	return N > n;
}
template <typename T>
bool operator <= (const T& n, const BigNumber& N){
	return N >= n;
}
template <typename T>
bool operator > (const T& n, const BigNumber& N){
	return N < n;
}
template <typename T>
bool operator >= (const T& n, const BigNumber& N){
	return N <= n;
}
template <typename T>
const T& operator += (T& n, const BigNumber& N){
	if(is_signed<T>::value){
		n = static_cast<long long int>(N + n);
	}else{
		n = static_cast<unsigned long long int>(N + n);
	}
	return n;
}
template <typename T>
const BigNumber operator + (const T& n, const BigNumber& N){
	BigNumber temp = n;
	return temp + N;
}
template <typename T>
const T& operator -= (T& n, const BigNumber& N){
	if(is_signed<T>::value){
		n = static_cast<long long int>(-N + n);
	}else{
		n = static_cast<unsigned long long int>(-N + n);
	}
	return n;
}
template <typename T>
const BigNumber operator - (const T& n, const BigNumber& N){
	BigNumber temp = n;
	return temp - N;
}
template <typename T>
const T& operator *= (T& n, const BigNumber& N){
	if(is_signed<T>::value){
		n = static_cast<long long int>(N * n);
	}else{
		n = static_cast<unsigned long long int>(N * n);
	}
	return n;
}
template <typename T>
const BigNumber operator * (const T& n, const BigNumber& N){
	BigNumber temp = n;
	return temp * N;
}
template <typename T>
const T& operator /= (T& n, const BigNumber& N){
	if(is_floating_point<T>::value){
		n /= static_cast<long double>(N);
	}else{
		if(is_signed<T>::value){
			n /= static_cast<long long int>(N);
		}else{
			n /= static_cast<unsigned long long int>(N);
		}
	}
	return n;
}
template <typename T>
const T operator / (const T& n, const BigNumber& N){ //special, for floating point
	T temp = n;
	if(is_floating_point<T>::value){
		temp /= static_cast<long double>(N);
	}else{
		if(is_signed<T>::value){
			temp /= static_cast<long long int>(N);
		}else{
			temp /= static_cast<unsigned long long int>(N);
		}
	}
	return temp;
}
template <typename T>
const T& operator %= (T& n, const BigNumber& N){
	BigNumber tmp = n;
	if(is_signed<T>::value){
		n = static_cast<T>(static_cast<long long int>(tmp % N));
	}else{
		n = static_cast<T>(static_cast<unsigned long long int>(tmp % N));
	}
	return n;
}
template <typename T>
const BigNumber operator % (const T& n, const BigNumber& N){
	BigNumber temp = n;
	return temp % N;
}
template <typename T>
const BigNumber operator ^ (const T& n, const BigNumber& N){
	BigNumber temp = n;
	return temp ^ N;
}

ostream& operator << (ostream& os, const BigNumber& n){

	#ifdef _BIG_NUMBER_DYNAMIC_
	n.resize();

	#endif
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

	#ifdef _BIG_NUMBER_DYNAMIC_
	n.resize();

	#endif
	string str;
	is >> str;
	BigNumber temp(str);
	n = temp;
	return is;
}

#ifdef _BIG_NUMBER_STATIC_
void BigBigNumber::PASS_BY_STRING(string str){
	if(str[0] == '-'){
		positive = false;
		str.erase(0, 1);
	}else{
		positive = true;
	}
	while(str.length() > 0 && str[0] == '0'){
		str.erase(0, 1);
	}
	if(str.length() > 9 * SIZE){
		while(str.length() > 9 * SIZE){
			str.erase(0, 1);
		}
	}
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
BigBigNumber::BigBigNumber(string str){
	PASS_BY_STRING(str);
}
template <typename T>
BigBigNumber::BigBigNumber(const T& n){
	stringstream ss;
	ss << n;
	string str = ss.str();
	PASS_BY_STRING(str);
}
BigBigNumber::BigBigNumber(const BigNumber& n){
	for(int i = 0;i < SIZE / 2;i++){
		a[i] = n.a[i];
	}
	for(int i = SIZE / 2;i < SIZE;i++){
		a[i] = 0;
	}
	positive = n.positive;
}
BigBigNumber::BigBigNumber(const BigBigNumber& n){
	for(int i = 0;i < SIZE;i++){
		a[i] = n.a[i];
	}
	positive = n.positive;
}
const BigBigNumber& BigBigNumber::PURE_assignment(const BigBigNumber& n){
	for(int i = 0;i < SIZE;i++){
		a[i] = n.a[i];
	}
}
const BigBigNumber& BigBigNumber::operator = (const BigBigNumber& n){
	PURE_assignment(n);
	positive = n.positive;
	return (*this);
}
template <typename T>
const BigBigNumber& BigBigNumber::operator = (const T& n){
	if(is_scalar<T>::value){ //faster
		if(is_signed<T>::value){
			long long int temp = static_cast<long long int>(n);
			positive = (n >= 0);
			for(int i = SIZE - 1;i > 2;i--){
				a[i] = 0;
			}
			a[2] = static_cast<int>((n / 1000000000000000000) * (n < 0 ? -1 : 1));
			a[1] = static_cast<int>((n % 1000000000000000000 / 1000000000) * (n < 0 ? -1 : 1));
			a[0] = static_cast<int>((n % 1000000000) * (n < 0 ? -1 : 1));
		}else{
			unsigned long long int temp = static_cast<long long int>(n);
			positive = true;
			for(int i = SIZE - 1;i > 2;i--){
				a[i] = 0;
			}
			a[2] = static_cast<int>(n / 1000000000000000000);
			a[1] = static_cast<int>(n % 1000000000000000000 / 1000000000);
			a[0] = static_cast<int>(n % 1000000000);
		}
	}else{
		stringstream ss;
		ss << n;
		string str = ss.str();
		PASS_BY_STRING(str);
	}
	return (*this);
}
const BigBigNumber& BigBigNumber::PURE_ADD_assignment(const BigBigNumber& n){
	for(int i = 0;i < SIZE;i++){
		a[i] += n.a[i];
		if(a[i] >= 1000000000 && i < SIZE - 1){
			a[i + 1] ++;
			a[i] -= 1000000000;
		}
	}
	return (*this);
}
const BigBigNumber& BigBigNumber::PURE_PSEUDOMULTIPLE_assignment(){ //pseudomultiple
	stringstream ss;
	ss << (*this) << 0;
	string str = ss.str();
	BigBigNumber temp(str);
	(*this) = temp;
	return (*this);
}
const BigBigNumber& BigBigNumber::PURE_PSEUDODIVIDE_assignment(){ //pseudodivide
	stringstream ss;
	ss << (*this);
	string str = ss.str();
	str.pop_back();
	BigBigNumber temp(str);
	(*this) = temp;
	return (*this);
}
void BigBigNumber::proliferate(){
	HI.positive = positive;
	LO.positive = positive;
	for(int i = 0;i < SIZE / 2;i++){
		LO.a[i] = a[i];
	}
	for(int i = 0;i < SIZE / 2;i++){
		HI.a[i] = a[BigNumber::SIZE + i];
	}
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

#endif

const BigNumber abs(const BigNumber& n){
	return n.abs();
}

const string convert_to_utf8(const string& str){
	return str;
}
template <typename T>
const string convert_to_utf8(const basic_string<T>& STR){ //precondition: T is wchar_t, char16_t, or char32_t
	size_t len = STR.length();
	string str;
	for(int i = 0;i < len;i++){
		char s1 = 0, s2 = 0, s3 = 0, s4 = 0;
		unsigned int ch = static_cast<unsigned int>(STR[i]);
		if(ch < 128){
			s4 = ch;
			str += s4;
		}else if(ch < 2048){
			s3 = -64 + (ch / 64);
			s4 = -128 + (ch % 64);
			str += s3;
			str += s4;
		}else if(ch < 65536){
			s2 = -32 + (ch / 4096);
			s3 = -128 + (ch % 4096 / 64);
			s4 = -128 + (ch % 4096 % 64);
			str += s2;
			str += s3;
			str += s4;
		}else if(ch < 1114112){
			s1 = -16 + (ch / 262144);
			s2 = -128 + (ch % 262144 / 4096);
			s3 = -128 + (ch % 262144 % 4096 / 64);
			s4 = -128 + (ch % 262144 % 4096 % 64);
			str += s1;
			str += s2;
			str += s3;
			str += s4;
		}
	}
	return str;
}

const string to_string(const string& str){
	return str;
}
template <typename T>
const string to_string(const basic_string<T>& STR){
	return convert_to_utf8(STR);
}

const string BigNumber::str(int notation = 10) const{
	stringstream ss;
	if(notation == 10){
		ss << (*this);
		return ss.str();
	}
	if(!positive && (*this) != 0){
		ss << '-';
	}
	ss << 0;
	char note = (notation != 16) ? ('a' - 1 + notation) : ('x');
	ss << note;
	string temp;
	BigNumber n = abs();
	do{
		BtoI t = n % notation;
		if(t < 10){
			temp += static_cast<char>('0' + t);
		}else{
			temp += static_cast<char>('A' + (t - 10));
		}
		n /= notation;
	}while(n != 0);
	for(int i = temp.length() - 1;i >= 0;i--){
		ss << temp[i];
	}
	return ss.str();
}

const string& operator += (string& str, const BigNumber& n){
	stringstream ss;
	ss << n;
	str += ss.str();
	return str;
}
const string operator "" _s(const char* literal_string){
	string str = literal_string;
	return str;
}
const string operator "" _s(const char* string_values, size_t num_chars){ //C++14 has ""s to use.
	string str = string_values;
	return str;
}
const BigNumber operator "" _b(const char* literal_string){
	BigNumber temp = literal_string;
	return temp;
}
const BigNumber operator "" _b(const char* string_values, size_t num_chars){
	BigNumber temp = string_values;
	return temp;
}

#endif