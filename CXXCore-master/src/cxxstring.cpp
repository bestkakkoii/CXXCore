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
#include <pch.h>

#include <cxxstring_p.h>
#include <cxxregex.h>
#include <cxxtextcodec.h>

#include <vector>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <bitset>
#include <regex>
#include <sstream>

#pragma region Private
CXXStringPrivate::CXXStringPrivate(CXXString* q, const std::wstring& d)
	: q_ptr(q)
	, data_(d)
{
}

CXXStringPrivate::~CXXStringPrivate()
{
}

std::wstring CXXStringPrivate::mapString(ULONG flags) const
{
	size_t size = data_.size();
	std::wstring dst(size, L'\0');
	LCMapStringEx(LOCALE_NAME_SYSTEM_DEFAULT, flags, data_.c_str(), (int)size, &dst[0], (int)dst.size(), CXX_NULLPTR, CXX_NULLPTR, 0);
	return dst.c_str();
}

std::wstring CXXString::convertArgToString(const wchar_t arg)
{
	std::wstring s;
	s.push_back(arg);
	return s;
}

std::wstring CXXString::convertArgToString(bool arg)
{
	return arg ? L"true" : L"false";
}

std::wstring CXXString::convertArgToString(const wchar_t* arg)
{
	return arg;
}

std::wstring CXXString::convertArgToString(const CXXString& arg)
{
	return arg.toStdWString();
}

std::string CXXStringPrivate::base64Encode(const BYTE* data, size_t len)
{
	std::string result;
	int val = 0, valb = -6;
	for (size_t i = 0; i < len; i++)
	{
		val = (val << 8) + data[i];
		valb += 8;
		while (valb >= 0)
		{
			result.push_back(base64_chars[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6) result.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
	while (result.size() % 4) result.push_back('=');
	return result;
}

std::vector<BYTE> CXXStringPrivate::base64Decode(const std::string& encoded)
{
	std::vector<int> T(256, -1);
	for (int i = 0; i < 64; i++) T[base64_chars[i]] = i;

	std::vector<BYTE> decoded;
	int val = 0, valb = -8;
	for (char c : encoded)
	{
		if (T[c] == -1) break;
		val = (val << 6) + T[c];
		valb += 6;
		if (valb >= 0)
		{
			decoded.push_back(static_cast<BYTE>((val >> valb) & 0xFF));
			valb -= 8;
		}
	}
	return decoded;
}
#pragma endregion
CXXString::CXXString()
	: d_ptr(new CXXStringPrivate(this, L""))
{
}


CXXString::CXXString(const CXXString& unicode, __int64 size)
	: d_ptr(new CXXStringPrivate(this, unicode.data()))
{
	if (size >= 0)
	{
		resize(size);
	}
}

CXXString::CXXString(const CXXString& s)
	: d_ptr(new CXXStringPrivate(this, s.data()))
{

}

CXXString::CXXString(const std::wstring& s)
	: d_ptr(new CXXStringPrivate(this, s))
{

}

CXXString::CXXString(const wchar_t* s)
	: d_ptr(new CXXStringPrivate(this, s))
{

}

CXXString::CXXString(wchar_t* s)
	: d_ptr(new CXXStringPrivate(this, s))
{

}

CXXString::CXXString(wchar_t s)
	: d_ptr(new CXXStringPrivate(this, std::wstring(1, s)))
{

}

CXXString::~CXXString()
{

}

CXXString::operator const wchar_t* () const
{
	CXX_D(const CXXString);
	return d->data_.c_str();
}

CXXString::operator wchar_t* () const
{
	CXX_D(const CXXString);
	return const_cast<wchar_t*>(d->data_.c_str());
}

CXXString& CXXString::operator=(const CXXString& s)
{
	CXX_D(CXXString);
	d->data_ = s.d_ptr->data_;
	return *this;
}

CXXString& CXXString::operator=(const wchar_t* s)
{
	CXX_D(CXXString);
	d->data_ = s;
	return *this;
}

CXXString::CXXString(CXXString&& other) noexcept
	: d_ptr(new CXXStringPrivate(this, std::move(other.d_ptr->data_)))
{
}


CXXString& CXXString::operator=(CXXString&& other) noexcept
{
	if (this != &other)
	{
		d_ptr->data_ = std::move(other.d_ptr->data_);
	}
	return *this;
}

// == 
bool CXXString::operator==(const CXXString& s) const
{
	CXX_D(const CXXString);
	return d->data_ == s.d_ptr->data_;
}

// !=
bool CXXString::operator!=(const CXXString& s) const
{
	CXX_D(const CXXString);
	return d->data_ != s.d_ptr->data_;
}

// >
bool CXXString::operator>(const CXXString& s) const
{
	CXX_D(const CXXString);
	return d->data_ > s.d_ptr->data_;
}

// <
bool CXXString::operator<(const CXXString& s) const
{
	CXX_D(const CXXString);
	return d->data_ < s.d_ptr->data_;
}

//@备注 与另一个文本交换内容
//@参数 other 另一个文本
//@别名 交换(文本)
void CXXString::swap(CXXString& other) noexcept
{
	CXX_D(CXXString);
	std::swap(d->data_, other.d_ptr->data_);
}

//@备注 返回文本的长度(字符数) 与 取数量 取长度 相同
//@返回 长度
//@别名 取大小()
__int64 CXXString::size() const
{
	CXX_D(const CXXString);
	return d->data_.size();
}

//@备注 返回文本的长度(字符数) 与 取数量 取大小 相同
//@返回 长度
//@别名 取数量()
__int64 CXXString::count() const
{
	return size();
}

//@备注 返回文本的长度(字符数) 与 取数量 取大小 相同
//@返回 长度
//@别名 取长度()
__int64 CXXString::length() const
{
	return size();
}

//@备注 检查文本是否为空
//@返回 如果文本为空则返回true，否则返回false
//@别名 是否空()
bool CXXString::isEmpty() const
{
	return size() <= 0;
}

//@备注 重新分配文本的大小
//@参数 size 新的大小
//@别名 重新分配(大小)
void CXXString::resize(__int64 size)
{
	CXX_D(CXXString);
	d->data_.resize(static_cast<size_t>(size));
}

//@备注 重新分配文本的大小，并用字符c填充
//@参数 size 新的大小
//@参数 fillChar 填充字符
//@别名 重新分配(大小, 填充字符)
void CXXString::resize(__int64 size, const CXXString& fillChar)
{
	CXX_D(CXXString);
	d->data_.resize(static_cast<size_t>(size), fillChar.front());
}

//@备注 用字符c填充文本
//@参数 c 填充字符
//@参数 size 填充的大小
//@返回 本文本
//@别名 填充(字符, 大小)
CXXString& CXXString::fill(const CXXString& c, __int64 size)
{
	CXX_D(CXXString);

	if (size < 0)
	{
		d->data_.assign(static_cast<size_t>(this->size()), c.front());
	}
	else
	{
		d->data_.assign(static_cast<size_t>(size), c.front());
	}
	return *this;
}

//@备注 将字符串缩短到指定的长度
//@参数 pos 截断位置
//@别名 截断(位置)
void CXXString::truncate(__int64 pos)
{
	if (pos < 0)
	{
		pos = 0;
	}

	if (pos < size())
	{
		resize(pos);
	}
}


//@备注 从字符串的末尾删除指定数量 n 的字符
//@参数 n 删除的数量
//@别名 切除(数量)
void CXXString::chop(__int64 n)
{
	if (n < 0)
	{
		n = 0;
	}

	if (n < size())
	{
		resize(size() - n);
	}
}

//@备注 返回文本的容量，单位为字符
// 这个容量通常等于或大于字符串的实际大小
//@返回 容量
//@别名 容量()
__int64 CXXString::capacity() const
{
	CXX_D(const CXXString);
	return d->data_.capacity();
}

//@备注 保留至少能容纳size个字符的内存，用于请求改变字符串底层存储的容量
//@参数 size 内存大小
//@别名 保留(大小)
void CXXString::reserve(__int64 size)
{
	CXX_D(CXXString);
	d->data_.reserve(static_cast<size_t>(size));
}

//@备注 释放多余的内存
//@别名 释放多余空间()
void CXXString::squeeze()
{
	CXX_D(CXXString);
	d->data_.shrink_to_fit();
}

//@备注 返回文本的数据指针
//@返回 数据指针
//@别名 取数据指针()
wchar_t* CXXString::data()
{
	CXX_D(CXXString);
	return const_cast<wchar_t*>(d->data_.c_str());
}

//@备注 返回文本的常量数据指针
//@返回 数据指针
//@别名 取常量指针()
const wchar_t* CXXString::data() const
{
	CXX_D(const CXXString);
	return d->data_.c_str();
}

//@备注 返回文本的常量数据指针
//@返回 数据指针
const wchar_t* CXXString::constData() const
{
	return data();
}

//@备注 返回文本的常量数据指针
//@返回 数据指针
const wchar_t* CXXString::c_str() const
{
	return data();
}

//@备注 返回文本的常量数据指针
//@返回 数据指针
char16_t* CXXString::utf16()
{
	return reinterpret_cast<char16_t*>(data());
}

//@备注 返回文本的常量数据指针
//@返回 数据指针
wchar_t* CXXString::get()
{
	return data();
}

const wchar_t* CXXString::get() const
{
	return data();
}

//@备注 返回文本的常量短整型数据指针
//@返回 数据指针
//@别名 取短整型数据指针()
USHORT* CXXString::ushort()
{
	return reinterpret_cast<USHORT*>(data());
}

//@备注 清空文本
//@别名 清空()
void CXXString::clear()
{
	CXX_D(CXXString);
	d->data_.clear();
}

//@备注 返回文本中指定位置的字符
//@参数 i 位置
//@返回 字符
//@别名 在(位置)
const CXXString CXXString::at(__int64 i, CXXString def) const
{
	CXX_D(const CXXString);

	if (i < 0 || i >= size())
	{
		return def;
	}

	return d->data_.at(static_cast<size_t>(i));
}

//@备注 返回文本中指定位置的字符
//@参数 i 位置
//@返回 字符
//@别名 在(位置)
const CXXString CXXString::value(__int64 i, CXXString def) const
{
	return at(i, def);
}

wchar_t& CXXString::operator[](__int64 i)
{
	CXX_ASSERT_X(i >= 0 && i < size(), L"string::operator[]: index out of range");
	if (i < 0 || i >= size())
	{
		static wchar_t null = '\0';
		return null;
	}

	CXX_D(CXXString);
	return d->data_[static_cast<size_t>(i)];
}

//@备注 返回最前面的字符
//@返回 字符
//@别名 首()
wchar_t CXXString::front() const
{
	CXX_D(const CXXString);
	return d->data_.front();
}

//@备注 返回最后面的字符
//@返回 字符
//@别名 尾()
wchar_t CXXString::back() const
{
	CXX_D(const CXXString);
	return d->data_.back();
}

//@备注 返回最前面的字符
//@返回 字符
wchar_t CXXString::first() const
{
	return front();
}

//@备注 返回最后面的字符
//@返回 字符
wchar_t CXXString::last() const
{
	return back();
}

//@备注 从文本最后方查找字符c所在的位置
//@参数 c 字符
//@参数 from 开始查找的位置
//@参数 cs 是否区分大小写
//@返回 位置
//@别名 从最后查找(字符, 开始位置, 是否区分大小写)
__int64 CXXString::lastIndexOf(const wchar_t* c, __int64 from, cxx::CaseSensitivity cs) const
{
	CXX_D(const CXXString);

	if (cxx::CaseInsensitive == cs)
	{
		std::wstring s = cxx_private_string_tool::tolower(d->data_);
		auto pos = s.rfind(c, static_cast<size_t>(from));
		if (pos != npos)
		{
			return pos;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		auto pos = d->data_.rfind(c, static_cast<size_t>(from));
		if (pos != npos)
		{
			return pos;
		}
		else
		{
			return -1;
		}
	}
}

//@备注 从文本最后方查找文本s所在的位置
//@参数 s 文本
//@参数 from 开始查找的位置
//@参数 n 查找的数量
//@参数 cs 是否区分大小写
//@返回 位置
//@别名 从最后查找(字符, 开始位置, 数量, 是否区分大小写)
__int64 CXXString::lastIndexOf(const CXXString& s, __int64 from, cxx::CaseSensitivity cs) const
{
	CXX_D(const CXXString);

	if (cxx::CaseInsensitive == cs)
	{
		std::wstring s1 = cxx_private_string_tool::tolower(d->data_);
		std::wstring s2 = cxx_private_string_tool::tolower(s.d_ptr->data_);
		auto pos = s1.rfind(s2, static_cast<size_t>(from));
		if (pos != npos)
		{
			return pos;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		auto pos = d->data_.rfind(s.d_ptr->data_, static_cast<size_t>(from));
		if (pos != npos)
		{
			return pos;
		}
		else
		{
			return -1;
		}
	}
}

//@备注 查找文本中是否存在指定的字符c
//@参数 c 字符
//@参数 cs 是否区分大小写
//@返回 如果存在则返回true，否则返回false
//@别名 包含(字符, 是否区分大小写)
bool CXXString::contains(const CXXString& c, cxx::CaseSensitivity cs) const
{
	if (isEmpty())
	{
		return false;
	}

	return indexOf(c, 0, cs) != -1;
}

//@备注 查找文本中是否存在指定的字符c
//@参数 c 字符
//@参数 from 开始查找的位置
//@参数 cs 是否区分大小写
//@返回 如果存在则返回true，否则返回false
//@别名 包含(字符, 开始位置, 是否区分大小写)
bool CXXString::contains(const CXXString& c, __int64 from, cxx::CaseSensitivity cs) const
{
	return indexOf(c, from, cs) != -1;
}

//@备注 查找文本中是否存在指定的字符c
//@参数 c 字符
//@参数 from 开始查找的位置
//@参数 n 查找的数量
//@参数 cs 是否区分大小写
//@返回 如果存在则返回true，否则返回false
//@别名 包含(字符, 开始位置, 数量, 是否区分大小写)
bool CXXString::contains(const CXXString& c, __int64 from, __int64 n, cxx::CaseSensitivity cs) const
{
	return indexOf(c, from, n, cs) != -1;
}

//@备注 计算指定字符在本文本中出现的次数
//@参数 c 指定字符
//@参数 cs 是否区分大小写
//@返回 出现的次数
//@别名 计数(文本, 是否区分大小写)
__int64 CXXString::count(wchar_t* c, cxx::CaseSensitivity cs) const
{
	CXX_D(const CXXString);

	std::wstring wcmp = c;
	std::wstring wsrc = d->data_;

	if (cxx::CaseInsensitive == cs)
	{
		wcmp = cxx_private_string_tool::tolower(wcmp);
		wsrc = cxx_private_string_tool::tolower(wsrc);
	}

	__int64 count = 0;
	__int64 pos = 0;
	while (pos != npos)
	{
		pos = wsrc.find(wcmp, static_cast<size_t>(pos));
		if (pos != npos)
		{
			count++;
			pos += wcmp.size();
		}
	}

	return count;
}

//@备注 计算指定文本在本文本中出现的次数
//@参数 s 指定文本
//@参数 cs 是否区分大小写
//@返回 出现的次数
//@别名 计数(文本, 是否区分大小写)
__int64 CXXString::count(const CXXString& s, cxx::CaseSensitivity cs) const
{
	CXX_D(const CXXString);

	std::wstring wcmp = s.d_ptr->data_;
	std::wstring wsrc = d->data_;

	if (cxx::CaseInsensitive == cs)
	{
		wcmp = cxx_private_string_tool::tolower(wcmp);
		wsrc = cxx_private_string_tool::tolower(wsrc);
	}

	__int64 count = 0;
	__int64 pos = 0;
	while (pos != npos)
	{
		pos = wsrc.find(wcmp, static_cast<size_t>(pos));
		if (pos != npos)
		{
			count++;
			pos += wcmp.size();
		}
	}

	return count;
}

//@备注 返回指定字符在本文本中的位置
//@参数 c 指定字符
//@参数 from 开始查找的位置
//@参数 cs 是否区分大小写
//@返回 位置 失败返回-1
//@别名 查找(字符, 开始位置, 是否区分大小写)
__int64 CXXString::indexOf(const CXXString& c, __int64 from, cxx::CaseSensitivity cs) const
{
	CXX_D(const CXXString);

	if (cxx::CaseInsensitive == cs)
	{
		auto pos = cxx_private_string_tool::tolower(d->data_).find(c.toLower().d_ptr->data_, static_cast<size_t>(from));
		if (pos != npos)
		{
			return pos;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		auto pos = d->data_.find(c.d_ptr->data_, static_cast<size_t>(from));
		if (pos != npos)
		{
			return pos;
		}
		else
		{
			return -1;
		}
	}
}

//@备注 返回指定字符在本文本中的位置
//@参数 c 指定字符
//@参数 from 开始查找的位置
//@参数 cs 是否区分大小写
//@返回 位置 失败返回-1
//@别名 查找(字符, 开始位置, 是否区分大小写)
__int64 CXXString::indexOf(const wchar_t* c, __int64 from, cxx::CaseSensitivity cs) const
{
	CXX_D(const CXXString);

	if (cxx::CaseInsensitive == cs)
	{
		auto pos = cxx_private_string_tool::tolower(d->data_).find(c, static_cast<size_t>(from));
		if (pos != npos)
		{
			return pos;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		auto pos = find(c, static_cast<size_t>(from));
		if (pos != npos)
		{
			return pos;
		}
		else
		{
			return -1;
		}
	}

}

//@备注 返回指定字符在本文本中的位置
//@参数 c 指定字符
//@参数 from 开始查找的位置
//@参数 n 查找的数量 默认为-1，表示查找全部
//@参数 cs 是否区分大小写
//@返回 位置 失败返回-1
//@别名 查找(字符, 开始位置, 数量, 是否区分大小写)
__int64 CXXString::indexOf(const wchar_t* c, __int64 from, __int64 n, cxx::CaseSensitivity cs) const
{
	CXX_D(const CXXString);

	if (cxx::CaseInsensitive == cs)
	{
		std::wstring s = cxx_private_string_tool::tolower(d->data_);

		auto pos = s.find(c, static_cast<size_t>(from), static_cast<size_t>(n));
		if (pos != npos)
		{
			return pos;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		auto pos = d->data_.find(c, static_cast<size_t>(from), static_cast<size_t>(n));
		if (pos != npos)
		{
			return pos;
		}
		else
		{
			return -1;
		}
	}
}

//@备注 使用正则表达式查找文本中的位置
//@参数 re 正则表达式
//@参数 from 开始查找的位置
//@返回 位置 失败返回-1
//@别名 查找(正则表达式, 开始位置)
__int64 CXXString::indexOf(const CXXRegex& re, __int64 from) const
{
	CXX_D(const CXXString);

	std::wstring mid = d->data_;
	if (from > 0)
	{
		mid = d->data_.substr(static_cast<size_t>(from));
	}

	std::wsmatch m;

	if (std::regex_search(mid, m, re.raw(), std::regex_constants::match_continuous))
	{
		return m.position();
	}
	else
	{
		return -1;
	}
}

//@备注 使用正则表达式查找文本中的位置
//@参数 re 正则表达式
//@参数 from 开始查找的位置
//@参数 rmatch 接收匹配结果的指针
//@返回 位置 失败返回-1
//@别名 查找(正则表达式, 开始位置, 匹配结果指针)
__int64 CXXString::indexOf(const CXXRegex& re, __int64 from, CXXRegexMatch* rmatch) const
{
	CXX_D(const CXXString);

	std::wstring mid = d->data_;
	if (from > 0)
	{
		mid = d->data_.substr(static_cast<size_t>(from));
	}

	std::wsmatch m;

	if (std::regex_search(mid, m, re.raw(), std::regex_constants::match_continuous))
	{
		rmatch->raw().swap(m);
		return m.position();
	}
	else
	{
		return -1;
	}
}

//@备注 从文本最后方查找正则匹配文本所在的位置
//@参数 re 正则表达式
//@参数 from 开始查找的位置
//@返回 位置
//@别名 从最后查找(正则表达式, 开始位置)
__int64 CXXString::lastIndexOf(const CXXRegex& re, __int64 from) const
{
	CXX_D(const CXXString);

	std::wstring mid = d->data_;
	if (from > 0)
	{
		// 从指定位置开始查找 逆向
		mid = d->data_.substr(0, static_cast<size_t>(from));
	}

	std::wsmatch m;

	if (std::regex_search(mid, m, re.raw(), std::regex_constants::match_continuous))
	{
		return m.position();
	}
	else
	{
		return -1;
	}


}

//@备注 从文本最后方查找正则匹配文本所在的位置
//@参数 re 正则表达式
//@参数 from 开始查找的位置
//@参数 rmatch 接收匹配结果的指针
//@返回 位置
//@别名 从最后查找(正则表达式, 开始位置, 匹配结果指针)
__int64 CXXString::lastIndexOf(const CXXRegex& re, __int64 from, CXXRegexMatch* rmatch) const
{
	CXX_D(const CXXString);

	std::wstring mid = d->data_;

	if (from > 0)
	{
		// 从指定位置开始查找 逆向
		mid = d->data_.substr(0, static_cast<size_t>(from));
	}

	std::wsmatch m;

	if (std::regex_search(mid, m, re.raw(), std::regex_constants::match_continuous))
	{
		rmatch->raw().swap(m);
		return m.position();
	}
	else
	{
		return -1;
	}
}

//@备注 使用正则表达式判断文本中是否存在指定的匹配
//@参数 re 正则表达式
//@返回 如果存在则返回true，否则返回false
//@别名 包含(正则表达式)
bool CXXString::contains(const CXXRegex& re) const
{
	return indexOf(re) != -1;
}

//@备注 使用正则表达式判断文本中是否存在指定的匹配
//@参数 re 正则表达式
//@参数 rmatch 接收匹配结果的指针
//@返回 如果存在则返回true，否则返回false
//@别名 包含(正则表达式, 匹配结果指针)
bool CXXString::contains(const CXXRegex& re, CXXRegexMatch* rmatch) const
{
	return indexOf(re, 0, rmatch) != -1;
}

//@备注 使用正则表达式判断文本中匹配的次数
//@参数 re 正则表达式
//@返回 匹配的次数
//@别名 计数(正则表达式)
__int64 CXXString::count(const CXXRegex& re) const
{
	CXX_D(const CXXString);

	std::wregex regexPattern(re.raw());
	std::wsregex_iterator begin = std::wsregex_iterator(d->data_.begin(), d->data_.end(), regexPattern);
	std::wsregex_iterator end = std::wsregex_iterator();

	return std::distance(begin, end);
}

//@备注 返回字符串从左边开始的前 n 个字符。如果 n 大于字符串长度，则返回整个字符串。
//@参数 n 截取的数量
//@返回 截取的字符串
//@别名 左(数量)
CXXString CXXString::left(__int64 n) const
{
	CXX_D(const CXXString);
	CXX_ASSERT(n >= 0); CXX_ASSERT(n <= size());
	return d->data_.substr(0, static_cast<size_t>(n)).c_str();
}

//@备注 返回字符串从右边开始的后 n 个字符。如果 n 大于字符串长度，则返回整个字符串。
//@参数 n 截取的数量
//@返回 截取的字符串
//@别名 右(数量)
CXXString CXXString::right(__int64 n) const
{
	CXX_D(const CXXString);
	CXX_ASSERT(n >= 0); CXX_ASSERT(n <= size());
	return d->data_.substr(static_cast<size_t>(size() - n)).c_str();
}

//@备注 从字符串的 position 位置开始，返回长度为 n 的子串。如果 n 为负值，则返回从 position 到字符串末尾的所有字符
//@参数 position 开始位置
//@参数 n 截取的数量
//@返回 截取的字符串
//@别名 取中(开始位置, 数量)
CXXString CXXString::mid(__int64 position, __int64 n) const
{
	CXX_D(const CXXString);
	CXX_ASSERT(position >= 0);
	CXX_ASSERT(position <= size());
	if (n < 0)
	{
		return d->data_.substr(static_cast<size_t>(position)).c_str();
	}
	else
	{
		return d->data_.substr(static_cast<size_t>(position), static_cast<size_t>(n)).c_str();
	}
}

//@备注 返回一个新的字符串，该字符串是原字符串去除尾部 n 个字符后的结果。如果 n 等于字符串长度，则返回空字符串
//@参数 n 删除的数量
//@返回 截取的字符串
//@别名 切除尾部(数量)
CXXString CXXString::chopped(__int64 n) const
{
	CXX_ASSERT(n >= 0); CXX_ASSERT(n <= size()); return left(size() - n);
}

//@备注 检查字符串是否以指定的子串 c 开头。可以指定大小写敏感或不敏感。
//@参数 c 子串
//@参数 cs 是否区分大小写
//@返回 如果以指定的子串开头则返回true，否则返回false
//@别名 是否起始于(文本, 是否区分大小写)
bool CXXString::startsWith(const CXXString& c, cxx::CaseSensitivity cs) const
{
	if (cxx::CaseInsensitive == cs)
	{
		return toLower().d_ptr->data_.find(c.toLower().d_ptr->data_) == 0;
	}
	else
	{
		CXX_D(const CXXString);
		return d->data_.find(c.d_ptr->data_) == 0;
	}
}

//@备注 检查字符串是否以指定的子串 c 结尾。可以指定大小写敏感或不敏感。
//@参数 c 子串
//@参数 cs 是否区分大小写
//@返回 如果以指定的子串结尾则返回true，否则返回false
//@别名 是否结束于(文本, 是否区分大小写)
bool CXXString::endsWith(const CXXString& c, cxx::CaseSensitivity cs) const
{
	// 子串比原字符串长，直接返回 false
	if (c.size() > size())
	{
		return false;
	}

	size_t startPos = static_cast<size_t>(size() - c.size());

	if (cxx::CaseInsensitive == cs)
	{
		// 使用 std::equal 进行大小写不敏感比较
		return std::equal(c.d_ptr->data_.begin(), c.d_ptr->data_.end(),
			d_ptr->data_.begin() + startPos, d_ptr->data_.end(),
			[](wchar_t a, wchar_t b) { return std::tolower(a) == std::tolower(b); });
	}
	else
	{
		// 大小写敏感的直接比较
		return std::equal(c.d_ptr->data_.begin(), c.d_ptr->data_.end(),
			d_ptr->data_.begin() + startPos);
	}
}

//@备注 判断文本是否全大写
//@返回 如果全大写则返回true，否则返回false
//@别名 是否全大写()
bool CXXString::isUpper() const
{
	CXX_D(const CXXString);
	return std::all_of(d->data_.begin(), d->data_.end(), ::iswupper);
}

//@备注 判断文本是否全小写
//@返回 如果全小写则返回true，否则返回false
//@别名 是否全小写()
bool CXXString::isLower() const
{
	CXX_D(const CXXString);
	return std::all_of(d->data_.begin(), d->data_.end(), ::iswlower);
}

//@备注 返回一个新的字符串，该字符串是原字符串左对齐，并使用 fill 字符填充至 width 个字符的长度。如果原字符串长度大于或等于 width，则返回原字符串。
//@参数 width 宽度
//@参数 fill 填充字符
//@参数 trunc 如果为true，则截断字符串
//@返回 左对齐的字符串
//@别名 到左对齐(宽度, 填充字符, 是否截断)
CXXString CXXString::leftJustified(__int64 width, const CXXString& fill, bool trunc) const
{
	CXX_D(const CXXString);

	CXXString result;
	if (width <= size())
	{
		return result;
	}

	std::wstringstream ss;
	ss << d->data_;
	size_t fillCount = static_cast<size_t>(width - size());
	while (fillCount > 0)
	{
		ss << fill.data();
		--fillCount;
	}

	result = ss.str().c_str();
	if (trunc && result.size() > width)
	{
		result.resize(width);
	}

	return result;
}

//@备注 返回一个新的字符串，该字符串是原字符串右对齐，并使用 fill 字符填充至 width 个字符的长度。如果原字符串长度大于或等于 width，则返回原字符串。
//@参数 width 宽度
//@参数 fill 填充字符
//@参数 trunc 如果为true，则截断字符串
//@返回 右对齐的字符串
//@别名 到右对齐(宽度, 填充字符, 是否截断)
CXXString CXXString::rightJustified(__int64 width, const CXXString& fill, bool trunc) const
{
	CXX_D(const CXXString);

	CXXString result;
	if (width <= size())
	{
		return result;
	}

	std::wstringstream ss;
	size_t fillCount = static_cast<size_t>(width - size());
	while (fillCount > 0)
	{
		ss << fill.data();
		--fillCount;
	}
	ss << d->data_;

	result = ss.str().c_str();
	if (trunc && result.size() > width)
	{
		result = result.d_ptr->data_.substr(static_cast<size_t>(result.size() - width), static_cast<size_t>(width)).c_str();
	}

	return result;
}

//@备注 转换为小写
//@返回 转换后的文本
//@别名 到小写()
CXXString CXXString::toLower() const
{
	CXX_D(const CXXString);
	std::wstring result = d->data_; // 拷贝一次
	std::transform(result.begin(), result.end(), result.begin(), ::towlower);
	return CXXString(std::move(result)); // 移动构造，避免再次拷贝
}

//@备注 转换为大写
//@返回 转换后的文本
//@别名 到大写()
CXXString CXXString::toUpper() const
{
	CXX_D(const CXXString);
	std::wstring result = d->data_; // 拷贝一次
	std::transform(result.begin(), result.end(), result.begin(), ::towupper);
	return CXXString(std::move(result)); // 移动构造，避免再次拷贝
}

//@备注 转换半角
//@返回 转换后的文本
//@别名 到半角()
CXXString CXXString::toHalf() const
{
	CXX_D(const CXXString);
	return d->mapString(LCMAP_HALFWIDTH);
}

//@备注 转换全角
//@返回 转换后的文本
//@别名 到全角()
CXXString CXXString::toFull() const
{
	CXX_D(const CXXString);
	return d->mapString(LCMAP_FULLWIDTH);
}

//@备注 修剪文本两端的空白字符，包含全角空格
//@返回 修剪后的文本
//@别名 修剪()
CXXString CXXString::trimmed() const
{
	CXX_D(const CXXString);
	return cxx_private_string_tool::trim(d->data_);
}

//@备注 修剪文本两端的空白字符，包含全角空格，字符串中间多余的空白字符被替换为一个空格
//@返回 修剪后的文本
//@别名 全修剪()
CXXString CXXString::simplified() const
{
	CXX_D(const CXXString);
	return cxx_private_string_tool::simplified(d->data_);
}

//@备注 将文本中部份字符替换为HTML转义字符
//@返回 转义后的文本
//@别名 到html()
CXXString CXXString::toHtmlEscaped() const
{
	CXX_D(const CXXString);

	CXXString rich;
	const size_t len = static_cast<size_t>(length());
	rich.reserve(__int64(1.1 * len));
	for (size_t i = 0; i < len; ++i)
	{
		if (at(i) == L'<')
			rich += L"&lt;";
		else if (at(i) == L'>')
			rich += L"&gt;";
		else if (at(i) == '&')
			rich += L"&amp;";
		else if (at(i) == '"')
			rich += L"&quot;";
		else if (at(i) == L'￠')
			rich += L"&cent;";
		else if (at(i) == L'£')
			rich += L"&pound;";
		else if (at(i) == L'¥')
			rich += L"&yen;";
		else if (at(i) == L'§')
			rich += L"&sect;";
		else if (at(i) == L'©')
			rich += L"&copy;";
		else if (at(i) == L'®')
			rich += L"&reg;";
		else if (at(i) == L'×')
			rich += L"&times;";
		else if (at(i) == L'÷')
			rich += L"&divide;";
		else if (at(i) == L'™')
			rich += L"&trade;";
		else if (at(i) == L'　')
			rich += L"&emsp;";
		else if (at(i) == L' ')
			rich += L"&ensp;";
		else
			rich += CXXString(L"&#%1;").format(static_cast<USHORT>(d->data_.at(i)));
	}
	rich.squeeze();
	return rich;
}

//@备注 将文本中部份字符替换为URL转义字符
//@返回 转义后的文本
//@别名 到url()
CXXString CXXString::toUrl() const
{
	CXX_D(const CXXString);

	std::wostringstream escaped;

	for (const wchar_t& wc : d->data_)
	{
		if ((wc >= L'a' && wc <= L'z') ||
			(wc >= L'A' && wc <= L'Z') ||
			(wc >= L'0' && wc <= L'9') ||
			wc == L'-' || wc == L'_' || wc == L'.' || wc == L'~')
		{
			escaped << wc;
		}
		else
		{
			unsigned high = (wc >> 8) & 0xFF;
			unsigned low = wc & 0xFF;

			if (high)
			{
				escaped << '%' << std::setw(2) << std::setfill(L'0') << std::uppercase << std::hex << high;
			}
			escaped << '%' << std::setw(2) << std::setfill(L'0') << std::uppercase << std::hex << low;
		}
	}

	return escaped.str().c_str();
}

//@备注 在指定位置插入字符
//@参数 i 位置
//@参数 c 字符
//@返回 插入后的文本
//@别名 插入(位置, 字符)
CXXString& CXXString::insert(__int64 i, wchar_t* c)
{
	CXX_D(CXXString);
	d->data_.insert(static_cast<size_t>(i), c);
	return *this;
}

//@备注 在指定位置插入字符
//@参数 i 位置
//@参数 uc 字符
//@参数 len 字符数量
//@返回 插入后的文本
//@别名 插入(位置, 字符, 数量)
CXXString& CXXString::insert(__int64 i, const wchar_t* uc, __int64 len)
{
	CXX_D(CXXString);
	d->data_.insert(static_cast<size_t>(i), uc, static_cast<size_t>(len));
	return *this;
}

//@备注 在指定位置插入文本
//@参数 i 位置
//@参数 s 文本
//@返回 插入后的文本
//@别名 插入(位置, 文本)
CXXString& CXXString::insert(__int64 i, const CXXString& s)
{
	CXX_D(CXXString);
	d->data_.insert(static_cast<size_t>(i), s.d_ptr->data_);
	return *this;
}

//@备注 从末尾添加字符
//@参数 c 字符
//@返回 添加后的文本
//@别名 追加(字符)
CXXString& CXXString::append(wchar_t* c)
{
	CXX_D(CXXString);
	d->data_ += c;
	return *this;
}

CXXString& CXXString::append(wchar_t c)
{
	CXX_D(CXXString);
	d->data_ += c;
	return *this;
}

//@备注 从末尾添加字符
//@参数 uc 常量字符
//@参数 len 字符数量
//@返回 添加后的文本
//@别名 追加(字符, 数量)
CXXString& CXXString::append(const wchar_t* uc, __int64 len)
{
	CXX_D(CXXString);
	d->data_.append(uc, static_cast<size_t>(len));
	return *this;
}

//@备注 从末尾添加文本
//@参数 s 文本
//@返回 添加后的文本
//@别名 追加(文本)
CXXString& CXXString::append(const CXXString& s)
{
	CXX_D(CXXString);
	d->data_ += s.d_ptr->data_;
	return *this;
}

//@备注 从文本最前方添加字符
//@参数 c 字符
//@返回 添加后的文本
//@别名 前置(字符)
CXXString& CXXString::prepend(wchar_t* c)
{
	CXX_D(CXXString);
	d->data_.insert(0, c);
	return *this;
}

//@备注 从文本最前方添加字符
//@参数 uc 常量字符
//@参数 len 字符数量
//@返回 添加后的文本
//@别名 前置(字符, 数量)
CXXString& CXXString::prepend(const wchar_t* uc, __int64 len)
{
	CXX_D(CXXString);
	d->data_.insert(0, uc, static_cast<size_t>(len));
	return *this;
}

//@备注 从文本最前方添加文本
//@参数 s 文本
//@返回 添加后的文本
//@别名 前置(文本)
CXXString& CXXString::prepend(const CXXString& s)
{
	CXX_D(CXXString);
	d->data_.insert(0, s.d_ptr->data_);
	return *this;
}

CXXString& CXXString::operator+=(const CXXString& s) { return append(s); }

CXXString& CXXString::operator+=(wchar_t* c) { return append(c); }

CXXString& CXXString::operator+=(const wchar_t* c) { return append(c); }

CXXString& CXXString::operator+=(wchar_t c) { return append(c); }

// +
CXXString& CXXString::operator+(const CXXString& s) { return append(s); }

CXXString& CXXString::operator+(wchar_t* c) { return append(c); }

CXXString& CXXString::operator+(const wchar_t* wc) { return append(wc); }

CXXString& CXXString::operator+(wchar_t wc) { return append(wc); }

//@隐藏}

//@备注 从指定位置开始删除指定数量的字符
//@参数 i 开始位置
//@参数 len 删除的数量
//@返回 删除后的文本
//@别名 移除(开始位置, 数量)
CXXString& CXXString::remove(__int64 i, __int64 len)
{
	CXX_D(CXXString);
	d->data_.erase(static_cast<size_t>(i), static_cast<size_t>(len));
	return *this;
}

//@备注 删除指定的字符
//@参数 c 字符
//@参数 cs 是否区分大小写
//@返回 删除后的文本
//@别名 移除(字符, 是否区分大小写)
CXXString& CXXString::remove(wchar_t* c, cxx::CaseSensitivity cs)
{
	CXX_D(CXXString);

	std::wstring s = d->data_;
	std::wstring s1 = c;

	if (cxx::CaseInsensitive == cs)
	{
		s = cxx_private_string_tool::tolower(s);
		s1 = cxx_private_string_tool::tolower(s1);
	}

	auto pos = s.find(s1);
	if (pos != npos)
	{
		d->data_.erase(pos, s1.size());
	}

	return *this;
}

//@备注 删除指定的文本
//@参数 s 文本
//@参数 cs 是否区分大小写
//@返回 删除后的文本
//@别名 移除(文本, 是否区分大小写)
CXXString& CXXString::remove(const CXXString& s, cxx::CaseSensitivity cs)
{
	CXX_D(CXXString);

	std::wstring s0 = d->data_;
	std::wstring s1 = s.d_ptr->data_;

	if (cxx::CaseInsensitive == cs)
	{
		s0 = cxx_private_string_tool::tolower(s0);
		s1 = cxx_private_string_tool::tolower(s1);
	}

	auto pos = s0.find(s1);
	if (pos != npos)
	{
		d->data_.erase(pos, s1.size());
	}

	return *this;
}

//@备注 从指定位置开始替换指定数量的字符
//@参数 i 开始位置
//@参数 len 替换的数量
//@参数 after 替换的字符
//@返回 替换后的文本
//@别名 替换(开始位置, 数量, 文本)
CXXString& CXXString::replace(__int64 i, __int64 len, wchar_t* after)
{
	CXX_D(CXXString);
	d->data_.replace(static_cast<size_t>(i), static_cast<size_t>(len), after);
	return *this;
}

//@备注 从指定位置开始替换指定数量的字符
//@参数 i 开始位置
//@参数 len 替换的数量
//@参数 s 替换的字符
//@参数 slen 替换的字符数量
//@返回 替换后的文本
//@别名 替换(开始位置, 数量, 文本, 数量)
CXXString& CXXString::replace(__int64 i, __int64 len, const wchar_t* s, __int64 slen)
{
	CXX_D(CXXString);
	d->data_.replace(static_cast<size_t>(i), static_cast<size_t>(len), s, static_cast<size_t>(slen));
	return *this;
}

//@备注 从指定位置开始替换指定数量的字符
//@参数 i 开始位置
//@参数 len 替换的数量
//@参数 after 替换的文本
//@返回 替换后的文本
//@别名 替换(开始位置, 数量, 文本)
CXXString& CXXString::replace(__int64 i, __int64 len, const CXXString& after)
{
	CXX_D(CXXString);
	d->data_.replace(static_cast<size_t>(i), static_cast<size_t>(len), after.d_ptr->data_);
	return *this;
}

//@备注 替换指定的字符
//@参数 before 被替换的字符
//@参数 after 替换的字符
//@参数 cs 是否区分大小写
//@返回 替换后的文本
//@别名 替换(字符, 字符, 是否区分大小写)
CXXString& CXXString::replace(const wchar_t* before, const wchar_t* after, cxx::CaseSensitivity cs)
{
	CXX_D(CXXString);

	for (auto i = d->data_.begin(); i != d->data_.end(); i++)
	{
		if (cxx::CaseInsensitive == cs)
		{
			if (std::toupper(*i) == std::toupper(before[0]))
			{
				auto pos = i - d->data_.begin();
				d->data_.replace(pos, std::wcslen(before), after);
			}
		}
		else
		{
			if (*i == before[0])
			{
				auto pos = i - d->data_.begin();
				d->data_.replace(pos, std::wcslen(before), after);
			}
		}
	}

	return *this;
}

//@备注 替换指定的文本
//@参数 before 被替换的文本
//@参数 blen 被替换的文本数量
//@参数 after 替换的文本
//@参数 alen 替换的文本数量
//@参数 cs 是否区分大小写
//@返回 替换后的文本
//@别名 替换(文本, 文本, 是否区分大小写)
CXXString& CXXString::replace(const wchar_t* before, __int64 blen, const wchar_t* after, __int64 alen, cxx::CaseSensitivity cs)
{
	std::wstring s1 = std::wstring(before, static_cast<size_t>(blen));
	std::wstring s2 = std::wstring(after, static_cast<size_t>(alen));
	return replace(s1.c_str(), s2.c_str(), cs);
}

//@备注 替换指定的文本
//@参数 before 被替换的文本
//@参数 after 替换的文本
//@参数 cs 是否区分大小写
//@返回 替换后的文本
//@别名 替换(文本, 文本, 是否区分大小写)
CXXString& CXXString::replace(const CXXString& before, const CXXString& after, cxx::CaseSensitivity cs)
{
	std::wstring s1 = std::wstring(before.data());
	std::wstring s2 = std::wstring(after.data());

	return replace(s1.c_str(), s2.c_str(), cs);
}

//@备注 替换指定的文本
//@参数 before 被替换的文本
//@参数 after 替换的文本
//@参数 cs 是否区分大小写
//@返回 替换后的文本
//@别名 替换(文本, 文本, 是否区分大小写)
CXXString& CXXString::replace(wchar_t* c, const CXXString& after, cxx::CaseSensitivity cs)
{
	return replace(CXXString(c), after, cs);
}

//@备注 正则替换指定的文本
//@参数 re 正则表达式
//@参数 after 替换的文本'
//@返回 替换后的文本
//@别名 替换(正则表达式, 文本)
CXXString& CXXString::replace(const CXXRegex& re, const CXXString& after)
{
	CXX_D(CXXString);

	d->data_ = std::regex_replace(d->data_, re.raw(), after.d_ptr->data_);
	return *this;
}

//@备注 使用正则表达式移除成功匹配的文本
//@参数 re 正则表达式
//@返回 移除后的文本
//@别名 移除(正则表达式)
CXXString& CXXString::remove(const CXXRegex& re)
{
	return replace(re, CXXString());
}

//@备注 将文本分割成炫::动态数组
	//@参数 sep 分割文本
	//@参数 behavior 分割行为
	//@参数 cs 是否区分大小写
	//@返回 分割后的数组
	//@别名 分割(分割文本, 分割行为, 是否区分大小写)
CXXVector<CXXString> CXXString::split(const CXXString& sep, cxx::SplitBehavior behavior, cxx::CaseSensitivity cs) const
{
	CXX_D(const CXXString);

	CXXVector<CXXString> parts;
	size_t startPos = 0;
	size_t foundPos = 0;

	if (cs == cxx::CaseInsensitive)
	{
		std::wstring data = cxx_private_string_tool::tolower(d->data_);
		std::wstring sepData = cxx_private_string_tool::tolower(sep.d_ptr->data_);

		while ((foundPos = data.find(sepData, startPos)) != npos)
		{
			std::wstring part = d->data_.substr(startPos, foundPos - startPos);
			if (behavior == cxx::KeepEmptyParts || part.size() > 0)
			{
				parts.append(part.c_str());
			}

			startPos = foundPos + static_cast<size_t>(sep.length());
		}
	}
	else
	{
		while ((foundPos = d->data_.find(sep.d_ptr->data_, startPos)) != npos)
		{
			std::wstring part = d->data_.substr(startPos, foundPos - startPos);
			if (behavior == cxx::KeepEmptyParts || part.size() > 0)
			{
				parts.append(part.c_str());
			}

			startPos = foundPos + static_cast<size_t>(sep.length());
		}
	}

	if (startPos < d->data_.size())
	{
		std::wstring part = d->data_.substr(startPos);
		if (behavior == cxx::KeepEmptyParts || part.size() > 0)
		{
			parts.append(part.c_str());
		}
	}

	return parts;

}

//@备注 将文本分割成炫::动态数组
//@参数 sep 分割字符
//@参数 behavior 分割行为
//@参数 cs 是否区分大小写
//@返回 分割后的数组
//@别名 分割(分割字符, 分割行为, 是否区分大小写)
CXXVector<CXXString> CXXString::split(wchar_t* sep, cxx::SplitBehavior behavior, cxx::CaseSensitivity cs) const
{
	return split(CXXString(sep), behavior, cs);
}

//@备注 将文本分割成炫::动态数组
//@参数 sep 正则表达式
//@参数 behavior 分割行为
//@返回 分割后的数组
//@别名 分割(正则表达式, 分割行为)
CXXVector<CXXString> CXXString::split(const CXXRegex& sep, cxx::SplitBehavior behavior) const
{
	CXX_D(const CXXString);

	CXXVector<CXXString> parts;
	const std::wregex wsep = sep.raw();
	std::wsregex_token_iterator iter(d->data_.cbegin(), d->data_.cend(), wsep, -1);
	std::wsregex_token_iterator end;

	for (; iter != end; ++iter)
	{
		if (behavior == cxx::KeepEmptyParts || iter->str().size() > 0)
		{
			parts.append(iter->str().c_str());
		}
	}

	return parts;
}

//@备注 返回重复n次的字符串
//@参数 times 重复次数
//@返回 重复后的文本
//@别名 重复(次数)
CXXString CXXString::repeated(__int64 times) const
{
	CXX_D(const CXXString);

	CXXString result;
	for (__int64 i = 0; i < times; i++)
	{
		result += d->data_;
	}
	return result;
}

CXXString CXXString::toHexString(const char* input, __int64 size)
{
	std::wstringstream ss;

	// 遍历字节集，将每个字节转换为 16 进制表示
	for (__int64 i = 0; i < size; ++i)
	{
		ss << std::hex << std::setw(2) << std::setfill(L'0') << static_cast<int>(input[i]);
	}

	return ss.str();
}

CXXString CXXString::toHexString(const CXXString& separator) const
{
	const std::string str = reinterpret_cast<char*>(toUtf8().data());
	const std::string sseparator = reinterpret_cast<char*>(separator.toUtf8().data());
	constexpr const char* hex = u8"0123456789ABCDEF";
	std::stringstream ss;

	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		ss << hex[(unsigned char)str[i] >> 4] << hex[(unsigned char)str[i] & 0xf] << sseparator;
	}

	return CXXString::fromUtf8(ss.str().c_str());
}

CXXString CXXString::toHexString(const CXXVector<BYTE>& input)
{
	std::wstringstream ss;

	// 遍历字节集，将每个字节转换为 16 进制表示
	for (const auto& byte : input)
	{
		ss << std::hex << std::setw(2) << std::setfill(L'0') << static_cast<int>(byte);
	}

	return ss.str();
}

CXXVector<BYTE> CXXString::fromHexString(const CXXString& hex)
{
	CXXVector<BYTE> result;

	// 将十六进制字符串转换为字节集
	for (__int64 i = 0; i < hex.size(); i += 2)
	{
		result.append(static_cast<BYTE>(std::stoi(hex.mid(i, 2).toStdWString(), nullptr, 16)));
	}

	return result;

}

CXXString CXXString::toBase64() const
{
	// 获取原始的 UTF-8 数据
	const std::string& utf8Data = this->toUtf8();

	// 将 UTF-8 数据进行 Base64 编码
	std::string base64Encoded = CXXStringPrivate::base64Encode(reinterpret_cast<const BYTE*>(utf8Data.data()), utf8Data.size());

	// 返回 Base64 编码的 CXXString
	return CXXString::fromUtf8(base64Encoded.c_str());
}

CXXString CXXString::toBase64(const CXXVector<BYTE>& input)
{
	// 将字节集进行 Base64 编码
	std::string base64Encoded = CXXStringPrivate::base64Encode(input.data(), static_cast<size_t>(input.size()));

	// 返回 Base64 编码的 CXXString
	return CXXString::fromUtf8(base64Encoded.c_str());
}

CXXVector<BYTE> CXXString::fromBase64(const CXXString& base64)
{
	// 将 Base64 编码的 CXXString 转换为 UTF-8 编码的 std::string
	const std::string& base64Data = base64.toUtf8();

	// 将 Base64 编码的数据解码为字节集
	return CXXStringPrivate::base64Decode(base64Data);
}

//@别名 到BASE64()
CXXString CXXString::toBase64(const char* input, __int64 size)
{
	// 将字节集进行 Base64 编码
	std::string base64Encoded = CXXStringPrivate::base64Encode(reinterpret_cast<const BYTE*>(input), static_cast<size_t>(size));

	// 返回 Base64 编码的 CXXString
	return CXXString::fromUtf8(base64Encoded.c_str());
}

//@备注 返回文本的UTF-8编码 std::string 类型
//@返回 UTF-8编码文本
//@别名 到utf8()
std::string CXXString::toUtf8() const
{
	CXX_D(const CXXString);
	static CXXTextCodec* codec = CXXTextCodec::codecFromName(L"UTF-8");
	return codec->fromUnicode(d->data_.c_str());
}

//@备注 返回文本的本地编码 std::string 类型
//@返回 本地编码文本
//@别名 到本地编码()
std::string CXXString::toLocal8Bit() const
{
	static CXXTextCodec* codec = CXXTextCodec::codecFromACP(GetACP());
	return codec->fromUnicode(*this);
}

//@备注 返回文本的std::wstring引用类型
//@返回 std::wstring&类型文本
std::wstring& CXXString::toStdWString()
{
	CXX_D(CXXString);
	return d->data_;
}


//@备注 返回文本的std::wstring类型
//@返回 std::wstring类型文本
std::wstring CXXString::toStdWString() const
{
	CXX_D(const CXXString);
	return d->data_;
}

//@备注 返回文本的std::string类型
//@返回 std::string类型文本
//@别名 到std::string()
std::string CXXString::toStdString() const
{
	return reinterpret_cast<char*>(CXXTextCodec::codecFromACP(GetACP())->fromUnicode(*this).data());
}

//@备注 从UTF-8编码文本来源创建文本
//@参数 str UTF-8编码文本
//@返回 文本
//@别名 从ut8(文本)
CXXString CXXString::fromUtf8(const char* str)
{
	static CXXTextCodec* codec = CXXTextCodec::codecFromName(L"UTF-8");
	return codec->toUnicode(str);
}

CXXString CXXString::fromUtf8(const char* str, __int64 size)
{
	static CXXTextCodec* codec = CXXTextCodec::codecFromName(L"UTF-8");
	return codec->toUnicode(std::string(str, static_cast<size_t>(size)));
}

//@别名 从ut8(文本)
CXXString CXXString::fromUtf8(const char str)
{
	static CXXTextCodec* codec = CXXTextCodec::codecFromName(L"UTF-8");
	const char s[2] = { str, '\0' };
	return codec->toUnicode(s);
}

CXXString CXXString::fromUtf8(CXXVector<BYTE> bytes)
{
	static CXXTextCodec* codec = CXXTextCodec::codecFromName(L"UTF-8");
	std::string str(reinterpret_cast<char*>(bytes.data()), static_cast<size_t>(bytes.size()));

	return codec->toUnicode(str);
}

//@备注 从本地编码文本来源创建文本
//@参数 str 本地编码文本
//@返回 文本
//@别名 从本地编码(文本)
CXXString CXXString::fromLocal8Bit(const char* str)
{
	static CXXTextCodec* codec = CXXTextCodec::codecFromACP(GetACP());
	return codec->toUnicode(str);
}

//@别名 从本地编码(文本)
CXXString CXXString::fromLocal8Bit(const char str)
{
	static CXXTextCodec* codec = CXXTextCodec::codecFromACP(GetACP());
	const char s[2] = { str, '\0' };
	return codec->toUnicode(s);
}

CXXString CXXString::fromLocal8Bit(CXXVector<BYTE> bytes)
{
	static CXXTextCodec* codec = CXXTextCodec::codecFromACP(GetACP());
	std::string str(reinterpret_cast<char*>(bytes.data()), static_cast<size_t>(bytes.size()));

	return codec->toUnicode(str);
}

//@备注 比较文本
//@参数 ch 文本
//@参数 cs 是否区分大小写
//@返回 比较结果 当返回值的结果小于0时，表示当前文本小于ch，等于0时表示相等，大于0时表示大于ch (单位:字符)
//@别名 比较(文本, 是否区分大小写)
__int64 CXXString::compare(const wchar_t* ch, cxx::CaseSensitivity cs) const
{
	CXX_D(const CXXString);

	std::wstring s1 = d->data_;
	std::wstring s2 = ch;

	if (cs == cxx::CaseInsensitive)
	{
		s1 = cxx_private_string_tool::tolower(s1);
		s2 = cxx_private_string_tool::tolower(s2);
	}

	return s1.compare(s2);
}

//@备注 比较文本
//@参数 ch 文本
//@参数 cs 是否区分大小写
//@返回 比较结果 当返回值的结果小于0时，表示当前文本小于ch，等于0时表示相等，大于0时表示大于ch (单位:字符)
//@别名 比较(文本, 是否区分大小写)
__int64 CXXString::compare(const CXXString& ch, cxx::CaseSensitivity cs) const
{
	return compare(ch.d_ptr->data_.c_str(), cs);
}

//@备注 比较文本
//@参数 s 文本
//@参数 cs 是否区分大小写
//@返回 比较结果 当返回值的结果小于0时，表示当前文本小于s，等于0时表示相等，大于0时表示大于s (单位:字符)
//@别名 比较(文本, 是否区分大小写)
__int64 CXXString::compare(const CXXString& s1, const CXXString& s2, cxx::CaseSensitivity cs)
{
	return s1.compare(s2.c_str(), cs);
}

//@备注 将文本转换为短整型
//@参数 ok 接收转换是否成功的指针
//@参数 base 进制
//@返回 转换后的短整型
//@别名 到短整型(成功指针, 进制)
short CXXString::toShort(bool* ok, __int64 base) const
{
	CXX_D(const CXXString);
	return d->toNumber<short>(ok, base);
}

//@备注 将文本转换为正短整型
//@参数 ok 接收转换是否成功的指针
//@参数 base 进制
//@返回 转换后的正短整型
//@别名 到正短整型(成功指针, 进制)
USHORT CXXString::toUShort(bool* ok, __int64 base) const
{
	CXX_D(const CXXString);
	return d->toNumber<USHORT>(ok, base);
}

//@备注 将文本转换为整型
//@参数 ok 接收转换是否成功的指针
//@参数 base 进制
//@返回 转换后的整型
//@别名 到整型(成功指针, 进制)
int CXXString::toInt(bool* ok, __int64 base) const
{
	CXX_D(const CXXString);
	return d->toNumber<int>(ok, base);
}

//@备注 将文本转换为正整型
//@参数 ok 接收转换是否成功的指针
//@参数 base 进制
//@返回 转换后的正整型
//@别名 到正整型(成功指针, 进制)
UINT CXXString::toUInt(bool* ok, __int64 base) const
{
	CXX_D(const CXXString);
	return d->toNumber<UINT>(ok, base);
}

//@备注 将文本转换为32位长整型
//@参数 ok 接收转换是否成功的指针
//@参数 base 进制
//@返回 转换后的长整型
//@别名 到三十二位长整型(成功指针, 进制)
long CXXString::toLong(bool* ok, __int64 base) const
{
	CXX_D(const CXXString);
	return d->toNumber<long>(ok, base);
}

//@备注 将文本转换为正32位长整型
//@参数 ok 接收转换是否成功的指针
//@参数 base 进制
//@返回 转换后的正长整型
//@别名 到三十二位正长整型(成功指针, 进制)
ULONG CXXString::toULong(bool* ok, __int64 base) const
{
	CXX_D(const CXXString);
	return d->toNumber<ULONG>(ok, base);
}

//@备注 将文本转换为64位长整型  (吐槽:事实上这东西叫超长整型)
//@参数 ok 接收转换是否成功的指针
//@参数 base 进制
//@返回 转换后的64位长整型
//@别名 到长整型(成功指针, 进制)
__int64 CXXString::toLongLong(bool* ok, __int64 base) const
{
	CXX_D(const CXXString);
	return d->toNumber<__int64>(ok, base);
}

//@备注 将文本转换为正64位长整型 (吐槽:事实上这东西叫超长整型)
//@参数 ok 接收转换是否成功的指针
//@参数 base 进制
//@返回 转换后的正64位长整型
//@别名 到正长整型(成功指针, 进制)
UINT64 CXXString::toULongLong(bool* ok, __int64 base) const
{
	CXX_D(const CXXString);
	return d->toNumber<UINT64>(ok, base);
}

//@备注 将文本转换为字节集
//@返回 转换后的字节集
//@别名 到字节集()
CXXVector<BYTE> CXXString::toByteArray() const
{
	// 先转换成 local8bit
	std::string str = toLocal8Bit();

	// 再转换成字节集
	CXXVector<BYTE> bytes;
	for (auto i : str)
	{
		bytes.append(static_cast<BYTE>(i));
	}

	return bytes;
}

//@备注 将文本转换为单精度浮点数
//@参数 ok 接收转换是否成功的指针
//@返回 转换后的单精度浮点数
//@别名 到浮点(成功指针)
float CXXString::toFloat(bool* ok) const
{
	CXX_D(const CXXString);
	return d->toFloat(ok);
}

//@备注 将文本转换为双精度浮点数
//@参数 ok 接收转换是否成功的指针
//@返回 转换后的双精度浮点数
//@别名 到双浮点(成功指针)
double CXXString::toDouble(bool* ok) const
{
	CXX_D(const CXXString);
	return d->toDouble(ok);
}

//@备注 使用指定类型数字转换为文本，会替换掉原始文本
//@参数 n 要转换的短整型
//@参数 base 进制
//@返回 转换后的字符串
//@别名 置数字(短整型, 进制)
CXXString& CXXString::setNum(short n, __int64 base)
{
	*this = CXXString::fromNumberic<short>(n, base);
	return *this;
}

//@备注 使用指定类型数字转换为文本，会替换掉原始文本
//@参数 n 要转换的正短整型
//@参数 base 进制
//@返回 转换后的字符串
//@别名 置数字(正短整型, 进制)
CXXString& CXXString::setNum(USHORT n, __int64 base)
{
	*this = CXXString::fromNumberic<USHORT>(n, base);
	return *this;
}

//@备注 使用指定类型数字转换为文本，会替换掉原始文本
//@参数 n 要转换的整型
//@参数 base 进制
//@返回 转换后的字符串
//@别名 置数字(整型, 进制)
CXXString& CXXString::setNum(int n, __int64 base)
{
	*this = CXXString::fromNumberic<int>(n, base);
	return *this;
}

//@备注 使用指定类型数字转换为文本，会替换掉原始文本
//@参数 n 要转换的正整型
//@参数 base 进制
//@返回 转换后的字符串
//@别名 置数字(正整型, 进制)
CXXString& CXXString::setNum(UINT n, __int64 base)
{
	*this = CXXString::fromNumberic<UINT>(n, base);
	return *this;
}

//@备注 使用指定类型数字转换为文本，会替换掉原始文本
//@参数 n 要转换的32位长整型
//@参数 base 进制
//@返回 转换后的字符串
//@别名 置数字(三十二位长整型, 进制)
CXXString& CXXString::setNum(long n, __int64 base)
{
	*this = CXXString::fromNumberic<long>(n, base);
	return *this;
}

//@备注 使用指定类型数字转换为文本，会替换掉原始文本
//@参数 n 要转换的正32位长整型
//@参数 base 进制
//@返回 转换后的字符串
//@别名 置数字(正三十二位长整型, 进制)
CXXString& CXXString::setNum(ULONG n, __int64 base)
{
	*this = CXXString::fromNumberic<ULONG>(n, base);
	return *this;
}

//@备注 使用指定类型数字转换为文本，会替换掉原始文本
//@参数 n 要转换的64位长整型
//@参数 base 进制
//@返回 转换后的字符串
//@别名 置数字(长整型, 进制)
CXXString& CXXString::setNum(__int64 n, __int64 base)
{
	*this = CXXString::fromNumberic<__int64>(n, base);
	return *this;
}

//@备注 使用指定类型数字转换为文本，会替换掉原始文本
//@参数 n 要转换的正64位长整型
//@参数 base 进制
//@返回 转换后的字符串
//@别名 置数字(正长整型, 进制)
CXXString& CXXString::setNum(UINT64 n, __int64 base)
{
	*this = CXXString::fromNumberic<UINT64>(n, base);
	return *this;
}

//@备注 使用指定类型数字转换为文本，会替换掉原始文本
//@参数 n 要转换的数字
//@参数 f 格式化方式
//'e': 以[-]9.9e[+| -]999的形式格式化，小数点后的数字数量。
//'E' : 以[-]9.9E[+| -]999的形式格式化，小数点后的数字数量。
//'f' : 以[-]9.9的形式格式化，小数点后的数字数量。
//'F' : 与'f'相同，除了对于无穷大和非数字的表示（具体看下面的说明），小数点后的数字数量。
//'g' : 使用'e'或'f'格式，取决于哪种更简洁，最大的有效数字数（省略尾随的零）。
//'G' : 使用'E'或'F'格式，取决于哪种更简洁，最大的有效数字数（省略尾随的零）。
//@参数 prec 精度
//@返回 转换后的字符串
//@别名 置数字(单精度, 格式, 精度)
CXXString& CXXString::setNum(float n, wchar_t f, __int64 prec)
{
	*this = CXXString::fromFloating<float>(n, f, prec);
	return *this;
}

//@参数 n 要转换的数字
//@参数 f 格式化方式
//'e': 以[-]9.9e[+| -]999的形式格式化，小数点后的数字数量。
//'E' : 以[-]9.9E[+| -]999的形式格式化，小数点后的数字数量。
//'f' : 以[-]9.9的形式格式化，小数点后的数字数量。
//'F' : 与'f'相同，除了对于无穷大和非数字的表示（具体看下面的说明），小数点后的数字数量。
//'g' : 使用'e'或'f'格式，取决于哪种更简洁，最大的有效数字数（省略尾随的零）。
//'G' : 使用'E'或'F'格式，取决于哪种更简洁，最大的有效数字数（省略尾随的零）。
//@参数 prec 精度
//@返回 转换后的字符串
//@别名 置数字(双精度, 格式, 精度)
CXXString& CXXString::setNum(double n, wchar_t f, __int64 prec)
{
	*this = CXXString::fromFloating<double>(n, f, prec);
	return *this;
}

//@备注 静态使用指定类型数字转换为文本
//@参数 n 要转换的短整型
//@参数 base 进制
//@返回 转换后的字符串
//@别名 到文本(短整型, 进制)
CXXString CXXString::number(short n, __int64 base)
{
	return CXXString::fromNumberic<short>(n, base);
}

//@备注 静态使用指定类型数字转换为文本
//@参数 n 要转换的短整型
//@参数 base 进制
//@返回 转换后的字符串
//@别名 到文本(整型, 进制)
CXXString CXXString::number(int n, __int64 base)
{
	return CXXString::fromNumberic<int>(n, base);
}

//@备注 静态使用指定类型数字转换为文本
//@参数 n 要转换的短整型
//@参数 base 进制
//@返回 转换后的字符串
//@别名 到文本(正整型, 进制)
CXXString CXXString::number(UINT n, __int64 base)
{
	return CXXString::fromNumberic<UINT>(n, base);
}

//@备注 静态使用指定类型数字转换为文本
//@参数 n 要转换的正短整型
//@参数 base 进制
//@返回 转换后的字符串
//@别名 到文本(正短整型, 进制)
CXXString CXXString::number(USHORT n, __int64 base)
{
	return CXXString::fromNumberic<USHORT>(n, base);
}

//@备注 静态使用指定类型数字转换为文本
//@参数 n 要转换的32位长整型
//@参数 base 进制
//@返回 转换后的字符串
//@别名 到文本(三十二位长整型, 进制)
CXXString CXXString::number(long n, __int64 base)
{
	return CXXString::fromNumberic<long>(n, base);
}

//@备注 静态使用指定类型数字转换为文本
//@参数 n 要转换的正32位长整型
//@参数 base 进制
//@返回 转换后的字符串
//@别名 到文本(正三十二位长整型, 进制)
CXXString CXXString::number(ULONG n, __int64 base)
{
	return CXXString::fromNumberic<ULONG>(n, base);
}

//@备注 静态使用指定类型数字转换为文本
//@参数 n 要转换的64位长整型
//@参数 base 进制
//@返回 转换后的字符串
//@别名 到文本(长整型, 进制)
CXXString CXXString::number(__int64 n, __int64 base)
{
	return CXXString::fromNumberic<__int64>(n, base);
}

//@备注 静态使用指定类型数字转换为文本
//@参数 n 要转换的正64位长整型
//@参数 base 进制
//@返回 转换后的字符串
//@别名 到文本(正长整型, 进制)
CXXString CXXString::number(UINT64 n, __int64 base)
{
	return CXXString::fromNumberic<UINT64>(n, base);
}


CXXString CXXString::number(float n, wchar_t f, __int64 prec)
{
	return CXXString::fromFloating<float>(n, f, prec);
}

CXXString CXXString::number(double n, wchar_t f, __int64 prec)
{
	return CXXString::fromFloating<double>(n, f, prec);
}

//@备注 取得文本的起始迭代器
	//@返回 起始迭代器
	//@别名 开始()
CXXString::iterator CXXString::begin()
{
	CXX_D(CXXString);
	return d->data_.begin();
}

//@备注 取得文本的常量起始迭代器
//@返回 常量起始迭代器
//@别名 常量开始()
CXXString::const_iterator CXXString::begin() const
{
	CXX_D(const CXXString);
	return d->data_.cbegin();
}

//@备注 取得文本的常量起始迭代器
//@返回 常量起始迭代器
//@别名 常量开始()
CXXString::const_iterator CXXString::cbegin() const
{
	CXX_D(const CXXString);
	return d->data_.cbegin();
}

//@备注 取得文本的常量起始迭代器
//@返回 常量起始迭代器
//@别名 常量开始()
CXXString::const_iterator CXXString::constBegin() const
{
	return cbegin();
}

//@备注 取得文本的结束迭代器
//@返回 结束迭代器
//@别名 结束()
CXXString::iterator CXXString::end()
{
	CXX_D(CXXString);
	return d->data_.end();
}

//@备注 取得文本的常量结束迭代器
//@返回 常量结束迭代器
//@别名 常量结束()
CXXString::const_iterator CXXString::end() const
{
	CXX_D(const CXXString);
	return d->data_.cend();
}

//@备注 取得文本的常量结束迭代器
//@返回 常量结束迭代器
//@别名 常量结束()
CXXString::const_iterator CXXString::cend() const
{
	CXX_D(const CXXString);
	return d->data_.cend();
}

//@备注 取得文本的常量结束迭代器
//@返回 常量结束迭代器
//@别名 常量结束()
CXXString::const_iterator CXXString::constEnd() const
{
	return cend();
}

//@备注 取得文本的反向迭代器
//@返回 反向迭代器
//@别名 反向开始()
CXXString::reverse_iterator CXXString::rbegin()
{
	CXX_D(CXXString);
	return d->data_.rbegin();
}

//@备注 取得文本的反向迭代器
//@返回 反向迭代器
//@别名 反向结束()
CXXString::reverse_iterator CXXString::rend()
{
	CXX_D(CXXString);
	return d->data_.rend();
}

//@备注 取得文本的常量反向迭代器
//@返回 常量反向迭代器
//@别名 常量反向开始()
CXXString::const_reverse_iterator CXXString::rbegin() const
{
	CXX_D(const CXXString);
	return d->data_.crbegin();
}

//@备注 取得文本的常量反向迭代器
//@返回 常量反向迭代器
//@别名 常量反向结束()
CXXString::const_reverse_iterator CXXString::rend() const
{
	CXX_D(const CXXString);
	return d->data_.crend();
}

//@备注 取得文本的常量反向迭代器
//@返回 常量反向迭代器
//@别名 常量反向开始()
CXXString::const_reverse_iterator CXXString::crbegin() const
{
	CXX_D(const CXXString);
	return d->data_.crbegin();
}

//@备注 取得文本的常量反向迭代器
//@返回 常量反向迭代器
//@别名 常量反向结束()
CXXString::const_reverse_iterator CXXString::crend() const
{
	CXX_D(const CXXString);
	return d->data_.crend();
}

size_t CXXString::find(const std::wstring& str, size_t pos) const noexcept
{
	CXX_D(const CXXString);
	return d->data_.find(str, static_cast<size_t>(pos));
}

size_t CXXString::find(const wchar_t* s, size_t pos) const
{
	CXX_D(const CXXString);
	return d->data_.find(s, static_cast<size_t>(pos));
}

size_t CXXString::find(const wchar_t* s, size_t pos, cxsizeype n) const
{
	CXX_D(const CXXString);
	return d->data_.find(s, static_cast<size_t>(pos), static_cast<size_t>(n));
}

size_t CXXString::find(wchar_t c, size_t pos) const noexcept
{
	CXX_D(const CXXString);
	return d->data_.find(c, static_cast<size_t>(pos));
}

//rfind
size_t CXXString::rfind(const std::wstring& str, size_t pos) const noexcept
{
	CXX_D(const CXXString);
	return d->data_.rfind(str, static_cast<size_t>(pos));
}

size_t CXXString::rfind(const wchar_t* s, size_t pos) const
{
	CXX_D(const CXXString);
	return d->data_.rfind(s, static_cast<size_t>(pos));
}

size_t CXXString::rfind(const wchar_t* s, size_t pos, cxsizeype n) const
{
	CXX_D(const CXXString);
	return d->data_.rfind(s, static_cast<size_t>(pos), static_cast<size_t>(n));
}

size_t CXXString::rfind(wchar_t c, size_t pos) const noexcept
{
	CXX_D(const CXXString);
	return d->data_.rfind(c, static_cast<size_t>(pos));
}

bool CXXString::empty() const noexcept { return isEmpty(); }

void CXXString::push_back(wchar_t* c) { append(c); }

void CXXString::push_back(const CXXString& s) { append(s); }

void CXXString::push_front(wchar_t* c) { prepend(c); }

void CXXString::push_front(const CXXString& s) { prepend(s); }

void CXXString::shrink_to_fit() { squeeze(); }

bool CXXString::isNull() const { return size() <= 0; }

std::wstring& CXXString::ref() noexcept
{
	CXX_D(CXXString);
	return d->data_;
}