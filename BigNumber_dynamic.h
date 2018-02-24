#ifdef _BIG_NUMBER_DYNAMIC_

constexpr size_t BASIC_SIZE = 4;
constexpr int DIGIT = 9;
typedef int Int;
constexpr Int NOTATION = 10;
constexpr Int IMax = 1'000'000'000;
static_assert(BASIC_SIZE >= 3, "BASIC_SIZE should be larger than 3.");
static_assert([](){
	Int tmp = 1;
	for(int i = 0;i < DIGIT;i++){
		tmp *= NOTATION;
	}
	return tmp;
}() == IMax, "NOTATION ^ DIGIT should equal to IMax.");

typedef intmax_t BtoI;
typedef uintmax_t uBtoI;
typedef long double BtoD;
//constexpr BtoI IIMax = 1'000'000'000'000'000'000;

constexpr auto BtoItimes = [](){
	intmax_t sol = 0, a = INTMAX_MAX;
	while(a > 0){
		sol++;
		a /= IMax;
	}
	return sol;
}();
constexpr auto uBtoItimes = [](){
	uintmax_t sol = 0, a = UINTMAX_MAX;
	while(a > 0){
		sol++;
		a /= IMax;
	}
	return sol;
}();
static_assert(BtoItimes >= 3, "BtoI should be able to contain at least 2 full array position of BigNumber::a.");

const string cvt_string(const string& str);
template <typename T>
const string cvt_string(const basic_string<T>& STR);

class BigNumber;

typedef BigNumber bnint;

class BigNumber{
private:
	//static BtoI m[2];
	size_t SIZE;
	size_t capacity;
	Int* a;
	bool positive;
	void resize() noexcept;
	void resize(size_t n);
	void simple_increment();
	void simple_decrement();
	void simple_add(const BigNumber& n);
	void simple_minus(const BigNumber& n, bool negative);
	static pair<BtoI, BtoI> simple_multi(Int x, Int y);
	void PASS_BY_STRING(string str);
	void PASS_BY_STRING_with_notation(string str);
	void COMMON_DIVIDE(const BigNumber& n, bool mod);
	template <typename T>
	void WIDE_CHAR_PASS(const T& ch);
	template <typename T>
	void WIDE_CHARARRAY_PASS(const T* C_STR);
public:
	using base_type = Int;
	static constexpr Int base_max = IMax - 1;
	friend ostream& operator << (ostream& os, const BigNumber& n);
	friend istream& operator >> (istream& is, BigNumber& n);
	BigNumber();
	template <typename T>
	BigNumber(const T& n);
	BigNumber(const BigNumber& n);
	BigNumber(BigNumber&& n);
	~BigNumber();
	size_t digit() const noexcept;
	int getDigit(int n) const noexcept;
	int getSize() const noexcept;
	int getRealSize() const noexcept;
	template <typename T>
	bool operator == (const T& n) const;
	bool operator == (const BigNumber& n) const noexcept;
	template <typename T>
	bool operator != (const T& n) const;
	bool operator != (const BigNumber& n) const noexcept;
	bool operator < (const BigNumber& n) const noexcept;
	template <typename T>
	bool operator < (const T& n) const;
	bool operator <= (const BigNumber& n) const noexcept;
	template <typename T>
	bool operator <= (const T& n) const;
	bool operator > (const BigNumber& n) const noexcept;
	template <typename T>
	bool operator > (const T& n) const;
	bool operator >= (const BigNumber& n) const noexcept;
	template <typename T>
	bool operator >= (const T& n) const;
	BigNumber operator + () const;
	BigNumber operator - () const;
	const BigNumber& operator = (const BigNumber& n);
	const BigNumber& operator = (BigNumber&& n);
	const BigNumber& operator = (const string& str);
	template <typename T>
	const BigNumber& operator = (const basic_string<T>& STR);
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
	string str(Int notation = NOTATION) const;
	size_t Sizeof() const noexcept{return SIZE;}
	void print() const{cout << (*this);}
	bool is_zero() const noexcept;
	bool get_positive() const noexcept{return positive;}
	void negate() noexcept{positive = !positive;}
	void divide2() noexcept;
	bool is_odd() const noexcept;
	bool is_even() const noexcept;
	Int get(size_t n) const;
	void reserve(size_t n);
	void upgrade(size_t n = 1);
	void downgrade(size_t n = 1) noexcept;
	void fit() noexcept;
	void operator << (size_t n);
	void operator >> (size_t n) noexcept;
	explicit operator BtoI() const;
	explicit operator uBtoI() const;
	explicit operator long double() const;
	explicit operator string() const;
};

//BtoI BigNumber::m[2] = {0};

void BigNumber::resize() noexcept{ //fit
	for(int i = SIZE - 1;i >= BASIC_SIZE;i--){
		if(a[i] != 0){
			SIZE = i + 1;
			return;
		}
	}
	SIZE = BASIC_SIZE;
}
void BigNumber::resize(size_t n){ //large
	if(n > SIZE){
		if(n <= capacity && capacity / n < 2){
			for(int i = SIZE;i < n;i++){
				a[i] = 0;
			}
		}else{
			capacity = n;
			Int* tmp = new Int[capacity];
			for(int i = 0;i < SIZE;i++){
				tmp[i] = a[i];
			}
			for(int i = SIZE;i < n;i++){
				tmp[i] = 0;
			}
			delete[] a;
			a = tmp;
		}
		SIZE = n;
	}
}
int BigNumber::getSize() const noexcept{
	return SIZE;
}
int BigNumber::getRealSize() const noexcept{
	for(int i = SIZE - 1;i > 0;i--){
		if(a[i] != 0){
			return i + 1;
		}
	}
	return 1;
}
void BigNumber::simple_increment(){
	a[0]++;
	for(int i = 0;i < SIZE - 1;i++){
		if(a[i] >= IMax){
			a[i] -= IMax;
			a[i + 1]++;
		}else{
			return;
		}
	}
	if(a[SIZE - 1] >= IMax){
		a[SIZE - 1] -= IMax;
		if(capacity < SIZE + 1 || capacity / (SIZE + 1) >= 2){ //same condition as resize(SIZE + 1)
			capacity = SIZE + 1;
			Int* tmp = new Int[capacity];
			for(int i = 0;i < SIZE;i++){
				tmp[i] = a[i];
			}
			tmp[SIZE++] = 1;
			delete[] a;
			a = tmp;
		}else{
			a[SIZE++] = 1;
		}
	}
}
void BigNumber::simple_decrement(){
	a[0]--;
	for(int i = 0;i < SIZE - 1;i++){
		if(a[i] < 0){
			a[i] += IMax;
			a[i + 1]--;
		}else{
			return;
		}
	}
	if(a[SIZE - 1] == 0){
		SIZE = (SIZE - 1 > BASIC_SIZE) ? (SIZE - 1) : BASIC_SIZE;
	}else if(a[SIZE - 1] < 0){ //MUST origin 0
		for(int i = 1;i < BASIC_SIZE;i++){
			a[i] = 0;
		}
		a[0] = 1;
		positive = false;
	}
}
//only reallocate when it is necessary to expand its space
void BigNumber::simple_add(const BigNumber& n){
	bool have_SIZE_plus1 = false, less_or_equal = false;
	if(SIZE < n.SIZE){
		resize(n.SIZE + 1);
		SIZE--;
		less_or_equal = true;
		have_SIZE_plus1 = true;
	}else if(SIZE == n.SIZE){
		less_or_equal = true;
	}
	for(int i = 0;i < n.SIZE - 1;i++){
		a[i] += n.a[i];
		if(a[i] >= IMax){
			a[i] -= IMax;
			a[i + 1]++;
		}
	}
	a[n.SIZE - 1] += n.a[n.SIZE - 1];
	if(a[n.SIZE - 1] >= IMax){
		if(!less_or_equal){
			a[n.SIZE - 1] -= IMax;
			a[n.SIZE]++;
			for(int i = n.SIZE;i < SIZE - 1;i++){
				if(a[i] < IMax){
					return;
				}
				a[i] -= IMax;
				a[i + 1]++;
			}
			if(a[SIZE - 1] >= IMax){
				resize(SIZE + 1);
				a[SIZE - 2] -= IMax;
				a[SIZE - 1]++;
			}
		}else{
			if(have_SIZE_plus1){
				a[SIZE - 1] -= IMax;
				a[SIZE]++;
				SIZE++;
			}else{
				resize(SIZE + 1);
				a[SIZE - 2] -= IMax;
				a[SIZE - 1]++;
			}
		}
	}
}
void BigNumber::simple_minus(const BigNumber& n, bool negative){
	const Int* b = nullptr;
	size_t b_size = 0;
	bool flag = false, equal = false;
	if(SIZE < n.SIZE){
		b = a;
		b_size = SIZE;
		a = nullptr;
		operator = (n);
		if(!negative){positive = n.positive;}
		else{positive = !n.positive;}
		flag = true;
	}else{
		b = n.a;
		b_size = n.SIZE;
		if(SIZE == n.SIZE){
			equal = true;
		}
	}
	if(flag || !equal){ //SIZE > BASIC_SIZE and SIZE > b_size
		for(int i = 0;i < b_size;i++){
			a[i] -= b[i];
			if(a[i] < 0){
				a[i] += IMax;
				a[i + 1]--;
			}
		}
		for(int i = b_size;i < SIZE - 1;i++){
			if(a[i] < 0){
				a[i] += IMax;
				a[i + 1]--;
			}
		}
		if(a[SIZE - 1] == 0){
			SIZE--;
		}
	}else{ //the most inefficient way
		for(int i = 0;i < b_size - 1;i++){
			a[i] -= b[i];
			if(a[i] < 0){
				a[i] += IMax;
				a[i + 1]--;
			}
		}
		a[b_size - 1] -= b[b_size - 1];
		if(a[b_size - 1] < 0){
			positive = !positive;
			for(int i = 0;i < SIZE - 1;i++){
				if(a[i] > 0){
					a[i] -= IMax;
					a[i + 1]++;
				}
				a[i] = -a[i];
			}
			a[SIZE - 1] = -a[SIZE - 1];
		}
		resize();
	}
	if(flag){
		delete[] b;
	}
}
pair<BtoI, BtoI> BigNumber::simple_multi(Int x, Int y){
	BtoI x_ = x, y_ = y;
	BtoI sol = x_ * y_;
	return make_pair(sol % IMax, sol / IMax);
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
		SIZE = (str.length() - 1) / DIGIT + 1;
	}
	capacity = SIZE;
	delete[] a;
	a = new Int[capacity];
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
	capacity = BASIC_SIZE;
	delete[] a;
	a = new Int[capacity];
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
BigNumber::BigNumber(){
	SIZE = BASIC_SIZE;
	capacity = BASIC_SIZE;
	a = new Int[capacity];
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
	if(this != &n){
		SIZE = n.SIZE;
		capacity = n.SIZE;
		a = new Int[capacity];
		for(int i = 0;i < SIZE;i++){
			a[i] = n.a[i];
		}
		positive = n.positive;
	}
}
BigNumber::BigNumber(BigNumber&& n){
	if(this != &n){
		SIZE = n.SIZE;
		capacity = n.capacity;
		a = n.a;
		n.a = nullptr;
		positive = n.positive;
	}
}

BigNumber::~BigNumber(){
	delete[] a;
}

size_t BigNumber::digit() const noexcept{
	size_t i = SIZE - 1;
	for(;i > 0;i--){
		if(a[i] != 0){
			break;
		}
	}
	size_t sol = DIGIT * i + 1;
	Int tmp = a[i];
	for(;tmp / 10 != 0;tmp /= 10, sol++);
	return sol;
}
int BigNumber::getDigit(int n) const noexcept{
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
bool BigNumber::operator == (const BigNumber& n) const noexcept{
	if(SIZE != n.SIZE){
		return false;
	}
	bool zero = true;
	for(int i = 0;i < SIZE;i++){
		if(a[i] != n.a[i]){
			return false;
		}
		if(a[i] != 0 && zero){
			if(positive != n.positive){
				return false;
			}
			zero = false;
		}
	}
	return true;
}
template <typename T>
bool BigNumber::operator != (const T& n) const{
	return !(*this == n);
}
bool BigNumber::operator != (const BigNumber& n) const noexcept{
	return !(*this == n);
}
bool BigNumber::operator < (const BigNumber& n) const noexcept{
	if(SIZE < n.SIZE){
		return n.positive;
	}else if(SIZE > n.SIZE){
		return !positive;
	}else{
		int pure_big = 0;
		for(int i = SIZE - 1;i >= 0;i--){
			if(a[i] > n.a[i]){
				pure_big = 1;
				break;
			}else if(a[i] < n.a[i]){
				pure_big = -1;
				break;
			}
		}
		if(pure_big == 0){
			return false;
		}else if(pure_big == 1){
			return !positive;
		}else{
			return n.positive;
		}
	}
}
template <typename T>
bool BigNumber::operator < (const T& n) const{
	BigNumber temp = n;
	return (*this) < temp;
}
bool BigNumber::operator <= (const BigNumber& n) const noexcept{
	return (*this) < n || (*this) == n;
}
template <typename T>
bool BigNumber::operator <= (const T& n) const{
	return (*this) < n || (*this) == n;
}
bool BigNumber::operator > (const BigNumber& n) const noexcept{
	if(SIZE < n.SIZE){
		return !n.positive;
	}else if(SIZE > n.SIZE){
		return positive;
	}else{
		int pure_big = 0;
		for(int i = SIZE - 1;i >= 0;i--){
			if(a[i] > n.a[i]){
				pure_big = 1;
				break;
			}else if(a[i] < n.a[i]){
				pure_big = -1;
				break;
			}
		}
		if(pure_big == 0){
			return false;
		}else if(pure_big == 1){
			return positive;
		}else{
			return !n.positive;
		}
	}
}
template <typename T>
bool BigNumber::operator > (const T& n) const{
	BigNumber temp = n;
	return (*this) > temp;
}
bool BigNumber::operator >= (const BigNumber& n) const noexcept{
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
		if(capacity < n.SIZE || a == nullptr){
			delete[] a;
			capacity = n.SIZE;
			a = new Int[capacity];
		}
		SIZE = n.SIZE;
		for(int i = 0;i < SIZE;i++){
			a[i] = n.a[i];
		}
		positive = n.positive;
	}
	return (*this);
}
const BigNumber& BigNumber::operator = (BigNumber&& n){
	if(this != &n){
		SIZE = n.SIZE;
		capacity = n.capacity;
		delete[] a;
		a = n.a;
		n.a = nullptr;
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
template <typename T>
const BigNumber& BigNumber::operator = (const T& n){
	if constexpr(is_same<T, char>::value){
		string str;
		str += n;
		PASS_BY_STRING(str);
	}else if constexpr(is_contain_first<T, wchar_t, char16_t, char32_t>::value){
		WIDE_CHAR_PASS(n);
		return (*this);
	}else{
		SIZE = BASIC_SIZE;
		capacity = BASIC_SIZE;
		delete[] a;
		a = new Int[capacity];
		if constexpr(is_scalar<T>::value){ //faster
			for(int i = SIZE - 1;i > 2;i--){
				a[i] = 0;
			}
			if constexpr(is_signed<T>::value){
				BtoI temp = static_cast<BtoI>(n);
				positive = (n >= 0);
				for(int i = 0;i < BtoItimes;i++, temp /= IMax){
					a[i] = temp % IMax * (temp < 0 ? -1 : 1);
				}
			}else{
				uBtoI temp = static_cast<uBtoI>(n);
				positive = true;
				for(int i = 0;i < BtoItimes;i++, temp /= IMax){
					a[i] = temp % IMax * (temp < 0 ? -1 : 1);
				}
			}
		}else{
			stringstream ss;
			ss << n;
			string str = ss.str();
			PASS_BY_STRING(str);
		}
	}
	return (*this);
}
template <typename T>
const BigNumber& BigNumber::operator = (T* const n){
	if constexpr(is_same<expr_type<T>, char>::value){
		string str = n;
		PASS_BY_STRING(str);
	}else if constexpr(is_contain_first<T, wchar_t, char16_t, char32_t>::value){
		WIDE_CHARARRAY_PASS(n);
	}else{
		ptrdiff_t temp = reinterpret_cast<ptrdiff_t>(n);
		(*this) = temp;
	}
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
	if(positive == n.positive){
		simple_add(n);
	}else{
		simple_minus(n, false);
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
	if(positive){
		simple_increment();
	}else{
		simple_decrement();
	}
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
	if(positive != n.positive){
		simple_add(n);
	}else{
		simple_minus(n, true);
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
	if(!positive){
		simple_increment();
	}else{
		simple_decrement();
	}
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
	if(n.SIZE == BASIC_SIZE){
		if(SIZE == BASIC_SIZE && getRealSize() + n.getRealSize() <= BASIC_SIZE){
			Int b[BASIC_SIZE] = {0};
			for(int i = 0;i < n.getRealSize();i++){
				Int tmp = 0;
				for(int j = 0;j < getRealSize();j++){
					auto [low, high] = simple_multi(a[j], n.a[i]);
					b[i + j] += low + tmp;
					tmp = high;
					if(b[i + j] >= IMax){
						b[i + j] -= IMax;
						tmp++;
					}
				}
				b[i + getRealSize()] += tmp;
			}
			for(int i = 0;i < BASIC_SIZE;i++){
				a[i] = b[i];
			}
			if(positive == n.positive){
				positive = true;
			}else{
				positive = false;
			}
			return (*this);
		}else if(n.getRealSize() == 1){
			Int tmp = 0;
			for(int i = 0;i < SIZE;i++){
				auto [low, high] = simple_multi(a[i], n.a[0]);
				a[i] = low + tmp;
				tmp = high;
				if(a[i] >= IMax){
					a[i] -= IMax;
					tmp++;
				}
			}
			if(tmp > 0){
				resize(SIZE + 1);
				a[SIZE - 1] = tmp;
			}
			if(positive == n.positive){
				positive = true;
			}else{
				positive = false;
			}
			return (*this);
		}
	}
	(*this) = (*this) * n; //move
	return (*this);
}
template <typename T>
const BigNumber& BigNumber::operator *= (const T& n){
	BigNumber temp = n;
	(*this) *= temp;
	return (*this);
}
BigNumber BigNumber::operator * (const BigNumber& n) const{
	BigNumber temp;
	size_t a_size = getRealSize(), b_size = n.getRealSize();
	temp.resize(a_size + b_size);
	for(int i = 0;i < b_size;i++){
		Int tmp = 0;
		for(int j = 0;j < a_size;j++){
			auto [low, high] = simple_multi(a[j], n.a[i]);
			//temp.a[i + j] += m[0] + tmp; //may over 2147483647
			temp.a[i + j] += tmp;
			tmp = high;
			if(temp.a[i + j] >= IMax){ //overflow avoid
				temp.a[i + j] -= IMax;
				tmp++;
			}
			temp.a[i + j] += low;
			if(temp.a[i + j] >= IMax){
				temp.a[i + j] -= IMax;
				tmp++;
			}
		}
		temp.a[i + a_size] += tmp;
	}
	if(positive == n.positive){
		temp.positive = true;
	}else{
		temp.positive = false;
	}
	temp.resize();
	return temp;
}
template <typename T>
BigNumber BigNumber::operator * (const T& n) const{
	BigNumber temp = (*this), tmp = n;
	temp *= tmp;
	return temp;
}
void BigNumber::COMMON_DIVIDE(const BigNumber& n, bool mod){
	BtoI BIMax = IMax, remain = 0;
	BtoI a_size = getRealSize(), b_size = n.getRealSize();
	BtoI target_size = a_size - b_size + 1;
	if(target_size <= 0){
		if(!mod){
			SIZE = BASIC_SIZE;
			for(int i = 0;i < BASIC_SIZE;i++){
				a[i] = 0;
			}
		}
		return;
	}
	Int* quo = nullptr;
	if(!mod){
		quo = new Int[target_size];
	}
	for(int i = target_size - 1;i >= 0;i--){
		Int newhead = (remain * BIMax + a[b_size - 1 + i]) / n.a[b_size - 1], head = 0;
		if(newhead == 0){
			if(!mod){
				quo[i] = 0;
			}
			remain = a[b_size - 1 + i];
			continue;
		}else if(remain != 0){
			a[b_size + i] = 0;
		}
		//try
		Int remin = -1, remax = -1, regap = 0;
		while(head != newhead){ //iterate
			if(newhead - head == regap){
				if(remin != head || remax != newhead){
					remin = head, remax = newhead;
				}else{ //if repeating
					if(regap == 1){ //the minimum one is solution
						break;
					}
					newhead = (newhead + head) / 2; //get into medium
				}
			}else{
				if(newhead > head){
					regap = newhead - head;
					remin = head, remax = newhead;
				}
			}
			BtoI differ = head - newhead;
			head = newhead;
			for(int j = 0;j < b_size - 1;j++){
				BtoI buffer = differ * n.a[j];
				a[i + j] += buffer % BIMax;
				a[i + j + 1] += buffer / BIMax;
				while(a[i + j] >= IMax){
					a[i + j] -= IMax;
					a[i + j + 1]++;
				}
				while(a[i + j] < 0){
					a[i + j] += IMax;
					a[i + j + 1]--;
				}
			}
			newhead = (remain * BIMax + a[b_size - 1 + i]) / n.a[b_size - 1];
		}
		if(head != newhead){
			newhead = head;
		}
		a[b_size - 1 + i] = (remain * BIMax + a[b_size - 1 + i]) - n.a[b_size - 1] * static_cast<BtoI>(newhead);
		if(!mod){
			quo[i] = head;
		}
		remain = a[b_size - 1 + i];
	}
	if(!mod){
		for(int i = 0;i < BASIC_SIZE;i++){
			a[i] = 0;
		}
		for(int i = 0;i < target_size;i++){
			a[i] = quo[i];
		}
		delete[] quo;
		SIZE = target_size;
		capacity - target_size;
		positive = positive == n.positive;
	}
	resize();
}
const BigNumber& BigNumber::operator /= (const BigNumber& n){
	if(SIZE < n.SIZE){
		SIZE = BASIC_SIZE;
		for(int i = 0;i < BASIC_SIZE;i++){
			a[i] = 0;
		}
		return (*this);
	}else if(n.is_zero()){
		throw domain_error("Cannot divide by zero within " + str() + " / " + n.str());
	}
	COMMON_DIVIDE(n, false);
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
	if(SIZE < n.SIZE){
		return (*this);
	}else if(n.is_zero()){
		throw domain_error("Cannot divide by zero within " + str() + " % " + n.str());
	}
	COMMON_DIVIDE(n, true);
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
	if(!n.positive){
		bool zero = is_zero();
		SIZE = BASIC_SIZE;
		for(int i = 1;i < BASIC_SIZE;i++){
			a[i] = 0;
		}
		a[0] = (zero) ? 1 : 0;
		positive = true;
		return (*this);
	}else{
		if(n.getRealSize() == 1){ //base case
			if(n.a[0] == 1){
				return (*this);
			}else if(n.a[0] == 0){
				SIZE = BASIC_SIZE;
				for(int i = 1;i < BASIC_SIZE;i++){
					a[i] = 0;
				}
				a[0] = 1;
				positive = true;
				return (*this);
			}
		}
	}
	/*BigNumber base = (*this);
	for(BigNumber i = 2;i <= n;i++){
		(*this) *= base;
	}*/
	BigNumber nd2 = n;
	nd2.divide2();
	if(n.is_even()){
		(*this) ^= nd2;
		(*this) *= (*this);
	}else{
		BigNumber base = (*this);
		(*this) ^= nd2;
		(*this) *= (*this);
		(*this) *= base;
	}
	//cout << base << " ^ " << n << " = " << (*this) << endl;
	return (*this);
}
template <typename T>
const BigNumber& BigNumber::operator ^= (const T& n){
	BigNumber temp = n;
	(*this) ^= temp;
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
bool BigNumber::is_zero() const noexcept{
	if(SIZE > BASIC_SIZE){
		return false;
	}
	for(int i = 0;i < SIZE;i++){
		if(a[i] != 0){
			return false;
		}
	}
	return true;
}
void BigNumber::divide2() noexcept{
	a[0] /= 2;
	for(int i = 1;i < SIZE;i++){
		if(a[i] % 2 != 0){
			a[i - 1] += IMax / 2;
		}
		a[i] /= 2;
	}
	if(a[SIZE - 1] == 0 && SIZE > BASIC_SIZE){
		SIZE--;
	}
}
bool BigNumber::is_odd() const noexcept{
	return a[0] % 2 == 1;
}
bool BigNumber::is_even() const noexcept{
	return a[0] % 2 == 0;
}
Int BigNumber::get(size_t n) const{
	return a[n];
}
void BigNumber::reserve(size_t n){
	if(n == capacity){
		return;
	}
	if(n > SIZE){
		capacity = n;
		Int* tmp = new Int[capacity];
		for(int i = 0;i < SIZE;i++){
			tmp[i] = a[i];
		}
		for(int i = SIZE;i < n;i++){
			tmp[i] = 0;
		}
		delete[] a;
		a = tmp;
	}
}
void BigNumber::upgrade(size_t n){
	if(SIZE == BASIC_SIZE){
		if(a[BASIC_SIZE - 1] != 0){
			if(capacity == BASIC_SIZE){
				resize(SIZE + 1);
			}else{
				SIZE++;
			}
		}
	}else{
		if(capacity < SIZE + 1){
			resize(SIZE + 1);
		}else{
			SIZE++;
		}
	}
	for(int i = SIZE - 1;i > 0;i--){
		a[i] = a[i - 1];
	}
	a[0] = 0;
}
void BigNumber::downgrade(size_t n) noexcept{
	for(int i = 0;i < SIZE - 1;i++){
		a[i] = a[i + 1];
	}
	a[SIZE - 1] = 0;
	resize();
}
void BigNumber::fit() noexcept{
	resize();
}
void BigNumber::operator << (size_t n){
	upgrade(n);
}
void BigNumber::operator >> (size_t n) noexcept{
	downgrade(n);
}
BigNumber::operator BtoI() const{
	intmax_t tmp = 0;
	if((*this) >= INTMAX_MAX){
		tmp = INTMAX_MAX;
	}else if((*this) <= INTMAX_MIN){
		tmp = INTMAX_MIN;
	}else{
		for(int i = BtoItimes - 1;i >= 0;i--){
			tmp *= static_cast<intmax_t>(IMax);
			tmp += a[i];
		}
		/*tmp += a[0];
		tmp += a[1] * static_cast<long long int>(IMax);
		tmp += a[2] * IIMax;*/
		tmp *= (positive ? 1 : -1);
	}
	return tmp;
}
BigNumber::operator uBtoI() const{
	uintmax_t tmp = 0;
	if((*this) >= UINTMAX_MAX){
		tmp = UINTMAX_MAX;
	}else if((*this) <= 0){
		tmp = 0;
	}else{
		for(int i = uBtoItimes - 1;i >= 0;i--){
			tmp *= static_cast<uintmax_t>(IMax);
			tmp += a[i];
		}
	}
	return tmp;
}
BigNumber::operator long double() const{
	long double tmp = static_cast<long double>(static_cast<long long int>((*this)));
	return tmp;
}
BigNumber::operator string() const{
	return str();
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
	if constexpr(is_signed<T>::value){
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
	if constexpr(is_signed<T>::value){
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
	if constexpr(is_signed<T>::value){
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
	if constexpr(is_floating_point<T>::value){
		n /= static_cast<long double>(N);
	}else{
		if constexpr(is_signed<T>::value){
			n /= static_cast<long long int>(N);
		}else{
			n /= static_cast<unsigned long long int>(N);
		}
	}
	return n;
}
template <typename T>
/*typename conditional<is_floating_point<T>::value, T, BigNumber>::type*/auto operator / (const T& n, const BigNumber& N){
	T temp = n;
	if constexpr(is_floating_point<T>::value){
		return temp / static_cast<long double>(N);
	}else{
		return temp / static_cast<long long int>(N);
	}
	//return temp / static_cast<typename conditional<is_floating_point<T>::value, long double, BigNumber>::type>(N);
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

string BigNumber::str(Int notation) const{
	if(notation <= 1){notation = NOTATION;}
	if(notation <= 36){
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
			BtoI t = static_cast<BtoI>(n % notation);
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
	}else{
		//...
		return string();
	}
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
/* it seems to be unuseful
namespace bnint_swap{
void swap(bnint& a, bnint& b){
	std::swap(a.SIZE, b.SIZE);
	std::swap(a.positive, b.positive);
	std::swap(a.a, b.a);
}
}*/
namespace std{
//bnint traits
template<>
struct is_integral<bnint> : true_type{};
//is_arithmetic is true if is_integral is true
//is_scalar becomes true if is_integral is true, which is not reasonable.
template<>
struct is_scalar<bnint> : false_type{};
//is_object     is true of course
//because is_integral is true, is_fundamental becomes true, which is not reasonable.
template<>
struct is_fundamental<bnint> : false_type{};
//is_compound_type is true
template<>
struct is_signed<bnint> : true_type{};
template<>
struct is_unsigned<bnint> : false_type{};
}

#endif