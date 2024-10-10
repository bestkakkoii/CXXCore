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
#ifndef CXXLIBRARY_P_H_
#define CXXLIBRARY_P_H_

#include <cxxlibrary.h>
#include <cxxatomic.h>

#include <mutex>
#include <unordered_map>

class CXXLibraryPrivate
{
public:
	explicit CXXLibraryPrivate(CXXLibrary* q, const CXXString& pluginName);

	virtual ~CXXLibraryPrivate();

	FARPROC resolve(const CXXString& functionName);

	bool loadLibrary(CXXString libraryName);

	void clear();

	void setFlags(__int64 flags);

	void setHandle(HANDLE handle);

private:
	std::wstring libraryName_;
	std::wstring pluginName_;
	CXXAtomic<HMODULE> hModule_ = CXX_NULLPTR;
	CXXAtomicUInt32 lastError_ = 0;
	CXXAtomicBool unFreeAbled_ = true;

	std::mutex mutex_;
	std::unordered_map<std::wstring, FARPROC> usedFunctions_;

	__int64 flags_ = 0;
	HANDLE handle_ = CXX_NULLPTR;

private:
	CXX_DECLARE_PUBLIC(CXXLibrary);
	CXXLibrary* q_ptr = CXX_NULLPTR;
};


#endif // CXXLIBRARY_P_H_