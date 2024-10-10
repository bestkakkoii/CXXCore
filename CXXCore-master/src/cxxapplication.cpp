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

#include <cxxapplication_p.h>

#include <vector>
#include <algorithm>

CXXApplicationPrivate::CXXApplicationPrivate(CXXApplication* q)
	: q_ptr(q)
{

}

CXXApplicationPrivate::~CXXApplicationPrivate()
{

}

CXXApplication::CXXApplication()
	: d_ptr(new CXXApplicationPrivate(this))
{

}

CXXApplication::~CXXApplication()
{
}


//@备注 主动处理消息队列，用于强制处理消息
//@返回 无
//@别名 处理事件()
void CXXApplication::processEvents()
{
#if defined(CXX_OS_WIN)
	MSG message = {};
	while (PeekMessageW(&message, CXX_NULLPTR, NULL, NULL, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessageW(&message);

	}
#endif
}

//@备注 获取当前进程绝对路径
	//@返回 进程绝对路径
	//@别名 路径()
CXXString CXXApplication::filePath()
{
	constexpr const wchar_t* ENV_NAME = L"_CURRENT_PATH";
	bool init = false;
	if (!init)
	{
		WCHAR buffer[MAX_PATH] = {};
		DWORD length = GetModuleFileNameW(CXX_NULLPTR, buffer, MAX_PATH);
		if (length == 0)
			return L"";

		std::wstring path(buffer);
		if (static_cast<__int64>(path.size()) <= 0)
			return L"";

		if (path.find(L"/") != std::wstring::npos)
		{
			std::replace(path.begin(), path.end(), L'/', L'\\');
		}

		cxx::putenv(ENV_NAME, path.c_str());
		init = true;

		return path;
	}
	return cxx::getenv(ENV_NAME);
}

//@备注 获取当前进程所在目录
//@返回 进程所在目录
//@别名 目录()
CXXString CXXApplication::dirPath()
{
	constexpr const wchar_t* ENV_NAME = L"_CURRENT_DIR";
	bool init = false;
	if (!init)
	{
		// 取进程完整路径
		std::wstring path(CXXApplication::filePath().toStdWString());
		if (static_cast<__int64>(path.size()) <= 0)
			return L"";

		if (path.find(L"/") != std::wstring::npos)
		{
			std::replace(path.begin(), path.end(), L'/', L'\\');
		}

		// remove applicaion name
		size_t last = path.find_last_of(L"\\");
		if (last != std::wstring::npos)
		{
			path = path.substr(0, last);
		}

		cxx::putenv(ENV_NAME, path.c_str());

		init = true;

		return path;
	}

	return cxx::getenv(ENV_NAME);
}

//@备注 获取当前进程名称
//@返回 进程名称
//@别名 名称()
CXXString CXXApplication::name()
{
	constexpr const wchar_t* ENV_NAME = L"_CURRENT_NAME";
	bool init = false;
	if (!init)
	{
		// 取进程完整路径
		std::wstring path(CXXApplication::filePath().toStdWString());
		if (static_cast<__int64>(path.size()) <= 0)
			return L"";

		if (path.find(L"/") != std::wstring::npos)
		{
			std::replace(path.begin(), path.end(), L'/', L'\\');
		}

		// remove directory keep name
		size_t last = path.find_last_of(L"\\");
		if (last != std::wstring::npos)
		{
			path = path.substr(last + 1);
		}

		cxx::putenv(ENV_NAME, path.c_str());

		init = true;

		return path;
	}

	return cxx::getenv(ENV_NAME);
}

//@备注 获取当前进程版本
//@返回 进程版本
//@别名 版本()
CXXString CXXApplication::version()
{
	CXXString filePath = CXXApplication::filePath();

	DWORD handle = 0;
	DWORD size = GetFileVersionInfoSizeW(filePath.get(), &handle);
	if (size == 0)
	{
		return L"";  // Cannot find version info size
	}

	std::vector<BYTE> versionInfo(size);
	if (handle != 0 || !GetFileVersionInfoW(filePath.get(), NULL, size, versionInfo.data()))
	{
		return L"";  // Failed to get version info
	}

	void* versionData = CXX_NULLPTR;
	UINT versionLength = 0;
	if (!VerQueryValueW(versionInfo.data(), L"\\", &versionData, &versionLength))
	{
		return L"";  // Failed to query version value
	}

	if (versionData != CXX_NULLPTR)
	{
		VS_FIXEDFILEINFO* verInfo = static_cast<VS_FIXEDFILEINFO*>(versionData);
		if (verInfo->dwSignature == 0xfeef04bd)
		{
			int major = HIWORD(verInfo->dwFileVersionMS);
			int minor = LOWORD(verInfo->dwFileVersionMS);
			int build = HIWORD(verInfo->dwFileVersionLS);
			int revision = LOWORD(verInfo->dwFileVersionLS);
			return std::to_wstring(major) + L"." + std::to_wstring(minor) + L"." + std::to_wstring(build) + L"." + std::to_wstring(revision);
		}
	}

	return L"";  // No readable version information found
}
