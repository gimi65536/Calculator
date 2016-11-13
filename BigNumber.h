#ifndef _BIG_NUMBER_
#define _BIG_NUMBER_
#include <iostream>
#include <string>
#include <sstream>
#include <type_traits>
#include <climits>
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

#ifdef _BIG_NUMBER_STATIC_
class BigNumber;
class BigBigNumber;
#endif

class BigNumber{
private:

	#ifdef _BIG_NUMBER_DYNAMIC_
	static int SIZE;
	mutable int size;
	mutable int* a;

	#else
	static const int SIZE = BASIC_SIZE;
	int a[SIZE];
	friend class BigBigNumber;
	
	#endif
	bool positive;

	#ifdef _BIG_NUMBER_DYNAMIC_
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

	#endif
	const BigNumber& PURE_assignment(const BigNumber& n){

		#ifdef _BIG_NUMBER_DYNAMIC_
		coresize(n);
		
		#endif
		for(int i = 0;i < SIZE;i++){
			a[i] = n.a[i];
		}
		return (*this);
	}
	void PASS_BY_STRING(string str){
		if(str.length() > 0 && str[0] == '-'){
			positive = false;
			str.erase(0, 1);
		}else if(str.length() > 0 && str[0] == '+'){
			positive = true;
			str.erase(0, 1);
		}else{
			positive = true;
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
public:
	const BigNumber& operator = (const BigNumber& n){

		#ifdef _BIG_NUMBER_DYNAMIC_
		coresize(n);

		#endif
		PURE_assignment(n);
		positive = n.positive;
		return (*this);
	}
private:
	const BigNumber& PURE_ADD_assignment(const BigNumber& n){

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

			#endif
		}
		return (*this);
	}
	const BigNumber& PURE_MINUS_assignment(const BigNumber& n){

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
	const BigNumber PURE_PSEUDOMULTIPLE_assignment(){ //pseudomultiple

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
	const BigNumber PURE_PSEUDODIVIDE_assignment(){ //pseudodivide

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
	void COMMON_DIVIDE(const BigNumber& n) const;
public:
	friend ostream& operator << (ostream& os, const BigNumber& n);
	friend istream& operator >> (istream& is, BigNumber& n);
	BigNumber(){

		#ifdef _BIG_NUMBER_DYNAMIC_
		size = SIZE;
		a = new int[SIZE];

		#endif
		positive = true;
		for(int i = 0;i < SIZE;i++){
			a[i] = 0;
		}
	}
	BigNumber(string str){

		#ifdef _BIG_NUMBER_DYNAMIC_
		a = nullptr;

		#endif
		PASS_BY_STRING(str);
	}
	template <typename T>
	BigNumber(const T& n){

		#ifdef _BIG_NUMBER_DYNAMIC_
		size = SIZE;
		a = new int[SIZE];

		#endif
		(*this) = n;
	}
	BigNumber(const BigNumber& n){

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

	#ifdef _BIG_NUMBER_DYNAMIC_
	~BigNumber(){
		delete[] a;
	}

	#endif
	size_t digit() const{

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
	int getDigit(int n) const{

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
	operator long long int() const{
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
	operator unsigned long long int() const{
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
	operator long double() const{
		long double tmp = static_cast<long double>(static_cast<long long int>((*this)));
		return tmp;
	}
	template <typename T>
	bool operator == (const T& n) const{

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber temp = n;
		return (*this) == temp;
	}
	bool operator == (const BigNumber& n) const{

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
	bool operator != (const T& n) const{
		return !(*this == n);
	}
	bool operator != (const BigNumber& n) const{
		return !(*this == n);
	}
	bool operator < (const BigNumber& n) const{

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
	bool operator < (const T& n) const{

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber temp = n;
		return (*this) < temp;
	}
	bool operator <= (const BigNumber& n) const{
		return (*this) < n || (*this) == n;
	}
	template <typename T>
	bool operator <= (const T& n) const{
		return (*this) < n || (*this) == n;
	}
	bool operator > (const BigNumber& n) const{

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
	bool operator > (const T& n) const{

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber temp = n;
		return (*this) > temp;
	}
	bool operator >= (const BigNumber& n) const{
		return (*this) > n || (*this) == n;
	}
	template <typename T>
	bool operator >= (const T& n) const{
		return (*this) > n || (*this) == n;
	}
	const BigNumber operator + () const{

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		return (*this);
	}
	const BigNumber operator - () const{

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber temp = (*this);
		temp.positive = !temp.positive;
		return temp;
	}
	template <typename T>
	const BigNumber& operator = (const T& n){

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
	const BigNumber abs() const{

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber temp = (*this);
		temp.positive = true;
		return temp;
	}
	const BigNumber& operator += (const BigNumber& n){

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
	const BigNumber& operator += (const T& temp){

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber n = temp;
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

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber temp = (*this);
		temp += n;
		return temp;
	}
	template<typename T>
	const BigNumber operator + (const T& tmp) const{

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber temp = (*this), n = tmp;
		temp += n;
		return temp;
	}
	const BigNumber& operator -= (const BigNumber& n){

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
	const BigNumber& operator -= (const T& temp){

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber n = temp;
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

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber temp = (*this);
		temp -= n;
		return temp;
	}
	template <typename T>
	const BigNumber operator - (const T& tmp) const{

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber temp = (*this), n = tmp;
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
	template <typename T>
	const BigNumber& operator *= (const T& n){

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber temp = n;
		(*this) *= temp;
		return (*this);
	}
	template <typename T>
	const BigNumber operator * (const T& n) const{

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber temp = n, tmp = (*this);
		tmp *= temp;
		return tmp;
	}
	template <typename T>
	const BigNumber& operator /= (const T& n){

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber temp = n;
		(*this) /= temp;
		return (*this);
	}
	template <typename T>
	const BigNumber operator / (const T& n) const{

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber temp = n, tmp = (*this);
		tmp /= temp;
		return tmp;
	}
	template <typename T>
	const BigNumber& operator %= (const T& n){

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber temp = n;
		(*this) %= temp;
		return (*this);
	}
	template <typename T>
	const BigNumber operator % (const T& n) const{

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber temp = n, tmp = (*this);
		tmp %= temp;
		return tmp;
	}
	template <typename T>
	const BigNumber operator ^ (const T& n) const{

		#ifdef _BIG_NUMBER_DYNAMIC_
		resize();

		#endif
		BigNumber temp = n;
		return (*this) ^ n;
	}

	#ifdef _BIG_NUMBER_DYNAMIC_
	friend void var_resize();

	#endif
};

#ifdef _BIG_NUMBER_DYNAMIC_
int BigNumber::SIZE = BASIC_SIZE;
#endif

BigNumber HI = 0, LO = 0;
static BigNumber be_divided = 0;
static BigNumber divide = 0;

#ifdef _BIG_NUMBER_STATIC_
class BigBigNumber{ //only use in overflow, user cannot access it directly
friend class BigNumber;
private:
	static const int SIZE = 2 * BASIC_SIZE;
	int a[SIZE];
	bool positive;
	void PASS_BY_STRING(string str){
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
			//cout << "Overflow when creating a number with too more digits!" << endl << "save the last " << 9 * SIZE << " digit only." << endl;
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
	friend ostream& operator << (ostream& os, const BigBigNumber& n);
	BigBigNumber(string str){
		PASS_BY_STRING(str);
	}
	template <typename T>
	BigBigNumber(const T& n){
		stringstream ss;
		ss << n;
		string str = ss.str();
		PASS_BY_STRING(str);
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
	template <typename T>
	const BigBigNumber& operator = (const T& n){
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

#endif
const BigNumber& BigNumber::operator *= (const BigNumber& n){

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
const BigNumber BigNumber::operator * (const BigNumber& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
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
const BigNumber BigNumber::operator / (const BigNumber& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = (*this);
	temp /= n;
	return temp;
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
const BigNumber BigNumber::operator % (const BigNumber& n) const{

	#ifdef _BIG_NUMBER_DYNAMIC_
	resize();

	#endif
	BigNumber temp = (*this);
	temp %= n;
	return temp;
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

#ifdef _BIG_NUMBER_STATIC_
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

#endif