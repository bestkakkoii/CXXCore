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

#include <cxxdate.h>

CXXDate::CXXDate()
{
	year_ = 0;
	month_ = 0;
	day_ = 0;
}

CXXDate::CXXDate(__int64 y, __int64 m, __int64 d)
{
	setDate(y, m, d);
}

bool CXXDate::setDate(__int64 y, __int64 m, __int64 d)
{
	if (isValid(y, m, d))
	{
		year_ = y;
		month_ = m;
		day_ = d;
		return true;
	}
	return false;
}

bool CXXDate::isValid(__int64 y, __int64 m, __int64 d)
{
	if (y <= 0 || m <= 0 || m > 12 || d <= 0)
	{
		return false;
	}

	__int64 daysInMonth[] = { 31, isLeapYear(y) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	return d <= static_cast<__int64>(daysInMonth[m - 1]);
}

bool CXXDate::isLeapYear(__int64 year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

__int64 CXXDate::year() const
{
	return year_;
}

__int64 CXXDate::month() const
{
	return month_;
}

__int64 CXXDate::day() const
{
	return day_;
}

__int64 CXXDate::dayOfWeek() const
{
	std::tm tm = {};
	tm.tm_year = static_cast<int>(year_) - 1900;
	tm.tm_mon = static_cast<int>(month_) - 1;
	tm.tm_mday = static_cast<int>(day_);

	std::mktime(&tm);
	return (tm.tm_wday == 0 ? 7 : tm.tm_wday);
}

__int64 CXXDate::daysInMonth() const
{
	__int64 daysInMonth[] = { 31, isLeapYear(year_) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	return daysInMonth[month_ - 1];
}

CXXString CXXDate::toString(const CXXString& format) const
{
	std::wostringstream ss;

	std::wstring wFormat = format.toStdWString();

	for (size_t i = 0; i < wFormat.length(); ++i)
	{
		if (wFormat[i] == L'd')
		{
			if (i + 1 < wFormat.length() && wFormat[i + 1] == L'd')
			{
				// dd: 日期，带前导零 (01-31)
				ss << std::setw(2) << std::setfill(L'0') << day_;
				++i;
			}
			else
			{
				// d: 日期，无前导零 (1-31)
				ss << day_;
			}
		}
		else if (wFormat[i] == L'M')
		{
			if (i + 1 < wFormat.length() && wFormat[i + 1] == L'M')
			{
				// MM: 月份，带前导零 (01-12)
				ss << std::setw(2) << std::setfill(L'0') << month_;
				++i;
			}
			else if (i + 2 < wFormat.length() && wFormat[i + 1] == L'M' && wFormat[i + 2] == L'M')
			{
				// MMM: 月份缩写 (Jan-Dec)
				ss << getMonthName(static_cast<__int64>(month_), true);
				i += 2;
			}
			else if (i + 3 < wFormat.length() && wFormat[i + 1] == L'M' && wFormat[i + 2] == L'M' && wFormat[i + 3] == L'M')
			{
				// MMMM: 完整月份名称 (January-December)
				ss << getMonthName(static_cast<__int64>(month_), false);
				i += 3;
			}
			else
			{
				// M: 月份，无前导零 (1-12)
				ss << month_;
			}
		}
		else if (wFormat[i] == L'y')
		{
			if (i + 3 < wFormat.length() && wFormat[i + 1] == L'y' && wFormat[i + 2] == L'y' && wFormat[i + 3] == L'y')
			{
				// yyyy: 年份，四位数
				ss << std::setw(4) << year_;
				i += 3;
			}
			else if (i + 1 < wFormat.length() && wFormat[i + 1] == L'y')
			{
				// yy: 年份，两位数
				ss << std::setw(2) << std::setfill(L'0') << (year_ % 100);
				++i;
			}
			else
			{
				ss << L'y';  // 默认输出 'y' 本身
			}
		}
		else if (wFormat[i] == L'd')
		{
			if (i + 2 < wFormat.length() && wFormat[i + 1] == L'd' && wFormat[i + 2] == L'd')
			{
				// ddd: 星期几缩写 (Mon-Sun)
				ss << getDayOfWeekName(dayOfWeek(), true);
				i += 2;
			}
			else if (i + 3 < wFormat.length() && wFormat[i + 1] == L'd' && wFormat[i + 2] == L'd' && wFormat[i + 3] == L'd')
			{
				// dddd: 星期几全称 (Monday-Sunday)
				ss << getDayOfWeekName(dayOfWeek(), false);
				i += 3;
			}
			else
			{
				ss << day_;  // 默认输出 'd'
			}
		}
		else
		{
			// 直接输出其他字符
			ss << wFormat[i];
		}
	}

	return CXXString(ss.str().c_str());
}

CXXDate CXXDate::addDays(__int64 day) const
{
	// 构建当前日期的 tm 结构
	std::tm tm = {};
	tm.tm_year = static_cast<int>(year_) - 1900; // tm_year 是自 1900 年开始的年数
	tm.tm_mon = static_cast<int>(month_) - 1;    // tm_mon 是从 0 开始的月份
	tm.tm_mday = static_cast<int>(day_);         // tm_mday 是当天的日期

	// 将日期转换为 time_t 类型
	std::time_t time = std::mktime(&tm);

	// 添加指定的天数（以秒为单位）
	time += day * 24 * 60 * 60;

	// 转换回 tm 结构，使用安全版本 localtime_s
	std::tm newTm = {};
	localtime_s(&newTm, &time);  // 使用 localtime_s 替代 localtime

	// 创建新的 CXXDate 对象并返回
	return CXXDate(newTm.tm_year + 1900, newTm.tm_mon + 1, newTm.tm_mday);
}

CXXDate CXXDate::currentDate()
{
	// 获取当前时间
	std::time_t nowTime = std::time(nullptr);

	// 转换为 tm 结构，使用安全版本 localtime_s
	std::tm nowTm = {};
	localtime_s(&nowTm, &nowTime);

	// 返回当前日期的 CXXDate 对象
	return CXXDate(nowTm.tm_year + 1900, nowTm.tm_mon + 1, nowTm.tm_mday);
}

CXXString CXXDate::getMonthName(__int64 month, bool abbreviated) const
{
	static const wchar_t* monthNames[] = {
		L"January", L"February", L"March", L"April", L"May", L"June",
		L"July", L"August", L"September", L"October", L"November", L"December"
	};

	static const wchar_t* monthAbbr[] = {
		L"Jan", L"Feb", L"Mar", L"Apr", L"May", L"Jun",
		L"Jul", L"Aug", L"Sep", L"Oct", L"Nov", L"Dec"
	};

	return abbreviated ? CXXString(monthAbbr[month - 1]) : CXXString(monthNames[month - 1]);
}

CXXString CXXDate::getDayOfWeekName(__int64 dayOfWeek, bool abbreviated) const
{
	static const wchar_t* dayNames[] = {
		L"Monday", L"Tuesday", L"Wednesday", L"Thursday", L"Friday", L"Saturday", L"Sunday"
	};

	static const wchar_t* dayAbbr[] = {
		L"Mon", L"Tue", L"Wed", L"Thu", L"Fri", L"Sat", L"Sun"
	};

	return abbreviated ? CXXString(dayAbbr[dayOfWeek - 1]) : CXXString(dayNames[dayOfWeek - 1]);
}
