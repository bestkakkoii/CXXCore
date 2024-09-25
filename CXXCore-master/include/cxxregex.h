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
#ifndef CXXREGEX_H_
//@隐藏{
#define CXXREGEX_H_
//@隐藏}



#include <string>
#include <vector>
#include <regex>

#include <cxxglobal.h>
#include <cxxscopedpointer.h>
#include <cxxatomic.h>
#include <cxxstring.h>

//@隐藏{
class CXXRegexPrivate;
class CXXRegexMatchPrivate;
class CXXRegexMatchIteratorPrivate;

class CXXRegexMatch;
class CXXRegexMatchIterator;
//@隐藏}

//@分组{［文本］.炫　正则表达式类

//@别名 炫正则表达式
class CXXRegex
{
public:
	//@备注 正则表达式
	//@参数 正则表达式，建议使用静态常量如: static const regex re("\\d+");
	CXXRegex(const CXXString& pattern);

	//@隐藏{
	virtual ~CXXRegex();
	//@隐藏}

	//@备注 是否有效
	//@参数 无
	//@返回 有效返回true，否则返回false
	//@别名 是否有效()
	bool isValid() const;

	//@备注 已设置的正则表达式
	//@参数 无
	//@返回 表达式
	//@别名 表达式()
	CXXString pattern() const;

	//@备注 设置正则表达式
	//@参数 正则表达式
	//@返回 无
	//@别名 置表达式(表达式)
	void setPattern(const CXXString& pattern);

	//@备注 匹配
	//@参数 文本
	//@返回 匹配结果
	//@别名 匹配(文本)
	CXXRegexMatch match(const CXXString& text) const;

	//@备注 全局匹配
	//@参数 文本
	//@返回 匹配结果迭代器
	//@别名 全局匹配(文本)
	CXXRegexMatchIterator globalMatch(const CXXString& text) const;

	std::wregex raw() const;

private:
	//@隐藏{
	CXX_DECLARE_PRIVATE(CXXRegex);
	CXXScopedPointer<CXXRegexPrivate> d_ptr;
	//@隐藏}
};

//@别名 炫正则表达式结果
class CXXRegexMatch
{
public:
	CXXRegexMatch(const std::wsmatch& m, bool v);

	CXXRegexMatch(const CXXRegexMatch& m, bool v);

	CXXRegexMatch();

	//@隐藏{
	CXXRegexMatch& operator=(const CXXRegexMatch& other);
	//@隐藏}

	//@备注 是否有匹配结果
	//@参数 无
	//@返回 有匹配结果返回true，否则返回false
	//@别名 有结果()
	bool hasMatch() const;

	//@备注 是否有部分匹配结果
	//@参数 无
	//@返回 有部分匹配结果返回true，否则返回false
	//@别名 有部分结果()
	bool hasPartialMatch() const;

	//@备注 是否有效
	//@参数 无
	//@返回 有效返回true，否则返回false
	//@别名 是否有效()
	bool isValid() const;

	//@备注 最后一个匹配结果的索引
	//@参数 无
	//@返回 最后匹配结果的索引
	//@别名 最后匹配结果索引()
	__int64 lastCapturedIndex() const;

	//@备注 匹配到一个或多个文本，如果包含子匹配结果通常nth为1
	//@参数 匹配结果索引
	//@返回 匹配的文本
	//@别名 结果文本(索引)
	CXXString captured(__int64 nth = 0) const;

	//@隐藏{
	std::wsmatch raw() const;
	//@隐藏}

private:
	//@隐藏{
	CXX_DECLARE_PRIVATE(CXXRegexMatch);
	CXXScopedPointer<CXXRegexMatchPrivate> d_ptr;
	//@隐藏}
};

//@别名 炫正则表达式结果迭代器
class CXXRegexMatchIterator
{
public:

	CXXRegexMatchIterator();

	CXXRegexMatchIterator(const std::wstring& t, const std::wregex& r);

	CXXRegexMatchIterator(const CXXString& t, const CXXRegex& r);

	//@隐藏{
	CXXRegexMatchIterator& operator=(const CXXRegexMatchIterator& other);
	//@隐藏}

	//@备注 是否有效
	//@参数 无
	//@返回 有效返回true，否则返回false
	//@别名 是否有效()
	bool isValid() const;

	//@备注 是否有下一个匹配结果
	//@参数 无
	//@返回 有下一个匹配结果返回true，否则返回false
	//@别名 是否有下一个()
	bool hasNext() const;

	//@备注 下一个匹配结果
	//@参数 无
	//@返回 匹配结果
	//@别名 下一个()
	CXXRegexMatch next();

private:
	//@隐藏{
	CXX_DECLARE_PRIVATE(CXXRegexMatchIterator);
	CXXScopedPointer<CXXRegexMatchIteratorPrivate> d_ptr;
	//@隐藏}
};


//@分组}

#endif // CXXREGEX_H_