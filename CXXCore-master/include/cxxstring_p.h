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
#ifndef CXXSTRING_P_H_
#define CXXSTRING_P_H_

#include <cxxstring.h>
#include <locale>
#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <bitset>
#include <sstream>
#include <cctype>

namespace cxx_private_string_tool
{
	template<typename E,
		typename TR = std::char_traits<E>,
		typename AL = std::allocator<E>>
		inline std::basic_string<E, TR, AL> toupper(const std::basic_string<E, TR, AL>& src)
	{
		auto dst = src;
		static const std::locale loc;
		std::transform(src.begin(), src.end(), dst.begin(), [&](E c)->E {return std::toupper(c, loc); });
		return dst;
	}

	template<typename E,
		typename TR = std::char_traits<E>,
		typename AL = std::allocator<E>>
		inline std::basic_string<E, TR, AL> tolower(const std::basic_string<E, TR, AL>& src)
	{
		auto dst = src;

		static const std::locale loc;
		std::transform(src.begin(), src.end(), dst.begin(), [&](E c)->E {return std::tolower(c, loc); });
		return dst;
	}

	template<typename E,
		typename TR = std::char_traits<E>,
		typename AL = std::allocator<E>>
		inline std::basic_string<E, TR, AL> trim(const std::basic_string<E, TR, AL>& src)
	{
		auto dst = src;
		constexpr const wchar_t* space = L" \t\n\v\f\r　";
		dst.erase(0, dst.find_first_not_of(space));
		dst.erase(dst.find_last_not_of(space) + 1);
		return dst;
	}

	template<typename E,
		typename TR = std::char_traits<E>,
		typename AL = std::allocator<E>>
		inline std::basic_string<E, TR, AL> simplified(const std::basic_string<E, TR, AL>& src)
	{
		auto dst = trim(src);
		std::basic_string<E, TR, AL> result;

		constexpr const wchar_t* space = L" \t\n\v\f\r　";
		bool in_space = false;

		for (const auto& ch : dst)
		{
			if (std::wcschr(space, ch))
			{
				if (!in_space)
				{
					result.push_back(ch);
					in_space = true;
				}
			}
			else
			{
				result.push_back(ch);
				in_space = false;
			}
		}

		return result;
	}
}

class CXXStringPrivate
{
public:
	CXXStringPrivate(CXXString* q, const std::wstring& d);
	virtual ~CXXStringPrivate();

	std::wstring mapString(ULONG flags) const;

	template <typename T>
	T toNumber(bool* ok, __int64 base) const
	{
		T n = 0;
		if (ok)
		{
			*ok = false;
		}

		if ((__int64)data_.size() <= 0)
		{
			return n;
		}
		if (base < 2 || base > 36)
		{
			return n;
		}

		std::wstring s = cxx_private_string_tool::tolower(data_);

		if (s[0] == L'0')
		{
			if (s.size() > 1)
			{
				if (s[1] == L'x')
				{
					s.erase(0, 2);
					base = 16;
				}
				else if (s[1] == L'b')
				{
					s.erase(0, 2);
					base = 2;
				}
				else if (s[1] == L'o')
				{
					s.erase(0, 2);
					base = 8;
				}
			}
		}

		try
		{
			n = static_cast<T>(std::stoll(s, CXX_NULLPTR, static_cast<int>(base)));
			if (ok)
			{
				*ok = true;
			}
		}
		catch (...)
		{
			if (ok)
			{
				*ok = false;
			}
		}
		return n;
	}

	float toFloat(bool* ok) const
	{
		float n = 0.0f;
		if (data_.size() <= 0)
		{
			return n;
		}

		std::wstring s = cxx_private_string_tool::tolower(data_);

		try
		{
			n = std::stof(s);
			if (ok)
			{
				*ok = true;
			}
		}
		catch (...)
		{
			n = 0.0f;
		}
		return n;

	}

	double toDouble(bool* ok) const
	{
		double n = 0.0;
		if (data_.size() <= 0)
		{
			return n;
		}

		std::wstring s = cxx_private_string_tool::tolower(data_);

		try
		{
			n = std::stod(s);
			if (ok)
			{
				*ok = true;
			}
		}
		catch (...)
		{
			n = 0.0;
		}
		return n;
	}

	static std::string base64Encode(const BYTE* data, size_t len);

	static std::vector<BYTE> base64Decode(const std::string& encoded);

private:
	static inline constexpr const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";

	std::wstring data_;

private:
	CXX_DECLARE_PUBLIC(CXXString);
	CXXString* q_ptr = CXX_NULLPTR;
};



#endif // CXXSTRING_P_H_
