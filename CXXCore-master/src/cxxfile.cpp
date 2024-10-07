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

#include <cxxfile_p.h>

#pragma comment(lib, "shlwapi.lib")

CXXFilePrivate::CXXFilePrivate(CXXFile* q)
	: q_ptr(q)
{
}

CXXFilePrivate::~CXXFilePrivate()
{
}

CXXFilePrivate::CXXFilePrivate(CXXFile* q, const CXXString& path)
	: q_ptr(q)
	, path_(path)
{
}

CXXFilePrivate::CXXFilePrivate(CXXFile* q, const CXXString& path, cxx::FileOpenMode type, bool isShared)
	: q_ptr(q)
	, path_(path)
{
	open(path, type, isShared);
}

bool CXXFilePrivate::isValidFileName(const std::wstring& fileName)
{
	static const std::wregex pattern(L"^[^\\\\/:*?\"<>|]*$");
	return std::regex_match(fileName, pattern);
}

bool CXXFilePrivate::open(const CXXString& path, cxx::FileOpenMode type, bool isShared)
{
	if (file_ != CXX_NULLPTR)
	{
		return false;
	}

	path_ = path;
	std::replace(path_.begin(), path_.end(), L'/', L'\\');

	// 文件不存在时失败
	if (type & cxx::Nocreate)
	{
		if (!CXXFile::exists(path_))
		{
			return false; // 文件不存在，返回失败
		}
	}

	// 文件存在时失败
	if (type & cxx::Noreplace)
	{
		if (CXXFile::exists(path_))
		{
			return false; // 文件存在，返回失败
		}
	}

	/*
	enum FileOpenModeFlag
	{
		Read = 0x1,       //@别名 读取
		Write = 0x2,      //@别名  写入
		ReadWrite = 0x3,  //@别名  读写
		Ate = 0x4,        //@别名  从文件尾
		Append = 0x8,     //@别名  附加文件尾
		Truncate = 0x10,  //@别名  清空文件
		ReadWriteTruncate = 0x13, //@别名  读写清空
		Binary = 0x20,    //@别名  二进制
		ReadWriteBinary = 0x23,   //@别名  读写二进制
		Nocreate = 0x40,  //@别名  文件不存在时失败
		Noreplace = 0x80, //@别名  文件存在时失败
	};

	"r"	Opens for reading. If the file doesn't exist or can't be found, the _fsopen call fails.
	"w"	Opens an empty file for writing. If the given file exists, its contents are destroyed.
	"a"	Opens for writing at the end of the file (appending); creates the file first if it doesn't exist.
	"r+"	Opens for both reading and writing. (The file must exist.)
	"w+"	Opens an empty file for both reading and writing. If the given file exists, its contents are destroyed.
	"a+"	Opens for reading and appending; creates the file first if it doesn't exist.

	Use the "w" and "w+" types with care, as they can destroy existing files.

	When a file is opened with the "a" or "a+" access type,
	all write operations occur at the end of the file.
	The file pointer can be repositioned using fseek or rewind,
	but it's always moved back to the end of the file before any write operation
	is carried out. Thus, existing data can't be overwritten.
	When the "r+", "w+", or "a+" access type is specified,
	both reading and writing are allowed (the file is said to be open for update).
	However, when switching between reading and writing,
	there must be an intervening fsetpos, fseek, or rewind operation.
	The current position can be specified for the fsetpos or fseek operation,
	if desired. In addition to the above values, one of the following characters
	can be included in mode to specify the translation mode for new lines,
	and for file management.

	t	Opens a file in text (translated) mode. In this mode, carriage return-line feed (CR-LF)
	combinations are translated into single line feeds (LF) on input and LF characters are translated to CR-LF
	combinations on output. Also, CTRL+Z is interpreted as an end-of-file character on input. In files opened
	for reading or reading/writing, _fsopen checks for a CTRL+Z at the end of the file and removes it,
	if possible. It's removed because using fseek and ftell to move within a file that ends
	with a CTRL+Z might cause fseek to behave improperly near the end of the file.

	b	Opens a file in binary (untranslated) mode; the above translations are suppressed.
	D	Specifies a temporary file that's deleted when the last file pointer to it is closed.
	R	Specifies that caching is optimized for, but not restricted to, random access from disk.
	S	Specifies that caching is optimized for, but not restricted to, sequential access from disk.
	T	Specifies a file that isn't written to disk unless memory pressure requires it.

	If t or b isn't given in mode, the translation mode is defined by
	the default-mode variable _fmode. If t or b is prefixed to the argument,
	the function fails and returns NULL.
	For a discussion of text and binary modes, see Text and binary mode file I/O.

	Regarding T and D:

	T avoids writing the file to disk as long as memory pressure doesn't require it.
	For more information, see FILE_ATTRIBUTE_TEMPORARY in File attribute constants,
	and also this blog post It's only temporary.

	D specifies a regular file that is written to disk.
	The difference is that it's automatically deleted when it's closed.
	You can combine TD to get both semantics.
	*/

	CXXString mode;
	// 根据type构建打开模式字符串
	if (type & cxx::ReadWrite)
	{
		mode = (type & cxx::Truncate) ? L"w+" : L"r+"; // 读写模式，如果有清空标志则使用w+
	}
	else if (type & cxx::Read)
	{
		mode = L"r"; // 只读
	}
	else if (type & cxx::Write)
	{
		mode = (type & cxx::Append) ? L"a" : L"w"; // 写入模式，如果有附加标志则使用a
	}

	// 是否使用二进制模式
	if (type & cxx::Binary)
	{
		mode += L"b";
	}

	if (isShared)
	{
		// 如果允许共享文件，则使用 _fsopen，允许读写共享
		file_ = _fsopen(path_.toUtf8().c_str(), mode.toUtf8().c_str(), _SH_DENYNO);
	}
	else
	{
		// 如果不允许共享文件，使用 fopen_s
		if (fopen_s(&file_, path_.toUtf8().c_str(), mode.toUtf8().c_str()) != 0)
		{
			file_ = nullptr;
			return false;
		}
	}

	if (type & cxx::Ate)
	{
		// 将指针移动到末尾
		_fseeki64(file_, 0, SEEK_END);
	}

	openMode_ = type;
	return true;
}

CXXFile::CXXFile()
{
};

CXXFile::CXXFile(const CXXString& path)
	: d_ptr(new CXXFilePrivate(this, path))
{
	std::replace(d_ptr->path_.begin(), d_ptr->path_.end(), L'/', L'\\');
}

CXXFile::CXXFile(const CXXString& path, cxx::FileOpenMode type, bool isShared)
	: d_ptr(new CXXFilePrivate(this, path, type, isShared))
{
}

CXXFile::~CXXFile()
{
	close();

	{
		std::lock_guard<std::mutex> lock(d_ptr->mutex_);
		d_ptr->mappedData_.clear();
	}
}

bool CXXFile::open(cxx::FileOpenMode type, bool isShared)
{
	return d_ptr->open(d_ptr->path_, type, isShared);
}

bool CXXFile::open(const CXXString& path, cxx::FileOpenMode type, bool isShared)
{
	return d_ptr->open(path, type, isShared);
}

bool CXXFile::isOpen() const
{
	return  d_ptr->file_ != CXX_NULLPTR;
}

bool CXXFile::exists() const
{
	if (d_ptr->path_.size() <= 0)
	{
		return false;
	}

	bool exist = false;
	if (d_ptr->path_.size() > 0)
	{
		std::wifstream wfile(d_ptr->path_);
		exist = wfile.good();
		wfile.close();
		return exist;
	}

	return exist;
}

CXXString CXXFile::fileName()
{
	__int64 slashIndex = d_ptr->path_.toStdWString().find_last_of(L"\\/");
	if (slashIndex != std::wstring::npos)
	{
		std::wstring fileName = d_ptr->path_.toStdWString().substr(static_cast<size_t>(slashIndex) + 1);
		if (CXXFilePrivate::isValidFileName(fileName))
		{
			return fileName;
		}
	}
	return L"";
}

CXXString CXXFile::directory() const
{
	auto getFullPath = [](const std::wstring& path)->std::wstring
		{
			wchar_t fullPath[MAX_PATH] = { 0 };
			if (GetFullPathNameW(path.c_str(), MAX_PATH, fullPath, CXX_NULLPTR) != 0)
			{
				return std::wstring(fullPath);
			}
			return L"";
		};

	std::wstring fullPath = getFullPath(d_ptr->path_.toStdWString());
	__int64 lastSlashIndex = fullPath.find_last_of(L"\\/");
	if (lastSlashIndex != std::wstring::npos)
	{
		return fullPath.substr(0, static_cast<size_t>(lastSlashIndex));
	}
	return L"";
}

bool CXXFile::rename(const CXXString& newpath)
{
	if (static_cast<__int64>(d_ptr->path_.size()) <= 0)
	{
		return false;
	}
	if (isOpen())
	{
		return false;
	}
	bool ret = CXXFile::rename(d_ptr->path_, newpath);
	if (ret)
	{
		d_ptr->path_ = newpath;
	}

	return ret;
}

bool CXXFile::remove()
{
	if (d_ptr->path_.size() <= 0)
	{
		return false;
	}
	if (isOpen())
	{
		return false;
	}

	return CXXFile::remove(d_ptr->path_);
}

bool CXXFile::copy(const std::wstring& newpath)
{
	if (static_cast<__int64>(d_ptr->path_.size()) <= 0)
	{
		return false;
	}
	if (isOpen())
	{
		return false;
	}
	return CXXFile::copy(d_ptr->path_, newpath);
}

bool CXXFile::link(const CXXString& newpath)
{
	if (static_cast<__int64>(d_ptr->path_.size()) <= 0)
	{
		return false;
	}
	if (isOpen())
	{
		return false;
	}
	return CXXFile::link(d_ptr->path_, newpath);
}

bool CXXFile::isExecutable() const
{
	if (d_ptr->path_.size() > 0)
	{
		return CXXFile::isExecutable(d_ptr->path_);
	}
	return false;
}

bool CXXFile::isFile() const
{
	if (d_ptr->path_.size() > 0)
	{
		return CXXFile::isFile(d_ptr->path_);
	}

	return false;
}

bool CXXFile::isDir() const
{
	if (d_ptr->path_.size() > 0)
	{
		return CXXFile::isDir(d_ptr->path_);
	}

	return false;
}

CXXString CXXFile::suffix() const
{
	if (static_cast<__int64>(d_ptr->path_.size()) > 0)
	{
		return CXXFile::suffix(d_ptr->path_);
	}
	return L"";
}

bool CXXFile::isValid() const
{
	if (d_ptr->path_.size() <= 0)
	{
		return false;
	}

	if (isOpen())
	{
		return true;
	}
	else
	{
		return exists();
	}
}

void CXXFile::flush()
{
	if (isOpen())
	{
		fflush(d_ptr->file_);
	}
}

bool CXXFile::close()
{
	if (d_ptr->file_ != CXX_NULLPTR)
	{
		fclose(d_ptr->file_);
		d_ptr->file_ = CXX_NULLPTR;
	}
	return true;
}

CXXString CXXFile::read() const
{
	if (!isOpen())
	{
		return L"";
	}

	CXXString content;
	char buffer[256] = {};
	while (fgets(buffer, sizeof(buffer), d_ptr->file_) != nullptr)
	{
		content += CXXString::fromUtf8(buffer);
	}

	return content;
}

CXXString CXXFile::readAll()
{
	if (!isOpen())
	{
		return L"";
	}

	_fseeki64(d_ptr->file_, 0, SEEK_SET); // 重置文件指针到文件开头

	CXXString content;
	char buffer[256];
	while (fgets(buffer, sizeof(buffer), d_ptr->file_) != nullptr)
	{
		content += CXXString::fromUtf8(buffer);
	}

	return content;
}

CXXString CXXFile::readLine(__int64 index)
{
	if (!isOpen() || index < 0)
	{
		return L"";
	}

	_fseeki64(d_ptr->file_, 0, SEEK_SET); // 确保从文件开头开始读取

	CXXString lineContent;
	char buffer[256];
	__int64 currentLine = 0;

	while (fgets(buffer, sizeof(buffer), d_ptr->file_) != nullptr)
	{
		if (currentLine == index)
		{
			lineContent = CXXString::fromUtf8(buffer);
			break;
		}
		++currentLine;
	}

	return lineContent;
}

__int64 CXXFile::read(char* data, __int64 size)
{
	if (!isOpen() || data == nullptr)
	{
		return 0;
	}

	if (size == static_cast<__int64>(-1))
	{
		_fseeki64(d_ptr->file_, 0, SEEK_END);
		size = static_cast<__int64>(_ftelli64(d_ptr->file_));
		_fseeki64(d_ptr->file_, 0, SEEK_SET);
	}

	__int64 bytesRead = fread_s(data, static_cast<size_t>(size), sizeof(char), static_cast<size_t>(size), d_ptr->file_);
	return bytesRead;
}

void CXXFile::write(const CXXString& content)
{
	if (!isOpen())
	{
		return;
	}

	std::string utf8 = content.toUtf8();
	if (d_ptr->openMode_ & cxx::Append)
	{
		utf8[utf8.size() - 1] = '\n';
	}

	fwrite(utf8.c_str(), sizeof(char), utf8.size(), d_ptr->file_);
}

void CXXFile::write(const char* data, __int64 size)
{
	if (!isOpen() || CXX_NULLPTR == data)
	{
		return;
	}

	fwrite(data, sizeof(char), static_cast<size_t>(size), d_ptr->file_);
}

void CXXFile::seek(__int64 pos, cxx::SeekDir dir)
{
	if (!isOpen())
	{
		return;
	}

	__int64 origin = 0;
	switch (dir)
	{
	case cxx::Begin:
	{
		origin = SEEK_SET;
		break;
	}
	case cxx::Current:
	{
		origin = SEEK_CUR;
		break;
	}
	case cxx::End:
	{
		origin = SEEK_END;
		break;
	}
	}

	_fseeki64(d_ptr->file_, pos, static_cast<int>(origin));
}

CXXFile& CXXFile::operator<<(wchar_t* data)
{
	if (isOpen())
	{
		write(CXXString(data));
	}
	return *this;
}

CXXFile& CXXFile::operator<<(const wchar_t* data)
{
	if (isOpen())
	{
		write(CXXString(data));
	}
	return *this;
}

CXXFile& CXXFile::operator<<(const CXXString& data)
{
	if (isOpen())
	{
		write(data);
	}
	return *this;
}

CXXFile& CXXFile::operator<<(const char* data)
{
	if (isOpen())
	{
		write(CXXString::fromUtf8(data));
	}
	return *this;
}

__int64 CXXFile::size() const
{
	return CXXFile::size(d_ptr->path_);
}

__int64 CXXFile::count() const
{
	return size();
}

__int64 CXXFile::length() const
{
	return size();
}

CXXString CXXFile::lastModified() const
{
	if (static_cast<__int64>(d_ptr->path_.size()) > 0)
	{
		return CXXFile::lastModified(d_ptr->path_);
	}
	return L"";
}

BYTE* CXXFile::map(__int64 offset, __int64 size)
{
	HANDLE fileHandle = INVALID_HANDLE_VALUE;
	HANDLE mappingHandle = CXX_NULLPTR;
	BYTE* ptr = CXX_NULLPTR;
	do
	{
		fileHandle = CreateFileW(d_ptr->path_.c_str(), GENERIC_READ, FILE_SHARE_READ, CXX_NULLPTR, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, CXX_NULLPTR);
		if (fileHandle == INVALID_HANDLE_VALUE)
		{
			break;
		}

		mappingHandle = CreateFileMappingW(fileHandle, CXX_NULLPTR, PAGE_READONLY, 0, 0, CXX_NULLPTR);
		if (mappingHandle == CXX_NULLPTR)
		{
			break;
		}

		LPVOID viewPtr = MapViewOfFile(mappingHandle, FILE_MAP_READ, static_cast<DWORD>(offset >> 32),
			static_cast<DWORD>(offset), static_cast<SIZE_T>(size));

		if (viewPtr == CXX_NULLPTR)
		{
			break;
		}

		std::unique_ptr<BYTE[]> buffer(new BYTE[static_cast<size_t>(size)]());
		if (buffer != CXX_NULLPTR)
		{
			std::lock_guard<std::mutex> lock(d_ptr->mutex_);
			memmove_s(buffer.get(), static_cast<rsize_t>(size), viewPtr, static_cast<rsize_t>(size));
			d_ptr->mappedData_.push_back(std::move(buffer));
			UnmapViewOfFile(viewPtr);
			ptr = d_ptr->mappedData_.back().get();
		}
	} while (false);

	if (mappingHandle)
	{
		MINT::NtClose(mappingHandle);
	}
	if (fileHandle)
	{
		MINT::NtClose(fileHandle);
	}

	return ptr;
}

void CXXFile::unmap(BYTE* ptr)
{
	std::lock_guard<std::mutex> lock(d_ptr->mutex_);
	auto it = std::find_if(d_ptr->mappedData_.begin(), d_ptr->mappedData_.end(), [ptr](const std::unique_ptr<BYTE[]>& buffer)
		{
			return buffer.get() == ptr;
		});

	if (it != d_ptr->mappedData_.end())
	{
		d_ptr->mappedData_.erase(it);
	}
}

bool CXXFile::exists(const CXXString& path)
{
	std::ifstream ifile(path);
	return ifile.good();
}

CXXString CXXFile::suffix(const CXXString& path)
{

	wchar_t ext[_MAX_EXT] = { 0 };
	auto w = PathFindExtensionW(path.c_str());
	if (w)
	{
		_snwprintf_s(ext, _MAX_EXT, _TRUNCATE, L"%s", w);
	}

	return ext;
}

bool CXXFile::remove(const CXXString& path)
{
	return ::DeleteFileW(path.c_str()) == TRUE;
}

bool CXXFile::rename(const CXXString& oldname, const CXXString& newname)
{
	return ::MoveFileW(oldname.c_str(), newname.c_str()) == TRUE;
}

bool CXXFile::copy(const CXXString& src, const CXXString& dst)
{
	return ::CopyFileW(src.c_str(), dst.c_str(), FALSE) == TRUE;
}

bool CXXFile::removeDir(const CXXString& path)
{
	return ::RemoveDirectoryW(path.c_str()) == TRUE;
}

bool CXXFile::CXXFile::link(const CXXString& src, const CXXString& dst)
{
	return ::CreateHardLinkW(dst.c_str(), src.c_str(), CXX_NULLPTR) == TRUE;
}

bool CXXFile::isDir(const CXXString& path)
{
	DWORD attr = ::GetFileAttributesW(path.c_str());
	return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY));
}

bool CXXFile::isFile(const CXXString& path)
{
	DWORD attr = ::GetFileAttributesW(path.c_str());
	return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
}

bool CXXFile::isExecutable(const CXXString& path)
{
	DWORD attributes = GetFileAttributesW(path.c_str());
	if (attributes != INVALID_FILE_ATTRIBUTES)
	{
		return (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0 && (attributes & FILE_ATTRIBUTE_READONLY) == 0;
	}
	return false;
}

CXXString CXXFile::lastModified(const CXXString& filename)
{
	WIN32_FIND_DATAW fileInfo;
	HANDLE hFind = FindFirstFileW(filename.c_str(), &fileInfo);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		FILETIME modifiedTime = fileInfo.ftLastWriteTime;
		SYSTEMTIME sysTime;
		if (FileTimeToSystemTime(&modifiedTime, &sysTime))
		{
			wchar_t buffer[256];
			_snwprintf_s(buffer, sizeof(buffer), L"%04d-%02d-%02d %02d:%02d:%02d",
				sysTime.wYear, sysTime.wMonth, sysTime.wDay,
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			return std::wstring(buffer);
		}
	}
	return L"";
}

bool CXXFile::isHidden(const CXXString& path)
{
	DWORD attr = ::GetFileAttributesW(path.c_str());
	return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_HIDDEN));
}

bool CXXFile::isSystem(const CXXString& path)
{
	DWORD attr = ::GetFileAttributesW(path.c_str());
	return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_SYSTEM));
}

bool CXXFile::setFileHidden(const CXXString& path, bool hidden)
{
	DWORD attr = ::GetFileAttributesW(path.c_str());
	if (hidden)
		attr |= FILE_ATTRIBUTE_HIDDEN;
	else
		attr &= ~FILE_ATTRIBUTE_HIDDEN;
	return ::SetFileAttributesW(path.c_str(), attr) == TRUE;
}

bool CXXFile::setFileReadOnly(const CXXString& path, bool readonly)
{
	DWORD attr = ::GetFileAttributesW(path.c_str());
	if (readonly)
	{
		if ((attr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
		{
			return true;
		}

		attr |= FILE_ATTRIBUTE_READONLY;
	}
	else
	{
		if ((attr & FILE_ATTRIBUTE_READONLY) != FILE_ATTRIBUTE_READONLY)
		{
			return true;
		}

		attr &= ~FILE_ATTRIBUTE_READONLY;
	}
	return ::SetFileAttributesW(path.c_str(), attr) == TRUE;
}

bool CXXFile::setFileNormal(const CXXString& path)
{
	DWORD attr = ::GetFileAttributesW(path.c_str());
	attr &= ~FILE_ATTRIBUTE_READONLY;
	return ::SetFileAttributesW(path.c_str(), attr) == TRUE;
}

__int64 CXXFile::size(const CXXString& path)
{
	WIN32_FILE_ATTRIBUTE_DATA fad = { 0 };
	if (!::GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &fad))
	{
		return 0;
	}
	LARGE_INTEGER size = { 0 };
	size.HighPart = fad.nFileSizeHigh;
	size.LowPart = fad.nFileSizeLow;
	return size.QuadPart;
}
