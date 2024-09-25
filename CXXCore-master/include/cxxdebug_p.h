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
#ifndef CXXDEBUG_P_H_
#define CXXDEBUG_P_H_

#include <cxxdebug.h>

#include <unordered_map>
#include <string>

class CXXDebugPrivate
{
public:
	explicit CXXDebugPrivate(CXXDebug* q, cxx::DebugOutputLevel flags);
	virtual ~CXXDebugPrivate();

	CXXDebug& maybeSpace();

	std::wstring getCurrentTimeString() const;

	void replacePrefixOrSuffix(const std::wstring& text, std::wstring& dst, bool forceNoColor = false) const;

	void replaceCustomFormat(std::wstring& text) const;

	bool replaceCustomColorFormat(std::wstring& text) const;

	void setConsoleTextColor(WORD color) const;

	static std::string::size_type findSubstring(const std::string& input, const std::string& search, std::string::size_type startPos, std::string::size_type* endPos);

	static bool subStringReplace(std::string& input, const std::string& search, const std::string& replace);

private:
	template<typename VT>
	std::wstring join(const CXXVector<VT>& other, const std::wstring& separator)
	{
		std::wostringstream ss;
		//get type name
		ss << L"CXXVector(";
		if (!other.isEmpty())
		{
			__int64 count = other.size();

			for (const VT& v : other)
			{
				if (std::is_floating_point<VT>::value)
				{
					ss << std::fixed << std::setprecision(std::numeric_limits<VT>::digits10 + 1) << v;
				}
				else if (std::is_same <VT, bool>::value)
				{
					ss << (v ? L"true" : L"false");
				}
				else if (std::is_integral<VT>::value)
				{
					ss << std::to_wstring(v);
				}
				else if (std::is_same<VT, wchar_t*>::value)
				{
					ss << v;
				}
				else
				{
					ss << v;
				}

				if (--count > 0)
				{
					ss << separator;
				}
			}
		}

		ss << L")";

		return ss.str();
	}



public:

private:
	static std::wstring replaceAll(const std::wstring& input, const std::wstring& search, const std::wstring& replace);
	static bool replaceAllToColor(const std::string& input, const std::string& search, const std::string& colorStr, std::string& output);

private:
	enum
	{
		ANSI,
		UTF8,
		UTF16
	};

	bool noQuote_ = false;
	bool noOutput_ = false;
	bool isRaw_ = false;

	std::wstring prefix_;
	std::wstring suffix_;
	std::wstring prefixOriginal_;
	std::wstring suffixOriginal_;

	std::wostringstream outputBuffer_;

	cxx::DebugOutputLevel flags_ = cxx::DebugOutputLevel::None;

	HANDLE hConsole_ = GetStdHandle(STD_OUTPUT_HANDLE);

	inline static constexpr WORD BACKGROUND_COLOR_MASK = 0x0070;

	size_t currentLine_ = 0;

	std::wstring currentFile_;

private:
	static CXXMessageHandler messageHandler_;
	static std::wstring prefixFormat_;
	static std::wstring suffixFormat_;
	static std::wstring timeFormat_;
	static std::wstring levelString_;
	static bool ansiColorEnabled_;
	static std::unordered_map<std::wstring, std::wstring> customFormatHash_;
	static std::unordered_map<std::wstring, std::string> customColorFormatHash_;
	static __int64 currentCodePage_;

private:
	CXX_DECLARE_PUBLIC(CXXDebug);
	CXXDebug* q_ptr = CXX_NULLPTR;
};

class _DateTimePrivate
{
public:
	enum Format
	{
		YEAR_FOUR_DIGITS, YEAR_TWO_DIGITS,
		MONTH_NUM,
		DAY_NUM,
		HOUR_24, HOUR_12,
		MINUTE,
		SECOND,
		MILLISECOND_THREE_DIGITS, MILLISECOND_TWO_DIGITS,
		AMPM,
		MONTH_SHORT_NAME, MONTH_FULL_NAME,
		DAY_SHORT_NAME, DAY_FULL_NAME
	};

	_DateTimePrivate()
	{
		now = std::chrono::system_clock::now();
		now_time_t = std::chrono::system_clock::to_time_t(now);
		localtime_s(&now_tm, &now_time_t);
		ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	}
	~_DateTimePrivate() = default;

	std::wstring format(const std::wstring& customFormat)
	{
		std::wstring output;
		for (size_t i = 0; i < customFormat.size(); ++i)
		{
			wchar_t ch = customFormat[i];
			Format formatToken = getFormatToken(ch, customFormat, i);
			if (formatToken != Format(-1))
			{
				output += formatValue(formatToken);
			}
			else
			{
				output += ch;
			}
		}
		return output;
	}

private:
	Format getFormatToken(wchar_t ch, const std::wstring& customFormat, size_t& index)
	{
		if (ch == L'y')
		{
			if (index + 3 < customFormat.size() && customFormat.substr(index, 4) == L"yyyy")
			{
				index += 3;
				return YEAR_FOUR_DIGITS;
			}
			else if (index + 1 < customFormat.size() && customFormat.substr(index, 2) == L"yy")
			{
				index += 1;
				return YEAR_TWO_DIGITS;
			}
		}
		else if (ch == L'M')
		{
			if (index + 2 < customFormat.size() && customFormat.substr(index, 3) == L"MMM")
			{
				if (index + 3 < customFormat.size() && customFormat[index + 3] == L'M')
				{
					index += 3;
					return MONTH_FULL_NAME;
				}
				else
				{
					index += 2;
					return MONTH_SHORT_NAME;
				}
			}
			else if (index + 1 < customFormat.size() && customFormat.substr(index, 2) == L"MM")
			{
				index += 1;
				return MONTH_NUM;
			}
		}
		else if (ch == L'd')
		{
			if (index + 2 < customFormat.size() && customFormat.substr(index, 3) == L"ddd")
			{
				if (index + 3 < customFormat.size() && customFormat[index + 3] == L'd')
				{
					index += 3;
					return DAY_FULL_NAME;
				}
				else
				{
					index += 2;
					return DAY_SHORT_NAME;
				}
			}
			else if (index + 1 < customFormat.size() && customFormat.substr(index, 2) == L"dd")
			{
				index += 1;
				return DAY_NUM;
			}
		}
		else if (ch == L'h')
		{
			if (index + 1 < customFormat.size() && customFormat.substr(index, 2) == L"hh")
			{
				index += 1;
				return HOUR_24;
			}
			else
			{
				return HOUR_12;
			}
		}
		else if (ch == L'm' && index + 1 < customFormat.size() && customFormat.substr(index, 2) == L"mm")
		{
			index += 1;
			return MINUTE;
		}
		else if (ch == L's' && index + 1 < customFormat.size() && customFormat.substr(index, 2) == L"ss")
		{
			index += 1;
			return SECOND;
		}
		else if (ch == L'z')
		{
			if (index + 2 < customFormat.size() && customFormat.substr(index, 3) == L"zzz")
			{
				index += 2;
				return MILLISECOND_THREE_DIGITS;
			}
			else if (index + 1 < customFormat.size() && customFormat.substr(index, 2) == L"zz")
			{
				index += 1;
				return MILLISECOND_TWO_DIGITS;
			}
		}
		else if (ch == L'a' && index + 1 < customFormat.size() && customFormat.substr(index, 2) == L"ap")
		{
			index += 1;
			return AMPM;
		}
		return Format(-1);
	}

	std::wstring formatValue(Format formatToken)
	{
		std::wstringstream wss;
		static DWORD acp = GetACP();
		static std::vector<std::wstring> dayShortNames;
		static const std::vector<std::wstring> dayFullNames = {
				L"星期日", L"星期一", L"星期二", L"星期三", L"星期四", L"星期五", L"星期六"
		};
		static const std::vector<std::wstring> monthShortNames = {
			L"一", L"二", L"三", L"四", L"五", L"六", L"七", L"八", L"九", L"十", L"十一", L"十二"
		};
		static const std::vector<std::wstring> monthFullNames = {
			L"一月", L"二月", L"三月", L"四月", L"五月", L"六月", L"七月", L"八月", L"九月", L"十月", L"十一月", L"十二月"
		};
		//big5
		if (acp == 950)
		{
			dayShortNames = {
				L"週日", L"週一", L"週二", L"週三", L"週四", L"週五", L"週六"
			};
		}
		//gb2312
		else if (acp == 936)
		{
			dayShortNames = {
				L"周日", L"周一", L"周二", L"周三", L"周四", L"周五", L"周六"
			};
		}

		switch (formatToken)
		{
		case YEAR_FOUR_DIGITS:
			wss << std::setw(4) << std::setfill(L'0') << (now_tm.tm_year + 1900);
			break;
		case YEAR_TWO_DIGITS:
			wss << std::setw(2) << std::setfill(L'0') << (now_tm.tm_year % 100);
			break;
		case MONTH_NUM:
			wss << std::setw(2) << std::setfill(L'0') << (now_tm.tm_mon + 1);
			break;
		case DAY_NUM:
			wss << std::setw(2) << std::setfill(L'0') << now_tm.tm_mday;
			break;
		case HOUR_24:
			wss << std::setw(2) << std::setfill(L'0') << now_tm.tm_hour;
			break;
		case HOUR_12:
			wss << std::setw(2) << std::setfill(L'0') << (now_tm.tm_hour % 12);
			break;
		case MINUTE:
			wss << std::setw(2) << std::setfill(L'0') << now_tm.tm_min;
			break;
		case SECOND:
			wss << std::setw(2) << std::setfill(L'0') << now_tm.tm_sec;
			break;
		case MILLISECOND_THREE_DIGITS:
			wss << std::setw(3) << std::setfill(L'0') << ms.count();
			break;
		case MILLISECOND_TWO_DIGITS:
			wss << std::setw(2) << std::setfill(L'0') << (ms.count() / 10);
			break;
		case AMPM:
			if ((acp == 950) || (acp == 936))
				wss << (now_tm.tm_hour < 12 ? L"上午" : L"下午");
			else
				wss << (now_tm.tm_hour < 12 ? L"AM" : L"PM");
			break;
		case MONTH_SHORT_NAME:
			if ((acp == 950) || (acp == 936))
				wss << monthShortNames.at(now_tm.tm_mon);
			else
				wss << std::put_time(&now_tm, L"%b");
			break;
		case MONTH_FULL_NAME:
			if ((acp == 950) || (acp == 936))
				wss << monthFullNames.at(now_tm.tm_mon);
			else
				wss << std::put_time(&now_tm, L"%B");
			break;
		case DAY_SHORT_NAME:
			if ((acp == 950) || (acp == 936))
				wss << dayShortNames.at(now_tm.tm_wday);
			else
				wss << std::put_time(&now_tm, L"%a");
			break;
		case DAY_FULL_NAME:
			if ((acp == 950) || (acp == 936))
				wss << dayFullNames.at(now_tm.tm_wday);
			else
				wss << std::put_time(&now_tm, L"%A");
			break;
		}
		return wss.str();
	}

	std::chrono::system_clock::time_point now;
	std::time_t now_time_t;
	std::tm now_tm;
	std::chrono::milliseconds ms;
};

#endif // CXXDEBUG_P_H_