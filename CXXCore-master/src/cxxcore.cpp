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



class CXXAnnounce
{
public:
	CXXAnnounce()
	{
		static bool init = false;
		if (init)
		{
			return;
		}

		init = true;
		CXXDebug::setCodePage();

		std::wstring out = LR"(＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
　　　　　　　CXXModule made by Fei (QQ: 1642687802)
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝)";
		out += L"\r\n";

		LPCWSTR wstr = out.c_str();

#ifndef CXX_NO_ANNOUNCE
		OutputDebugStringW(wstr);
#endif

	}

	virtual ~CXXAnnounce() {}
private:
	CXX_DISABLE_COPY_MOVE(CXXAnnounce);
};

CXXAnnounce _cxxannounce = CXXAnnounce();