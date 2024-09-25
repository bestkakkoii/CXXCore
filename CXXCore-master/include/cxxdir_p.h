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
#ifndef CXXDIR_P_H_
#define CXXDIR_P_H_

#include <cxxdir.h>
#include <cxxstring.h>

#include <shlobj_core.h>
#include <guiddef.h>

class CXXDirPrivate
{
public:
	explicit CXXDirPrivate(CXXDir* q);
	virtual ~CXXDirPrivate();

	void processPath(const CXXString& path);
	void updateFullPath();

	bool pathExists(const CXXString& path) const;

	bool createDirectory(const CXXString& path) const;

	static CXXString getSystemPath(const wchar_t* name, GUID u, bool& init);

public:

private:
	CXXString root_;
	CXXVector<CXXString> pathComponents_;
	CXXString fullPath_;

private:
	CXX_DECLARE_PUBLIC(CXXDir);
	CXXDir* q_ptr = CXX_NULLPTR;
};


#endif // CXXDIR_P_H_