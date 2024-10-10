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

#include <cxxtextcodec_p.h>
#include <cxxstring_p.h>

namespace
{
	static std::unordered_map<__int64, std::wstring> _PRIVATE_CODEPAGE_MAP = {
		{US_ASCII, L"US-ASCII"},
		{ISO_8859_1, L"ISO-8859-1"},
		{ISO_8859_2, L"ISO-8859-2"},
		{ISO_8859_3, L"ISO-8859-3"},
		{ISO_8859_4, L"ISO-8859-4"},
		{ISO_8859_5, L"ISO-8859-5"},
		{ISO_8859_6, L"ISO-8859-6"},
		{ISO_8859_7, L"ISO-8859-7"},
		{ISO_8859_8, L"ISO-8859-8"},
		{ISO_8859_9, L"ISO-8859-9"},
		{ISO_8859_10, L"ISO-8859-10"},
		{SHIFT_JIS, L"SHIFT-JIS"},
		{EUC_JP, L"EUC-JP"},
		{EUC_KR, L"EUC-KR"},
		{ISO_2022_KR, L"ISO-2022-KR"},
		{ISO_2022_JP, L"ISO-2022-JP"},
		{ISO_2022_JP_2, L"ISO-2022-JP-2"},
		{GB2312, L"GB2312"},
		{UTF_8, L"UTF-8"},
		{GBK, L"GBK"},
		{GB18030, L"GB18030"},
		{BIG5, L"BIG5"},
		{CP851, L"CP851"},
	};
}

std::unordered_map<__int64, std::atomic<void*>> CXXTextCodecPrivate::instances_;
std::mutex CXXTextCodecPrivate::mutex_;

CXXTextCodecPrivate::CXXTextCodecPrivate(CXXTextCodec* q, __int64 acp)
	: q_ptr(q)
	, acp_(acp)
{

}

CXXTextCodecPrivate::~CXXTextCodecPrivate()
{

}

CXXTextCodec::CXXTextCodec()
	: d_ptr(new CXXTextCodecPrivate(this, ::GetACP()))
{

}

CXXTextCodec::CXXTextCodec(__int64 acp)
	: d_ptr(new CXXTextCodecPrivate(this, acp))
{

}

CXXTextCodec* CXXTextCodec::codecFromACP(__int64 acp)
{
	CXXTextCodec* instance = reinterpret_cast<CXXTextCodec*>(CXXTextCodecPrivate::instances_[acp].load(std::memory_order_acquire));
	if (instance == CXX_NULLPTR)
	{
		std::lock_guard<std::mutex> lock(CXXTextCodecPrivate::mutex_);
		instance = reinterpret_cast<CXXTextCodec*>(CXXTextCodecPrivate::instances_[acp].load(std::memory_order_relaxed));
		if (instance == CXX_NULLPTR)
		{
			instance = new CXXTextCodec(acp);
			CXXTextCodecPrivate::instances_[acp].store(instance, std::memory_order_release);
		}
	}

	return instance;
}

CXXTextCodec* CXXTextCodec::codecFromName(const CXXString& name)
{
	std::wstring upperName(name.data());
	upperName = cxx_private_string_tool::toupper(upperName);
	for (const auto& pair : _PRIVATE_CODEPAGE_MAP)
	{
		if (upperName == cxx_private_string_tool::toupper(pair.second))
		{
			return codecFromACP(pair.first);
		}
	}

	return CXX_NULLPTR;
}

CXXString CXXTextCodec::toUnicode(const std::string& input) const
{
	CXX_D(const CXXTextCodec);

	int requiredSize = MultiByteToWideChar(static_cast<UINT>(d->acp_), 0, reinterpret_cast<const LPCCH>(input.c_str()), -1, CXX_NULLPTR, 0);
	if (requiredSize > 0)
	{
		std::wstring result(requiredSize, L'\0');
		if (MultiByteToWideChar(static_cast<UINT>(d->acp_), 0, reinterpret_cast<const LPCCH>(input.c_str()), -1, &result[0], requiredSize) > 0)
		{
			result.resize(static_cast<size_t>(requiredSize - 1));
			return result;
		}
	}

	return L"";
}

std::string CXXTextCodec::fromUnicode(const CXXString& input) const
{
	CXX_D(const CXXTextCodec);
	std::string result;

	int requiredSize = WideCharToMultiByte(static_cast<UINT>(d->acp_), 0, input.data(), -1, CXX_NULLPTR, 0, CXX_NULLPTR, CXX_NULLPTR);
	if (requiredSize > 0)
	{
		result = std::string(requiredSize, '\0');
		if (WideCharToMultiByte(static_cast<UINT>(d->acp_), 0, input.data(), -1, &result[0], requiredSize, CXX_NULLPTR, CXX_NULLPTR) > 0)
		{
			result.resize(static_cast<size_t>(requiredSize - 1));
			return result;
		}
	}

	return result;
}

CXXString CXXTextCodec::mapString(UINT dwMapFlags, const CXXString& input) const
{
	CXX_D(const CXXTextCodec);

	//lang
	DWORD lang;
	DWORD sublang;
	switch (d->acp_)
	{
	case US_ASCII:
	{
		lang = LANG_ENGLISH;
		sublang = SUBLANG_ENGLISH_US;
		break;
	}
	case ISO_8859_1:
	{
		lang = LANG_ENGLISH;
		sublang = SUBLANG_ENGLISH_US;
		break;
	}
	case ISO_8859_2:
	{
		lang = LANG_POLISH;
		sublang = SUBLANG_POLISH_POLAND;
		break;
	}
	case ISO_8859_3:
	{
		lang = LANG_ITALIAN;
		sublang = SUBLANG_ITALIAN;
		break;
	}
	case ISO_8859_4:
	{
		lang = LANG_LATVIAN;
		sublang = SUBLANG_LATVIAN_LATVIA;
		break;
	}
	case ISO_8859_5:
	{
		lang = LANG_RUSSIAN;
		sublang = SUBLANG_RUSSIAN_RUSSIA;
		break;
	}
	case ISO_8859_6:
	{
		lang = LANG_ARABIC;
		sublang = SUBLANG_ARABIC_SAUDI_ARABIA;
		break;
	}
	case ISO_8859_7:
	{
		lang = LANG_GREEK;
		sublang = SUBLANG_GREEK_GREECE;
		break;
	}
	case ISO_8859_9:
	{
		lang = LANG_TURKISH;
		sublang = SUBLANG_TURKISH_TURKEY;
		break;
	}
	case ISO_8859_10:
	{
		lang = LANG_NORWEGIAN;
		sublang = SUBLANG_NORWEGIAN_BOKMAL;
		break;
	}
	case SHIFT_JIS:
	{
		lang = LANG_JAPANESE;
		sublang = SUBLANG_JAPANESE_JAPAN;
		break;
	}
	case EUC_JP:
	{
		lang = LANG_JAPANESE;
		sublang = SUBLANG_JAPANESE_JAPAN;
		break;
	}
	case EUC_KR:
	{
		lang = LANG_KOREAN;
		sublang = SUBLANG_KOREAN;
		break;
	}
	case ISO_2022_KR:
	{
		lang = LANG_KOREAN;
		sublang = SUBLANG_KOREAN;
		break;
	}
	case ISO_2022_JP:
	{
		lang = LANG_JAPANESE;
		sublang = SUBLANG_JAPANESE_JAPAN;
		break;
	}
	case ISO_2022_JP_2:
	{
		lang = LANG_JAPANESE;
		sublang = SUBLANG_JAPANESE_JAPAN;
		break;
	}
	case GB2312:
	{
		lang = LANG_CHINESE;
		sublang = SUBLANG_CHINESE_SIMPLIFIED;
		break;
	}
	case UTF_8:
	{
		lang = LANG_ENGLISH;
		sublang = SUBLANG_ENGLISH_US;
		break;
	}
	case GB18030:
	{
		lang = LANG_CHINESE;
		sublang = SUBLANG_CHINESE_SIMPLIFIED;
		break;
	}
	case BIG5:
	{
		lang = LANG_CHINESE;
		sublang = SUBLANG_CHINESE_TRADITIONAL;
		break;
	}
	case CP851:
	{
		lang = LANG_ENGLISH;
		sublang = SUBLANG_ENGLISH_US;
		break;
	}
	default:
	{
		lang = LANG_NEUTRAL;
		sublang = SUBLANG_NEUTRAL;
		break;
	}
	}

	auto  lgid = MAKELANGID(lang, sublang);
	auto srtid = SORT_DEFAULT;
	LCID lcid = MAKELCID(lgid, srtid);

	int size = lstrlenW(input.c_str());
	std::wstring result(size, L'\0');
	std::ignore = LCMapStringW(lcid, dwMapFlags, input.c_str(), size, &result[0], size);

	return std::move(result);
}
