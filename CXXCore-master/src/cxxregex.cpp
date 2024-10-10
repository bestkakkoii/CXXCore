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
#include <pch.h>

#include <cxxregex_p.h>

CXXRegexPrivate::CXXRegexPrivate(CXXRegex* q, const CXXString& pattern)
	: q_ptr(q)
	, patternStr_(pattern)
	, regex_(std::wregex(pattern.toStdWString()))
	, valid_(!pattern.isEmpty())
{
}

CXXRegexPrivate::~CXXRegexPrivate()
{
}

CXXRegex::CXXRegex(const CXXString& pattern)
	: d_ptr(new CXXRegexPrivate(this, pattern))
{
}

CXXRegex::~CXXRegex()
{

}

bool CXXRegex::isValid() const
{
	CXX_D(const CXXRegex);
	return  d->valid_;
}

std::wregex CXXRegex::raw() const
{
	CXX_D(const CXXRegex);
	return d->regex_;
}

void CXXRegex::setPattern(const CXXString& pattern)
{
	CXX_D(CXXRegex);

	d->patternStr_ = pattern;
	try
	{
		d->regex_ = std::wregex(pattern.toStdWString());
		d->valid_ = true;
	}
	catch (std::regex_error&)
	{
		d->valid_ = false;
	}
}

CXXString CXXRegex::pattern() const
{
	CXX_D(const CXXRegex);
	return d->patternStr_;
}

CXXRegexMatch CXXRegex::match(const CXXString& text) const
{
	CXX_D(const CXXRegex);

	std::wsmatch m;
	std::wstring wtext = text.toStdWString();
	bool foundMatch = std::regex_search(wtext, m, d->regex_);
	return CXXRegexMatch(m, foundMatch);
}

CXXRegexMatchIterator CXXRegex::globalMatch(const CXXString& text) const
{
	CXX_D(const CXXRegex);

	std::wstring wtext = text.toStdWString();
	return CXXRegexMatchIterator(wtext, d->regex_);
}

CXXRegexMatchPrivate::CXXRegexMatchPrivate(CXXRegexMatch* q, const std::wsmatch& match)
	: q_ptr(q)
	, match_(match)
	, valid_(true)
{
}
CXXRegexMatchPrivate::~CXXRegexMatchPrivate()
{
}

CXXRegexMatch::CXXRegexMatch(const std::wsmatch& m, bool v)
	: d_ptr(new CXXRegexMatchPrivate(this, m))
{
	CXX_D(CXXRegexMatch);
	d->valid_ = v;
}

CXXRegexMatch::CXXRegexMatch(const CXXRegexMatch& r, bool v)
	: d_ptr(new CXXRegexMatchPrivate(this, r.d_ptr->match_))
{
	CXX_D(CXXRegexMatch);
	d->valid_ = v;
}


CXXRegexMatch::CXXRegexMatch()
	: d_ptr(new CXXRegexMatchPrivate(this, std::wsmatch()))
{
	CXX_D(CXXRegexMatch);
	d->valid_ = false;
}

CXXRegexMatch& CXXRegexMatch::operator=(const CXXRegexMatch& other)
{
	CXX_D(CXXRegexMatch);
	d->match_ = other.d_ptr->match_;
	d->valid_ = other.d_ptr->valid_;

	return *this;
}
//@隐藏}

//@备注 是否有匹配结果
//@参数 无
//@返回 有匹配结果返回true，否则返回false
//@别名 有结果()
bool CXXRegexMatch::hasMatch() const
{
	CXX_D(const CXXRegexMatch);
	return d->match_.ready() && d->match_.size() > 0;
}

//@备注 是否有部分匹配结果
//@参数 无
//@返回 有部分匹配结果返回true，否则返回false
//@别名 有部分结果()
bool CXXRegexMatch::hasPartialMatch() const
{
	CXX_D(const CXXRegexMatch);
	return d->match_.ready() && d->match_.prefix().matched;
}

//@备注 是否有效
//@参数 无
//@返回 有效返回true，否则返回false
//@别名 是否有效()
bool CXXRegexMatch::isValid() const
{
	CXX_D(const CXXRegexMatch);
	return d->valid_;
}

//@备注 最后一个匹配结果的索引
//@参数 无
//@返回 最后匹配结果的索引
//@别名 最后匹配结果索引()
__int64 CXXRegexMatch::lastCapturedIndex() const
{
	CXX_D(const CXXRegexMatch);
	__int64 size = static_cast<__int64>(d->match_.size());
	return size > 0 ? size - 1 : -1;
}

CXXString CXXRegexMatch::captured(__int64 nth) const
{
	CXX_D(const CXXRegexMatch);
	return std::wstring(nth < static_cast<__int64>(d->match_.size()) ? d->match_[static_cast<size_t>(nth)].str().c_str() : L"");
}

std::wsmatch CXXRegexMatch::raw() const
{
	CXX_D(const CXXRegexMatch);
	return d->match_;
}

CXXRegexMatchIteratorPrivate::CXXRegexMatchIteratorPrivate(CXXRegexMatchIterator* q, const std::wstring& t, const std::wregex& r)
	: q_ptr(q)
	, text_(t)
	, iterator_(std::wsregex_iterator(text_.cbegin(), text_.cend(), r))
	, end_(std::wsregex_iterator())
	, valid_(iterator_ != end_)
{
}

CXXRegexMatchIteratorPrivate::~CXXRegexMatchIteratorPrivate()
{
}

CXXRegexMatchIterator::CXXRegexMatchIterator()
{
}

CXXRegexMatchIterator::CXXRegexMatchIterator(const std::wstring& t, const std::wregex& r)
	: d_ptr(new CXXRegexMatchIteratorPrivate(this, t, r))
{
}

CXXRegexMatchIterator::CXXRegexMatchIterator(const CXXString& t, const CXXRegex& r)
	: d_ptr(new CXXRegexMatchIteratorPrivate(this, t.toStdWString(), r.raw()))
{
}

CXXRegexMatchIterator& CXXRegexMatchIterator::operator=(const CXXRegexMatchIterator& other)
{
	CXX_D(CXXRegexMatchIterator);
	d->text_ = other.d_ptr->text_;
	d->iterator_ = other.d_ptr->iterator_;
	d->end_ = other.d_ptr->end_;
	d->valid_ = other.d_ptr->valid_;

	return *this;
}
//@隐藏}

//@备注 是否有效
//@参数 无
//@返回 有效返回true，否则返回false
//@别名 是否有效()
bool CXXRegexMatchIterator::isValid() const
{
	CXX_D(const CXXRegexMatchIterator);
	return d->valid_;
}

//@备注 是否有下一个匹配结果
//@参数 无
//@返回 有下一个匹配结果返回true，否则返回false
//@别名 是否有下一个()
bool CXXRegexMatchIterator::hasNext() const
{
	CXX_D(const CXXRegexMatchIterator);
	return d->iterator_ != d->end_;
}

//@备注 下一个匹配结果
//@参数 无
//@返回 匹配结果
//@别名 下一个()
CXXRegexMatch CXXRegexMatchIterator::next()
{
	CXX_D(CXXRegexMatchIterator);

	if (hasNext())
	{
		return CXXRegexMatch(*d->iterator_++, true);
	}
	else
	{
		return CXXRegexMatch(std::wsmatch(), false);
	}
}