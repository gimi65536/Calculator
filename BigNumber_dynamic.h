#ifdef _BIG_NUMBER_DYNAMIC_

constexpr int BASIC_SIZE = 4;
constexpr int DIGIT = 9;
typedef int Int;
constexpr Int IMax = 1000000000;
static_assert(BASIC_SIZE >= 3, "BASIC_SIZE should >= 3.");

typedef long long int BtoI;
typedef unsigned long long int uBtoI;
typedef long double BtoD;
constexpr BtoI IIMax = 1000000000000000000;

const string cvt_string(const string& str);
template <typename T>
const string cvt_string(const basic_string<T>& STR);

class BigNumber;
class BigBigNumber;

typedef BigNumber bnint;

class BigNumber{
friend class BigBigNumber;
private:
	mutable int SIZE;
	mutable Int* a;
	bool positive;
	void resize() const;
	void resize(size_t n) const;
	void coresize(const BigNumber& n) const;
	void cofit(const BigNumber& n) const;
	int getSize() const;
	const BigNumber& PURE_assignment(const BigNumber& n);
	void PASS_BY_STRING(string str);
	void PASS_BY_STRING_with_notation(string str);
	const BigNumber& PURE_ADD_assignment(const BigNumber& n);
	const BigNumber& PURE_MINUS_assignment(const BigNumber& n);
	const BigNumber& PURE_PSEUDOMULTIPLE_assignment();
	const BigNumber& PURE_PSEUDODIVIDE_assignment();
	void COMMON_DIVIDE(const BigNumber& n) const;
	template <typename T>
	void WIDE_CHAR_PASS(const T& ch);
	template <typename T>
	void WIDE_CHARARRAY_PASS(const T* C_STR);
	int is_ten(const BigNumber& n, bool strict);
public:
	friend ostream& operator << (ostream& os, const BigNumber& n);
	friend istream& operator >> (istream& is, BigNumber& n);
	BigNumber();
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
	BigNumber operator + () const;
	BigNumber operator - () const;
	const BigNumber& operator = (const BigNumber& n);
	const BigNumber& operator = (const string& str);
	template <typename T>
	const BigNumber& operator = (const basic_string<T>& STR);
	const BigNumber& operator = (const char& ch);
	const BigNumber& operator = (char* const n);
	const BigNumber& operator = (const char* const n);
	const BigNumber& operator = (const wchar_t& ch);
	const BigNumber& operator = (wchar_t* const n);
	const BigNumber& operator = (const wchar_t* const n);
	const BigNumber& operator = (const char16_t& ch);
	const BigNumber& operator = (char16_t* const n);
	const BigNumber& operator = (const char16_t* const n);
	const BigNumber& operator = (const char32_t& ch);
	const BigNumber& operator = (char32_t* const n);
	const BigNumber& operator = (const char32_t* const n);
	template <typename T>
	const BigNumber& operator = (const T& n);
	template <typename T>
	const BigNumber& operator = (T* const n);
	BigNumber abs() const;
	BigNumber abs_inverse() const;
	const BigNumber& operator += (const BigNumber& n);
	template <typename T>
	const BigNumber& operator += (const T& n);
	const BigNumber& operator ++ ();
	BigNumber operator ++ (int null);
	BigNumber operator + (const BigNumber& n) const;
	template<typename T>
	BigNumber operator + (const T& n) const;
	const BigNumber& operator -= (const BigNumber& n);
	template <typename T>
	const BigNumber& operator -= (const T& n);
	const BigNumber& operator -- ();
	BigNumber operator -- (int null);
	BigNumber operator - (const BigNumber& n) const;
	template <typename T>
	BigNumber operator - (const T& n) const;
	const BigNumber& operator *= (const BigNumber& n);
	template <typename T>
	const BigNumber& operator *= (const T& n);
	BigNumber operator * (const BigNumber& n) const;
	template <typename T>
	BigNumber operator * (const T& n) const;
	const BigNumber& operator /= (const BigNumber& n);
	template <typename T>
	const BigNumber& operator /= (const T& n);
	BigNumber operator / (const BigNumber& n) const;
	template <typename T>
	BigNumber operator / (const T& n) const;
	const BigNumber& operator %= (const BigNumber& n);
	template <typename T>
	const BigNumber& operator %= (const T& n);
	BigNumber operator % (const BigNumber& n) const;
	template <typename T>
	BigNumber operator % (const T& n) const;
	const BigNumber& operator ^= (const BigNumber& n);
	template <typename T>
	const BigNumber& operator ^= (const T& n);
	BigNumber operator ^ (const BigNumber& n) const;
	template <typename T>
	BigNumber operator ^ (const T& n) const;
	const string str(int notation = 10) const;
	size_t Sizeof() const{return SIZE;}
	void print() const{cout << (*this);}
	bool is_zero() const;
	bool get_positive() const{return positive;}
	void negate(){positive = !positive;}
	void divide2();
	operator long long int() const;
	operator unsigned long long int() const;
	operator long double() const;

	void coresize(const BigNumber&& n) const;
	void cofit(const BigNumber&& n) const;
	const BigNumber& PURE_assignment(BigNumber&& n);
	const BigNumber& PURE_ADD_assignment(const BigNumber&& n);
	const BigNumber& PURE_MINUS_assignment(const BigNumber&& n);
	BigNumber(BigNumber&& n);
	bool operator == (const BigNumber&& n) const;
	bool operator != (const BigNumber&& n) const;
	bool operator < (const BigNumber&& n) const;
	bool operator <= (const BigNumber&& n) const;
	bool operator > (const BigNumber&& n) const;
	bool operator >= (const BigNumber&& n) const;
	const BigNumber& operator = (BigNumber&& n);
	const BigNumber& operator += (BigNumber&& n);
	BigNumber operator + (BigNumber&& n) const;
	const BigNumber& operator -= (BigNumber&& n);
	BigNumber operator - (BigNumber&& n) const;
	const BigNumber& operator *= (BigNumber&& n);
	BigNumber operator * (BigNumber&& n) const;
};

BigNumber HI = 0, LO = 0;
static BigNumber be_divided = 0;
static BigNumber divide = 0;

void BigNumber::resize() const{ //fit
	if(getSize() < SIZE && getSize() > BASIC_SIZE){
		size_t target_size = getSize();
		Int* tmp = new Int[target_size];
		for(int i = 0;i < target_size;i++){
			tmp[i] = a[i];
		}
		delete[] a;
		SIZE = target_size;
		a = tmp;
	}
}
void BigNumber::resize(size_t n) const{ //large
	if(n > BASIC_SIZE && n > SIZE){
		Int* tmp = new Int[n];
		for(int i = 0;i < SIZE;i++){
			tmp[i] = a[i];
		}
		for(int i = SIZE;i < n;i++){
			tmp[i] = 0;
		}
		delete[] a;
		SIZE = n;
		a = tmp;
	}
}
void BigNumber::coresize(const BigNumber& n) const{
	cofit(n);
	size_t target_size = (SIZE > n.SIZE) ? SIZE : n.SIZE;
	resize(target_size);
	n.resize(target_size);
}
void BigNumber::cofit(const BigNumber& n) const{
	resize();
	n.resize();
}
int BigNumber::getSize() const{
	for(int i = SIZE - 1;i >= 0;i--){
		if(a[i] != 0){
			return i + 1;
		}
	}
	return 1;
}
const BigNumber& BigNumber::PURE_assignment(const BigNumber& n){
	if(this == &n){
		return (*this);
	}
	resize(n.SIZE);
	for(int i = 0;i < SIZE && i < n.SIZE;i++){
		a[i] = n.a[i];
	}
	for(int i = n.SIZE;i < SIZE;i++){
		a[i] = 0;
	}
	resize();
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
	}else if(str.length() > 0 && str[0] == '+'){
		positive = true;
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
	SIZE = BASIC_SIZE;
	if(str.length() > DIGIT * SIZE){
		SIZE = (str.length() - 1) / 9 + 1;
	}
	delete[] a;
	a = new Int[SIZE];
	for(int i = 0;i < SIZE;i++){
		a[i] = 0;
	}
	if(str.length() == 0 || str.find_first_of("0123456789") == string::npos){
		return;
	}
	stringstream ss;
	int t = (str.length() - 1) / DIGIT;
	for(int i = 0;i < t;i++){
		string sub = str.substr(str.length() - DIGIT, DIGIT);
		ss << sub;
		ss >> a[i];
		ss.clear();
		ss.str("");
		str = str.erase(str.length() - DIGIT, DIGIT);
	}
	ss << str;
	ss >> a[t];
}
void BigNumber::PASS_BY_STRING_with_notation(string str){
	bool remain_positive = true;
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
	SIZE = BASIC_SIZE;
	delete[] a;
	a = new Int[SIZE];
	for(int i = 0;i < SIZE;i++){
		a[i] = 0;
	}
	for(int i = 0;i < str.length();i++){
		BigNumber num;
		if(isdigit(str[i])){
			num = str[i];
		}else{
			num = static_cast<BtoI>(str[i] - 'A' + 10);
		}
		(*this) = num + ((*this) * notation);
	}
	positive = remain_positive;
}
const BigNumber& BigNumber::PURE_ADD_assignment(const BigNumber& n){
	coresize(n);
	for(int i = 0;i < SIZE;i++){
		a[i] += n.a[i];
		if(a[i] >= IMax && i < SIZE - 1){
			a[i + 1] ++;
			a[i] -= IMax;
		}
	}
	if(a[SIZE - 1] >= IMax){
		int temp = SIZE;
		resize(SIZE + 1);
		a[temp - 1] -= IMax;
		a[temp] ++;
	}
	cofit(n);
	return (*this);
}
const BigNumber& BigNumber::PURE_MINUS_assignment(const BigNumber& n){
	coresize(n);
	for(int i = 0;i < SIZE;i++){
		a[i] -= n.a[i];
		if(a[i] < 0 && i < SIZE - 1){
			a[i + 1] --;
			a[i] += IMax;
		}
	}
	cofit(n);
	return (*this);
}
const BigNumber& BigNumber::PURE_PSEUDOMULTIPLE_assignment(){ //pseudomultiple
	stringstream ss;
	ss << (*this) << 0;
	string str = ss.str();
	BigNumber temp = str;
	(*this) = temp;
	return (*this);
}
const BigNumber& BigNumber::PURE_PSEUDODIVIDE_assignment(){ //pseudodivide
	stringstream ss;
	ss << (*this);
	string str = ss.str();
	str.pop_back();
	BigNumber temp = str;
	(*this) = temp;
	return (*this);
}
template <typename T>
void BigNumber::WIDE_CHAR_PASS(const T& ch){
	basic_string<T> STR;
	STR += ch;
	PASS_BY_STRING(cvt_string(STR));
}
template <typename T>
void BigNumber::WIDE_CHARARRAY_PASS(const T* C_STR){
	basic_string<T> STR = C_STR;
	PASS_BY_STRING(cvt_string(STR));
}
int BigNumber::is_ten(const BigNumber& n, bool strict = false){
	if(is_zero()){
		return -1;
	}
	string str = n.str();
	if(str[0] == '-'){
		str.erase(0, 1);
	}
	if(strict){
		if(str.find_last_not_of("0") != 0 || str[0] != 1){
			return -1;
		}
		return str.length() - 1;
	}else{
		int count = 0;
		for(int i = str.length() - 1;i >= 0;i--){
			if(str[i] == '0'){
				count ++;
			}else{
				break;
			}
		}
		if(count == 0){
			return -1;
		}
		return count;
	}
}
BigNumber::BigNumber(){
	SIZE = BASIC_SIZE;
	a = new Int[SIZE];
	positive = true;
	for(int i = 0;i < SIZE;i++){
		a[i] = 0;
	}
}
template <typename T>
BigNumber::BigNumber(const T& n){
	a = nullptr;
	(*this) = n;
}
BigNumber::BigNumber(const BigNumber& n){
	if(this == &n){
		return;
	}
	SIZE = n.SIZE;
	a = new Int[SIZE];
	for(int i = 0;i < SIZE;i++){
		a[i] = n.a[i];
	}
	positive = n.positive;
}

BigNumber::~BigNumber(){
	delete[] a;
}

size_t BigNumber::digit() const{
	stringstream ss;
	ss << (*this);
	string str = ss.str();
	if(str[0] == '-'){
		str.erase(0, 1);
	}
	return str.length();
}
int BigNumber::getDigit(int n) const{
	resize();
	if(n <= 0 || n > DIGIT * SIZE){
		return -1;
	}
	int index = (n - 1) / DIGIT;
	int target = a[index];
	n = (n - 1) % DIGIT;
	for(int i = 1;i <= n;i++){
		target /= 10;
	}
	return target % 10;
}
template <typename T>
bool BigNumber::operator == (const T& n) const{
	BigNumber temp = n;
	return (*this) == temp;
}
bool BigNumber::operator == (const BigNumber& n) const{
	coresize(n);
	bool zero = true;
	for(int i = 0;i < SIZE;i++){
		if(a[i] != n.a[i]){
			cofit(n);
			return false;
		}
		if(a[i] != 0){
			zero = false;
		}
	}
	cofit(n);
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
	cofit(n);
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
	cofit(n);
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
BigNumber BigNumber::operator + () const{
	return (*this);
}
BigNumber BigNumber::operator - () const{
	BigNumber temp = (*this);
	temp.positive = !temp.positive;
	return temp;
}
const BigNumber& BigNumber::operator = (const BigNumber& n){
	if(this != &n){
		PURE_assignment(n);
		positive = n.positive;
	}
	return (*this);
}
const BigNumber& BigNumber::operator = (const string& str){
	PASS_BY_STRING(str);
	return (*this);
}
template <typename T>
const BigNumber& BigNumber::operator = (const basic_string<T>& STR){
	PASS_BY_STRING(cvt_string(STR));
	return (*this);
}
const BigNumber& BigNumber::operator = (const char& ch){
	string str;
	str += ch;
	PASS_BY_STRING(str);
	return (*this);
}
const BigNumber& BigNumber::operator = (char* const n){
	string str = n;
	PASS_BY_STRING(str);
	return (*this);
}
const BigNumber& BigNumber::operator = (const char* const n){
	string str = n;
	PASS_BY_STRING(str);
	return (*this);
}
const BigNumber& BigNumber::operator = (const wchar_t& ch){
	WIDE_CHAR_PASS(ch);
	return (*this);
}
const BigNumber& BigNumber::operator = (wchar_t* const n){
	WIDE_CHARARRAY_PASS(n);
	return (*this);
}
const BigNumber& BigNumber::operator = (const wchar_t* const n){
	WIDE_CHARARRAY_PASS(n);
	return (*this);
}
const BigNumber& BigNumber::operator = (const char16_t& ch){
	WIDE_CHAR_PASS(ch);
	return (*this);
}
const BigNumber& BigNumber::operator = (char16_t* const n){
	WIDE_CHARARRAY_PASS(n);
	return (*this);
}
const BigNumber& BigNumber::operator = (const char16_t* const n){
	WIDE_CHARARRAY_PASS(n);
	return (*this);
}
const BigNumber& BigNumber::operator = (const char32_t& ch){
	WIDE_CHAR_PASS(ch);
	return (*this);
}
const BigNumber& BigNumber::operator = (char32_t* const n){
	WIDE_CHARARRAY_PASS(n);
	return (*this);
}
const BigNumber& BigNumber::operator = (const char32_t* const n){
	WIDE_CHARARRAY_PASS(n);
	return (*this);
}
template <typename T>
const BigNumber& BigNumber::operator = (const T& n){
	SIZE = BASIC_SIZE;
	delete[] a;
	a = new Int[SIZE];
	if(is_scalar<T>::value){ //faster
		for(int i = SIZE - 1;i > 2;i--){
			a[i] = 0;
		}
		if(is_signed<T>::value){
			long long int temp = static_cast<long long int>(n);
			positive = (n >= 0);
			a[2] = static_cast<int>((temp / IIMax) * (temp < 0 ? -1 : 1));
			a[1] = static_cast<int>((temp % IIMax / IMax) * (temp < 0 ? -1 : 1));
			a[0] = static_cast<int>((temp % IMax) * (temp < 0 ? -1 : 1));
		}else{
			unsigned long long int temp = static_cast<long long int>(n);
			positive = true;
			a[2] = static_cast<int>(temp / IIMax);
			a[1] = static_cast<int>(temp % IIMax / IMax);
			a[0] = static_cast<int>(temp % IMax);
		}
	}else{
		stringstream ss;
		ss << n;
		string str = ss.str();
		PASS_BY_STRING(str);
	}
	return (*this);
}
template <typename T>
const BigNumber& BigNumber::operator = (T* const n){
	ptrdiff_t temp = reinterpret_cast<ptrdiff_t>(n);
	(*this) = temp;
	return (*this);
}
BigNumber BigNumber::abs() const{
	BigNumber temp = (*this);
	temp.positive = true;
	return temp;
}
BigNumber BigNumber::abs_inverse() const{
	BigNumber temp = (*this);
	temp.positive = false;
	return temp;
}
const BigNumber& BigNumber::operator += (const BigNumber& n){
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
const BigNumber& BigNumber::operator += (const T& n){
	BigNumber temp = n;
	(*this) += temp;
	return (*this);
}
const BigNumber& BigNumber::operator ++ (){
	(*this) += 1;
	return (*this);
}
BigNumber BigNumber::operator ++ (int null){
	BigNumber tmp = (*this);
	++(*this);
	return tmp;
}
BigNumber BigNumber::operator + (const BigNumber& n) const{
	BigNumber temp = (*this);
	temp += n;
	return temp;
}
template<typename T>
BigNumber BigNumber::operator + (const T& n) const{
	BigNumber temp = (*this), tmp = n;
	temp += tmp;
	return temp;
}
const BigNumber& BigNumber::operator -= (const BigNumber& n){
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
const BigNumber& BigNumber::operator -= (const T& n){
	BigNumber temp = n;
	(*this) -= temp;
	return (*this);
}
const BigNumber& BigNumber::operator -- (){
	(*this) -= 1;
	return (*this);
}
BigNumber BigNumber::operator -- (int null){
	BigNumber tmp = (*this);
	--(*this);
	return tmp;
}
BigNumber BigNumber::operator - (const BigNumber& n) const{
	BigNumber temp = (*this);
	temp -= n;
	return temp;
}
template <typename T>
BigNumber BigNumber::operator - (const T& n) const{
	BigNumber temp = (*this), tmp = n;
	temp -= tmp;
	return temp;
}
const BigNumber& BigNumber::operator *= (const BigNumber& n){
	static constexpr int inteval = 700;
	if(n.is_zero()){
		(*this) = n;
		return (*this);
	}
	if(is_ten(n) != -1 && n.getSize() - getSize() < inteval){
		int time = is_ten(n);
		BigNumber tmp = n;
		for(int i = 1;i <= time;i++){
			PURE_PSEUDOMULTIPLE_assignment();
			tmp.PURE_PSEUDODIVIDE_assignment();
		}
		(*this) *= tmp;
		return (*this);
	}
	if(getSize() < n.getSize()){
		BigNumber tmp = n;
		tmp *= (*this);
		(*this) = tmp;
		return (*this);
	}
	coresize(n);
	bool solution_positive = (positive == n.positive);
	if(digit() + n.digit() > DIGIT * SIZE){
		resize(getSize() + n.getSize());
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
	resize();
	return (*this);
}
template <typename T>
const BigNumber& BigNumber::operator *= (const T& n){
	BigNumber temp = n;
	(*this) *= temp;
	return (*this);
}
BigNumber BigNumber::operator * (const BigNumber& n) const{
	BigNumber temp = (*this);
	temp *= n;
	return temp;
}
template <typename T>
BigNumber BigNumber::operator * (const T& n) const{
	BigNumber temp = (*this), tmp = n;
	temp *= tmp;
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
	HI.coresize(LO);
}
const BigNumber& BigNumber::operator /= (const BigNumber& n){
	if(divide == n && be_divided == (*this)){
		(*this) = LO;
		return (*this);
	}
	if(is_ten(n, true) != -1){
		int time = is_ten(n, true);
		for(int i = 1;i <= time;i++){
			PURE_PSEUDODIVIDE_assignment();
		}
		if(!n.positive){
			positive = !positive;
		}
		return (*this);
	}
	COMMON_DIVIDE(n);
	(*this) = LO;
	return (*this);
}
template <typename T>
const BigNumber& BigNumber::operator /= (const T& n){
	BigNumber temp = n;
	(*this) /= temp;
	return (*this);
}
BigNumber BigNumber::operator / (const BigNumber& n) const{
	BigNumber temp = (*this);
	temp /= n;
	return temp;
}
template <typename T>
BigNumber BigNumber::operator / (const T& n) const{
	BigNumber temp = (*this), tmp = n;
	temp /= tmp;
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
template <typename T>
const BigNumber& BigNumber::operator %= (const T& n){
	BigNumber temp = n;
	(*this) %= temp;
	return (*this);
}
BigNumber BigNumber::operator % (const BigNumber& n) const{
	BigNumber temp = (*this);
	temp %= n;
	return temp;
}
template <typename T>
BigNumber BigNumber::operator % (const T& n) const{
	BigNumber temp = n, tmp = (*this);
	tmp %= temp;
	return tmp;
}
const BigNumber& BigNumber::operator ^= (const BigNumber& n){
	if(n < 0){
		(*this) = 0;
		return (*this);
	}else if(n == 0){
		(*this) = 1;
		return (*this);
	}else if(n == 1){
		return (*this);
	}
	BigNumber base = (*this);
	for(BigNumber i = 2;i <= n;i++){
		(*this) *= base;
	}
	return (*this);
}
template <typename T>
const BigNumber& BigNumber::operator ^= (const T& n){
	BigNumber temp = n;
	(*this) ^= n;
	return (*this);
}
BigNumber BigNumber::operator ^ (const BigNumber& n) const{
	BigNumber temp = (*this);
	temp ^= n;
	return temp;
}
template <typename T>
BigNumber BigNumber::operator ^ (const T& n) const{
	BigNumber temp = n;
	return (*this) ^ temp;
}
bool BigNumber::is_zero() const{
	for(int i = 0;i < SIZE;i++){
		if(a[i] != 0){
			return false;
		}
	}
	return true;
}
void BigNumber::divide2(){
	a[0] /= 2;
	for(int i = 1;i < SIZE;i++){
		if(a[i] % 2 != 0){
			a[i - 1] += IMax / 2;
		}
		a[i] /= 2;
	}
}
BigNumber::operator long long int() const{
	long long int tmp = 0;
	if((*this) >= LLONG_MAX){
		tmp = LLONG_MAX;
	}else if((*this) <= LLONG_MIN){
		tmp = LLONG_MIN;
	}else{
		tmp += a[0];
		tmp += a[1] * static_cast<long long int>(IMax);
		tmp += a[2] * IIMax;
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
		tmp += a[1] * static_cast<unsigned long long int>(IMax);
		tmp += a[2] * IIMax;
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
typename conditional<is_floating_point<T>::value, T, BigNumber>::type operator / (const T& n, const BigNumber& N){
	T temp = n;
	return temp / static_cast<typename conditional<is_floating_point<T>::value, long double, BigNumber>::type>(N);
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
	for(int i = n.SIZE - 1;i >= 0;i--){
		if(n.a[i] != 0){
			if(!n.positive){
				os << '-';
			}
			os << n.a[i];
			for(int j = i - 1;j >= 0;j--){
				os.width(DIGIT);
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
	string str;
	is >> str;
	BigNumber temp(str);
	n = temp;
	return is;
}

const BigNumber abs(const BigNumber& n){
	return n.abs();
}

const BigNumber abs_inverse(const BigNumber& n){
	return n.abs_inverse();
}

size_t Sizeof(const BigNumber& n){
	return n.Sizeof();
}

void print(const BigNumber& n){
	cout << n;
}

bool is_zero(const BigNumber& n){
	return n.is_zero();
}

bool get_positive(const BigNumber& n){
	return n.get_positive();
}

void negate(BigNumber& n){
	n.negate();
}

void Typeof(const BigNumber& n){
	cout << "BigNumber";
}

void BigNumber::coresize(const BigNumber&& n) const{
	cofit(move(n));
}
void BigNumber::cofit(const BigNumber&& n) const{
	resize();
}
const BigNumber& BigNumber::PURE_assignment(BigNumber&& n){
	if(this != &n){
		a = n.a;
		n.a = nullptr;
	}
	return (*this);
}
const BigNumber& BigNumber::PURE_ADD_assignment(const BigNumber&& n){
	coresize(n); //as lvalue
	for(int i = 0;i < SIZE;i++){
		a[i] += n.a[i];
		if(a[i] >= IMax && i < SIZE - 1){
			a[i + 1] ++;
			a[i] -= IMax;
		}
	}
	if(a[SIZE - 1] >= IMax){
		int temp = SIZE;
		resize(SIZE + 1);
		a[temp - 1] -= IMax;
		a[temp] ++;
	}
	cofit(move(n));
	return (*this);
}
const BigNumber& BigNumber::PURE_MINUS_assignment(const BigNumber&& n){
	coresize(n); //as lvalue
	for(int i = 0;i < SIZE;i++){
		a[i] -= n.a[i];
		if(a[i] < 0 && i < SIZE - 1){
			a[i + 1] --;
			a[i] += IMax;
		}
	}
	cofit(move(n));
	return (*this);
}
BigNumber::BigNumber(BigNumber&& n){
	if(this != &n){
		SIZE = n.SIZE;
		a = n.a;
		n.a = nullptr;
		positive = n.positive;
	}
}
bool BigNumber::operator == (const BigNumber&& n) const{
	coresize(n); //as lvalue
	bool zero = true;
	for(int i = 0;i < SIZE;i++){
		if(a[i] != n.a[i]){
			cofit(move(n));
			return false;
		}
		if(a[i] != 0){
			zero = false;
		}
	}
	cofit(move(n));
	return zero || (positive == n.positive); //if 0, don't ask +-
}
bool BigNumber::operator != (const BigNumber&& n) const{
	return !(*this == move(n));
}
bool BigNumber::operator < (const BigNumber&& n) const{
	coresize(n); //as lvalue
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
	cofit(move(n));
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
bool BigNumber::operator <= (const BigNumber&& n) const{
	return (*this) < move(n) || (*this) == move(n);
}
bool BigNumber::operator > (const BigNumber&& n) const{
	coresize(n); //as lvalue
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
	cofit(move(n));
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
bool BigNumber::operator >= (const BigNumber&& n) const{
	return (*this) > move(n) || (*this) == move(n);
}
const BigNumber& BigNumber::operator = (BigNumber&& n){
	if(this != &n){
		SIZE = n.SIZE;
		a = n.a;
		n.a = nullptr;
		positive = n.positive;
	}
	return (*this);
}
const BigNumber& BigNumber::operator += (BigNumber&& n){
	if(positive == n.positive){ //straightly add
		PURE_ADD_assignment(move(n));
	}else if(abs() >= n.abs()){
		PURE_MINUS_assignment(move(n));
	}else{
		const BigNumber temp = (*this);
		(*this) = move(n);
		PURE_MINUS_assignment(temp);
	}
	return (*this);
}
BigNumber BigNumber::operator + (BigNumber&& n) const{
	BigNumber temp = (*this);
	temp += move(n);
	return temp;
}
const BigNumber& BigNumber::operator -= (BigNumber&& n){
	if(positive != n.positive){ //straightly add
		PURE_ADD_assignment(move(n));
	}else if(abs() >= n.abs()){
		PURE_MINUS_assignment(move(n));
	}else{
		const BigNumber temp = (*this);
		(*this) = -move(n);
		PURE_MINUS_assignment(temp);
	}
	return (*this);
}
BigNumber BigNumber::operator - (BigNumber&& n) const{
	BigNumber temp = (*this);
	temp -= move(n);
	return temp;
}
const BigNumber& BigNumber::operator *= (BigNumber&& n){
	static constexpr int inteval = 700;
	if(n.is_zero()){
		(*this) = n;
		return (*this);
	}
	if(is_ten(n) != -1 && n.getSize() - getSize() < inteval){
		int time = is_ten(n);
		BigNumber tmp = move(n);
		for(int i = 1;i <= time;i++){
			PURE_PSEUDOMULTIPLE_assignment();
			tmp.PURE_PSEUDODIVIDE_assignment();
		}
		(*this) *= tmp;
		return (*this);
	}
	if(getSize() < n.getSize()){
		BigNumber tmp = move(n);
		tmp *= (*this);
		(*this) = tmp;
		return (*this);
	}
	coresize(n); //as lvalue
	bool solution_positive = (positive == n.positive);
	if(digit() + n.digit() > DIGIT * SIZE){
		resize(getSize() + n.getSize());
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
	resize();
	return (*this);
}
BigNumber BigNumber::operator * (BigNumber&& n) const{
	BigNumber temp = (*this);
	temp *= move(n);
	return temp;
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

const string cvt_string(const string& str){
	return str;
}
template <typename T>
const string cvt_string(const basic_string<T>& STR){
	return convert_to_utf8(STR);
}

const string BigNumber::str(int notation) const{
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
const string operator "" _s(const char literal){
	string str;
	str += literal;
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
const string operator "" _s(const wchar_t literal){
	wstring STR;
	STR += literal;
	return cvt_string(STR);
}
const string operator "" _s(const wchar_t* string_values, size_t num_chars){ //C++14 has ""s to use.
	wstring STR = string_values;
	return cvt_string(STR);
}
const string operator "" _s(const char16_t literal){
	u16string STR;
	STR += literal;
	return cvt_string(STR);
}
const string operator "" _s(const char16_t* string_values, size_t num_chars){ //C++14 has ""s to use.
	u16string STR = string_values;
	return cvt_string(STR);
}
const string operator "" _s(const char32_t literal){
	u32string STR;
	STR += literal;
	return cvt_string(STR);
}
const string operator "" _s(const char32_t* string_values, size_t num_chars){ //C++14 has ""s to use.
	u32string STR = string_values;
	return cvt_string(STR);
}
const BigNumber operator "" _b(const char literal){
	BigNumber temp = literal;
	return temp;
}
const BigNumber operator "" _b(const char* literal_string){
	BigNumber temp = literal_string;
	return temp;
}
const BigNumber operator "" _b(const char* string_values, size_t num_chars){
	BigNumber temp = string_values;
	return temp;
}
const BigNumber operator "" _b(const wchar_t literal){
	BigNumber temp = literal;
	return temp;
}
const BigNumber operator "" _b(const wchar_t* string_values, size_t num_chars){
	BigNumber temp = string_values;
	return temp;
}
const BigNumber operator "" _b(const char16_t literal){
	BigNumber temp = literal;
	return temp;
}
const BigNumber operator "" _b(const char16_t* string_values, size_t num_chars){
	BigNumber temp = string_values;
	return temp;
}
const BigNumber operator "" _b(const char32_t literal){
	BigNumber temp = literal;
	return temp;
}
const BigNumber operator "" _b(const char32_t* string_values, size_t num_chars){
	BigNumber temp = string_values;
	return temp;
}
const BigNumber operator "" _bnint(const char literal){
	BigNumber temp = literal;
	return temp;
}
const BigNumber operator "" _bnint(const char* literal_string){
	BigNumber temp = literal_string;
	return temp;
}
const BigNumber operator "" _bnint(const char* string_values, size_t num_chars){
	BigNumber temp = string_values;
	return temp;
}
const BigNumber operator "" _bnint(const wchar_t literal){
	BigNumber temp = literal;
	return temp;
}
const BigNumber operator "" _bnint(const wchar_t* string_values, size_t num_chars){
	BigNumber temp = string_values;
	return temp;
}
const BigNumber operator "" _bnint(const char16_t literal){
	BigNumber temp = literal;
	return temp;
}
const BigNumber operator "" _bnint(const char16_t* string_values, size_t num_chars){
	BigNumber temp = string_values;
	return temp;
}
const BigNumber operator "" _bnint(const char32_t literal){
	BigNumber temp = literal;
	return temp;
}
const BigNumber operator "" _bnint(const char32_t* string_values, size_t num_chars){
	BigNumber temp = string_values;
	return temp;
}

#endif