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
#ifndef CXXFILE_P_H_
#define CXXFILE_P_H_

#include <cxxfile.h>

class CXXFilePrivate
{
public:
	explicit CXXFilePrivate(CXXFile* q);

	virtual ~CXXFilePrivate();

	CXXFilePrivate(CXXFile* q, const CXXString& path);

	CXXFilePrivate(CXXFile* q, const CXXString& path, cxx::FileOpenMode type, bool isShared);

	bool open(const CXXString& path, cxx::FileOpenMode type, bool isShared);

	static bool isValidFileName(const std::wstring& fileName);

private:
	CXXString path_;
	FILE* file_ = CXX_NULLPTR;
	cxx::FileOpenMode openMode_ = cxx::ReadWrite;
	std::vector<std::unique_ptr<BYTE[]>> mappedData_;

private:
	CXX_DECLARE_PUBLIC(CXXFile);
	CXXFile* q_ptr = CXX_NULLPTR;
};

#endif // CXXFILE_P_H_