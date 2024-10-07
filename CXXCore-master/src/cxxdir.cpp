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

#include <cxxdir_p.h>

#include <algorithm>
#include <sstream>

CXXDirPrivate::CXXDirPrivate(CXXDir* q)
	: q_ptr(q)
{
}

CXXDirPrivate::~CXXDirPrivate()
{
}

bool CXXDirPrivate::pathExists(const CXXString& path) const
{
	return GetFileAttributesW(reinterpret_cast<LPCWSTR>(path.toStdWString().c_str())) != INVALID_FILE_ATTRIBUTES;
}

bool CXXDirPrivate::createDirectory(const CXXString& path) const
{
	return CreateDirectoryW(reinterpret_cast<LPCWSTR>(path.toStdWString().c_str()), nullptr);
}

void CXXDirPrivate::processPath(const CXXString& path)
{
	// Handle converting to absolute path if needed and split into components
	wchar_t absolutePath[MAX_PATH];
	GetFullPathNameW(reinterpret_cast<LPCWSTR>(path.toStdWString().c_str()), MAX_PATH, absolutePath, nullptr);
	CXXString absPath(absolutePath);

	// Split into components and store them
	pathComponents_.clear();
	std::wstringstream ss(absPath.toStdWString());
	CXXString component;
	while (std::getline(ss, component.toStdWString(), L'\\'))
	{
		if (!component.empty())
		{
			pathComponents_.push_back(component);
		}
	}

	// Set the root to the drive (e.g., C:) and the first component
	if (!pathComponents_.empty())
	{
		root_ = pathComponents_[0] + L"\\";
		pathComponents_.erase(pathComponents_.begin()); // Remove the root from components
	}

	updateFullPath();
}

void CXXDirPrivate::updateFullPath()
{
	fullPath_ = root_;
	for (const auto& component : pathComponents_)
	{
		fullPath_ += component + L"\\";
	}
}

CXXString CXXDirPrivate::getSystemPath(const wchar_t* name, GUID u, bool& init)
{
	if (!init)
	{
		WCHAR buffer[MAX_PATH] = {};
		PWSTR path = CXX_NULLPTR;
		if (SUCCEEDED(SHGetKnownFolderPath(u, 0, CXX_NULLPTR, &path)))
		{
			wcscpy_s(buffer, path);
			CoTaskMemFree(path);
		}

		std::wstring startup(buffer);
		if (static_cast<__int64>(startup.size()) <= 0)
		{
			return L"";
		}

		if (startup.find(L"/") != std::wstring::npos)
		{
			std::replace(startup.begin(), startup.end(), L'/', L'\\');
		}

		cxx::putenv(name, startup.c_str());

		init = true;

		return startup;
	}

	return 	cxx::getenv(name);
}

CXXDir::CXXDir(const CXXString& path)
	: d_ptr(new CXXDirPrivate(this))
{
	CXX_D(CXXDir);
	d->processPath(path);
}

CXXDir::~CXXDir()
{
}

CXXString CXXDir::absoluteFilePath(const CXXString& fileName) const
{
	CXX_D(const CXXDir);
	return d->fullPath_ + fileName;
}

CXXString CXXDir::absolutePath() const
{
	CXX_D(const CXXDir);
	return d->fullPath_;
}

CXXString CXXDir::canonicalPath() const
{
	CXX_D(const CXXDir);
	HANDLE hFile = CreateFileW(reinterpret_cast<LPCWSTR>(d->fullPath_.toStdWString().c_str()),
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return CXXString(); // Failed to open file/directory
	}

	wchar_t canonicalPath[MAX_PATH] = {};
	DWORD result = GetFinalPathNameByHandleW(hFile, canonicalPath, MAX_PATH, FILE_NAME_NORMALIZED);
	MINT::NtClose(hFile);

	if (result > 0 && result < MAX_PATH)
	{
		return CXXString(canonicalPath);
	}

	return CXXString(); // Error handling
}

bool CXXDir::cd(const CXXString& dirName)
{
	CXX_D(CXXDir);
	d->processPath(dirName); // Change to the new path
	return true;
}

bool CXXDir::cdUp()
{
	CXX_D(CXXDir);
	if (!d->pathComponents_.empty())
	{
		d->pathComponents_.pop_back(); // Remove the last directory component
		d->updateFullPath();
		return true;
	}
	return false; // Already at the root
}

CXXString CXXDir::name() const
{
	CXX_D(const CXXDir);
	return d->pathComponents_.empty() ? d->root_ : d->pathComponents_.back();
}

bool CXXDir::isReadable() const
{
	CXX_D(const CXXDir);
	return _waccess(reinterpret_cast<LPCWSTR>(d->fullPath_.toStdWString().c_str()), 04) == 0;
}

bool CXXDir::isRoot() const
{
	CXX_D(const CXXDir);
	return d->pathComponents_.empty();
}

bool CXXDir::make(const CXXString& path)
{
	CXX_D(CXXDir);
	return d->createDirectory(path);
}

bool CXXDir::remove()
{
	CXX_D(CXXDir);
	return RemoveDirectoryW(reinterpret_cast<LPCWSTR>(d->fullPath_.toStdWString().c_str())) != 0;
}

bool CXXDir::rename(const CXXString& newName)
{
	CXX_D(CXXDir);
	bool ret = MoveFileW(reinterpret_cast<LPCWSTR>(d->fullPath_.toStdWString().c_str()), reinterpret_cast<LPCWSTR>(newName.toStdWString().c_str())) != 0;
	if (ret)
	{
		d->processPath(newName);
	}

	return ret;
}

bool CXXDir::operator!=(const CXXDir& dir) const
{
	CXX_D(const CXXDir);
	return d->fullPath_ != dir.d_ptr->fullPath_;
}

CXXDir& CXXDir::operator=(const CXXDir& dir)
{
	if (this != &dir)
	{
		CXX_D(CXXDir);
		d->processPath(dir.d_ptr->fullPath_);
	}
	return *this;
}

CXXDir& CXXDir::operator=(CXXDir&& other)
{
	if (this != &other)
	{
		CXX_D(CXXDir);
		d->processPath(std::move(other.d_ptr->fullPath_));
	}
	return *this;
}

bool CXXDir::operator==(const CXXDir& dir) const
{
	return !(*this != dir);
}

//@备注 获取系统暂存目录
//@返回 临时目录
//@别名 取暂存目录()
CXXString CXXDir::tempPath()
{
	constexpr const wchar_t* ENV_NAME = L"_TEMP_PATH";
	static bool init = false;
	if (!init)
	{
		WCHAR buffer[MAX_PATH] = {};
		DWORD length = GetTempPathW(MAX_PATH, buffer);
		if (length == 0)
		{
			return L"";
		}

		std::wstring path(buffer);
		if (static_cast<__int64>(path.size()) <= 0)
		{
			return L"";
		}

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

//@备注 获取AppData
//@返回 AppData目录
//@别名 roamingAppData()
CXXString CXXDir::roamingAppDataPath()
{
	static bool init = false;
	return CXXDirPrivate::getSystemPath(L"_ROAMING_APPDATA_PATH", FOLDERID_RoamingAppData, init);
}

//@备注 获取LocalAppData
//@返回 LocalAppData目录
//@别名 localAppData()
CXXString CXXDir::localAppDataPath()
{
	static bool init = false;
	return CXXDirPrivate::getSystemPath(L"_LOCAL_APPDATA_PATH", FOLDERID_LocalAppData, init);
}

//@备注 取桌面目录
//@返回 桌面目录
//@别名 桌面目录()
CXXString CXXDir::desktopPath()
{
	static bool init = false;
	return CXXDirPrivate::getSystemPath(L"_DESKTOP_PATH", FOLDERID_Desktop, init);
}

//@备注 取启动程序目录
//@返回 启动程序目录
//@别名 取启动程序目录()
CXXString CXXDir::startupPath()
{
	static bool init = false;
	return CXXDirPrivate::getSystemPath(L"_STARTUP_PATH", FOLDERID_Startup, init);
}

bool CXXDir::mkpath(const CXXString& path)
{
	if (CreateDirectoryW(path.c_str(), CXX_NULLPTR))
	{
		return true;
	}
	else
	{
		DWORD errorCode = GetLastError();
		if (errorCode == ERROR_ALREADY_EXISTS)
		{
			return true;
		}
		else if (errorCode == ERROR_PATH_NOT_FOUND)
		{
			std::wstring wpath = path.c_str();
			size_t pos = wpath.find_last_of(L"\\/");
			if (pos != std::wstring::npos)
			{
				std::wstring parentPath = wpath.substr(0, pos);
				if (static_cast<__int64>(parentPath.size()) > 0 && parentPath != wpath)
				{
					if (mkpath(parentPath))
					{
						return CreateDirectoryW(wpath.c_str(), CXX_NULLPTR) != 0;
					}
				}
			}
		}
	}

	return false;
}

bool CXXDir::remove(const CXXString& path)
{
	if (RemoveDirectoryW(path.c_str()))
	{
		return true;
	}
	else
	{
		DWORD errorCode = GetLastError();
		if (errorCode == ERROR_FILE_NOT_FOUND)
		{
			return true;
		}
	}

	return false;
}

bool CXXDir::open(const CXXString& path)
{
	HINSTANCE hInstance = ShellExecuteW(CXX_NULLPTR, L"open", path.c_str(), CXX_NULLPTR, CXX_NULLPTR, SW_SHOWNORMAL);

	return reinterpret_cast<INT_PTR>(hInstance) > 32;
}

bool CXXDir::setCurrentDir(const CXXString& path)
{
	return SetCurrentDirectoryW(path.c_str()) != 0;
}

CXXString CXXDir::currentDir()
{
	wchar_t buffer[MAX_PATH] = {};
	GetCurrentDirectoryW(MAX_PATH, buffer);
	return CXXString(buffer);
}

bool CXXDir::rename(const CXXString& oldName, const CXXString& newName)
{
	return MoveFileW(oldName.c_str(), newName.c_str()) != 0;
}