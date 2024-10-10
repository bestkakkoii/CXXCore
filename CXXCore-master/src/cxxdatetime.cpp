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

#include <cxxdatetime.h>

CXXDateTime::CXXDateTime()
{

}

CXXDateTime::~CXXDateTime()
{

}

CXXDateTime::CXXDateTime(const CXXDate& date, const CXXTime& time)
	: date_(date)
	, time_(time) {}

CXXDateTime CXXDateTime::currentDateTime()
{
	return CXXDateTime(CXXDate::currentDate(), CXXTime::currentTime());
}

void CXXDateTime::setDate(const CXXDate& date)
{
	date_ = date;
}

CXXDate CXXDateTime::date() const
{
	return date_;
}

void CXXDateTime::setTime(const CXXTime& time)
{
	time_ = time;
}

CXXTime CXXDateTime::time() const
{
	return time_;
}

__int64 CXXDateTime::toMSecsSinceEpoch() const
{
	std::tm tm = {};
	tm.tm_year = static_cast<int>(date_.year()) - 1900;
	tm.tm_mon = static_cast<int>(date_.month()) - 1;
	tm.tm_mday = static_cast<int>(date_.day());
	tm.tm_hour = static_cast<int>(time_.hours());
	tm.tm_min = static_cast<int>(time_.minutes());
	tm.tm_sec = static_cast<int>(time_.seconds());

	auto timepoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(timepoint.time_since_epoch()).count();
	return milliseconds + time_.milliseconds();
}

CXXDateTime CXXDateTime::fromMSecsSinceEpoch(__int64 msecs)
{
	// 将毫秒数转换为 time_point 对象
	auto timepoint = std::chrono::system_clock::time_point(std::chrono::milliseconds(msecs));

	// 将 time_point 转换为 time_t
	std::time_t rawTime = std::chrono::system_clock::to_time_t(timepoint);

	// 使用安全的 gmtime_s 将 time_t 转换为 tm 结构
	std::tm timeinfo = {};
	gmtime_s(&timeinfo, &rawTime);  // 替换为安全的 gmtime_s

	// 计算剩余的毫秒数
	__int64 remainingMillis = msecs % 1000;

	// 创建 CXXDate 和 CXXTime 对象
	CXXDate date(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
	CXXTime time(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, remainingMillis);

	// 返回组合的 CXXDateTime 对象
	return CXXDateTime(date, time);
}

CXXDateTime CXXDateTime::addDays(__int64 ndays) const
{
	CXXDate newDate = date_.addDays(ndays);
	return CXXDateTime(newDate, time_);
}

CXXDateTime CXXDateTime::addSecs(__int64 secs) const
{
	__int64 totalSecs = time_.hours() * 3600 + time_.minutes() * 60 + time_.seconds() + secs;
	__int64 daysToAdd = totalSecs / 86400;
	totalSecs %= 86400;
	__int64 newHour = totalSecs / 3600;
	__int64 newMinute = (totalSecs % 3600) / 60;
	__int64 newSecond = totalSecs % 60;

	CXXDate newDate = date_.addDays(daysToAdd);
	CXXTime newTime(newHour, newMinute, newSecond, time_.milliseconds());

	return CXXDateTime(newDate, newTime);
}

CXXDateTime CXXDateTime::addMSecs(__int64 msecs) const
{
	__int64 totalMSecs = toMSecsSinceEpoch() + msecs;
	return fromMSecsSinceEpoch(totalMSecs);
}

CXXString CXXDateTime::toString(const CXXString& format) const
{
	CXXString dateString = date_.toString(format);
	CXXString timeString = time_.toString(format);
	return dateString + L" " + timeString;
}

CXXDateTime CXXDateTime::fromString(const CXXString& string, const CXXString& format)
{
	// 将格式化字符串转换为正则表达式
	std::wstring regexPattern = format.toStdWString();  // 转换为 std::wstring 以进行正则处理
	regexPattern = std::regex_replace(regexPattern, std::wregex(L"%d"), L"(\\d{1,2})");
	regexPattern = std::regex_replace(regexPattern, std::wregex(L"%M"), L"(\\d{1,2})");
	regexPattern = std::regex_replace(regexPattern, std::wregex(L"%yyyy"), L"(\\d{4})");
	regexPattern = std::regex_replace(regexPattern, std::wregex(L"%H"), L"(\\d{1,2})");
	regexPattern = std::regex_replace(regexPattern, std::wregex(L"%m"), L"(\\d{1,2})");
	regexPattern = std::regex_replace(regexPattern, std::wregex(L"%s"), L"(\\d{1,2})");
	regexPattern = std::regex_replace(regexPattern, std::wregex(L"%z"), L"(\\d{1,3})");

	// 将正则表达式应用到输入的字符串
	std::wregex re(regexPattern);
	std::wsmatch match;

	// 使用 std::wstring 来调用 regex_search
	std::wstring inputStr = string.toStdWString();
	if (std::regex_search(inputStr, match, re))
	{
		if (match.size() == 8) // 确保捕获了所有部分
		{
			__int64 day = std::stoll(match[1].str());
			__int64 month = std::stoll(match[2].str());
			__int64 year = std::stoll(match[3].str());
			__int64 hour = std::stoll(match[4].str());
			__int64 minute = std::stoll(match[5].str());
			__int64 second = std::stoll(match[6].str());
			__int64 millisecond = std::stoll(match[7].str());

			// 创建日期和时间对象
			CXXDate date(year, month, day);
			CXXTime time(hour, minute, second, millisecond);

			return CXXDateTime(date, time);
		}
	}

	// 如果匹配失败，返回默认构造的无效 CXXDateTime 对象
	return CXXDateTime();
}

bool CXXDateTime::operator==(const CXXDateTime& other) const
{
	return toMSecsSinceEpoch() == other.toMSecsSinceEpoch();
}

bool CXXDateTime::operator!=(const CXXDateTime& other) const
{
	return !(*this == other);
}

bool CXXDateTime::operator<(const CXXDateTime& other) const
{
	return toMSecsSinceEpoch() < other.toMSecsSinceEpoch();
}

bool CXXDateTime::operator<=(const CXXDateTime& other) const
{
	return toMSecsSinceEpoch() <= other.toMSecsSinceEpoch();
}

bool CXXDateTime::operator>(const CXXDateTime& other) const
{
	return toMSecsSinceEpoch() > other.toMSecsSinceEpoch();
}

bool CXXDateTime::operator>=(const CXXDateTime& other) const
{
	return toMSecsSinceEpoch() >= other.toMSecsSinceEpoch();
}
