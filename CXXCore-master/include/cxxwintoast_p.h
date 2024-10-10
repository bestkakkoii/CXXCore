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
#ifndef CXXWINTOAST_P_H_
#define CXXWINTOAST_P_H_

#include <cxxwintoast.h>

class CXXWinToastPrivate
{
public:
	explicit CXXWinToastPrivate(CXXWinToast* q);
	CXXWinToastPrivate(CXXWinToast* q, cxx::WinToastType type);
	virtual ~CXXWinToastPrivate();

private:
	WinToastLib::WinToastTemplate templ_;
	bool isInlineImage_ = false;
	cxx::WinToastType type_ = cxx::WinToastType::Text01;

private:
	CXX_DECLARE_PUBLIC(CXXWinToast);
	CXXWinToast* q_ptr = CXX_NULLPTR;
};

#endif // CXXWINTOAST_P_H_