/*
				GNU GENERAL PUBLIC LICENSE
				   Version 2, June 1991
COPYRIGHT (C) Bestkakkoii 2024 All Rights Reserved.
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

*/

#ifndef CXXATOMIC_H_
//@隐藏{
#define CXXATOMIC_H_
//@隐藏}

#include <cxxglobal.h>
#include <atomic>

//@分组{［线程安全］.炫　原子类
//@备注 
// ------------------------
// 用法:
// cxx::atomic_int n = 100;
// ++n;
// --n;
// n += 1;
// n -= 1;
// n *= 1;
// n /= 1;
// n %= 1;
// n &= 1;
// n |= 1;
// n = 1 + n + 1;
// n = 1 - n - 1;
// n = 1 * n * 1;
// n = 1 / n / 1;
// n = 1 % n % 1;
// n = 1 & n & 1;
// n = 1 | n | 1;
//@别名 炫原子
template <typename T>
class CXXAtomic
{
public:
	// all load using std::memory_order_acquire
	// all store using std::memory_order_release

	inline CXXAtomic() = default;
	inline CXXAtomic(T desired) : d_(desired) {}
	inline CXXAtomic(const CXXAtomic& desired) : d_(desired.load()) {}
	inline CXXAtomic(CXXAtomic&& desired) noexcept : d_(desired.load()) {}

	//@备注 显式读取原子数值(非必须) 
	// CXXAtomic<int> a = 0;
	// int n = a; //这样也是可以的
	//@参数 无
	//@返回 原子数值
	//@别名 读()
	inline T load() const { return d_.load(std::memory_order_acquire); }

	//@备注 显式写入原子数值, 建议直接使用 = 运算符
	// CXXAtomic<int> a(123);
	// a = 1; //这样也是可以的
	//@参数 desired 原子数值
	//@返回 无
	//@别名 写(数值)
	inline void store(T desired) { d_.store(desired, std::memory_order_release); }

	//@备注 fetch_add 显式原子加法, 建议直接使用 += 运算符
	// CXXAtomic<int> a = 0;
	// a += 1; //这样也是可以的
	// a = 1 + a + 1; //这样也是可以的
	//@参数 arg 加数
	//@返回 原始值
	//@别名 加(加数)
	inline T fetch_add(T arg)
	{
		T value = load();
		store(value + arg);
		return value;
	}

	//@备注 fetch_sub 显式原子减法, 建议直接使用 -= 运算符
	// CXXAtomic<int> a = 0;
	// a -= 1; //这样也是可以的
	// a = 1 - a - 1; //这样也是可以的
	//@参数 arg 减数
	//@返回 原始值
	//@别名 减(减数)
	inline T fetch_sub(T arg)
	{
		T value = load();
		store(value - arg);
		return value;
	}

	//@备注 fetch_and 显式原子位与, 建议直接使用 &= 运算符
	// CXXAtomic<int> a = 0;
	// a &= 1; //这样也是可以的
	// a = 1 & a & 1; //这样也是可以的
	//@参数 arg 按位与数
	//@返回 原始值
	//@别名 位与(按位与数)
	inline T fetch_and(T arg)
	{
		T value = load();
		store(value & arg);
		return value;
	}

	//@备注 fetch_or 显式原子位或, 建议直接使用 |= 运算符
	// CXXAtomic<int> a = 0;
	// a |= 1; //这样也是可以的
	// a = 1 | a | 1; //这样也是可以的
	//@参数 arg 按位或数
	//@返回 原始值
	//@别名 位或(按位或数)
	inline T fetch_or(T arg)
	{
		T value = load();
		store(value | arg);
		return value;
	}

	//@备注 fetch_xor 显式原子位异或, 建议直接使用 ^= 运算符
	// CXXAtomic<int> a = 0;
	// a ^= 1; //这样也是可以的
	// a = 1 ^ a ^ 1; //这样也是可以的
	//@参数 arg 按位异或数
	//@返回 原始值
	//@别名 异或(按位异或数)
	inline T fetch_xor(T arg)
	{
		T value = load();
		store(value ^ arg);
		return value;
	}

	//@备注 fetch_mul 显式原子乘法, 建议直接使用 *= 运算符
	// CXXAtomic<int> a = 0;
	// a *= 1; //这样也是可以的
	// a = 1 * a * 1; //这样也是可以的
	//@参数 arg 乘数
	//@返回 原始值
	//@别名 乘(乘数)
	inline T fetch_mul(T arg)
	{
		T value = load();
		store(value * arg);
		return value;
	}

	//@备注 fetch_div 显式原子除法, 建议直接使用 /= 运算符
	// CXXAtomic<int> a = 0;
	// a /= 1; //这样也是可以的
	// a = 1 / a / 1; //这样也是可以的
	//@参数 arg 除数
	//@返回 原始值
	//@别名 除(除数)
	inline T fetch_div(T arg)
	{
		CXX_ASSERT_X(arg == 0, L"CXXAtomic<T> divide by zero");

		T value = load();
#pragma warning(push)
#pragma warning(disable: 4804)
		store(value / arg);
#pragma warning(pop)
		return value;
	}

	//@备注 fetch_mod 显式原子取模, 建议直接使用 %= 运算符
	// CXXAtomic<int> a = 0;
	// a %= 1; //这样也是可以的
	// a = 1 % a % 1; //这样也是可以的
	//@参数 arg 取模数
	//@返回 原始值
	//@别名 模(取模数)
	inline T fetch_mod(T arg)
	{
		CXX_ASSERT_X(arg == 0, L"CXXAtomic<T> divide by zero");

		T value = load();
#pragma warning(push)
#pragma warning(disable: 4804)
		store(value % arg);
#pragma warning(pop)
		return value;
	}

	//@隐藏{
	//move assignment
	inline CXXAtomic& operator=(CXXAtomic&& desired) noexcept
	{
		d_.store(desired.load(), std::memory_order_release);
		return *this;
	}

	//copy assignment
	inline CXXAtomic& operator=(const CXXAtomic& desired)
	{
		d_.store(desired.load(), std::memory_order_release);
		return *this;
	}

	inline CXXAtomic& operator+=(T value) { fetch_add(value); return *this; }
	inline CXXAtomic& operator-=(T value) { fetch_sub(value); return *this; }
	inline CXXAtomic& operator*=(T value) { fetch_mul(value); return *this; }
	inline CXXAtomic& operator/=(T value) { fetch_div(value); return *this; }
	inline CXXAtomic& operator%=(T value) { fetch_mod(value); return *this; }
	inline CXXAtomic& operator&=(T value) { fetch_and(value); return *this; }
	inline CXXAtomic& operator|=(T value) { fetch_or(value); return *this; }
	inline CXXAtomic& operator^=(T value) { fetch_xor(value); return *this; }

	inline operator T() const { return load(); }

	//member only of CXXAtomic<Integral ﻿> specializations
	inline T operator++() { return fetch_add(1); }
	inline T operator++(int) { fetch_add(1); return load(); }
	inline T operator--() { return fetch_sub(1); }
	inline T operator--(int) { fetch_sub(1); return load(); }

	//// ~
	//inline T operator~() { return ~load(); }

	// !
	inline bool operator!() { return !load(); }

	// ==
	inline bool operator==(T value) { return load() == value; }

	// !=
	inline bool operator!=(T value) { return load() != value; }

	// >
	inline bool operator>(T value) { return load() > value; }

	// <
	inline bool operator<(T value) { return load() < value; }

	// >=
	inline bool operator>=(T value) { return load() >= value; }

	// <=
	inline bool operator<=(T value) { return load() <= value; }

	//--------------------------------------------------------------------------------------------

	template<typename U>
	friend T operator+(U lhs, const CXXAtomic<T>& rhs) { return lhs + rhs.load(); }

	template<typename U>
	friend T operator-(U lhs, const CXXAtomic<T>& rhs) { return lhs - rhs.load(); }

	template<typename U>
	friend T operator*(U lhs, const CXXAtomic<T>& rhs) { return lhs * rhs.load(); }

	template<typename U>
	friend T operator/(U lhs, const CXXAtomic<T>& rhs)
	{
		CXX_ASSERT_X(rhs.load() == 0, L"CXXAtomic<T> divide by zero");
		return lhs / rhs.load();
	}

	template<typename U>
	friend T operator%(U lhs, const CXXAtomic<T>& rhs)
	{
		CXX_ASSERT_X(rhs.load() == 0, L"CXXAtomic<T> divide by zero");
		return lhs % rhs.load();
	}

	template<typename U>
	friend T operator&(U lhs, const CXXAtomic<T>& rhs) { return lhs & rhs.load(); }

	template<typename U>
	friend T operator|(U lhs, const CXXAtomic<T>& rhs) { return lhs | rhs.load(); }

	template<typename U>
	friend T operator^(U lhs, const CXXAtomic<T>& rhs) { return lhs ^ rhs.load(); }

	//--------------------------------------------------------------------------------------------

	template<typename U>
	friend T operator+(const CXXAtomic<T>& rhs, U lhs) { return rhs.load() + (T)lhs; }

	template<typename U>
	friend T operator-(const CXXAtomic<T>& rhs, U lhs) { return rhs.load() - (T)lhs; }

	template<typename U>
	friend T operator*(const CXXAtomic<T>& rhs, U lhs) { return rhs.load() * (T)lhs; }

	template<typename U>
	friend T operator/(const CXXAtomic<T>& rhs, U lhs)
	{
		CXX_ASSERT_X(lhs == 0, L"CXXAtomic<T> divide by zero");
		return rhs.load() / (T)lhs;
	}

	template<typename U>
	friend T operator%(const CXXAtomic<T>& rhs, U lhs)
	{
		CXX_ASSERT_X(lhs == 0, L"CXXAtomic<T> divide by zero");
		return rhs.load() % (T)lhs;
	}

	template<typename U>
	friend T operator&(const CXXAtomic<T>& rhs, U lhs) { return rhs.load() & (T)lhs; }

	template<typename U>
	friend T operator|(const CXXAtomic<T>& rhs, U lhs) { return rhs.load() | (T)lhs; }

	template<typename U>
	friend T operator^(const CXXAtomic<T>& rhs, U lhs) { return rhs.load() ^ (T)lhs; }

	//--------------------------------------------------------------------------------------------

	// ==
	template<typename U>
	friend bool operator==(U lhs, const CXXAtomic<T>& rhs) { return (T)lhs == rhs.load(); }

	// !=
	template<typename U>
	friend bool operator!=(U lhs, const CXXAtomic<T>& rhs) { return (T)lhs != rhs.load(); }

	// >
	template<typename U>
	friend bool operator>(U lhs, const CXXAtomic<T>& rhs) { return (T)lhs > rhs.load(); }

	// <
	template<typename U>
	friend bool operator<(U lhs, const CXXAtomic<T>& rhs) { return (T)lhs < rhs.load(); }

	// >=
	template<typename U>
	friend bool operator>=(U lhs, const CXXAtomic<T>& rhs) { return (T)lhs >= rhs.load(); }

	// <=
	template<typename U>
	friend bool operator<=(U lhs, const CXXAtomic<T>& rhs) { return (T)lhs <= rhs.load(); }

	//--------------------------------------------------------------------------------------------

	// ==
	template<typename U>
	friend bool operator==(const CXXAtomic<T>& rhs, U lhs) { return rhs.load() == (T)lhs; }

	// !=
	template<typename U>
	friend bool operator!=(const CXXAtomic<T>& rhs, U lhs) { return rhs.load() != (T)lhs; }

	// >
	template<typename U>
	friend bool operator>(const CXXAtomic<T>& rhs, U lhs) { return rhs.load() > (T)lhs; }

	// <
	template<typename U>
	friend bool operator<(const CXXAtomic<T>& rhs, U lhs) { return rhs.load() < (T)lhs; }

	// >=
	template<typename U>
	friend bool operator>=(const CXXAtomic<T>& rhs, U lhs) { return rhs.load() >= (T)lhs; }

	// <=
	template<typename U>
	friend bool operator<=(const CXXAtomic<T>& rhs, U lhs) { return rhs.load() <= (T)lhs; }

	// atomic<T> + atomic<T>
	friend T operator+(const CXXAtomic<T>& lhs, const CXXAtomic<T>& rhs) { return lhs.load() + rhs.load(); }

	// atomic<T> - atomic<T>
	friend T operator-(const CXXAtomic<T>& lhs, const CXXAtomic<T>& rhs) { return lhs.load() - rhs.load(); }

	// atomic<T> * atomic<T>
	friend T operator*(const CXXAtomic<T>& lhs, const CXXAtomic<T>& rhs) { return lhs.load() * rhs.load(); }

#pragma warning(push)
#pragma warning(disable: 4804)
	// atomic<T> / atomic<T>
	friend T operator/(const CXXAtomic<T>& lhs, const CXXAtomic<T>& rhs)
	{
		CXX_ASSERT_X(rhs.load() == 0, L"CXXAtomic<T> divide by zero");
		return lhs.load() / rhs.load();
	}
	//恢复 警告模式
#pragma warning(pop)

	//// atomic<T> % atomic<T>
	//friend T operator%(const CXXAtomic<T>& lhs, const CXXAtomic<T>& rhs)
	//{
	//	CXX_ASSERT_X(rhs.load() == 0, L"CXXAtomic<T> divide by zero");
	//	return lhs.load() % rhs.load();
	//}

	//// atomic<T> & atomic<T>
	//friend T operator&(const CXXAtomic<T>& lhs, const CXXAtomic<T>& rhs) { return lhs.load() & rhs.load(); }

	//// atomic<T> | atomic<T>
	//friend T operator|(const CXXAtomic<T>& lhs, const CXXAtomic<T>& rhs) { return lhs.load() | rhs.load(); }

	//// atomic<T> ^ atomic<T>
	//friend T operator^(const CXXAtomic<T>& lhs, const CXXAtomic<T>& rhs) { return lhs.load() ^ rhs.load(); }

	//// atomic<T> == atomic<T>
	//friend bool operator==(const CXXAtomic<T>& lhs, const CXXAtomic<T>& rhs) { return lhs.load() == rhs.load(); }

	//// atomic<T> != atomic<T>
	//friend bool operator!=(const CXXAtomic<T>& lhs, const CXXAtomic<T>& rhs) { return lhs.load() != rhs.load(); }

	//// atomic<T> > atomic<T>
	//friend bool operator>(const CXXAtomic<T>& lhs, const CXXAtomic<T>& rhs) { return lhs.load() > rhs.load(); }

	//// atomic<T> < atomic<T>
	//friend bool operator<(const CXXAtomic<T>& lhs, const CXXAtomic<T>& rhs) { return lhs.load() < rhs.load(); }

	//// atomic<T> >= atomic<T>
	//friend bool operator>=(const CXXAtomic<T>& lhs, const CXXAtomic<T>& rhs) { return lhs.load() >= rhs.load(); }

	//// atomic<T> <= atomic<T>
	//friend bool operator<=(const CXXAtomic<T>& lhs, const CXXAtomic<T>& rhs) { return lhs.load() <= rhs.load(); }
	//@隐藏}

private:
	//@隐藏{
	std::atomic<T> d_;
	//@隐藏}
};

//@备注 Win32/x64 自适应
typedef CXXAtomic<intptr_t> CXXAtomicInt;	 //@别名 原子整数型

//@备注 Win32/x64 自适应
typedef CXXAtomic<size_t> CXXAtomicUInt;	 //@别名 原子正整数型.0

typedef CXXAtomic<char> CXXAtomicInt8;		 //@别名 原子字符型A
typedef CXXAtomic<wchar_t> CXXAtomicWInt16;  //@别名 原子字符型
typedef CXXAtomic<BYTE> CXXAtomicUInt8;      //@别名 原子字节型
typedef CXXAtomic<short> CXXAtomicInt16;     //@别名 原子短整型
typedef CXXAtomic<USHORT> CXXAtomicUInt16;   //@别名 原子正短整型
typedef CXXAtomic<int> CXXAtomicInt32;       //@别名 原子整型
typedef CXXAtomic<UINT> CXXAtomicUInt32;     //@别名 原子正整型
typedef CXXAtomic<__int64> CXXAtomicInt64;   //@别名 原子长整型
typedef CXXAtomic<UINT64> CXXAtomicUInt64;   //@别名 原子正长整型
typedef CXXAtomic<bool> CXXAtomicBool;       //@别名 原子逻辑型
//@分组}

#endif // CXXATOMIC_H_