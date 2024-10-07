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
#ifndef CXXSTRING_H_
//@隐藏{
#define CXXSTRING_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxvector.h>
#include <cxxscopedpointer.h>

#include <windows.h>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <bitset>

//@别名 炫
namespace cxx
{
	//@分组{［文本］.炫　文本类

	//@别名 大小写敏感性
	enum CaseSensitivity
		//@隐藏{
		: __int64
		//@隐藏}
	{
		CaseInsensitive, //@别名 大小写不敏感
		CaseSensitive    //@别名 大小写敏感
	};

	//@别名 分割行为
	enum SplitBehavior
		//@隐藏{
		: __int64
		//@隐藏}
	{
		KeepEmptyParts, //@别名 保留空部分
		SkipEmptyParts, //@别名 跳过空部分
	};

	//@分组}
}

//@分组{［文本］.炫　文本类


//@隐藏{
class CXXStringPrivate;
class CXXRegex;
class CXXRegexMatch;
class CXXRegexMatchIterator;
//@隐藏}

//@别名 炫文本
class CXXString
{
public:
	CXXString();

	CXXString(const CXXString& unicode, __int64 size);

	CXXString(const CXXString& s);

	CXXString(const std::wstring& s);

	CXXString(const wchar_t* s);

	CXXString(wchar_t* s);

	CXXString(wchar_t s);

	CXXString(CXXString&& other) noexcept;

	virtual ~CXXString();

	//@隐藏{
	operator const wchar_t* () const;

	operator wchar_t* () const;

	CXXString& operator=(const CXXString& s);

	CXXString& operator=(const wchar_t* s);

	CXXString& operator=(CXXString&& other) noexcept;

	// == 
	bool operator==(const CXXString& s) const;

	// !=
	bool operator!=(const CXXString& s) const;

	// >
	bool operator>(const CXXString& s) const;

	// <
	bool operator<(const CXXString& s) const;

	wchar_t& operator[](__int64 i);
	//@隐藏}

	//@备注 与另一个文本交换内容
	//@参数 other 另一个文本
	//@别名 交换(文本)
	void swap(CXXString& other) noexcept;

	//@备注 返回文本的长度(字符数) 与 取数量 取长度 相同
	//@返回 长度
	//@别名 大小()
	__int64 size() const;

	//@备注 返回文本的长度(字符数) 与 取数量 取大小 相同
	//@返回 长度
	__int64 count() const;

	//@备注 返回文本的长度(字符数) 与 取数量 取大小 相同
	//@返回 长度
	__int64 length() const;

	//@备注 检查文本是否为空
	//@返回 如果文本为空则返回true，否则返回false
	//@别名 是否空()
	bool isEmpty() const;

	//@备注 重新分配文本的大小
	//@参数 size 新的大小
	//@别名 置大小(大小)
	void resize(__int64 size);

	//@备注 重新分配文本的大小，并用字符c填充
	//@参数 size 新的大小
	//@参数 fillChar 填充字符
	//@别名 置大小(大小, 填充字符)
	void resize(__int64 size, const CXXString& fillChar);

	//@备注 用字符c填充文本
	//@参数 c 填充字符
	//@参数 size 填充的大小
	//@返回 本文本
	//@别名 填充(字符, 大小)
	CXXString& fill(const CXXString& c, __int64 size = -1);

	//@备注 将字符串缩短到指定的长度
	//@参数 pos 截断位置
	//@别名 截断(位置)
	void truncate(__int64 pos);

	//@备注 从字符串的末尾删除指定数量 n 的字符
	//@参数 n 删除的数量
	//@别名 切除(数量)
	void chop(__int64 n);

	//@备注 返回文本的容量，单位为字符
	// 这个容量通常等于或大于字符串的实际大小
	//@返回 容量
	//@别名 容量()
	__int64 capacity() const;

	//@备注 保留至少能容纳size个字符的内存，用于请求改变字符串底层存储的容量
	//@参数 size 内存大小
	//@别名 预留(大小)
	void reserve(__int64 size);

	//@备注 释放多余的内存
	//@别名 释放多余空间()
	void squeeze();

	//@备注 返回文本的常量数据指针
	//@返回 数据指针
	//@别名 取()
	wchar_t* get();

	//@备注 返回文本的数据指针
	//@返回 数据指针
	//@别名 取()
	const wchar_t* get() const;

	//@备注 返回文本的数据指针
	//@返回 数据指针
	//@别名 地址()
	wchar_t* data();

	//@备注 返回文本的常量数据指针
	//@返回 数据常量指针
	//@别名 地址()
	const wchar_t* data() const;

	//@备注 返回文本的常量数据指针
	//@返回 数据指针
	//@别名 常量地址()
	const wchar_t* constData() const;

	//@备注 返回文本的常量数据指针(STL兼容)
	//@返回 数据指针
	const wchar_t* c_str() const;

	//@备注 返回文本的常量数据指针
	//@返回 char16_t指针
	//@别名 UTF16()
	char16_t* utf16();

	//@备注 返回文本的常量短整型数据指针
	//@返回 数据指针
	//@别名 短整型地址()
	USHORT* ushort();

	//@备注 清空文本
	//@别名 清空()
	void clear();

	//@备注 返回文本中指定位置的字符
	//@参数 i 位置
	//@参数 def 默认值
	//@返回 字符
	//@别名 在(位置)
	const CXXString at(__int64 i, CXXString def = {}) const;

	//@备注 返回文本中指定位置的字符
	//@参数 i 位置
	//@参数 def 默认值
	//@返回 字符
	//@别名 值(位置)
	const CXXString value(__int64 i, CXXString def = {}) const;

	//@备注 返回最前面的字符
	//@返回 字符
	//@别名 首()
	wchar_t front() const;

	//@备注 返回最后面的字符
	//@返回 字符
	//@别名 尾()
	wchar_t back() const;

	//@备注 返回最前面的字符
	//@返回 字符
	wchar_t first() const;

	//@备注 返回最后面的字符
	//@返回 字符
	wchar_t last() const;

	//@备注 从文本最后方查找字符c所在的位置
	//@参数 c 字符
	//@参数 from 开始查找的位置
	//@参数 cs 是否区分大小写
	//@返回 位置
	//@别名 从最后查找(字符, 开始位置, 是否区分大小写)
	__int64 lastIndexOf(const wchar_t* c, __int64 from = -1, cxx::CaseSensitivity cs = cxx::CaseSensitive) const;

	//@备注 从文本最后方查找文本s所在的位置
	//@参数 s 文本
	//@参数 from 开始查找的位置
	//@参数 n 查找的数量
	//@参数 cs 是否区分大小写
	//@返回 位置
	//@别名 从最后查找(字符, 开始位置, 数量, 是否区分大小写)
	__int64 lastIndexOf(const CXXString& s, __int64 from = -1, cxx::CaseSensitivity cs = cxx::CaseSensitive) const;

	//@备注 查找文本中是否存在指定的字符c
	//@参数 c 字符
	//@参数 cs 是否区分大小写
	//@返回 如果存在则返回true，否则返回false
	//@别名 包含(字符, 是否区分大小写)
	bool contains(const CXXString& c, cxx::CaseSensitivity cs = cxx::CaseSensitive) const;

	//@备注 查找文本中是否存在指定的字符c
	//@参数 c 字符
	//@参数 from 开始查找的位置
	//@参数 cs 是否区分大小写
	//@返回 如果存在则返回true，否则返回false
	//@别名 包含(字符, 开始位置, 是否区分大小写)
	bool contains(const CXXString& c, __int64 from, cxx::CaseSensitivity cs = cxx::CaseSensitive) const;

	//@备注 查找文本中是否存在指定的字符c
	//@参数 c 字符
	//@参数 from 开始查找的位置
	//@参数 n 查找的数量
	//@参数 cs 是否区分大小写
	//@返回 如果存在则返回true，否则返回false
	//@别名 是否包含(字符, 开始位置, 数量, 是否区分大小写)
	bool contains(const CXXString& c, __int64 from, __int64 n, cxx::CaseSensitivity cs = cxx::CaseSensitive) const;

	//@备注 计算指定字符在本文本中出现的次数
	//@参数 c 指定字符
	//@参数 cs 是否区分大小写
	//@返回 出现的次数
	//@别名 计数(文本, 是否区分大小写)
	__int64 count(wchar_t* c, cxx::CaseSensitivity cs = cxx::CaseSensitive) const;

	//@备注 计算指定文本在本文本中出现的次数
	//@参数 s 指定文本
	//@参数 cs 是否区分大小写
	//@返回 出现的次数
	//@别名 计数(文本, 是否区分大小写)
	__int64 count(const CXXString& s, cxx::CaseSensitivity cs = cxx::CaseSensitive) const;

	//@备注 返回指定字符在本文本中的位置
	//@参数 c 指定字符
	//@参数 from 开始查找的位置
	//@参数 cs 是否区分大小写
	//@返回 位置 失败返回-1
	//@别名 查找(字符, 开始位置, 是否区分大小写)
	__int64 indexOf(const CXXString& c, __int64 from = 0, cxx::CaseSensitivity cs = cxx::CaseSensitive) const;

	//@备注 返回指定字符在本文本中的位置
	//@参数 c 指定字符
	//@参数 from 开始查找的位置
	//@参数 cs 是否区分大小写
	//@返回 位置 失败返回-1
	//@别名 查找(字符, 开始位置, 是否区分大小写)
	__int64 indexOf(const wchar_t* c, __int64 from = 0, cxx::CaseSensitivity cs = cxx::CaseSensitive) const;

	//@备注 返回指定字符在本文本中的位置
	//@参数 c 指定字符
	//@参数 from 开始查找的位置
	//@参数 n 查找的数量 默认为-1，表示查找全部
	//@参数 cs 是否区分大小写
	//@返回 位置 失败返回-1
	//@别名 查找(字符, 开始位置, 数量, 是否区分大小写)
	__int64 indexOf(const wchar_t* c, __int64 from, __int64 n, cxx::CaseSensitivity cs = cxx::CaseSensitive) const;

	//@备注 使用正则表达式查找文本中的位置
	//@参数 re 正则表达式
	//@参数 from 开始查找的位置
	//@返回 位置 失败返回-1
	//@别名 查找(正则表达式, 开始位置)
	__int64 indexOf(const CXXRegex& re, __int64 from = 0) const;

	//@备注 使用正则表达式查找文本中的位置
	//@参数 re 正则表达式
	//@参数 from 开始查找的位置
	//@参数 rmatch 接收匹配结果的指针
	//@返回 位置 失败返回-1
	//@别名 查找(正则表达式, 开始位置, 匹配结果指针)
	__int64 indexOf(const CXXRegex& re, __int64 from, CXXRegexMatch* rmatch) const;

	//@备注 从文本最后方查找正则匹配文本所在的位置
	//@参数 re 正则表达式
	//@参数 from 开始查找的位置
	//@返回 位置
	//@别名 从最后查找(正则表达式, 开始位置)
	__int64 lastIndexOf(const CXXRegex& re, __int64 from = -1) const;

	//@备注 从文本最后方查找正则匹配文本所在的位置
	//@参数 re 正则表达式
	//@参数 from 开始查找的位置
	//@参数 rmatch 接收匹配结果的指针
	//@返回 位置
	//@别名 从最后查找(正则表达式, 开始位置, 匹配结果指针)
	__int64 lastIndexOf(const CXXRegex& re, __int64 from, CXXRegexMatch* rmatch) const;

	//@备注 使用正则表达式判断文本中是否存在指定的匹配
	//@参数 re 正则表达式
	//@返回 如果存在则返回true，否则返回false
	//@别名 包含(正则表达式)
	bool contains(const CXXRegex& re) const;

	//@备注 使用正则表达式判断文本中是否存在指定的匹配
	//@参数 re 正则表达式
	//@参数 rmatch 接收匹配结果的指针
	//@返回 如果存在则返回true，否则返回false
	//@别名 包含(正则表达式, 匹配结果指针)
	bool contains(const CXXRegex& re, CXXRegexMatch* rmatch) const;

	//@备注 使用正则表达式判断文本中匹配的次数
	//@参数 re 正则表达式
	//@返回 匹配的次数
	//@别名 数量(正则表达式)
	__int64 count(const CXXRegex& re) const;

	//@备注 返回字符串从左边开始的前 n 个字符。如果 n 大于字符串长度，则返回整个字符串。
	//@参数 n 截取的数量
	//@返回 截取的字符串
	//@别名 左(数量)
	CXXString left(__int64 n) const;

	//@备注 返回字符串从右边开始的后 n 个字符。如果 n 大于字符串长度，则返回整个字符串。
	//@参数 n 截取的数量
	//@返回 截取的字符串
	//@别名 右(数量)
	CXXString right(__int64 n) const;

	//@备注 从字符串的 position 位置开始，返回长度为 n 的子串。如果 n 为负值，则返回从 position 到字符串末尾的所有字符
	//@参数 position 开始位置
	//@参数 n 截取的数量
	//@返回 截取的字符串
	//@别名 中(开始位置, 数量)
	CXXString mid(__int64 position, __int64 n = -1) const;

	//@备注 返回一个新的字符串，该字符串是原字符串去除尾部 n 个字符后的结果。如果 n 等于字符串长度，则返回空字符串
	//@参数 n 删除的数量
	//@返回 截取的字符串
	//@别名 切除尾(数量)
	CXXString chopped(__int64 n) const;

	//@备注 检查字符串是否以指定的子串 c 开头。可以指定大小写敏感或不敏感。
	//@参数 c 子串
	//@参数 cs 是否区分大小写
	//@返回 如果以指定的子串开头则返回true，否则返回false
	//@别名 是否始于(文本, 是否区分大小写)
	bool startsWith(const CXXString& c, cxx::CaseSensitivity cs = cxx::CaseSensitive) const;

	//@备注 检查字符串是否以指定的子串 c 结尾。可以指定大小写敏感或不敏感。
	//@参数 c 子串
	//@参数 cs 是否区分大小写
	//@返回 如果以指定的子串结尾则返回true，否则返回false
	//@别名 是否终于(文本, 是否区分大小写)
	bool endsWith(const CXXString& c, cxx::CaseSensitivity cs = cxx::CaseSensitive) const;

	//@备注 判断文本是否全大写
	//@返回 如果全大写则返回true，否则返回false
	//@别名 是否大写()
	bool isUpper() const;

	//@备注 判断文本是否全小写
	//@返回 如果全小写则返回true，否则返回false
	//@别名 是否小写()
	bool isLower() const;

	//@备注 返回一个新的字符串，该字符串是原字符串左对齐，并使用 fill 字符填充至 width 个字符的长度。如果原字符串长度大于或等于 width，则返回原字符串。
	//@参数 width 宽度
	//@参数 fill 填充字符
	//@参数 trunc 如果为true，则截断字符串
	//@返回 左对齐的字符串
	//@别名 左对齐(宽度, 填充字符, 是否截断)
	CXXString leftJustified(__int64 width, const CXXString& fill = L" ", bool trunc = false) const;

	//@备注 返回一个新的字符串，该字符串是原字符串右对齐，并使用 fill 字符填充至 width 个字符的长度。如果原字符串长度大于或等于 width，则返回原字符串。
	//@参数 width 宽度
	//@参数 fill 填充字符
	//@参数 trunc 如果为true，则截断字符串
	//@返回 右对齐的字符串
	//@别名 右对齐(宽度, 填充字符, 是否截断)
	CXXString rightJustified(__int64 width, const CXXString& fill = L" ", bool trunc = false) const;

	//@备注 转换为小写
	//@返回 转换后的文本
	//@别名 到小写()
	CXXString toLower() const;

	//@备注 转换为大写
	//@返回 转换后的文本
	//@别名 到大写()
	CXXString toUpper() const;

	//@备注 转换半角
	//@返回 转换后的文本
	//@别名 到半角()
	CXXString toHalf() const;

	//@备注 转换全角
	//@返回 转换后的文本
	//@别名 到全角()
	CXXString toFull() const;

	//@备注 修剪文本两端的空白字符，包含全角空格
	//@返回 修剪后的文本
	//@别名 修剪()
	CXXString trimmed() const;

	//@备注 修剪文本两端的空白字符，包含全角空格，字符串中间多余的空白字符被替换为一个空格
	//@返回 修剪后的文本
	//@别名 全修剪()
	CXXString simplified() const;

	//@备注 将文本中部份字符替换为HTML转义字符
	//@返回 转义后的文本
	//@别名 到HTML()
	CXXString toHtmlEscaped() const;

	//@备注 将文本中部份字符替换为URL转义字符
	//@返回 转义后的文本
	//@别名 到URL()
	CXXString toUrl() const;

	//@备注 在指定位置插入字符
	//@参数 i 位置
	//@参数 c 字符
	//@返回 插入后的文本
	//@别名 插入(位置, 字符)
	CXXString& insert(__int64 i, wchar_t* c);

	//@备注 在指定位置插入字符
	//@参数 i 位置
	//@参数 uc 字符
	//@参数 len 字符数量
	//@返回 插入后的文本
	//@别名 插入(位置, 字符, 数量)
	CXXString& insert(__int64 i, const wchar_t* uc, __int64 len);

	//@备注 在指定位置插入文本
	//@参数 i 位置
	//@参数 s 文本
	//@返回 插入后的文本
	//@别名 插入(位置, 文本)
	CXXString& insert(__int64 i, const CXXString& s);

	//@备注 从末尾添加字符
	//@参数 c 字符
	//@返回 添加后的文本
	//@别名 追加(字符指针)
	CXXString& append(wchar_t* c);

	//@别名 追加(字符)
	CXXString& append(wchar_t c);

	//@备注 从末尾添加字符
	//@参数 uc 常量字符
	//@参数 len 字符数量
	//@返回 添加后的文本
	//@别名 追加(字符指针, 数量)
	CXXString& append(const wchar_t* uc, __int64 len);

	//@备注 从末尾添加文本
	//@参数 s 文本
	//@返回 添加后的文本
	//@别名 追加(文本)
	CXXString& append(const CXXString& s);

	//@备注 从文本最前方添加字符
	//@参数 c 字符
	//@返回 添加后的文本
	//@别名 追加首(字符指针)
	CXXString& prepend(wchar_t* c);

	//@备注 从文本最前方添加字符
	//@参数 uc 常量字符
	//@参数 len 字符数量
	//@返回 添加后的文本
	//@别名 追加首(字符, 数量)
	CXXString& prepend(const wchar_t* uc, __int64 len);

	//@备注 从文本最前方添加文本
	//@参数 s 文本
	//@返回 添加后的文本
	//@别名 追加首(文本)
	CXXString& prepend(const CXXString& s);

	//@隐藏{
	CXXString& operator+=(const CXXString& s);

	CXXString& operator+=(wchar_t* c);

	CXXString& operator+=(const wchar_t* c);

	CXXString& operator+=(wchar_t c);


	// +
	CXXString& operator+(const CXXString& s);

	CXXString& operator+(wchar_t* c);

	CXXString& operator+(const wchar_t* wc);

	CXXString& operator+(wchar_t wc);


	friend CXXString operator+(const CXXString& lhs, const CXXString& rhs) noexcept { return CXXString(lhs) += rhs; }
	friend CXXString operator+(const CXXString& lhs, const wchar_t* rhs) noexcept { return CXXString(lhs) += rhs; }
	friend CXXString operator+(const wchar_t* lhs, const CXXString& rhs) noexcept { return CXXString(lhs) += rhs; }
	friend CXXString operator+(const CXXString& lhs, wchar_t* rhs) noexcept { return CXXString(lhs) += rhs; }
	friend CXXString operator+(wchar_t* lhs, const CXXString& rhs) noexcept { return CXXString(lhs) += rhs; }
	//@隐藏}

	//@备注 从指定位置开始删除指定数量的字符
	//@参数 i 开始位置
	//@参数 len 删除的数量
	//@返回 删除后的文本
	//@别名 移除(开始位置, 数量)
	CXXString& remove(__int64 i, __int64 len);

	//@备注 删除指定的字符
	//@参数 c 字符
	//@参数 cs 是否区分大小写
	//@返回 删除后的文本
	//@别名 移除(字符, 是否区分大小写)
	CXXString& remove(wchar_t* c, cxx::CaseSensitivity cs = cxx::CaseSensitive);

	//@备注 删除指定的文本
	//@参数 s 文本
	//@参数 cs 是否区分大小写
	//@返回 删除后的文本
	//@别名 移除(文本, 是否区分大小写)
	CXXString& remove(const CXXString& s, cxx::CaseSensitivity cs = cxx::CaseSensitive);

	//@备注 从指定位置开始替换指定数量的字符
	//@参数 i 开始位置
	//@参数 len 替换的数量
	//@参数 after 替换的字符
	//@返回 替换后的文本
	//@别名 替换(开始位置, 数量, 文本)
	CXXString& replace(__int64 i, __int64 len, wchar_t* after);

	//@备注 从指定位置开始替换指定数量的字符
	//@参数 i 开始位置
	//@参数 len 替换的数量
	//@参数 s 替换的字符
	//@参数 slen 替换的字符数量
	//@返回 替换后的文本
	//@别名 替换(开始位置, 数量, 文本, 数量)
	CXXString& replace(__int64 i, __int64 len, const wchar_t* s, __int64 slen);

	//@备注 从指定位置开始替换指定数量的字符
	//@参数 i 开始位置
	//@参数 len 替换的数量
	//@参数 after 替换的文本
	//@返回 替换后的文本
	//@别名 替换(开始位置, 数量, 文本)
	CXXString& replace(__int64 i, __int64 len, const CXXString& after);

	//@备注 替换指定的字符
	//@参数 before 被替换的字符
	//@参数 after 替换的字符
	//@参数 cs 是否区分大小写
	//@返回 替换后的文本
	//@别名 替换(字符, 字符, 是否区分大小写)
	CXXString& replace(const wchar_t* before, const wchar_t* after, cxx::CaseSensitivity cs = cxx::CaseSensitive);

	//@备注 替换指定的文本
	//@参数 before 被替换的文本
	//@参数 blen 被替换的文本数量
	//@参数 after 替换的文本
	//@参数 alen 替换的文本数量
	//@参数 cs 是否区分大小写
	//@返回 替换后的文本
	//@别名 替换(文本, 文本, 是否区分大小写)
	CXXString& replace(const wchar_t* before, __int64 blen, const wchar_t* after, __int64 alen, cxx::CaseSensitivity cs = cxx::CaseSensitive);

	//@备注 替换指定的文本
	//@参数 before 被替换的文本
	//@参数 after 替换的文本
	//@参数 cs 是否区分大小写
	//@返回 替换后的文本
	//@别名 替换(文本, 文本, 是否区分大小写)
	CXXString& replace(const CXXString& before, const CXXString& after, cxx::CaseSensitivity cs = cxx::CaseSensitive);

	//@备注 替换指定的文本
	//@参数 before 被替换的文本
	//@参数 after 替换的文本
	//@参数 cs 是否区分大小写
	//@返回 替换后的文本
	//@别名 替换(文本, 文本, 是否区分大小写)
	CXXString& replace(wchar_t* c, const CXXString& after, cxx::CaseSensitivity cs = cxx::CaseSensitive);

	//@备注 正则替换指定的文本
	//@参数 re 正则表达式
	//@参数 after 替换的文本'
	//@返回 替换后的文本
	//@别名 替换(正则表达式, 文本)
	CXXString& replace(const CXXRegex& re, const CXXString& after);

	//@备注 使用正则表达式移除成功匹配的文本
	//@参数 re 正则表达式
	//@返回 移除后的文本
	//@别名 移除(正则表达式)
	CXXString& remove(const CXXRegex& re);

public:

	//@隐藏{
	template<typename... Args>
	CXXString format(Args... args)
	{
		std::wstring fmt = ref();
		std::wstringstream ss;
		std::vector<std::wstring> v;

		int dummy[] = { 0, ((v.push_back(CXXString::convertArgToString(args))), 0)... };

		std::vector<bool> used(v.size(), false);

		for (size_t i = 0; i < v.size(); ++i)
		{
			std::wstring s = L"%" + std::to_wstring(i + 1);
			size_t pos = 0;
			bool found = false;
			while ((pos = fmt.find(s, pos)) != std::wstring::npos)
			{
				found = true;
				used[i] = true;
				fmt.replace(pos, s.size(), v[i]);
				pos += v[i].size();
			}


			if (!found)
			{
				OutputDebugStringW((L"Warning: Argument '" + v[i] + L"' has no corresponding '%" + std::to_wstring(i + 1) + L"' in format string.\n").c_str());
			}
		}

		ss << fmt;

		std::wstring ws = ss.str();

#ifdef max
#undef max
#endif
		for (size_t i = 0; i < std::numeric_limits<USHORT>::max(); ++i)
		{
			std::wstring s = L"%" + std::to_wstring(i + 1);
			if (ws.find(s) != std::wstring::npos)
			{
				OutputDebugStringW((L"Warning: Extra argument '%" + std::to_wstring(i + 1) + L"' has been passed to format.\n").c_str());
			}
		}

		return ss.str();
	}
	//@隐藏}

	/*@声明
	//@备注 格式化文本，会改变原始文本
	// 如:
	//  CXXString s
	//  CXXString("%1 %3 %2").format(123, "hello", 3.14);
	//  实际输出: "123 3.140 hello"
	//@参数 fmt 格式化字符串
	//@参数 可变参数
	//@返回 格式化后的文本
	//@别名 格式化(格式化字符串, 可变参数)
	template<typename Args>
	CXXString format(Args);
	*/

public:

	//@备注 将文本分割成炫::动态数组
	//@参数 sep 分割文本
	//@参数 behavior 分割行为
	//@参数 cs 是否区分大小写
	//@返回 分割后的数组
	//@别名 分割(分割文本, 分割行为, 是否区分大小写)
	CXXVector<CXXString> split(const CXXString& sep, cxx::SplitBehavior behavior = cxx::KeepEmptyParts,
		cxx::CaseSensitivity cs = cxx::CaseSensitive) const;

	//@备注 将文本分割成炫::动态数组
	//@参数 sep 分割字符
	//@参数 behavior 分割行为
	//@参数 cs 是否区分大小写
	//@返回 分割后的数组
	//@别名 分割(分割字符, 分割行为, 是否区分大小写)
	CXXVector<CXXString> split(wchar_t* sep, cxx::SplitBehavior behavior = cxx::KeepEmptyParts,
		cxx::CaseSensitivity cs = cxx::CaseSensitive) const;

	//@备注 将文本分割成炫::动态数组
	//@参数 sep 正则表达式
	//@参数 behavior 分割行为
	//@返回 分割后的数组
	//@别名 分割(正则表达式, 分割行为)
	CXXVector<CXXString> split(const CXXRegex& sep, cxx::SplitBehavior behavior = cxx::KeepEmptyParts) const;

	//@备注 返回重复n次的字符串
	//@参数 times 重复次数
	//@返回 重复后的文本
	//@别名 重复(次数)
	CXXString repeated(__int64 times) const;

	//@隐藏{
	CXXString toHexString(const CXXString& separator = L"") const;
	//@隐藏}

	/*@声明
	//@备注 到16进制
	//@返回 16进制文本
	//@别名 到十六进制文本()
	CXXString toHexString(const CXXString& separator = "") const;
	*/

	//@备注 将字节集转换为十六进制文本
	//@别名 到十六进制文本()
	static CXXString toHexString(const char* input, __int64 size);

	//@备注 将字节集转换为十六进制文本
	//@别名 到十六进制文本()
	static CXXString toHexString(const CXXVector<BYTE>& input);

	//@备注 将十六进制文本转换为字节集
	//@别名 从十六进制文本()
	static CXXVector<BYTE> fromHexString(const CXXString& hex);

	//@备注 返回文本的Base64编码
	//@返回 Base64编码文本
	//@别名 到BASE64()
	CXXString toBase64() const;

	//@备注 返回字节集Base64解码
	//@返回 解码后的字节集
	//@别名 从BASE64()
	static CXXVector<BYTE> fromBase64(const CXXString& base64);

	//@别名 到BASE64()
	static CXXString toBase64(const char* input, __int64 size);

	//@备注 返回文本的Base64编码
	//@参数 input 字节集
	//@返回 Base64编码文本
	//@别名 到BASE64()
	static CXXString toBase64(const CXXVector<BYTE>& input);

	//@备注 返回文本的UTF-8编码 std::string 类型
	//@返回 UTF-8编码文本
	//@别名 到UTF8()
	std::string toUtf8() const;

	//@备注 返回文本的本地编码 std::string 类型
	//@返回 本地编码文本
	//@别名 到本地编码()
	std::string toLocal8Bit() const;

	//@备注 返回文本的std::wstring引用类型
	//@返回 std::wstring&类型文本
	//@别名 到STDWSTRING()
	std::wstring& toStdWString();

	//@备注 返回文本的std::wstring类型
	//@返回 std::wstring类型文本
	//@别名 到STDWSTRING()
	std::wstring toStdWString() const;

	//@备注 返回文本的std::string类型
	//@返回 std::string类型文本
	//@别名 到STDSTRING()
	std::string toStdString() const;

	//@备注 从UTF-8编码文本来源创建文本
	//@参数 str UTF-8编码文本
	//@返回 文本
	//@别名 从UTF8(字符A指针)
	static CXXString fromUtf8(const char* str);

	//@备注 从UTF-8编码文本来源创建文本
	//@参数 str UTF-8编码文本
	//@参数 size 文本大小
	//@返回 文本
	//@别名 从UTF8(字符A指针, 大小)
	static CXXString fromUtf8(const char* str, __int64 size);

	//@别名 从UTF8(字符A)
	static CXXString fromUtf8(const char str);

	//@别名 从UTF8(字节集)
	static CXXString fromUtf8(CXXVector<BYTE> bytes);

	//@备注 从本地编码文本来源创建文本
	//@参数 str 本地编码文本
	//@返回 文本
	//@别名 从本地编码(文本)
	static CXXString fromLocal8Bit(const char* str);

	//@别名 从本地编码(文本)
	static CXXString fromLocal8Bit(const char str);

	//@别名 从本地编码(字节集)
	static CXXString fromLocal8Bit(CXXVector<BYTE> bytes);

	//@备注 比较文本
	//@参数 ch 文本
	//@参数 cs 是否区分大小写
	//@返回 比较结果 当返回值的结果小于0时，表示当前文本小于ch，等于0时表示相等，大于0时表示大于ch (单位:字符)
	//@别名 比较(文本, 是否区分大小写)
	__int64 compare(const wchar_t* ch, cxx::CaseSensitivity cs = cxx::CaseSensitive) const;

	//@备注 比较文本
	//@参数 ch 文本
	//@参数 cs 是否区分大小写
	//@返回 比较结果 当返回值的结果小于0时，表示当前文本小于ch，等于0时表示相等，大于0时表示大于ch (单位:字符)
	//@别名 比较(文本, 是否区分大小写)
	__int64 compare(const CXXString& ch, cxx::CaseSensitivity cs = cxx::CaseSensitive) const;

	//@备注 比较文本
   //@参数 s 文本
   //@参数 cs 是否区分大小写
   //@返回 比较结果 当返回值的结果小于0时，表示当前文本小于s，等于0时表示相等，大于0时表示大于s (单位:字符)
   //@别名 比较(文本, 是否区分大小写)
	static __int64 compare(const CXXString& s1, const CXXString& s2, cxx::CaseSensitivity cs = cxx::CaseSensitive);

	//@备注 将文本转换为短整型
	//@参数 ok 接收转换是否成功的指针
	//@参数 base 进制
	//@返回 转换后的短整型
	//@别名 到短整型(成功指针, 进制)
	short  toShort(bool* ok = nullptr, __int64 base = 10) const;

	//@备注 将文本转换为正短整型
	//@参数 ok 接收转换是否成功的指针
	//@参数 base 进制
	//@返回 转换后的正短整型
	//@别名 到正短整型(成功指针, 进制)
	USHORT toUShort(bool* ok = nullptr, __int64 base = 10) const;

	//@备注 将文本转换为整型
	//@参数 ok 接收转换是否成功的指针
	//@参数 base 进制
	//@返回 转换后的整型
	//@别名 到整型(成功指针, 进制)
	int toInt(bool* ok = nullptr, __int64 base = 10) const;

	//@备注 将文本转换为正整型
	//@参数 ok 接收转换是否成功的指针
	//@参数 base 进制
	//@返回 转换后的正整型
	//@别名 到正整型(成功指针, 进制)
	UINT toUInt(bool* ok = nullptr, __int64 base = 10) const;

	//@备注 将文本转换为32位长整型
	//@参数 ok 接收转换是否成功的指针
	//@参数 base 进制
	//@返回 转换后的长整型
	//@别名 到长整型32(成功指针, 进制)
	long toLong(bool* ok = nullptr, __int64 base = 10) const;

	//@备注 将文本转换为正32位长整型
	//@参数 ok 接收转换是否成功的指针
	//@参数 base 进制
	//@返回 转换后的正长整型
	//@别名 到正长整型32(成功指针, 进制)
	ULONG toULong(bool* ok = nullptr, __int64 base = 10) const;

	//@备注 将文本转换为64位长整型  (吐槽:事实上这东西叫超长整型)
	//@参数 ok 接收转换是否成功的指针
	//@参数 base 进制
	//@返回 转换后的64位长整型
	//@别名 到长整型(成功指针, 进制)
	__int64 toLongLong(bool* ok = nullptr, __int64 base = 10) const;

	//@备注 将文本转换为正64位长整型 (吐槽:事实上这东西叫超长整型)
	//@参数 ok 接收转换是否成功的指针
	//@参数 base 进制
	//@返回 转换后的正64位长整型
	//@别名 到正长整型(成功指针, 进制)
	UINT64 toULongLong(bool* ok = nullptr, __int64 base = 10) const;

	//@备注 将文本转换为字节集
	//@返回 转换后的字节集
	//@别名 到字节集()
	CXXVector<BYTE> toByteArray() const;

	//@备注 将文本转换为单精度浮点数
	//@参数 ok 接收转换是否成功的指针
	//@返回 转换后的单精度浮点数
	//@别名 到浮点型(成功指针)
	float toFloat(bool* ok = nullptr) const;

	//@备注 将文本转换为双精度浮点数
	//@参数 ok 接收转换是否成功的指针
	//@返回 转换后的双精度浮点数
	//@别名 到双浮点型(成功指针)
	double toDouble(bool* ok = nullptr) const;

	//@备注 使用指定类型数字转换为文本，会替换掉原始文本
	//@参数 n 要转换的短整型
	//@参数 base 进制
	//@返回 转换后的字符串
	//@别名 置整型(短整型, 进制)
	CXXString& setNum(short n, __int64 base = 10);

	//@备注 使用指定类型数字转换为文本，会替换掉原始文本
	//@参数 n 要转换的正短整型
	//@参数 base 进制
	//@返回 转换后的字符串
	//@别名 置整型(正短整型, 进制)
	CXXString& setNum(USHORT n, __int64 base = 10);

	//@备注 使用指定类型数字转换为文本，会替换掉原始文本
	//@参数 n 要转换的整型
	//@参数 base 进制
	//@返回 转换后的字符串
	//@别名 置整型(整型, 进制)
	CXXString& setNum(int n, __int64 base = 10);

	//@备注 使用指定类型数字转换为文本，会替换掉原始文本
	//@参数 n 要转换的正整型
	//@参数 base 进制
	//@返回 转换后的字符串
	//@别名 置整型(正整型, 进制)
	CXXString& setNum(UINT n, __int64 base = 10);

	//@备注 使用指定类型数字转换为文本，会替换掉原始文本
	//@参数 n 要转换的32位长整型
	//@参数 base 进制
	//@返回 转换后的字符串
	//@别名 置整型(三十二位长整型, 进制)
	CXXString& setNum(long n, __int64 base = 10);

	//@备注 使用指定类型数字转换为文本，会替换掉原始文本
	//@参数 n 要转换的正32位长整型
	//@参数 base 进制
	//@返回 转换后的字符串
	//@别名 置整型(正三十二位长整型, 进制)
	CXXString& setNum(ULONG n, __int64 base = 10);

	//@备注 使用指定类型数字转换为文本，会替换掉原始文本
	//@参数 n 要转换的64位长整型
	//@参数 base 进制
	//@返回 转换后的字符串
	//@别名 置整型(长整型, 进制)
	CXXString& setNum(__int64 n, __int64 base = 10);

	//@备注 使用指定类型数字转换为文本，会替换掉原始文本
	//@参数 n 要转换的正64位长整型
	//@参数 base 进制
	//@返回 转换后的字符串
	//@别名 置整型(正长整型, 进制)
	CXXString& setNum(UINT64 n, __int64 base = 10);

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
	//@别名 置整型(单精度, 格式, 精度)
	CXXString& setNum(float n, wchar_t f = 'g', __int64 prec = 8);

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
	//@别名 置整型(双精度, 格式, 精度)
	CXXString& setNum(double n, wchar_t f = 'g', __int64 prec = 16);

	//@备注 静态使用指定类型数字转换为文本
	//@参数 n 要转换的短整型
	//@参数 base 进制
	//@返回 转换后的字符串
	//@别名 到文本(短整型, 进制)
	static CXXString number(short n, __int64 base = 10);

	//@备注 静态使用指定类型数字转换为文本
	//@参数 n 要转换的短整型
	//@参数 base 进制
	//@返回 转换后的字符串
	//@别名 到文本(整型, 进制)
	static CXXString number(int n, __int64 base = 10);

	//@备注 静态使用指定类型数字转换为文本
	//@参数 n 要转换的短整型
	//@参数 base 进制
	//@返回 转换后的字符串
	//@别名 到文本(正整型, 进制)
	static CXXString number(UINT n, __int64 base = 10);

	//@备注 静态使用指定类型数字转换为文本
	//@参数 n 要转换的正短整型
	//@参数 base 进制
	//@返回 转换后的字符串
	//@别名 到文本(正短整型, 进制)
	static CXXString number(USHORT n, __int64 base = 10);

	//@备注 静态使用指定类型数字转换为文本
	//@参数 n 要转换的32位长整型
	//@参数 base 进制
	//@返回 转换后的字符串
	//@别名 到文本(三十二位长整型, 进制)
	static CXXString number(long n, __int64 base = 10);

	//@备注 静态使用指定类型数字转换为文本
	//@参数 n 要转换的正32位长整型
	//@参数 base 进制
	//@返回 转换后的字符串
	//@别名 到文本(正三十二位长整型, 进制)
	static CXXString number(ULONG n, __int64 base = 10);

	//@备注 静态使用指定类型数字转换为文本
	//@参数 n 要转换的64位长整型
	//@参数 base 进制
	//@返回 转换后的字符串
	//@别名 到文本(长整型, 进制)
	static CXXString number(__int64 n, __int64 base = 10);

	//@备注 静态使用指定类型数字转换为文本
	//@参数 n 要转换的正64位长整型
	//@参数 base 进制
	//@返回 转换后的字符串
	//@别名 到文本(正长整型, 进制)
	static CXXString number(UINT64 n, __int64 base = 10);


	/*@声明
	//@备注 静态使用指定类型数字转换为文本
	//@参数 n 要转换的单精度浮点数
	//@参数 f 格式化方式
	//'e': 以[-]9.9e[+| -]999的形式格式化，小数点后的数字数量。
	//'E' : 以[-]9.9E[+| -]999的形式格式化，小数点后的数字数量。
	//'f' : 以[-]9.9的形式格式化，小数点后的数字数量。
	//'F' : 与'f'相同，除了对于无穷大和非数字的表示（具体看下面的说明），小数点后的数字数量。
	//'g' : 使用'e'或'f'格式，取决于哪种更简洁，最大的有效数字数（省略尾随的零）。
	//'G' : 使用'E'或'F'格式，取决于哪种更简洁，最大的有效数字数（省略尾随的零）。
	//@参数 prec 精度
	//@返回 转换后的字符串
	//@别名 到文本(单精度, 格式, 精度)
	static CXXString number(float n, wchar_t f = 'g', __int64 prec = 6);

	//@备注 静态使用指定类型数字转换为文本
	//@参数 n 要转换的双精度浮点数
	//@参数 f 格式化方式
	//'e': 以[-]9.9e[+| -]999的形式格式化，小数点后的数字数量。
	//'E' : 以[-]9.9E[+| -]999的形式格式化，小数点后的数字数量。
	//'f' : 以[-]9.9的形式格式化，小数点后的数字数量。
	//'F' : 与'f'相同，除了对于无穷大和非数字的表示（具体看下面的说明），小数点后的数字数量。
	//'g' : 使用'e'或'f'格式，取决于哪种更简洁，最大的有效数字数（省略尾随的零）。
	//'G' : 使用'E'或'F'格式，取决于哪种更简洁，最大的有效数字数（省略尾随的零）。
	//@参数 prec 精度
	//@返回 转换后的字符串
	//@别名 到文本(双精度, 格式, 精度)
	static CXXString number(double n, wchar_t f = 'g', __int64 prec = 6);

	*/
	//@隐藏{

	static CXXString number(float n, wchar_t f = L'g', __int64 prec = 6);

	static CXXString number(double n, wchar_t f = L'g', __int64 prec = 6);

	typedef std::wstring::iterator iterator;
	typedef const std::wstring::const_iterator const_iterator;
	typedef iterator Iterator;
	typedef const_iterator ConstIterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	//@隐藏}

	//@备注 取得文本的起始迭代器
	//@返回 起始迭代器
	//@别名 开始()
	iterator begin();

	//@备注 取得文本的常量起始迭代器
	//@返回 常量起始迭代器
	const_iterator begin() const;

	//@备注 取得文本的常量起始迭代器
	//@返回 常量起始迭代器
	//@别名 常量开始()
	const_iterator cbegin() const;

	//@备注 取得文本的常量起始迭代器
	//@返回 常量起始迭代器
	const_iterator constBegin() const;

	//@备注 取得文本的结束迭代器
	//@返回 结束迭代器
	//@别名 结束()
	iterator end();

	//@备注 取得文本的常量结束迭代器
	//@返回 常量结束迭代器
	const_iterator end() const;

	//@备注 取得文本的常量结束迭代器
	//@返回 常量结束迭代器
	//@别名 常量结束()
	const_iterator cend() const;

	//@备注 取得文本的常量结束迭代器
	//@返回 常量结束迭代器
	const_iterator constEnd() const;

	//@备注 取得文本的反向迭代器
	//@返回 反向迭代器
	//@别名 反向开始()
	reverse_iterator rbegin();

	//@备注 取得文本的反向迭代器
	//@返回 反向迭代器
	//@别名 反向结束()
	reverse_iterator rend();

	//@备注 取得文本的常量反向迭代器
	//@返回 常量反向迭代器
	const_reverse_iterator rbegin() const;

	//@备注 取得文本的常量反向迭代器
	//@返回 常量反向迭代器
	const_reverse_iterator rend() const;

	//@备注 取得文本的常量反向迭代器
	//@返回 常量反向迭代器
	//@别名 常量反向开始()
	const_reverse_iterator crbegin() const;

	//@备注 取得文本的常量反向迭代器
	//@返回 常量反向迭代器
	//@别名 常量反向结束()
	const_reverse_iterator crend() const;

	// STL compatibility
	//@隐藏{
	typedef __int64 cxsizeype;
	typedef const wchar_t*& const_reference;
	typedef wchar_t*& reference;
	typedef wchar_t* pointer;
	typedef const wchar_t* const_pointer;
	typedef wchar_t* value_type;
	//@隐藏}
	size_t find(const std::wstring& str, size_t pos) const noexcept;
	size_t find(const wchar_t* s, size_t pos = 0) const;
	size_t find(const wchar_t* s, size_t pos, cxsizeype n) const;
	size_t find(wchar_t c, size_t pos = 0) const noexcept;
	//rfind
	size_t rfind(const std::wstring& str, size_t pos = npos) const noexcept;
	size_t rfind(const wchar_t* s, size_t pos = npos) const;
	size_t rfind(const wchar_t* s, size_t pos, cxsizeype n) const;
	size_t rfind(wchar_t c, size_t pos = npos) const noexcept;

	bool empty() const noexcept;

	void push_back(wchar_t* c);

	void push_back(const CXXString& s);

	void push_front(wchar_t* c);

	void push_front(const CXXString& s);

	void shrink_to_fit();

	bool isNull() const;

	//@隐藏{
	static constexpr auto npos{ static_cast<cxsizeype>(-1) };
	//@隐藏}

private:
	template <typename T>
	static std::wstring fromNumberic(T n, __int64 base)
	{
		CXXString s;
		std::stringstream ss;
		switch (base)
		{
		case 10:
		{
			s = CXXString::fromUtf8(std::to_string(n).c_str());
			break;
		}
		case 16:
		{
			ss << "0x" << std::hex << static_cast<ULONG>(n);
			s = CXXString::fromUtf8(ss.str().c_str());
			break;
		}
		case 8:
		{
			ss << "0" << std::oct << static_cast<ULONG>(n);
			s = CXXString::fromUtf8(ss.str().c_str());
			break;
		}
		case 2:
		{
			ss << std::bitset<sizeof(int) * 8>(static_cast<ULONG>(n));
			s = CXXString::fromUtf8(ss.str().c_str());
			break;
		}
		default:
		{
			if (base < 2 || base > 16) {
				return L"";
			}

			constexpr const char* digits = "0123456789ABCDEF";
			__int64 number = static_cast<__int64>(n);
			bool negative = number < 0;
			if (negative)
				number = -number;

			do
			{
				__int64 remainder = number % base;
				number /= base;
				s += CXXString::fromUtf8(digits[remainder]);
			} while (number > 0);

			if (negative)
				s += L'-';

			std::reverse(s.begin(), s.end());

			break;
		}
		}

		return s.c_str();
	}

	//强制floating类型 double 或 float 或 long double
	template <typename T>
	static std::wstring fromFloating(typename std::enable_if<
		std::is_same<T, float>::value ||
		std::is_same<T, double>::value ||
		std::is_same<T, long double>::value, T>::type n, wchar_t f, __int64 prec)

	{

		std::wstringstream ss;
		if (f == L'e' || f == L'E')
		{
			ss << std::scientific << std::setprecision(prec) << n;
		}
		else if (f == L'f' || f == L'F')
		{
			ss << std::fixed << std::setprecision(prec) << n;
		}
		else if (f == L'g' || f == L'G')
		{
			ss << std::defaultfloat << std::setprecision(prec) << n;
		}
		else
		{
			CXX_ASSERT_X(false, L"string::toFloating: invalid format");
			ss << std::defaultfloat << std::setprecision(prec) << n; // Default format (fixed or scientific)
		}

		std::wstring s;
		s = ss.str();
		return s;
	}

	template<typename T>
	static std::wstring convertArgToString(const T& arg, std::enable_if_t<std::is_integral<T>::value>* = CXX_NULLPTR)
	{
		return CXXString::fromNumberic<T>(arg, 10).data();
	}

	// 转换浮点数类型参数为字符串
	template<typename T>
	static std::wstring convertArgToString(const T& arg, std::enable_if_t<std::is_floating_point<T>::value>* = CXX_NULLPTR)

	{
		CXXString s = CXXString::fromFloating<T>(arg, L'f', 10);
		std::wstring str = s.toStdWString();

		for (size_t i = str.size() - 1; i > 0; --i)
		{
			//found a zero
			if (str[i] == L'0')
			{
				if (((i - 1) > 0) && str[i - 1] == L'0')
				{
					str.pop_back();
				}
			}
			else if (str[i] != L'0' || str[i] != L'.')
			{
				break; // Stop if we find a non-zero digit or a decimal point (.)
			}
		}

		return str;
	}

	static std::wstring convertArgToString(const wchar_t arg);

	static std::wstring convertArgToString(bool arg);

	static std::wstring convertArgToString(const wchar_t* arg);

	template<typename T>
	static std::wstring convertArgToString(const T* arg
		, std::enable_if_t<
		!std::is_same<T, wchar_t*>::value
		>* = CXX_NULLPTR)
	{
		std::wostringstream ss;
		ss << L"0x" << std::hex << reinterpret_cast<uintptr_t>(arg);
		return ss.str();
	}

	static std::wstring convertArgToString(const CXXString& arg);

	template<typename T>
	static std::wstring convertArgToString(const T& arg,
		std::enable_if_t<!std::is_integral<T>::value
		&& !std::is_floating_point<T>::value
		&& !std::is_same<T, const wchar_t*>::value
		&& !std::is_same<T, const wchar_t>::value
		&& !std::is_same<T, const bool>::value
		&& !std::is_same<T, const BYTE>::value
		&& !std::is_pointer<T>::value
		&& !std::is_same<T, std::string>::value
		&& !std::is_same<T, std::wstring>::value>* = CXX_NULLPTR)
	{
		// 其他类型参数的处理逻辑，这里先返回空字符串
		return L"";
	}

private:
	//@隐藏{
	std::wstring& ref() noexcept;

	friend bool operator==(const wchar_t* w, const CXXString& s) noexcept { return s.toStdWString() == std::wstring(w); }
	friend bool operator!=(const wchar_t* w, const CXXString& s) noexcept { return s.toStdWString() != std::wstring(w); }
	friend bool operator<(const wchar_t* w, const CXXString& s) noexcept { return s.toStdWString() < std::wstring(w); }
	friend bool operator>(const wchar_t* w, const CXXString& s) noexcept { return s.toStdWString() > std::wstring(w); }
	friend bool operator<=(const wchar_t* w, const CXXString& s) noexcept { return s.toStdWString() <= std::wstring(w); }
	friend bool operator>=(const wchar_t* w, const CXXString& s) noexcept { return s.toStdWString() >= std::wstring(w); }
	friend bool operator==(const CXXString& s, const wchar_t* w) noexcept { return s.toStdWString() == std::wstring(w); }
	friend bool operator!=(const CXXString& s, const wchar_t* w) noexcept { return s.toStdWString() != std::wstring(w); }
	friend bool operator<(const CXXString& s, const wchar_t* w) noexcept { return s.toStdWString() < std::wstring(w); }
	friend bool operator>(const CXXString& s, const wchar_t* w) noexcept { return s.toStdWString() > std::wstring(w); }
	friend bool operator<=(const CXXString& s, const wchar_t* w) noexcept { return s.toStdWString() <= std::wstring(w); }
	friend bool operator>=(const CXXString& s, const wchar_t* w) noexcept { return s.toStdWString() >= std::wstring(w); }

	//friend with std::wstring
	friend std::wstring;

	CXX_DECLARE_PRIVATE(CXXString);
	CXXScopedPointer<CXXStringPrivate> d_ptr;
	//@隐藏}
};

//@备注 等同 CXXVector<CXXString> ，但是提供了更多的文本数组专用的操作
//@别名 炫文本数组
class CXXStringList : public CXXVector<CXXString>
{
public:
	CXXStringList()
	{

	}

	CXXStringList(const CXXString& str)
		//@隐藏{
		: CXXVector<CXXString>({ str })
		//@隐藏}
	{
	}

	CXXStringList(const CXXStringList& other)
		: CXXVector<CXXString>(other)
	{

	}

	CXXStringList(const CXXVector<CXXString>& other)
		: CXXVector<CXXString>(other)
	{

	}

	virtual ~CXXStringList()
	{

	}

	//@隐藏{
	CXXStringList(CXXStringList&& other) noexcept
		: CXXVector<CXXString>(std::move(other))
	{

	}

	CXXStringList(CXXVector<CXXString>&& other) noexcept
		: CXXVector<CXXString>(std::move(other))
	{

	}

	CXXStringList(std::vector<CXXString>&& other) noexcept
		: CXXVector<CXXString>(std::move(other))
	{

	}

	CXXStringList(const std::vector<CXXString>& other)
		: CXXVector<CXXString>(other)
	{

	}

	CXXStringList(std::initializer_list<CXXString> args)
		: CXXVector<CXXString>(args)
	{

	}



	CXXStringList& operator=(const CXXStringList& other)
	{
		CXXVector<CXXString>::operator=(other);
		return *this;
	}

	CXXStringList& operator=(const CXXVector<CXXString>& other)
	{
		CXXVector<CXXString>::operator=(other);
		return *this;
	}

	CXXStringList& operator=(CXXStringList&& other) noexcept
	{
		CXXVector<CXXString>::operator=(std::move(other));
		return *this;
	}

	CXXStringList& operator=(std::initializer_list<CXXString> args)
	{
		CXXVector<CXXString>::operator=(args);
		return *this;
	}

	CXXStringList& operator=(const std::vector<CXXString>& other)
	{
		CXXVector<CXXString>::operator=(other);
		return *this;
	}

	CXXStringList& operator=(std::vector<CXXString>&& other) noexcept
	{
		CXXVector<CXXString>::operator=(std::move(other));
		return *this;
	}




	//@隐藏}

	//@备注 检查文本数组中是否包含文本
	//@参数 str 文本
	//@参数 cs 是否区分大小写
	//@返回 是否包含
	//@别名 包含(文本)
	bool contains(const CXXString& str, cxx::CaseSensitivity cs = cxx::CaseSensitive) const
	{
		for (const auto& s : *this)
		{
			if (s.compare(str, cs) == 0)
			{
				return true;
			}
		}

		return false;
	}

	//@备注 查找文本在数组中的位置
	//@参数 str 文本
	//@参数 from 开始位置
	//@参数 cs 是否区分大小写
	//@返回 位置
	//@别名 查找(文本, 开始位置, 是否区分大小写)
	__int64 indexOf(const CXXString& str, __int64 from = 0, cxx::CaseSensitivity cs = cxx::CaseSensitive) const
	{
		if (from < 0)
		{
			from = 0;
		}

		for (__int64 i = from; i < size(); ++i)
		{
			if (at(i).compare(str, cs) == 0)
			{
				return i;
			}
		}

		return -1;
	}

	//@备注 将文本数组透过指定文本连接合并成一个文本
	//@参数 separator 分隔文本
	//@返回 合并后的文本
	//@别名 合并(分隔文本)
	CXXString join(const CXXString& separator) const
	{
		CXXString str;
		for (const auto& s : *this)
		{
			str += s + separator;
		}

		if (!str.isEmpty())
		{
			str.remove(str.size() - separator.size(), separator.size());
		}

		return str;
	}

};

//@备注 将文本按token的方式取出
//@别名 炫分词器
class CXXTokenizer
{
public:
	//@备注 构造函数
	//@参数 source 源文本
	//@参数 delimiters 分隔符
	//@参数 options 选项
	//@别名 构造(源文本, 分隔符, 选项)
	CXXTokenizer(const CXXString& source, const CXXString& delimiters, cxx::SplitBehavior options = cxx::KeepEmptyParts)
		: source_(source)
		, delimiters_(delimiters)
		, length_(source.size())
		, keepEmptyParts_(options == cxx::KeepEmptyParts)
		, hasMoreTokens_(true)
	{
	}

	//@备注 构造函数
	//@参数 source 源文本
	//@参数 delimiters 分隔符
	//@参数 options 选项
	//@别名 构造(源文本, 分隔符, 选项)
	CXXTokenizer(const wchar_t* source, const wchar_t* delimiters, cxx::SplitBehavior options = cxx::KeepEmptyParts)
		: source_(source)
		, delimiters_(delimiters)
		, length_(source_.size())
		, keepEmptyParts_(options == cxx::KeepEmptyParts)
		, hasMoreTokens_(true)
	{
	}

	//@备注 构造函数
	//@参数 source 源文本
	//@参数 delimiters 分隔符
	//@参数 options 选项
	//@别名 构造(源文本, 分隔符, 选项)
	CXXTokenizer(const CXXString& source, wchar_t delimiter, cxx::SplitBehavior options = cxx::KeepEmptyParts)
		: source_(source)
		, delimiters_(delimiter)
		, length_(source_.size())
		, keepEmptyParts_(options == cxx::KeepEmptyParts)
		, hasMoreTokens_(true)
	{
	}


	//@备注 获取下一个token
	//@返回 token
	//@别名 下一个()
	CXXString next()
	{
		if (!hasMoreTokens_)
		{
			return CXXString();
		}

		lastPos_ = pos_;
		pos_ = source_.indexOf(delimiters_, pos_);
		if (pos_ == -1)
		{
			pos_ = length_;
			hasMoreTokens_ = false;
		}

		CXXString token = source_.mid(lastPos_, pos_ - lastPos_);
		pos_ += delimiters_.size();
		if (!keepEmptyParts_ && token.isEmpty())
		{
			return next();
		}

		return token;
	}

	//@备注 是否还有下一个token
	//@返回 是否有
	//@别名 是否尾()
	bool isEnd() const
	{
		return !hasMoreTokens_;
	}

	//@备注 重置
	//@别名 重置()
	void reset()
	{
		pos_ = 0;
		lastPos_ = 0;
		hasMoreTokens_ = true;
		size_ = 0;
	}

	//@备注 设置源文本
	//@参数 source 源文本
	//@别名 置文本()
	void setString(const CXXString& source)
	{
		source_ = source;
		length_ = source.size();
		size_ = 0;
	}

	//@备注 设置分隔符
	//@参数 delimiters 分隔符
	//@别名 置分隔符()
	void setDelimiters(const CXXString& delimiters)
	{
		delimiters_ = delimiters;
		size_ = 0;
	}

	//@备注 设置选项
	//@参数 options 选项
	//@别名 置选项()
	void setOptions(cxx::SplitBehavior options)
	{
		keepEmptyParts_ = options == cxx::KeepEmptyParts;
		size_ = 0;
	}

	//@备注 设置位置
	//@参数 pos 位置
	//@别名 置位置()
	void setPosition(__int64 pos)
	{
		pos_ = pos;
	}

	//@备注 获取位置
	//@返回 位置
	//@别名 位置()
	__int64 position() const
	{
		return pos_;
	}

	//@备注 获取源文本
	//@返回 源文本
	//@别名 文本()
	CXXString string() const
	{
		return source_;
	}

	//@备注 获取分隔符
	//@返回 分隔符
	//@别名 分隔符()
	CXXString delimiters() const
	{
		return delimiters_;
	}

	//@备注 获取选项
	//@返回 选项
	//@别名 选项()
	cxx::SplitBehavior options() const
	{
		return keepEmptyParts_ ? cxx::KeepEmptyParts : cxx::SkipEmptyParts;
	}

	//@备注 获取以当前分隔符分割的token数量
	//@返回 数量
	//@别名 大小()
	__int64 size()
	{
		if (size_ == 0)
		{
			CXXTokenizer temp(*this);
			while (!temp.isEnd())
			{
				temp.next();
				++size_;
			}
		}

		return size_;
	}

	//@备注 获取以当前分隔符分割的token数量
	//@返回 数量
	//@别名 大小()
	__int64 size() const
	{
		return size_;
	}

	//@备注 获取以当前分隔符分割的token数量
	__int64 count() const
	{
		return size_;
	}

	//@备注 获取以当前分隔符分割的token数量
	__int64 length() const
	{
		return size_;
	}

private:
	//@隐藏{
	CXXString source_;
	CXXString token_;
	CXXString delimiters_;
	__int64 size_ = 0;
	__int64 pos_ = 0;
	__int64 lastPos_ = 0;
	__int64 length_ = 0;
	bool keepEmptyParts_;
	bool hasMoreTokens_;
	//@隐藏}
};


//@隐藏{
namespace std
{
	template<>
	struct hash<CXXString>
	{
		size_t operator()(const CXXString& key) const noexcept
		{
			return hash<std::wstring>()(key.data());
		}
	};
}

// 字面量操作符 ""_s
inline CXXString operator""_s(const char16_t* str, size_t size)
{
	return CXXString(std::wstring(str, str + size));
}

// 重載 << 操作符
inline std::wostream& operator<<(std::wostream& stream, const CXXString& string)
{
	stream << string.toStdWString();
	return stream;
}

// 重载输入运算符 (>>) 用于std::wistream
inline std::wistream& operator>>(std::wistream& stream, CXXString& string)
{
	std::wstring wstr;
	stream >> wstr;
	string = CXXString(wstr);
	return stream;
}

//@隐藏}

//@分组}

#endif //CXXSTRING_H_