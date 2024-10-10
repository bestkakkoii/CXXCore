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

#include <cxxlibrary_p.h>

CXXLibraryPrivate::CXXLibraryPrivate(CXXLibrary* q, const CXXString& pluginName)
	: q_ptr(q)
{
	loadLibrary(pluginName);
}

CXXLibraryPrivate::~CXXLibraryPrivate()
{
	clear();
}

FARPROC CXXLibraryPrivate::resolve(const CXXString& functionName)
{
	CXX_Q(CXXLibrary);

	if (!q->isValid())
	{
		CXX_ASSERT_X(false, std::wstring(L"Library '") + libraryName_ + L"' is not loaded");
		return CXX_NULLPTR;
	}

	FARPROC procAddress = GetProcAddress(hModule_, reinterpret_cast<char*>(functionName.toUtf8().data()));
	if (CXX_NULLPTR == procAddress)
	{
		lastError_ = GetLastError();
		CXX_ASSERT_X(false, std::wstring(L"Unable to found function '") + functionName.toStdWString() + L"' in library '" + libraryName_ + L"'");
		return CXX_NULLPTR;
	}

	return procAddress;
}

bool CXXLibraryPrivate::loadLibrary(CXXString libraryName)
{
	if (!libraryName.endsWith(L".dll") && !libraryName.endsWith(L".dll/") && !libraryName.endsWith(L".dll\\"))
	{
		libraryName += L".dll";
	}

	libraryName_ = libraryName;

	HMODULE hModule = GetModuleHandleW(libraryName_.c_str());
	if (hModule == CXX_NULLPTR)
	{
		unFreeAbled_ = false;
		hModule = LoadLibraryExW(libraryName.c_str(), handle_, static_cast<DWORD>(flags_));
		lastError_ = GetLastError();
	}

	if (hModule != CXX_NULLPTR)
	{
		hModule_ = hModule;
	}

	return hModule != CXX_NULLPTR;
}

void CXXLibraryPrivate::clear()
{
	usedFunctions_.clear();
	if (hModule_.load() != CXX_NULLPTR && !unFreeAbled_)
	{
		FreeLibrary(hModule_);
	}
}

void CXXLibraryPrivate::setFlags(__int64 flags)
{
	flags_ = flags;
}

void CXXLibraryPrivate::setHandle(HANDLE handle)
{
	handle_ = handle;
}

//@参数 插件名称(纯名称、相对路径、绝对路径) .dll扩展名可选
CXXLibrary::CXXLibrary(const CXXString& libraryName)
	: d_ptr(new CXXLibraryPrivate(this, libraryName))
{
}

CXXLibrary::~CXXLibrary()
{

}

void CXXLibrary::setFlags(__int64 flags)
{
	CXX_D(CXXLibrary);

	d->setFlags(flags);
}

void CXXLibrary::setHandle(HANDLE handle)
{
	CXX_D(CXXLibrary);

	d->setHandle(handle);
}

//@备注 重新加载插件
//@参数 插件名称(纯名称、相对路径、绝对路径) .dll扩展名可选
//@返回 是否加载成功
//@别名 载入(插件名称)
bool CXXLibrary::load(const CXXString& libraryName)
{
	CXX_D(CXXLibrary);

	std::unique_lock<std::mutex> lock(d->mutex_);
	d->clear();

	return 	d->loadLibrary(libraryName);
}

//@备注 获取插件名称
//@返回 插件名称
//@别名 文件名()
CXXString CXXLibrary::fileName() const
{
	CXX_D(const CXXLibrary);

	return 	d->libraryName_;
}

//@备注 检查库是否有效
//@返回 是否有效
//@别名 是否有效()
bool CXXLibrary::isValid() const
{
	CXX_D(const CXXLibrary);

	return d->hModule_ != CXX_NULLPTR;
}

//@备注 调用 GetLastError() 获取错误叙述，如果失败则直接返回错误代码文本
//@返回 错误信息文本
//@别名 错误文本()
CXXString CXXLibrary::errorString() const
{
	CXX_D(const CXXLibrary);

	DWORD error = d->lastError_;
	if (error == 0)
	{
		return L"";
	}

	LPWSTR lpMsgBuf = CXX_NULLPTR;
	DWORD ret = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		CXX_NULLPTR, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&lpMsgBuf, 0, CXX_NULLPTR);

	if (ret == 0 || CXX_NULLPTR == lpMsgBuf)
	{
		return CXXString::number(error);
	}

	CXXString msg(lpMsgBuf);
	LocalFree(lpMsgBuf);
	return msg;
}

//@备注 取模块地址
//@返回 模块地址
//@别名 地址()
HMODULE CXXLibrary::data() const
{
	CXX_D(const CXXLibrary);

	return d->hModule_;
}

//@备注 获取函数地址
//@参数 函数名称
//@返回 函数地址
//@别名 解析地址(函数名称)
FARPROC CXXLibrary::resolve(const CXXString& functionName)
{
	CXX_D(CXXLibrary);

	std::unique_lock<std::mutex> lock(d->mutex_);
	return resolveHelper(functionName);
}

//@备注 隐藏插件
//@别名 隐藏()
bool CXXLibrary::hide()
{
	CXX_D(CXXLibrary);

	struct LDR_MODULE
	{
		LIST_ENTRY				  InLoadOrderModuleList;
		LIST_ENTRY				  InMemoryOrderModuleList;
		LIST_ENTRY                InInitializationOrderModuleList;
		void* BaseAddress;
		void* EntryPoint;
		ULONG                     SizeOfImage;
		MINT::UNICODE_STRING      FullDllName;
		MINT::UNICODE_STRING      BaseDllName;
		ULONG					  Flags;
		SHORT					  LoadCount;
		SHORT                     TlsIndex;
		HANDLE                    SectionHandle;
		ULONG                     CheckSum;
		ULONG                     TimeDateStamp;
	};

	MINT::PPEB_LDR_DATA pLdr = CXX_NULLPTR;
	PLIST_ENTRY pFirstEntry = CXX_NULLPTR;
	PLIST_ENTRY pCurrentEntry = CXX_NULLPTR;
	LDR_MODULE* pLdrModule = CXX_NULLPTR;

	MINT::PROCESS_BASIC_INFORMATION pbi = {};
	ULONG returnLength = 0;

	__try
	{
		MINT::NTSTATUS status = MINT::NtQueryInformationProcess(
			GetCurrentProcess(),
			MINT::ProcessBasicInformation,
			&pbi,
			sizeof(pbi),
			&returnLength
		);

		if (!MINT::NT_SUCCESS(status))
		{
			return false;
		}

		pLdr = reinterpret_cast<MINT::PPEB_LDR_DATA>(pbi.PebBaseAddress->Ldr);

		if (pLdr == CXX_NULLPTR)
		{
			return false;
		}

		pFirstEntry = &pLdr->InLoadOrderModuleList;
		pCurrentEntry = pFirstEntry->Flink;

		do
		{
			pLdrModule = CONTAINING_RECORD(pCurrentEntry, LDR_MODULE, InLoadOrderModuleList);

			if (pLdrModule->BaseAddress == d->hModule_.load())
			{
				pLdrModule->InLoadOrderModuleList.Blink->Flink = pLdrModule->InLoadOrderModuleList.Flink;

				pLdrModule->InLoadOrderModuleList.Flink->Blink = pLdrModule->InLoadOrderModuleList.Blink;

				pLdrModule->InInitializationOrderModuleList.Blink->Flink = pLdrModule->InInitializationOrderModuleList.Flink;

				pLdrModule->InInitializationOrderModuleList.Flink->Blink = pLdrModule->InInitializationOrderModuleList.Blink;

				pLdrModule->InMemoryOrderModuleList.Blink->Flink = pLdrModule->InMemoryOrderModuleList.Flink;

				pLdrModule->InMemoryOrderModuleList.Flink->Blink = pLdrModule->InMemoryOrderModuleList.Blink;

				return true;

			}

			pCurrentEntry = pCurrentEntry->Flink;

		} while (pFirstEntry != pCurrentEntry);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}

	return false;
}

std::unordered_map<std::wstring, FARPROC>& CXXLibrary::usedFunctions()
{
	CXX_D(CXXLibrary);

	return d->usedFunctions_;
}

std::wstring& CXXLibrary::libraryName()
{
	CXX_D(CXXLibrary);

	return d->libraryName_;
}

FARPROC CXXLibrary::resolveHelper(const CXXString& functionName)
{
	CXX_D(CXXLibrary);

	return d->resolve(functionName);
}

std::mutex& CXXLibrary::mutex()
{
	CXX_D(CXXLibrary);

	return d->mutex_;
}

CXXAtomicUInt32& CXXLibrary::lastError()
{
	CXX_D(CXXLibrary);

	return d->lastError_;
}