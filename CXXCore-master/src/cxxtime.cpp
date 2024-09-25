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

#include <cxxtime.h>

CXXTime::CXXTime()
{
	setHMS(0, 0, 0, 0);
	isNull_ = true;
}

CXXTime::CXXTime(__int64 h, __int64 m, __int64 s, __int64 ms)
{
	setHMS(h, m, s, ms);
}

CXXString CXXTime::toString(const CXXString& format) const
{
	std::wostringstream ss;

	std::wstring wFormat = format.toStdWString();

	bool isAM = hours_ < 12;
	__int64 displayHour = hours_;

	for (size_t i = 0; i < static_cast<size_t>(format.length()); ++i)
	{
		if (wFormat[i] == L'h')
		{
			if (i + 1 < static_cast<size_t>(format.length()) && wFormat[i + 1] == L'h')
			{
				// hh: 小时，带前导零 (01-12)
				displayHour = (hours_ % 12 == 0) ? 12 : hours_ % 12;
				ss << std::setw(2) << std::setfill(L'0') << displayHour;
				++i;
			}
			else
			{
				// h: 小时，无前导零 (1-12)
				displayHour = (hours_ % 12 == 0) ? 12 : hours_ % 12;
				ss << displayHour;
			}
		}
		else if (wFormat[i] == L'H')
		{
			if (i + 1 < static_cast<size_t>(format.length()) && wFormat[i + 1] == L'H')
			{
				// HH: 小时，带前导零 (00-23)
				ss << std::setw(2) << std::setfill(L'0') << hours_;
				++i;
			}
			else
			{
				// H: 小时，无前导零 (0-23)
				ss << hours_;
			}
		}
		else if (wFormat[i] == L'm')
		{
			if (i + 1 < static_cast<size_t>(format.length()) && wFormat[i + 1] == L'm')
			{
				// mm: 分钟，带前导零 (00-59)
				ss << std::setw(2) << std::setfill(L'0') << minutes_;
				++i;
			}
			else
			{
				// m: 分钟，无前导零 (0-59)
				ss << minutes_;
			}
		}
		else if (wFormat[i] == L's')
		{
			if (i + 1 < static_cast<size_t>(format.length()) && wFormat[i + 1] == L's')
			{
				// ss: 秒，带前导零 (00-59)
				ss << std::setw(2) << std::setfill(L'0') << seconds_;
				++i;
			}
			else
			{
				// s: 秒，无前导零 (0-59)
				ss << seconds_;
			}
		}
		else if (wFormat[i] == L'z')
		{
			if (i + 1 < static_cast<size_t>(format.length()) && wFormat[i + 1] == L'z')
			{
				// zzz: 毫秒，带前导零 (000-999)
				ss << std::setw(3) << std::setfill(L'0') << milliseconds_;
				++i;
				if (i + 1 < static_cast<size_t>(format.length()) && wFormat[i + 1] == L'z')
				{
					++i;
				}
			}
			else
			{
				// z: 毫秒，去掉后面的零 (0-999)
				ss << milliseconds_;
			}
		}
		else if (wFormat[i] == L'A')
		{
			if (isAM)
			{
				ss << L"AM";
			}
			else
			{
				ss << L"PM";
			}
		}
		else if (wFormat[i] == L'a')
		{
			if (isAM)
			{
				ss << L"am";
			}
			else
			{
				ss << L"pm";
			}
		}
		else
		{
			// 处理其他字符直接拼接
			ss << wFormat[i];
		}
	}

	return CXXString(ss.str().c_str());
}

bool CXXTime::setHMS(__int64 h, __int64 m, __int64 s, __int64 ms)
{
	if (isValid(h, m, s, ms))
	{
		hours_ = h;
		minutes_ = m;
		seconds_ = s;
		milliseconds_ = ms;
		isNull_ = false;
		return true;
	}
	return false;
}

bool CXXTime::isValid() const
{
	return isValid(hours_, minutes_, seconds_, milliseconds_);
}

bool CXXTime::isNull() const
{
	return isNull_;
}

bool CXXTime::isValid(__int64 h, __int64 m, __int64 s, __int64 ms)
{
	return (h >= 0 && h < 24 && m >= 0 && m < 60 && s >= 0 && s < 60 && ms >= 0 && ms < 1000);
}

CXXTime CXXTime::currentTime()
{
	auto now = std::chrono::system_clock::now();
	auto currentMillisecs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 86400000;
	return fromMSecsSinceStartOfDay(currentMillisecs);
}

CXXTime CXXTime::fromMSecsSinceStartOfDay(__int64 msecs)
{
	__int64 h = msecs / 3600000LL;
	msecs %= 3600000LL;
	__int64 m = msecs / 60000LL;
	msecs %= 60000LL;
	__int64 s = msecs / 1000LL;
	__int64 ms = msecs % 1000LL;
	return CXXTime(h, m, s, ms);
}

CXXTime CXXTime::fromString(const CXXString& string, const CXXString& format)
{
	__int64 h = 0, m = 0, s = 0, ms = 0;
	bool isAM = false;
	bool hasAMPM = false;

	std::wstring wString = string.toStdWString();
	std::wstring wFormat = format.toStdWString();

	size_t strIndex = 0;
	for (size_t i = 0; i < wString.length(); ++i)
	{
		if (wFormat[i] == L'h')
		{
			if (i + 1 < wString.length() && wFormat[i + 1] == L'h')
			{
				h = std::wcstol(wFormat.substr(strIndex, 2).c_str(), nullptr, 10);
				strIndex += 2;
				++i;
			}
			else
			{
				h = std::wcstol(wFormat.substr(strIndex, 1).c_str(), nullptr, 10);
				strIndex += 1;
			}
		}
		else if (wString[i] == L'H')
		{
			if (i + 1 < wString.length() && wFormat[i + 1] == L'H')
			{
				h = std::wcstol(wString.substr(strIndex, 2).c_str(), nullptr, 10);
				strIndex += 2;
				++i;
			}
			else
			{
				h = std::wcstol(wString.substr(strIndex, 1).c_str(), nullptr, 10);
				strIndex += 1;
			}
		}
		else if (wString[i] == L'm')
		{
			if (i + 1 < wString.length() && wString[i + 1] == L'm')
			{
				m = std::wcstol(wString.substr(strIndex, 2).c_str(), nullptr, 10);
				strIndex += 2;
				++i;
			}
			else
			{
				m = std::wcstol(wString.substr(strIndex, 1).c_str(), nullptr, 10);
				strIndex += 1;
			}
		}
		else if (wString[i] == L's')
		{
			if (i + 1 < wString.length() && wString[i + 1] == L's')
			{
				s = std::wcstol(wString.substr(strIndex, 2).c_str(), nullptr, 10);
				strIndex += 2;
				++i;
			}
			else
			{
				s = std::wcstol(wString.substr(strIndex, 1).c_str(), nullptr, 10);
				strIndex += 1;
			}
		}
		else if (wString[i] == L'z')
		{
			if (i + 1 < wString.length() && wString[i + 1] == L'z')
			{
				ms = std::wcstol(wString.substr(strIndex, 3).c_str(), nullptr, 10);
				strIndex += 3;
				++i;
				if (i + 1 < wString.length() && wString[i + 1] == L'z')
				{
					++i;
				}
			}
			else
			{
				ms = std::wcstol(wString.substr(strIndex, 1).c_str(), nullptr, 10);
				strIndex += 1;
			}
		}
		else if (wString[i] == L'A')
		{
			if (wString.substr(strIndex, 2) == L"AM")
			{
				isAM = true;
				strIndex += 2;
			}
			else if (wString.substr(strIndex, 2) == L"PM")
			{
				isAM = false;
				strIndex += 2;
			}
			hasAMPM = true;
		}
		else if (wString[i] == L'a')
		{
			if (wString.substr(strIndex, 2) == L"am")
			{
				isAM = true;
				strIndex += 2;
			}
			else if (wString.substr(strIndex, 2) == L"pm")
			{
				isAM = false;
				strIndex += 2;
			}
			hasAMPM = true;
		}
		else
		{
			++strIndex; // 跳过非格式字符
		}
	}

	// 如果有 AM/PM 格式，则需要转换为 24 小时制
	if (hasAMPM)
	{
		if (isAM && h == 12)
		{
			h = 0;  // AM 12 点表示午夜 0 点
		}
		else if (!isAM && h < 12)
		{
			h += 12;  // PM 1-11 点表示 13-23 点
		}
	}

	return CXXTime(h, m, s, ms);
}

__int64 CXXTime::hours() const
{
	return hours_;
}

__int64 CXXTime::minutes() const
{
	return minutes_;
}

__int64 CXXTime::seconds() const
{
	return seconds_;
}

__int64 CXXTime::milliseconds() const
{
	return milliseconds_;
}

__int64 CXXTime::toMilliseconds() const
{
	return hours_ * 3600000LL + minutes_ * 60000LL + seconds_ * 1000LL + milliseconds_;
}