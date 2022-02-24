#pragma once

#include <string>
#include <algorithm>
#include <limits>
#include <cmath>

class Number {
public:
	using number_type = double;
	Number(int num = 0) {
		number = num;
	}
	Number(double num) {
		number = num;
	}
	Number(const std::string& s) {
		sscanf(s.c_str(), "%lf", &number);
	}
	virtual ~Number() = default;
	Number opposite() const {
		return -number;
	}
	Number operator+(const Number& rhs) const {
		return Number(number + rhs.number);
	}
	Number operator-(const Number& rhs) const {
		return Number(number - rhs.number);
	}
	Number operator*(const Number& rhs) const {
		return Number(number * rhs.number);
	}
	Number operator/(const Number& rhs) const {
		return Number(number / rhs.number);
	}
	// TODO
	static Number eNumber(const Number& base, const Number& exp) {
		return base * Number(pow(10, exp));
	}
	operator double() const {
		return number;
	}
	std::string to_string() const {
		return std::to_string(number);
	}
private:
	number_type number;
};
class IntegerNumber {
public:
	static int const kDigitBase = 10;
	static int const kMaxDigit = 10000;
	static int const kMaxDigitSize = 4;
	using number_type = int;
	number_type getD(int pos) const {
		if (pos < 0 || pos >= len) return 0;
		return d[pos];
	}
	IntegerNumber() {
		d = nullptr; len = 0;
	}
	~IntegerNumber() {
		if (len && d != nullptr) delete[] d;
	}
	IntegerNumber(long long num) {
		bool isMinus = false;
		len = 0;
		if (num == 0) {
			d = nullptr;
			return;
		}
		if (num < 0) isMinus = true, num = -num;
		long long tmp = num;
		while (tmp) ++len, tmp /= kMaxDigit;
		d = new number_type[len];
		for (int i = 0; i < len; ++i)
			d[i] = num % kMaxDigit, num /= kMaxDigit;
		if (isMinus) len = -len;
	}
	IntegerNumber(int num) : IntegerNumber((long long)num) {}
	IntegerNumber(short num) : IntegerNumber((long long)num) {}
	IntegerNumber(double) {
		// TODO: Error
	}
	IntegerNumber(const IntegerNumber& num) {
		*this = num;
	}
	IntegerNumber& operator=(const IntegerNumber& num) {
		d = nullptr; len = 0;
		if (num.len == 0) return *this;
		d = new number_type[abs(num.len)];
		len = num.len;
		for (int i = 0; i < abs(num.len); ++i)
			d[i] = num.d[i];
		return *this;
	}
	static int countDigit(number_type num) {
		int res = 0;
		while (num) num /= kDigitBase, ++res;
		return res;
	}
	static number_type parseNumber(const std::string& s) {
		number_type res = 0;
		for (std::size_t i = 0; i < s.length(); ++i) 
			res = res * kDigitBase + (s[i] - '0');
		return res;
	}
	IntegerNumber(const std::string& s, bool isMinus = false) {
		if (s[0] == '+' || s[0] == '-') {
			new (this)IntegerNumber(s.substr(1), isMinus ^ (s[0] == '-'));
			return;
		}
		new (this)IntegerNumber();
		std::size_t slen = s.length();
		if (s[0] == '0' && slen == 1) return;
		len = slen / kMaxDigitSize;
		if (slen % kMaxDigitSize) ++len;
		d = new number_type[len];
		for (std::size_t i = 0; i < (std::size_t)len; ++i) {
			std::size_t beginPos = slen - (i + 1) * kMaxDigitSize;
			std::size_t nlen = kMaxDigitSize;
			if (slen < (i + 1) * kMaxDigitSize) 
				beginPos = 0, nlen = slen - i * kMaxDigitSize;
			d[i] = parseNumber(s.substr(beginPos, nlen));
		}
		if (isMinus) len = -len;
	}
	IntegerNumber opposite() const {
		IntegerNumber res = *this;
		res.len = -len;
		return res;
	}
	IntegerNumber operator+(const IntegerNumber& rhs) const {
		if (len == 0) return rhs;
		if (rhs.len == 0) return *this;
		if (len > 0) {
			if (rhs.len < 0) return *this - rhs.opposite();
		} else {
			if (rhs.len > 0) return rhs - this->opposite();
			else return (this->opposite() + rhs.opposite()).opposite();
		}
		if (len < rhs.len) return rhs + *this;
		IntegerNumber res;
		res.d = new number_type[len + 1];
		res.len = len;
		number_type tmp = 0;
		for (int i = 0; i < len; ++i) {
			res.d[i] = d[i] + (i < rhs.len ? rhs.d[i] : 0) + tmp;
			tmp = res.d[i] / kMaxDigit;
			res.d[i] %= kMaxDigit;
		}
		if (tmp) {
			res.d[len] = tmp;
			++res.len;
		}
		return res;
	}
	IntegerNumber operator-(const IntegerNumber& rhs) const {
		if (len == 0) return rhs.opposite();
		if (rhs.len == 0) return *this;
		if (len > 0) {
			if (rhs.len < 0) return *this + rhs.opposite();
		} else {
			if (rhs.len > 0) return (this->opposite() + rhs).opposite();
			else {
				return rhs.opposite() - this->opposite();
			}
		}
		// TODO
		if (len < rhs.len) return (rhs - *this).opposite();
		int dpos = len;
		while (dpos--) {
			if (d[dpos] != rhs.d[dpos])
				break;
		}
		++dpos;
		if (dpos <= 0) return IntegerNumber();
		IntegerNumber res;
		res.d = new number_type[dpos];
		res.len = dpos;
		number_type tmp = 0;
		for (int i = 0; i < dpos; ++i) {
			res.d[i] = d[i] - rhs.d[i] - tmp;
			tmp = 0;
			if (res.d[i] < 0) {
				res.d[i] += kMaxDigit;
				tmp = 1;
			}
		}
		if (tmp) {
			res.d[0] = kMaxDigit - res.d[0];
			for (int i = 1; i < dpos; ++i) {
				res.d[i] = kMaxDigit - rhs.d[i] - 1;
			}
			res.len = -res.len;
		}
		return res;
	}
	IntegerNumber operator*(const IntegerNumber& rhs) const {
		if (len == 0 || rhs.len == 0) return IntegerNumber();
		if (len < 0) {
			if (rhs.len < 0) return this->opposite() * rhs.opposite();
			else return (this->opposite() * rhs).opposite();
		}
		if (rhs.len < 0) return (*this * rhs.opposite()).opposite();
		// TODO: Karatsuba
		IntegerNumber res;
		res.len = len + rhs.len;
		res.d = new number_type[res.len];
		for (int i = 0; i < res.len; ++i) res.d[i] = 0;
		for (int i = 0; i < len; ++i) {
			for (int j = 0; j < rhs.len; ++j) {
				res.d[i + j] += d[i] * rhs.d[j];
				res.d[i + j + 1] += res.d[i + j] / kMaxDigit;
				res.d[i + j] %= kMaxDigit;
			}
		}
		res.d[res.len - 1] += res.d[res.len - 2] / kMaxDigit;
		res.d[res.len - 2] %= kMaxDigit;
		if (res.d[res.len - 1] == 0) --res.len;
		return res;
	}
	static IntegerNumber& sub_mul(IntegerNumber& r, const IntegerNumber &rhs, number_type mul, int offset) {
		if (mul == 0) return r;
		number_type borrow = 0;
		for (int i = 0; i < rhs.len; ++i) {
			borrow += r.d[i + offset] - rhs.d[i] * mul - kMaxDigit + 1;
			r.d[i + offset] = borrow % kMaxDigit + kMaxDigit - 1;
			borrow /= kMaxDigit;
		}
		for (int i = rhs.len; borrow; ++i) {
			borrow += r.d[i + offset] - kMaxDigit + 1;
			r.d[i + offset] = borrow % kMaxDigit + kMaxDigit - 1;
			borrow /= kMaxDigit;
		}
		return r;
	}
	static std::pair<IntegerNumber, IntegerNumber> FloorDiv(const IntegerNumber& lhs, const IntegerNumber& rhs) {
		if (rhs.len == 0) 
			return std::make_pair(IntegerNumber(), IntegerNumber());
		if (lhs.len == 0)
			return std::make_pair(IntegerNumber(), IntegerNumber());
		if (lhs.len > 0) {
			if (rhs.len < 0) {
				std::pair<IntegerNumber, IntegerNumber> res = FloorDiv(lhs, rhs.opposite());
				res.first = res.first.opposite();
				return res;
			}
		} else {
			if (rhs.len > 0) {
				std::pair<IntegerNumber, IntegerNumber> res = FloorDiv(lhs, rhs.opposite());
				res.first = res.first.opposite();
				res.second = res.second.opposite();
				return res;
			} else {
				return FloorDiv(lhs.opposite(), rhs.opposite());
			}
		}
		if (lhs < rhs) return std::make_pair(IntegerNumber(), lhs);
		IntegerNumber res;
		res.len = lhs.len - rhs.len + 1;
		res.d = new number_type[res.len];
		for (int i = 0; i < res.len; ++i)
			res.d[i] = 0;
		// TODO
		double t = rhs.getD(rhs.len - 2) + (rhs.getD(rhs.len - 3) + 1.0) / kMaxDigit;
		double db = 1.0 / (rhs.getD(rhs.len - 1) + t / kMaxDigit);
		IntegerNumber r = lhs;
		
		for (int i = lhs.len - 1, j = res.len - 1; j >= 0; ) {
			number_type rm = r.getD(i + 1) * kMaxDigit + r.getD(i);
			number_type m = std::max((number_type)(db * rm), r.getD(i + 1));
			sub_mul(r, rhs, m, j);
			res.d[j] += m;
			if (!rhs.getD(i + 1))
				--i, --j;
		}
		r.trim();
		number_type carry = 0;
		while (r >= rhs) {
			r = r - rhs;
			++carry;
		}
		for (int i = 0; i < res.len; ++i) {
			carry += res.d[i];
			res.d[i] = carry % kMaxDigit;
			carry /= kMaxDigit;
		}
		res.trim(); r.trim();
		return std::make_pair(res, r);
	}
	IntegerNumber operator/(const IntegerNumber& rhs) const {
		return FloorDiv(*this, rhs).first;
	}
	IntegerNumber operator%(const IntegerNumber& rhs) const {
		return FloorDiv(*this, rhs).second;
	}
	IntegerNumber& trim() {
		int l = abs(len);
		while (l - 1 >= 0 && d[l - 1] == 0) --l;
		if (len < 0) l = -l;
		len = l;
		return *this;
	}
	bool operator<(const IntegerNumber& rhs) const {
		if (len != rhs.len) return len < rhs.len;
		int dpos = abs(len);
		while (dpos--) {
			if (d[dpos] != rhs.d[dpos])
				break;
		}
		if (dpos < 0) return false;
		return (d[dpos] < rhs.d[dpos]) ^ (len < 0);
	}
	bool operator==(const IntegerNumber& rhs) const { 
		if (len != rhs.len) return false;
		for (int i = len >= 0 ? len : -len; i-- > 0; )
			if (d[i] != rhs.d[i]) return false;
		return true;
	}
	bool operator<=(const IntegerNumber& rhs) const {
		if (len != rhs.len) return len < rhs.len;
		int dpos = abs(len);
		while (dpos--) {
			if (d[dpos] != rhs.d[dpos])
				break;
		}
		if (dpos < 0) return true;
		return (d[dpos] < rhs.d[dpos]) ^ (len < 0);
	}
	bool operator>(const IntegerNumber& rhs) const { return !(*this <= rhs); }
	bool operator>=(const IntegerNumber& rhs) const { return !(*this < rhs); }
	bool operator!=(const IntegerNumber& rhs) const { return !(*this == rhs); }
	// TODO
	static IntegerNumber eNumber(const IntegerNumber& base, const IntegerNumber& exp) {
		//std::cout << base.to_string() << " " << exp.to_string() << std::endl;
		if (exp.len == 0) return IntegerNumber(1);
		if (exp.len < 0) {
			int moveDigit = (int)(exp.opposite()); 
			int len = abs(base.len); 
			if (moveDigit >= (len - 1) * kMaxDigitSize + countDigit(base.d[len - 1])) 
				return IntegerNumber();
			IntegerNumber res;
			int moveLen = moveDigit / kMaxDigitSize;
			res.len = len - moveLen;
			res.d = new number_type[res.len];
			for (int i = moveLen; i < len; ++i) 
				res.d[i - moveLen] = base.d[i];
			if (moveDigit % kMaxDigitSize) {
				number_type tmp = 1;
				for (int i = 0; i < moveDigit % kMaxDigitSize; ++i)
					tmp *= kDigitBase;
				for (int i = res.len; i-- > 1; ) {
					res.d[i - 1] += res.d[i] % tmp * kMaxDigit;
					res.d[i] /= tmp;
				}
				res.d[0] /= tmp;
				if (res.d[res.len - 1] == 0) --res.len;
			}
			if (base.len < 0) res.len = -res.len;
			return res;
		}
		int moveDigit = (int)exp;
		int len = abs(base.len);
		IntegerNumber res;
		int moveLen = moveDigit / kMaxDigitSize;
		res.len = len + moveLen;
		res.d = new number_type[res.len + 1];
		for (int i = 0; i < len; ++i) 
			res.d[i + moveLen] = base.d[i];
		for (int i = 0; i < moveLen; ++i)
			res.d[i] = 0;
		res.d[res.len] = 0;
		if (moveDigit % kMaxDigitSize) { 
			number_type tmp = 1;
			for (int i = 0; i < moveDigit % kMaxDigitSize; ++i)
				tmp *= kDigitBase;
			for (int i = 0; i < res.len; ++i) {
				res.d[i] *= tmp;
				res.d[i + 1] += res.d[i] / kMaxDigit;
				res.d[i] %= kMaxDigit;
			}
			if (res.d[res.len]) ++res.len;
		}
		if (base.len < 0) res.len = -res.len;
		return res;
	}
	operator int() const {
		// TODO
		if (len == 0) return 0;
		int res = 0; long long base = len > 0 ? 1 : -1; int i = 0;
		while (i < abs(len)) {
			if (base * kMaxDigit > std::numeric_limits<int>::max()) break;
			if (base * kMaxDigit < std::numeric_limits<int>::min()) break;
			res += d[i] * base;
			base *= kMaxDigit; ++i;
		} 
		if (i < abs(len)) {
			if (i + 1 < abs(len)) {
				if (len > 0) return std::numeric_limits<int>::max();
				else return std::numeric_limits<int>::min();
			}
			if (len > 0) {
				int less = (std::numeric_limits<int>::max() - res) / base;
				if (d[i] > less) return std::numeric_limits<int>::max();
			} else {
				int less = (std::numeric_limits<int>::min() - res) / base;
				if (d[i] > less) return std::numeric_limits<int>::min();
			}
			res += d[i] * base;
		}
		return res;
	}
	operator double() const { return (int)(*this); }
	std::string to_string() const {
		if (len == 0) return "0";
		std::string res;
		if (len < 0) res += "-";
		res += std::to_string(d[abs(len) - 1]);
		for (int i = abs(len) - 1; i-- > 0; ) {
			for (number_type j = kMaxDigit / kDigitBase; j; j /= kDigitBase) 
				if (d[i] < j) res += "0";
			if(d[i]) res += std::to_string(d[i]);
		}
		return res;
	}
private:
	number_type* d;
	int len;
};

namespace Calc 
{

using NumberType = Number;
}
