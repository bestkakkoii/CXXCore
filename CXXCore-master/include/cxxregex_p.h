/*
				GNU GENERAL PUBLIC LICENSE
				   Version 3, June 1991
COPYRIGHT (C) Bestkakkoii 2024 All Rights Reserved.
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

*/
#ifndef CXXREGEX_P_H_
#define CXXREGEX_P_H_

#include <regex>

#include <cxxregex.h>
#include <cxxstring.h>
#include <cxxatomic.h>

class CXXRegexPrivate
{

public:
	explicit CXXRegexPrivate(CXXRegex* q, const CXXString& pattern);
	virtual ~CXXRegexPrivate();

private:
	std::wregex regex_;
	std::wstring patternStr_;
	CXXAtomicBool valid_ = false;

private:

	CXX_DECLARE_PUBLIC(CXXRegex);
	CXXRegex* q_ptr = CXX_NULLPTR;

};

class CXXRegexMatchPrivate
{
public:
	explicit CXXRegexMatchPrivate(CXXRegexMatch* q, const std::wsmatch& match);
	virtual ~CXXRegexMatchPrivate();

private:
	std::wsmatch match_;
	CXXAtomicBool valid_ = false;

private:
	CXX_DECLARE_PUBLIC(CXXRegexMatch);
	CXXRegexMatch* q_ptr = CXX_NULLPTR;
};

class CXXRegexMatchIteratorPrivate
{
public:
	explicit CXXRegexMatchIteratorPrivate(CXXRegexMatchIterator* q, const std::wstring& t, const std::wregex& r);
	virtual ~CXXRegexMatchIteratorPrivate();

public:
	std::wstring text_;
	std::wsregex_iterator iterator_;
	std::wsregex_iterator end_;

	CXXAtomicBool valid_ = false;

private:
	CXX_DECLARE_PUBLIC(CXXRegexMatchIterator);
	CXXRegexMatchIterator* q_ptr = CXX_NULLPTR;
};

#endif // CXXREGEX_P_H_
