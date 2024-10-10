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

#include <cxxmemory.h>
#include <cxxelapsedtimer.h>

#include <cassert>
#include <memory>
#include <psapi.h>
#include <thread>
#include <tlhelp32.h>

#ifndef MINT_USE_SEPARATE_NAMESPACE
#define MINT_USE_SEPARATE_NAMESPACE
#include <MINT.h>
#endif


#pragma comment(lib,"psapi.lib")

namespace
{
	struct MemoryRegion
	{
		__int64 BaseAddress;
		__int64 RegionSize;
	};

	using handle_data = struct
	{
		DWORD dwProcessId;
		HWND hWnd;
	};

	//@隐藏{
	typedef struct _IAT_EAT_INFO
	{
		char ModuleName[256] = {};
		char FuncName[64] = {};
		ULONG64 Address = 0;
		ULONG64 RecordAddr = 0;
		ULONG64 ModBase = 0;//just for export table
	} IAT_EAT_INFO, * PIAT_EAT_INFO;
	//@隐藏}
}

class CXXMemoryPrivate
{
public:
	CXXMemoryPrivate(CXXMemory* q)
		: q_ptr(q)
		, hProcess_(GetCurrentProcess())
	{}

	CXXMemoryPrivate(CXXMemory* q, __int64 processId, UINT64 baseAddress)
		: q_ptr(q)
		, hProcess_(CXXMemory::openProcess(processId))
		, baseAddress_(baseAddress)
	{
		assert(hProcess_ != nullptr);
	}
	CXXMemoryPrivate(CXXMemory* q, HANDLE hProcess, UINT64 baseAddress)
		: q_ptr(q)
		, hProcess_(hProcess)
		, baseAddress_(baseAddress)
	{
		assert(hProcess_ != nullptr);
	}

	virtual ~CXXMemoryPrivate() {}

	static UINT64 getProcessExportTable32(HANDLE hProcess, const CXXString& moduleName, IAT_EAT_INFO tbinfo[], int tb_info_max)
	{
		ULONG64 muBase = 0;
		ULONG64 count = 0;

		PIMAGE_DOS_HEADER pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(new BYTE[sizeof(IMAGE_DOS_HEADER)]);
		assert(pDosHeader != nullptr);
		PIMAGE_NT_HEADERS32 pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS32>(new BYTE[sizeof(IMAGE_NT_HEADERS32)]);
		assert(pNtHeaders != nullptr);
		PIMAGE_EXPORT_DIRECTORY pExport = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(new BYTE[sizeof(IMAGE_EXPORT_DIRECTORY)]);
		assert(pExport != nullptr);

		char strName[130] = {};
		memset(strName, 0, sizeof(strName));

		//拿到目標模塊的BASE
		muBase = static_cast<ULONG>(CXXMemory::getRemoteModuleHandle(hProcess, moduleName));
		if (!muBase)
		{
			return 0;
		}

		//獲取IMAGE_DOS_HEADER
		CXXMemory::read(hProcess, muBase, pDosHeader, sizeof(IMAGE_DOS_HEADER));
		//獲取IMAGE_NT_HEADERS
		CXXMemory::read(hProcess, (muBase + pDosHeader->e_lfanew), pNtHeaders, sizeof(IMAGE_NT_HEADERS32));
		if (pNtHeaders->OptionalHeader.DataDirectory[0].VirtualAddress == 0)
		{
			return 0;
		}

		CXXMemory::read(hProcess, (muBase + pNtHeaders->OptionalHeader.DataDirectory[0].VirtualAddress), pExport, sizeof(IMAGE_EXPORT_DIRECTORY));
		CXXMemory::read(hProcess, (muBase + pExport->Name), strName, sizeof(strName));
		ULONG64 i = 0;

		if (pExport->NumberOfNames < 0 || pExport->NumberOfNames > 8192)
		{
			return 0;
		}

		for (i = 0; i < pExport->NumberOfNames; i++)
		{
			char bFuncName[100] = {};
			ULONG64 ulPointer;
			USHORT usFuncId;
			ULONG64 ulFuncAddr;
			ulPointer = static_cast<ULONG64>(CXXMemory::read<int>(hProcess, (muBase + pExport->AddressOfNames + i * static_cast<ULONG64>(sizeof(DWORD)))));
			memset(bFuncName, 0, sizeof(bFuncName));
			CXXMemory::read(hProcess, (muBase + ulPointer), bFuncName, sizeof(bFuncName));
			usFuncId = CXXMemory::read<short>(hProcess, (muBase + pExport->AddressOfNameOrdinals + i * sizeof(short)));
			ulPointer = static_cast<ULONG64>(CXXMemory::read<int>(hProcess, (muBase + pExport->AddressOfFunctions + static_cast<ULONG64>(sizeof(DWORD)) * usFuncId)));
			ulFuncAddr = muBase + ulPointer;
			std::string smoduleName = moduleName.toUtf8();
			_snprintf_s(tbinfo[count].ModuleName, sizeof(tbinfo[count].ModuleName), _TRUNCATE, "%s", smoduleName.c_str());
			_snprintf_s(tbinfo[count].FuncName, sizeof(tbinfo[count].FuncName), _TRUNCATE, "%s", bFuncName);
			tbinfo[count].Address = ulFuncAddr;
			tbinfo[count].RecordAddr = (ULONG64)(muBase + pExport->AddressOfFunctions + static_cast<ULONG64>(sizeof(DWORD)) * usFuncId);
			tbinfo[count].ModBase = muBase;
			++count;
			if (count > (ULONG)tb_info_max)
				break;
		}

		delete[]pDosHeader;
		delete[]pExport;
		delete[]pNtHeaders;
		return count;
	}

	static BOOL isCurrentWindow(const HWND hWnd)
	{
		if ((nullptr == GetWindow(hWnd, GW_OWNER)) && (TRUE == IsWindowVisible(hWnd)))
			return TRUE;
		else
			return FALSE;
	}

	static BOOL __stdcall enumWindowsCallback(HWND hWnd, LPARAM lParam)
	{
		handle_data& data = *reinterpret_cast<handle_data*>(lParam);
		DWORD process_id = 0UL;

		GetWindowThreadProcessId(hWnd, &process_id);

		if ((data.dwProcessId != process_id) || (FALSE == isCurrentWindow(hWnd)))
			return TRUE;

		data.hWnd = hWnd;
		return FALSE;
	}

	static HWND findCurrentWindow(DWORD dwProcessId)
	{
		handle_data data = {};
		data.dwProcessId = dwProcessId;
		data.hWnd = nullptr;

		EnumWindows(enumWindowsCallback, reinterpret_cast<LPARAM>(&data));

		return data.hWnd;
	}

	static HANDLE createToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID)
	{
		using namespace MINT;

		CXXMemory::enablePrivilege(::GetCurrentProcess());
		HANDLE hSnapshot = ::CreateToolhelp32Snapshot(dwFlags, th32ProcessID);
		if (hSnapshot && ((hSnapshot) != INVALID_HANDLE_VALUE))
		{
			return hSnapshot;
		}

		return nullptr;
	}

	static HANDLE openProcessToken(HANDLE ProcessHandle, ACCESS_MASK DesiredAccess)
	{
		using namespace MINT;

		HANDLE hToken = nullptr;
		BOOL ret = NT_SUCCESS(MINT::NtOpenProcessToken(ProcessHandle, DesiredAccess, &hToken));
		if (ret && hToken && ((hToken) != INVALID_HANDLE_VALUE))
		{
			return hToken;
		}
		else
			return nullptr;
	}

	static bool isHandleValid(HANDLE handle)
	{
		if (handle == ::GetCurrentProcess() || handle == ::GetCurrentThread() || handle == NtCurrentProcess())
			return true;

		__try
		{
			DWORD dwFlags = NULL;
			return  handle
				&& ((handle) != INVALID_HANDLE_VALUE)
				&& GetHandleInformation(handle, &dwFlags) != FALSE;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return false;
		}
	}

	static HANDLE ntOpenProcess(DWORD dwProcess)
	{
		HANDLE ProcessHandle = NULL;
		using namespace MINT;
		OBJECT_ATTRIBUTES ObjectAttribute = {
			sizeof(OBJECT_ATTRIBUTES), 0, 0, 0, 0, 0 };
		CLIENT_ID ClientId = {};

		InitializeObjectAttributes(&ObjectAttribute, 0, 0, 0, 0);
		ClientId.UniqueProcess = reinterpret_cast<void*>(static_cast<long long>(dwProcess));
		ClientId.UniqueThread = (void*)0;

		BOOL ret = NT_SUCCESS(MINT::NtOpenProcess(&ProcessHandle, MAXIMUM_ALLOWED,
			&ObjectAttribute, &ClientId));

		return ret && ProcessHandle && ((ProcessHandle) != INVALID_HANDLE_VALUE) ? ProcessHandle : nullptr;
	};

	static HANDLE zwOpenProcess(DWORD dwProcess)
	{
		using namespace MINT;

		HANDLE ProcessHandle = (HANDLE)0;
		MINT::OBJECT_ATTRIBUTES ObjectAttribute = {
			sizeof(MINT::OBJECT_ATTRIBUTES), 0, 0, 0, 0, 0 };
		ObjectAttribute.Attributes = NULL;
		MINT::CLIENT_ID ClientIds = {};
		ClientIds.UniqueProcess = reinterpret_cast<HANDLE>(static_cast<long long>(dwProcess));
		ClientIds.UniqueThread = (HANDLE)0;
		BOOL ret = NT_SUCCESS(MINT::ZwOpenProcess(&ProcessHandle, PROCESS_ALL_ACCESS, &ObjectAttribute,
			&ClientIds));

		return ret && ProcessHandle && ((ProcessHandle) != INVALID_HANDLE_VALUE) ? ProcessHandle : nullptr;
	};

private:
	HANDLE hProcess_ = nullptr;
	UINT64 baseAddress_ = 0;
	CXXByteArray buffer_;
	CXXByteArray newBuffer_;
private:
	CXX_DECLARE_PUBLIC(CXXMemory);
	CXXMemory* q_ptr = CXX_NULLPTR;
};


class CXXMemorySearcherPrivate
{
public:
	CXXMemorySearcherPrivate(HANDLE hProcess, const CXXVector<BYTE>& pattern, __int64 baseAddress);
	bool search(CXXVector<__int64>& foundAddresses);

private:
	HANDLE hProcess_;
	CXXVector<BYTE> pattern_;
	__int64 baseAddress_;
	CXXVector<MemoryRegion> memRegions_;

	void enumerateMemoryRegions();
	CXXVector<__int64> searchInRegions(const CXXVector<MemoryRegion>& regions);
};


CXXMemory::CXXMemory()
	: d_ptr(new CXXMemoryPrivate(this))
{

}

CXXMemory::CXXMemory(__int64 processId, UINT64 baseAddress)
	: d_ptr(new CXXMemoryPrivate(this, processId, baseAddress))
{

}

CXXMemory::CXXMemory(HANDLE hProcess, UINT64 baseAddress)
	: d_ptr(new CXXMemoryPrivate(this, hProcess, baseAddress))
{

}

CXXMemory::~CXXMemory()
{

}

#pragma region CXXMemory



template<typename Arg>
bool CXXMemory::read(HANDLE hProcess, UINT64 baseAddress, Arg* buffer, __int64 size)
{
	if (!size)
	{
		return false;
	}

	if (!buffer)
	{
		return false;
	}

	if (!CXXMemoryPrivate::isHandleValid(hProcess))
	{
		return false;
	}

	if (!baseAddress)
	{
		return false;
	}

	CXXMemory::enablePrivilege(::GetCurrentProcess());

	void* address = reinterpret_cast<void*>(baseAddress);

	SIZE_T bytesRead = 0;
	BOOL ret = MINT::NT_SUCCESS(MINT::NtReadVirtualMemory(hProcess, address, buffer, static_cast<size_t>(size), &bytesRead));

	return ret == TRUE;
}

template<typename T, typename>
T CXXMemory::read(HANDLE hProcess, UINT64 baseAddress)
{
	if (!CXXMemoryPrivate::isHandleValid(hProcess))
	{
		return T();
	}

	T buffer{};
	size_t size = sizeof(T);

	if (!size)
	{
		return T();
	}

	BOOL ret = CXXMemory::read(hProcess, baseAddress, &buffer, size);

	return ret ? buffer : T();
}

CXXString CXXMemory::read(HANDLE hProcess, UINT64 baseAddress, __int64 size, __int64 acp)
{
	if (!CXXMemoryPrivate::isHandleValid(hProcess))
	{
		return L"";
	}

	if (!baseAddress)
	{
		return L"";
	}

	CXXString retString;

	switch (acp)
	{
	case 0: // UNICODE
	{
		std::unique_ptr<wchar_t[]> pBuffer(new wchar_t[static_cast<size_t>(size) + 1]());
		assert(pBuffer != nullptr);
		memset(pBuffer.get(), 0, static_cast<size_t>(size) + 1);

		bool ret = read(hProcess, baseAddress, pBuffer.get(), size);
		if (!ret)
		{
			return CXXString();
		}

		CXXString retstring = (ret == TRUE) ? CXXString(pBuffer.get()) : CXXString();

		return retstring;
	}
	default:
	{
		if (-1 == acp)
		{
			acp = GetACP();
		}

		std::unique_ptr <char[]> pBuffer(new char[static_cast<size_t>(size) + 1]);
		assert(pBuffer != nullptr);
		memset(pBuffer.get(), 0, static_cast<size_t>(size) + 1);

		std::wstring wret;

		bool ret = read(hProcess, baseAddress, pBuffer.get(), size);
		if (!ret)
		{
			return CXXString();
		}

		static CXXTextCodec* codec = CXXTextCodec::codecFromACP(acp);

		retString = codec->toUnicode(std::string(pBuffer.get(), static_cast<size_t>(size)));

		return wret;
	}
	}
}

template<typename Arg>
bool CXXMemory::write(HANDLE hProcess, UINT64 baseAddress, Arg* buffer, __int64 size)
{
	if (!CXXMemoryPrivate::isHandleValid(hProcess))
	{
		return false;
	}

	ULONG oldProtect = NULL;

	CXXMemory::enablePrivilege(::GetCurrentProcess());

	void* address = reinterpret_cast<void*>(baseAddress);

	::VirtualProtectEx(hProcess, address, static_cast<size_t>(size), PAGE_EXECUTE_READWRITE, &oldProtect);
	BOOL ret = ::WriteProcessMemory(hProcess, address, buffer, static_cast<size_t>(size), NULL);
	::VirtualProtectEx(hProcess, address, static_cast<size_t>(size), oldProtect, &oldProtect);

	return ret == TRUE;
}

template<typename T, typename>
bool CXXMemory::write(HANDLE hProcess, UINT64 baseAddress, T data)
{
	if (!CXXMemoryPrivate::isHandleValid(hProcess))
	{
		return false;
	}

	if (!baseAddress)
	{
		return false;
	}

	void* pBuffer = &data;
	return write(hProcess, baseAddress, pBuffer, sizeof(T));
}

bool CXXMemory::write(HANDLE hProcess, UINT64 baseAddress, const CXXString& str, __int64 acp, __int64* pSize)
{
	if (!CXXMemoryPrivate::isHandleValid(hProcess))
	{
		return false;
	}

	switch (acp)
	{
	case 0:
	{
		std::wstring wstr(str.get());
		*pSize = static_cast<UINT64>(wstr.length()) * sizeof(wchar_t) + 1;
		return write(hProcess, baseAddress, const_cast<wchar_t*>(wstr.c_str()), static_cast<UINT64>(wstr.length()) * sizeof(wchar_t) + 1);

	}
	default:
	{
		if (-1 == acp)
		{
			acp = GetACP();
		}

		static CXXTextCodec* codec = CXXTextCodec::codecFromACP(acp);

		std::string gb2312str = codec->fromUnicode(str);
		*pSize = static_cast<UINT64>(gb2312str.length()) * sizeof(char) + 1;
		return write(hProcess, baseAddress, const_cast<char*>(gb2312str.c_str()), static_cast<UINT64>(gb2312str.length()) * sizeof(char) + 1);
	}
	}
}

UINT64 CXXMemory::virtualAlloc(HANDLE hProcess, __int64 size)
{
	if (!CXXMemoryPrivate::isHandleValid(hProcess))
	{
		return 0;
	}

	UINT64 ptr = NULL;
	SIZE_T sizet = static_cast<SIZE_T>(size);

	BOOL ret = MINT::NT_SUCCESS(MINT::NtAllocateVirtualMemory(hProcess, reinterpret_cast<void**>(&ptr), NULL, &sizet, MEM_COMMIT, PAGE_EXECUTE_READWRITE));
	if (ret == TRUE)
	{
		return static_cast<UINT64>(ptr);
	}

	return 0;
}

UINT64 CXXMemory::virtualAlloc(HANDLE hProcess, const CXXString& str, __int64 acp, UINT64* size)
{
	if (0 == acp)
	{
		UINT64 ret = NULL;

		do
		{
			if (!CXXMemoryPrivate::isHandleValid(hProcess))
			{
				break;
			}

			ret = virtualAlloc(hProcess, static_cast<UINT64>(str.length()) * 2 + 1);
			if (ret == FALSE)
			{
				break;
			}

			if (!write(hProcess, ret, str, acp))
			{
				virtualFree(hProcess, ret);
				ret = NULL;
				break;
			}

			if (size != nullptr)
			{
				*size = static_cast<UINT64>(str.length()) * 2 + 1;
			}

		} while (false);
		return ret;
	}

	UINT64 ret = NULL;
	std::string ansiStr;
	if (1 == acp)
	{
		ansiStr = str.toUtf8();
	}
	else
	{
		static CXXTextCodec* codec = CXXTextCodec::codecFromACP(static_cast<UINT>(acp));
		ansiStr = codec->fromUnicode(str);
	}

	do
	{
		if (!CXXMemoryPrivate::isHandleValid(hProcess))
		{
			break;
		}

		ret = virtualAlloc(hProcess, static_cast<UINT64>(ansiStr.length()) + 1);
		if (ret == FALSE)
		{
			break;
		}

		if (!write(hProcess, ret, str, acp))
		{
			virtualFree(hProcess, ret);
			ret = NULL;
			break;
		}

		if (size != nullptr)
		{
			*size = static_cast<UINT64>(ansiStr.length()) + 1;
		}

	} while (false);
	return ret;
}

bool CXXMemory::virtualFree(HANDLE hProcess, UINT64 baseAddress)
{
	if (!CXXMemoryPrivate::isHandleValid(hProcess))
	{
		return false;
	}

	if (baseAddress == NULL)
		return false;

	SIZE_T size = 0;
	BOOL ret = MINT::NT_SUCCESS(MINT::NtFreeVirtualMemory(hProcess, reinterpret_cast<void**>(&baseAddress), &size, MEM_RELEASE));

	return ret == TRUE;
}

void CXXMemory::freeUnuseMemory(HANDLE hProcess)
{
	if (!CXXMemoryPrivate::isHandleValid(hProcess))
	{
		return;
	}

	SetProcessWorkingSetSizeEx(hProcess, static_cast<SIZE_T>(-1), static_cast<SIZE_T>(-1), 0);
	K32EmptyWorkingSet(hProcess);
}

HWND CXXMemory::getWindowHandle(__int64 dwProcessId)
{
	if (dwProcessId < 0)
	{
		dwProcessId = _getpid();
	}

	HWND hwnd = nullptr;

	while (nullptr == hwnd)
	{
		hwnd = CXXMemoryPrivate::findCurrentWindow(static_cast<DWORD>(dwProcessId));
	}

	return hwnd;
}

__int64 CXXMemory::getProcessId(HWND hWnd)
{
	DWORD dwProcessId = 0UL;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	return dwProcessId;
}

bool CXXMemory::enablePrivilege(HANDLE hProcess, const CXXString& SE)
{
	if (!CXXMemoryPrivate::isHandleValid(hProcess))
	{
		return false;
	}

	bool fOk = false;
	do
	{
		HANDLE hToken = CXXMemoryPrivate::openProcessToken(hProcess, TOKEN_ALL_ACCESS);
		if (!CXXMemoryPrivate::isHandleValid(hToken))
		{
			break;
		}

		TOKEN_PRIVILEGES tp = {};

		if (LookupPrivilegeValueW(NULL, SE.get(), &tp.Privileges[0].Luid) == FALSE)
		{
			break;
		}

		tp.PrivilegeCount = 1;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		MINT::NtAdjustPrivilegesToken(hToken, FALSE, &tp, sizeof(tp), nullptr, nullptr);

		fOk = (::GetLastError() == ERROR_SUCCESS);
	} while (false);

	return fOk;
}

bool CXXMemory::closeHandle(HANDLE handle)
{
	if (!CXXMemoryPrivate::isHandleValid(handle))
	{
		return false;
	}

	enablePrivilege(::GetCurrentProcess());

	BOOL ret = MINT::NT_SUCCESS(MINT::NtClose(handle));

	return ret == TRUE;
}

HANDLE CXXMemory::openProcess(__int64 dwProcessId)
{
	if (dwProcessId < 0)
	{
		dwProcessId = _getpid();
	}

	enablePrivilege(::GetCurrentProcess());
	HANDLE hprocess = CXXMemoryPrivate::ntOpenProcess(static_cast<DWORD>(dwProcessId));
	if (!hprocess || ((hprocess) == INVALID_HANDLE_VALUE))
	{
		hprocess = CXXMemoryPrivate::zwOpenProcess(static_cast<DWORD>(dwProcessId));
		if (!hprocess || ((hprocess) == INVALID_HANDLE_VALUE))
		{
			hprocess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, static_cast<DWORD>(dwProcessId));
			if (!hprocess || ((hprocess) == INVALID_HANDLE_VALUE))
			{
				hprocess = nullptr;
			}
		}
	}

	if (CXXMemoryPrivate::isHandleValid(hprocess))
	{
		enablePrivilege(hprocess);
		return hprocess;
	}

	return nullptr;
}

HANDLE CXXMemory::createRemoteThread(__int64 processId, UINT64 startRoutine, void* argument)
{
	HANDLE hProcess = openProcess(processId);
	if (!CXXMemoryPrivate::isHandleValid(hProcess))
	{
		return nullptr;
	}

	HANDLE hThread = createRemoteThread(hProcess, startRoutine, argument);
	closeHandle(hProcess);
	return hThread;
}

HANDLE CXXMemory::createRemoteThread(HANDLE hProcess, UINT64 startRoutine, void* argument)
{
	using namespace MINT;

	DWORD id = GetCurrentProcessId();
	if (GetProcessId(hProcess) == id)
	{
		return nullptr;
	}

	enablePrivilege(::GetCurrentProcess());

	HANDLE hThread = nullptr;
	BOOL ret = NT_SUCCESS(MINT::NtCreateThreadEx(&hThread,
		THREAD_ALL_ACCESS,
		nullptr,
		hProcess,
		(void*)startRoutine,
		argument,
		FALSE,
		NULL,
		NULL,
		NULL,
		nullptr));

	if (TRUE == ret && hThread != NULL && ((hThread) != INVALID_HANDLE_VALUE))
	{
		LARGE_INTEGER pTimeout = {};
		pTimeout.QuadPart = -1ll * 10000000ll;
		MINT::NtWaitForSingleObject(hThread, FALSE, &pTimeout);

		return hThread;
	}

	return nullptr;
}

HANDLE CXXMemory::duplicateHandle(HANDLE hSourceProcessHandle, HANDLE hSourceHandle, HANDLE hTargetProcessHandle, __int64 dwOptions)
{
	using namespace MINT;

	enablePrivilege(::GetCurrentProcess());
	HANDLE hTargetHandle = nullptr;

	BOOL ret = NT_SUCCESS(MINT::NtDuplicateObject(hSourceProcessHandle, hSourceHandle, hTargetProcessHandle, &hTargetHandle, 0, FALSE, static_cast<ULONG>(dwOptions)));
	if (ret && hTargetHandle && ((hTargetHandle) != INVALID_HANDLE_VALUE))
	{
		return hTargetHandle;
	}

	return nullptr;
}

UINT64 CXXMemory::getRemoteModuleHandle(__int64 processId, const CXXString& moduleName)
{
	HANDLE hSnapshot = CXXMemoryPrivate::createToolhelp32Snapshot(TH32CS_SNAPMODULE, static_cast<DWORD>(processId));
	if (!CXXMemoryPrivate::isHandleValid(hSnapshot))
	{
		return NULL;
	}

	std::wstring cmpModuleName(moduleName.get());

	MODULEENTRY32W moduleEntry = {};
	memset(&moduleEntry, 0, sizeof(MODULEENTRY32W));
	moduleEntry.dwSize = sizeof(MODULEENTRY32W);
	if (!Module32FirstW(hSnapshot, &moduleEntry))
	{
		MINT::NtClose(hSnapshot);
		return NULL;
	}
	else
	{
		const std::wstring str(moduleEntry.szModule);
		if (str == cmpModuleName)
			return reinterpret_cast<UINT64>(moduleEntry.hModule);
	}

	do
	{
		const std::wstring str(moduleEntry.szModule);
		if (str == cmpModuleName)
		{
			MINT::NtClose(hSnapshot);
			return reinterpret_cast<UINT64>(moduleEntry.hModule);
		}
	} while (Module32NextW(hSnapshot, &moduleEntry));

	MINT::NtClose(hSnapshot);
	return NULL;
}

UINT64 CXXMemory::getRemoteModuleHandle(HANDLE hProcess, const CXXString& szModuleName)
{
	if (!CXXMemoryPrivate::isHandleValid(hProcess))
	{
		return false;
	}

	HMODULE hMods[1024] = {};
	DWORD cbNeeded = 0, i = 0;
	wchar_t szModName[MAX_PATH] = {};
	memset(szModName, 0, sizeof(szModName));

	std::wstring cmpModuleName(szModuleName.get());
	if (cmpModuleName.empty())
	{
		return NULL;
	}

	if (nullptr == hProcess)
	{
		return NULL;
	}

	if (K32EnumProcessModulesEx(hProcess, hMods, sizeof(hMods), &cbNeeded, 3)) //http://msdn.microsoft.com/en-us/library/ms682633(v=vs.85).aspx
	{
		for (i = 0; i <= cbNeeded / sizeof(HMODULE); i++)
		{
			if (K32GetModuleFileNameExW(hProcess, hMods[i], szModName, _countof(szModName)) == NULL)
				continue;

			std::wstring fileName(szModName);

			for (size_t j = 0; j < fileName.size(); j++)
			{
				if (fileName[i] == L'/')
				{
					fileName[i] = L'\\';
				}
			}

			//get file name only like xxxx.xxx only get xxxx
			size_t pos = fileName.find_last_of(L'\\');
			if (pos != std::wstring::npos)
			{
				fileName = fileName.substr(pos + 1);
			}

			if (fileName != cmpModuleName)
				continue;

			return reinterpret_cast<UINT64>(hMods[i]);
		}
	}

	return NULL;
}

UINT64 CXXMemory::getProcAddress(__int64 processId, const CXXString& moduleName, const CXXString& funcName)
{
	HANDLE hProcess = openProcess(processId);
	if (!CXXMemoryPrivate::isHandleValid(hProcess))
	{
		return false;
	}

	UINT64 ret = getProcAddress(hProcess, moduleName, funcName);
	closeHandle(hProcess);
	return ret;
}

UINT64 CXXMemory::getProcAddress(HANDLE hProcess, const CXXString& moduleName, const CXXString& funcName)
{
	if (!CXXMemoryPrivate::isHandleValid(hProcess))
	{
		return false;
	}

	ULONG64 RetAddr = 0;
	PIAT_EAT_INFO pInfo = (PIAT_EAT_INFO)malloc(4096 * sizeof(IAT_EAT_INFO));
	if (nullptr == pInfo)
		return NULL;

	long count = static_cast<long>(CXXMemoryPrivate::getProcessExportTable32(hProcess, moduleName, pInfo, 2048));
	if (!count)
	{
		return NULL;
	}

	//tolower
	auto toLower = [](std::string& str)
		{
			for (size_t i = 0; i < str.size(); i++)
			{
				if (str[i] >= 'A' && str[i] <= 'Z')
					str[i] += 32;
			}
		};

	std::string cmpFuncName = funcName.toLower().toUtf8();

	for (long i = 0; i < count; i++)
	{
		std::string curFuncName(pInfo[i].FuncName);
		toLower(curFuncName);

		if (curFuncName == cmpFuncName)
		{
			RetAddr = pInfo[i].Address;
			break;
		}
	}

	free(pInfo);
	return RetAddr;
}

bool CXXMemory::enumProcess(CXXVector<__int64>* pprocesses, const CXXString& moduleName, const CXXString& withoutModuleName)
{
	if (CXX_NULLPTR == pprocesses)
	{
		return false;
	}

	pprocesses->clear();

	HANDLE hSnapshot = CXXMemoryPrivate::createToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (!CXXMemoryPrivate::isHandleValid(hSnapshot))
	{
		return false;
	}

	PROCESSENTRY32W pe32 = {};
	pe32.dwSize = sizeof(PROCESSENTRY32W);

	//toLower
	auto toLower = [](std::wstring& str)
		{
			for (size_t i = 0; i < str.size(); i++)
			{
				if (str[i] >= L'A' && str[i] <= L'Z')
					str[i] += 32;
			}
		};

	std::wstring cmpModuleName(moduleName.get());
	toLower(cmpModuleName);

	std::wstring withoutModuleNameStr(withoutModuleName.get());
	toLower(withoutModuleNameStr);

	if (Process32FirstW(hSnapshot, &pe32))
	{
		do
		{
			HANDLE hProcess = openProcess(static_cast<__int64>(pe32.th32ProcessID));
			if (!CXXMemoryPrivate::isHandleValid(hProcess))
			{
				continue;
			}

			HMODULE hModules[1024] = {};
			DWORD cbNeeded;
			if (K32EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded) == FALSE)
			{
				closeHandle(hProcess);
				continue;
			}

			bool bret = false;
			for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
			{
				TCHAR szModule[MAX_PATH];
				if (K32GetModuleBaseNameW(hProcess, hModules[i], szModule, sizeof(szModule) / sizeof(TCHAR)) == 0)
				{
					continue;
				}

				std::wstring moduleNameStr(szModule);
				if (!cmpModuleName.empty())
				{
					toLower(moduleNameStr);
					//is contain
					if (!moduleName.isEmpty() && moduleNameStr.find(cmpModuleName) == std::wstring::npos)
					{
						continue;
					}

					bret = true;
				}

				if (!withoutModuleNameStr.empty())
				{
					if (moduleNameStr.find(withoutModuleNameStr) != std::wstring::npos)
					{
						bret = false;
						break;
					}
				}
			}

			if (bret)
			{
				if (pprocesses != nullptr)
				{
					pprocesses->append(static_cast<__int64>(pe32.th32ProcessID));
				}
			}

		} while (Process32NextW(hSnapshot, &pe32));
	}

	closeHandle(hSnapshot);

	if (pprocesses != nullptr)
	{
		return !pprocesses->empty();
	}
	else
	{
		return false;
	}
}

bool CXXMemory::isProcessExist(__int64 dwProcessId)
{
	if (dwProcessId < 0)
	{
		dwProcessId = _getpid();
	}

	HANDLE hProcess = openProcess(dwProcessId);
	bool ret = CXXMemoryPrivate::isHandleValid(hProcess);
	closeHandle(hProcess);
	return ret;
}

bool CXXMemory::inject(__int64 dwProcessId, HANDLE hProcess, CXXString dllPath, HMODULE* phDllModule, HWND hWnd)
{
	if (dwProcessId < 0)
		dwProcessId = _getpid();

	if (!CXXMemoryPrivate::isHandleValid(hProcess))
	{
		return false;
	}

	HMODULE hModule = nullptr;
	CXXElapsedTimer timer;

	std::wstring wdllPath(dllPath.get());
	std::wstring dllName;
	//get name only
	size_t pos = wdllPath.find_last_of(L'\\');
	if (pos != std::wstring::npos)
	{
		dllName = wdllPath.substr(pos + 1);
	}

	HMODULE kernel32Module = GetModuleHandleW(L"kernel32.dll");
	if (nullptr == kernel32Module)
	{
		return false;
	}

	FARPROC loadLibraryProc = GetProcAddress(kernel32Module, "LoadLibraryW");
	if (nullptr == loadLibraryProc)
	{
		return false;
	}

	for (long long i = 0; i < 2; ++i)
	{
		timer.restart();
		CXXVirtualMemory dllFullPathAddr(hProcess, dllPath, 0, true);
		if (!dllFullPathAddr.isValid())
		{
			return false;
		}

		//遠程執行線程
		HANDLE hThreadHandle = createRemoteThread(
			hProcess,
			reinterpret_cast<UINT64>(loadLibraryProc),
			reinterpret_cast<LPVOID>(static_cast<UINT64>(dllFullPathAddr)));

		if (!CXXMemoryPrivate::isHandleValid(hThreadHandle))
		{
			return false;
		}

		timer.restart();

		for (;;)
		{
			hModule = reinterpret_cast<HMODULE>(CXXMemory::getRemoteModuleHandle(static_cast<int>(dwProcessId), dllName.c_str()));
			if (hModule != nullptr)
				break;

			if (timer.hasExpired(3000))
				break;

			if (!CXXMemory::isProcessExist(dwProcessId))
			{
				closeHandle(hThreadHandle);
				return false;
			}

			if (hWnd != nullptr)
			{
				if (!IsWindow(hWnd))
				{
					closeHandle(hThreadHandle);
					return false;
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		if (phDllModule != nullptr)
		{
			*phDllModule = hModule;
		}

		if (CXXMemoryPrivate::isHandleValid(hThreadHandle))
		{
			closeHandle(hThreadHandle);
		}

		if (hModule != nullptr)
		{
			break;
		}
	}

	//if (hModule != nullptr)
		//qDebug() << "inject OK" << "0x" + util::toQString(reinterpret_cast<long long>(hModule), 16) << "time:" << timer.cost() << "ms";

	return true;
}

bool CXXMemory::inject64To32(__int64 dwProcessId, HANDLE hProcess, CXXString dllPath, HMODULE* phDllModule, HWND hWnd)
{
	if (dwProcessId < 0)
		dwProcessId = _getpid();

	if (!CXXMemoryPrivate::isHandleValid(hProcess))
	{
		return false;
	}

	CXXElapsedTimer timer;
	static unsigned char data[128] = {
		0x55,										//push ebp
		0x8B, 0xEC,									//mov ebp,esp
		0x56,										//push esi

		//LoadLibraryW
		0x8B, 0x75, 0x08,							//mov esi,[ebp+08]
		0xFF, 0x76, 0x0C,							//push [esi+0C]
		0x8B, 0x06,									//mov eax,[esi]
		0xFF, 0xD0,									//call eax
		0x89, 0x46, 0x10,							//mov [esi+10],eax

		//GetLastError
		0x8B, 0x46, 0x04,							//mov eax,[esi+04]
		0xFF, 0xD0,									//call eax
		0x89, 0x46, 0x14,							//mov [esi+14],eax

		//GetModuleHandleW
		0x8B, 0x46, 0x08,							//mov eax,[esi+08]
		0x6A, 0x00,									//push 00 { 0 }
		0xFF, 0xD0,									//call eax
		0x89, 0x46, 0x18,							//mov [esi+18],eax

		//return 0
		0x33, 0xC0,									//xor eax,eax

		0x5E,										//pop esi
		0x5D,										//pop ebp
		0xC2, 0x04, 0x00							//ret 0004 { 4 }
	};

	struct InjectData
	{
		DWORD loadLibraryWPtr = 0;
		DWORD getLastErrorPtr = 0;
		DWORD getModuleHandleWPtr = 0;
		DWORD dllFullPathAddr = 0;
		DWORD remoteModule = 0;
		DWORD lastError = 0;
		DWORD gameModule = 0;
	}d;

	CXXVirtualMemory dllFullPathAddr(hProcess, dllPath, 0, true);
	d.dllFullPathAddr = static_cast<DWORD>(dllFullPathAddr);

	d.loadLibraryWPtr = static_cast<DWORD>(getProcAddress(hProcess, L"kernel32.dll", L"LoadLibraryW"));
	d.getLastErrorPtr = static_cast<DWORD>(getProcAddress(hProcess, L"kernel32.dll", L"GetLastError"));
	d.getModuleHandleWPtr = static_cast<DWORD>(getProcAddress(hProcess, L"kernel32.dll", L"GetModuleHandleW"));

	//寫入待傳遞給CallBack的數據
	CXXVirtualMemory injectdata(hProcess, sizeof(InjectData), true);
	CXXMemory::write(hProcess, injectdata, &d, sizeof(InjectData));

	//寫入匯編版的CallBack函數
	CXXVirtualMemory remoteFunc(hProcess, sizeof(data), true);
	CXXMemory::write(hProcess, remoteFunc, data, sizeof(data));

	//qDebug() << "time:" << timer.cost() << "ms";
	timer.restart();

	//遠程執行線程
	{
		HANDLE hThreadHandle = CXXMemory::createRemoteThread(
			hProcess,
			static_cast<UINT64>(remoteFunc),
			reinterpret_cast<LPVOID*>(static_cast<UINT64>(injectdata)));

		if (!CXXMemoryPrivate::isHandleValid(hThreadHandle))
		{
			return false;
		}

		timer.restart();
		for (;;)
		{
			CXXMemory::read(hProcess, injectdata, &d, sizeof(InjectData));

			if (d.remoteModule != NULL)
				break;

			if (timer.hasExpired(3000))
				break;

			if (!CXXMemory::isProcessExist(dwProcessId))
			{
				closeHandle(hThreadHandle);
				return false;
			}

			if (hWnd != nullptr)
			{
				if (!IsWindow(hWnd))
				{
					closeHandle(hThreadHandle);
					return false;
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		closeHandle(hThreadHandle);
	}

	if (d.lastError != 0)
	{
		return false;
	}

	if (phDllModule != nullptr)
		*phDllModule = reinterpret_cast<HMODULE>(static_cast<long long>(d.remoteModule));

	//qDebug() << "inject OK" << "0x" + util::toQString(d.remoteModule, 16) << "time:" << timer.cost() << "ms";
	return d.gameModule > 0 && d.remoteModule > 0;
}

void* CXXMemory::fill(void* buffer, __int64 size, BYTE value)
{
	if (buffer == nullptr)
	{
		return nullptr;
	}

	DWORD dwOldProtect = 0UL;
	VirtualProtect(buffer, static_cast<size_t>(size), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	void* p = memset(buffer, value, static_cast<size_t>(size));
	VirtualProtect(buffer, static_cast<size_t>(size), dwOldProtect, &dwOldProtect);

	return p;
}

void CXXMemory::reset(void* buffer, __int64 size)
{
	fill(buffer, size, 0);
}

void* CXXMemory::clear(void* buffer, __int64 size)
{
	if (buffer == nullptr)
	{
		return nullptr;
	}

	if (size == 0)
	{
		return nullptr;
	}

	DWORD dwOldProtect = 0UL;
	VirtualProtect(buffer, static_cast<size_t>(size), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	void* p = memset(buffer, 0, static_cast<size_t>(size));
	VirtualProtect(buffer, static_cast<size_t>(size), dwOldProtect, &dwOldProtect);

	return p;
}

template<class T, class T2>
static void* CXXMemory::copy(T* dst, T2* src, __int64 size)
{
	if (dst == nullptr)
	{
		return nullptr;
	}

	if (src == nullptr)
	{
		return nullptr;
	}

	if (size == 0)
	{
		return nullptr;
	}

	DWORD dwOldProtect = 0UL;
	VirtualProtect(dst, static_cast<size_t>(size), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	void* p = memcpy_s((void*)dst, static_cast<size_t>(size), (void*)src, static_cast<size_t>(size));
	VirtualProtect(dst, static_cast<size_t>(size), dwOldProtect, &dwOldProtect);
	return p;
}

template<class T, class T2>
void* CXXMemory::move(T* dis, T2* src, __int64 size)
{
	if (dis == nullptr)
	{
		return nullptr;
	}

	if (src == nullptr)
	{
		return nullptr;
	}

	if (size == 0)
	{
		return nullptr;
	}

	DWORD dwOldProtect = 0UL;
	VirtualProtect((void*)dis, size, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	void* p = memmove_s((void*)dis, static_cast<size_t>(size), (void*)src, static_cast<size_t>(size));
	VirtualProtect((void*)dis, size, dwOldProtect, &dwOldProtect);
	return p;
}

void* CXXMemory::copy(void* dst, const void* src, __int64 size, UINT64 offset)
{
	if (dst == nullptr)
	{
		return nullptr;
	}

	if (src == nullptr)
	{
		return nullptr;
	}

	if (size == 0)
	{
		return nullptr;
	}

	DWORD dwOldProtect = 0UL;
	VirtualProtect(reinterpret_cast<LPVOID>(reinterpret_cast<__int64*>(dst) + offset), static_cast<size_t>(size), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	void* p = memcpy(reinterpret_cast<__int64*>(dst) + offset, src, static_cast<size_t>(size));
	VirtualProtect(reinterpret_cast<LPVOID>(reinterpret_cast<__int64*>(dst) + offset), static_cast<size_t>(size), dwOldProtect, &dwOldProtect);

	return p;
}

__int64 CXXMemory::compare(const void* dst, const void* src, __int64 size)
{
	if (dst == nullptr)
	{
		return 0;
	}

	if (src == nullptr)
	{
		return 0;
	}

	if (size == 0)
	{
		return 0;
	}

	return static_cast<__int64>(memcmp(dst, src, static_cast<size_t>(size)));
}

template <class ToType, class FromType>
void CXXMemory::getFuncAddr(ToType* addr, FromType f)
{
	union
	{
		FromType _f;
		ToType   _t;
	}ut{};

	ut._f = f;

	*addr = ut._t;
}

template<class T, class T2>
void CXXMemory::detour(T pfnHookFunc, __int64 bOri, T2* bOld, T2* bNew, const __int64 nByteSize, const __int64 offest)
{
	__int64 hookfunAddr = 0UL;
	getFuncAddr(&hookfunAddr, pfnHookFunc);//獲取函數HOOK目標函數指針(地址)
	__int64 dwOffset = (hookfunAddr + offest) - (__int64)bOri - nByteSize;//計算偏移

	constexpr __int64 addressSize = sizeof(intptr_t);

	move((__int64)&bNew[static_cast<size_t>(nByteSize - addressSize)], &dwOffset, sizeof(dwOffset));//將計算出的結果寫到緩存 CALL XXXXXXX 或 JMP XXXXXXX

	move((__int64)bOld, (void*)bOri, nByteSize);//將原始內容保存到bOld (之後需要還原時可用)

	move((__int64)bOri, bNew, nByteSize);//將緩存內的東西寫到要HOOK的地方(跳轉到hook函數 或調用hook函數)
}

template<class T>
void CXXMemory::undetour(T ori, BYTE* oldBytes, __int64 size)
{
	move(ori, oldBytes, size);
}
#pragma endregion

bool CXXMemory::open(__int64 processId)
{
	d_ptr->hProcess_ = openProcess(processId);

	return CXXMemoryPrivate::isHandleValid(d_ptr->hProcess_);
}

void CXXMemory::setHandle(HANDLE h)
{
	d_ptr->hProcess_ = h;
}

void CXXMemory::setBaseAddress(UINT64 baseAddress)
{
	d_ptr->baseAddress_ = baseAddress;
}

bool CXXMemory::restore()
{
	if (d_ptr->hProcess_ == nullptr)
	{
		return false;
	}

	if (d_ptr->baseAddress_ == 0)
	{
		return false;
	}

	if (d_ptr->buffer_.isEmpty())
	{
		return false;
	}

	return write(d_ptr->hProcess_, d_ptr->baseAddress_, d_ptr->buffer_.data(), d_ptr->buffer_.size());
}

bool CXXMemory::rewrite()
{
	if (d_ptr->hProcess_ == nullptr)
	{
		return false;
	}

	if (d_ptr->baseAddress_ == 0)
	{
		return false;
	}

	if (d_ptr->newBuffer_.isEmpty())
	{
		return false;
	}

	return write(d_ptr->hProcess_, d_ptr->baseAddress_, d_ptr->newBuffer_.data(), d_ptr->newBuffer_.size());
}

template<typename Arg>
bool CXXMemory::read(UINT64 baseAddress, Arg* buffer, __int64 size)
{
	return CXXMemory::read(d_ptr->hProcess_, baseAddress, buffer, size);
}

CXXString CXXMemory::read(UINT64 baseAddress, __int64 size, __int64 acp)
{
	return CXXMemory::read(d_ptr->hProcess_, baseAddress, size, acp);
}

template<typename T, typename>
T CXXMemory::read(UINT64 baseAddress)
{
	return CXXMemory::read<T>(d_ptr->hProcess_, baseAddress);
}

template<typename Arg>
bool CXXMemory::write(UINT64 baseAddress, Arg* buffer, __int64 size)
{
	return CXXMemory::write(d_ptr->hProcess_, baseAddress, buffer, size);
}

bool CXXMemory::write(UINT64 baseAddress, const CXXString& str, __int64 acp)
{
	return CXXMemory::write(d_ptr->hProcess_, baseAddress, str, acp);
}

template<typename T>
bool CXXMemory::write(UINT64 baseAddress, T data)
{
	return CXXMemory::write(d_ptr->hProcess_, baseAddress, data);
}

template<typename Arg>
bool CXXMemory::read(__int64 size, Arg* buffer)
{
	return CXXMemory::read(d_ptr->baseAddress_, size, buffer);
}

CXXString CXXMemory::read(__int64 size, __int64 acp)
{
	return CXXMemory::read(d_ptr->baseAddress_, size, acp);
}

template<typename T, typename>
T CXXMemory::read()
{
	return CXXMemory::read<T>(d_ptr->baseAddress_);
}

template<typename Arg>
bool CXXMemory::write(Arg* buffer, __int64 size)
{
	d_ptr->buffer_.resize(size);
	d_ptr->newBuffer_.resize(size);

	// store new data to newBuffer
	memmove_s(d_ptr->newBuffer_.data(), size, buffer, size);

	// read old data
	Arg oldData = read<Arg>(d_ptr->hProcess_, d_ptr->baseAddress_);
	memmove_s(d_ptr->buffer_.data(), size, &oldData, size);

	return CXXMemory::write(d_ptr->baseAddress_, buffer, size);
}

bool CXXMemory::write(const CXXString& str, __int64 acp)
{
	if (!CXXMemoryPrivate::isHandleValid(d_ptr->hProcess_))
	{
		return false;
	}

	switch (acp)
	{
	case 0:
	{
		std::wstring wstr(str.get());
		__int64 size = static_cast<UINT64>(wstr.length()) * sizeof(wchar_t) + 1;
		d_ptr->buffer_.resize(size);
		d_ptr->newBuffer_.resize(size);

		// store new data to newBuffer
		memmove_s(d_ptr->newBuffer_.data(), static_cast<size_t>(size), const_cast<wchar_t*>(wstr.c_str()), static_cast<size_t>(size));

		// read old data
		wchar_t oldData = read<wchar_t>(d_ptr->hProcess_, d_ptr->baseAddress_);
		memmove_s(d_ptr->buffer_.data(), static_cast<size_t>(size), &oldData, static_cast<size_t>(size));

		return write(d_ptr->baseAddress_, const_cast<wchar_t*>(wstr.c_str()), static_cast<UINT64>(wstr.length()) * sizeof(wchar_t) + 1);

	}
	default:
	{
		static CXXTextCodec* codec = CXXTextCodec::codecFromACP(acp);

		std::string gb2312str = codec->fromUnicode(str);
		__int64 size = static_cast<UINT64>(gb2312str.length()) * sizeof(char) + 1;
		d_ptr->buffer_.resize(size);
		d_ptr->newBuffer_.resize(size);

		// store new data to newBuffer
		memmove_s(d_ptr->newBuffer_.data(), static_cast<size_t>(size), const_cast<char*>(gb2312str.c_str()), static_cast<size_t>(size));

		// read old data
		char oldData = read<char>(d_ptr->hProcess_, d_ptr->baseAddress_);
		memmove_s(d_ptr->buffer_.data(), static_cast<size_t>(size), &oldData, static_cast<size_t>(size));

		return write(d_ptr->baseAddress_, const_cast<char*>(gb2312str.c_str()), static_cast<UINT64>(gb2312str.length()) * sizeof(char) + 1);
	}
	}
}

template<typename T, typename>
bool CXXMemory::write(T data)
{
	const __int64 size = sizeof(T);
	d_ptr->buffer_.resize(size);
	d_ptr->newBuffer_.resize(size);

	// store new data to newBuffer
	memmove_s(d_ptr->newBuffer_.data(), size, &data, size);

	// read old data
	T oldData = read<T>(d_ptr->hProcess_, d_ptr->baseAddress_);
	memmove_s(d_ptr->buffer_.data(), size, &oldData, size);

	return CXXMemory::write(d_ptr->baseAddress_, data);
}

HANDLE CXXMemory::createRemoteThread(UINT64 startRoutine, void* argument)
{
	return CXXMemory::createRemoteThread(d_ptr->hProcess_, startRoutine, argument);
}

#pragma region CXXVirtualMemory
CXXVirtualMemory::CXXVirtualMemory(__int64 processId, __int64 size, bool autoClear)
	: autoClear_(autoClear)
	, hProcess_(CXXMemory::openProcess(processId))
{
	if (!CXXMemoryPrivate::isHandleValid(hProcess_))
	{
		lpAddress_ = NULL;
		hProcess_ = nullptr;
		return;
	}

	hasTempHandle_ = true;

	DWORD id = GetCurrentProcessId();
	if (GetProcessId(hProcess_) == id)
	{
		lpAddress_ = NULL;
		hProcess_ = nullptr;
		return;
	}

	lpAddress_ = CXXMemory::virtualAlloc(hProcess_, size);
	if (lpAddress_ != NULL)
	{
		size_ = size;
	}
}

CXXVirtualMemory::CXXVirtualMemory(__int64 processId, const CXXString& str, __int64 acp, bool autoClear)
	: autoClear_(autoClear)
	, hProcess_(CXXMemory::openProcess(processId))
{
	if (!CXXMemoryPrivate::isHandleValid(hProcess_))
	{
		lpAddress_ = NULL;
		hProcess_ = nullptr;
		return;
	}

	hasTempHandle_ = true;

	DWORD id = GetCurrentProcessId();
	if (GetProcessId(hProcess_) == id)
	{
		lpAddress_ = NULL;
		hProcess_ = nullptr;
		return;
	}

	lpAddress_ = CXXMemory::virtualAlloc(hProcess_, str, acp, &size_);
}

CXXVirtualMemory::CXXVirtualMemory(HANDLE h, __int64 size, bool autoClear)
	: autoClear_(autoClear)
	, hProcess_(h)
{
	if (size == 0)
	{
		lpAddress_ = NULL;
		hProcess_ = nullptr;
		return;
	}

	if (!CXXMemoryPrivate::isHandleValid(h))
	{
		lpAddress_ = NULL;
		hProcess_ = nullptr;
		return;
	}

	DWORD id = GetCurrentProcessId();
	if (GetProcessId(h) == id)
	{
		lpAddress_ = NULL;
		hProcess_ = nullptr;
		return;
	}

	lpAddress_ = CXXMemory::virtualAlloc(h, size);
	if (lpAddress_ != NULL)
	{
		size_ = size;
	}
}

CXXVirtualMemory::CXXVirtualMemory(HANDLE h, const CXXString& str, __int64 acp, bool autoClear)
	: autoClear_(autoClear)
	, hProcess_(h)
{
	if (!CXXMemoryPrivate::isHandleValid(h))
	{
		lpAddress_ = NULL;
		hProcess_ = nullptr;
		return;
	}

	DWORD id = GetCurrentProcessId();
	if (GetProcessId(h) == id)
	{
		lpAddress_ = NULL;
		hProcess_ = nullptr;
		return;
	}

	lpAddress_ = CXXMemory::virtualAlloc(h, str, acp, &size_);
}

CXXVirtualMemory& CXXVirtualMemory::operator=(__int64 other)
{
	lpAddress_ = other;
	return *this;
}

CXXVirtualMemory* CXXVirtualMemory::operator&()
{
	return this;
}

// copy constructor
CXXVirtualMemory::CXXVirtualMemory(const CXXVirtualMemory& other)
	: autoClear_(other.autoClear_)
	, lpAddress_(other.lpAddress_)
{
}

//copy assignment
CXXVirtualMemory& CXXVirtualMemory::operator=(const CXXVirtualMemory& other)
{
	lpAddress_ = other.lpAddress_;
	autoClear_ = other.autoClear_;
	hProcess_ = other.hProcess_;
	return *this;
}

// move constructor
CXXVirtualMemory::CXXVirtualMemory(CXXVirtualMemory&& other) noexcept
	: autoClear_(other.autoClear_)
	, lpAddress_(other.lpAddress_)
	, hProcess_(other.hProcess_)

{
	other.lpAddress_ = NULL;
}

// move assignment
CXXVirtualMemory& CXXVirtualMemory::operator=(CXXVirtualMemory&& other) noexcept
{
	lpAddress_ = other.lpAddress_;
	autoClear_ = other.autoClear_;
	hProcess_ = other.hProcess_;
	other.lpAddress_ = NULL;
	return *this;
}

CXXVirtualMemory::~CXXVirtualMemory()
{
	if ((autoClear_) && (lpAddress_ != NULL) && (hProcess_ != nullptr))
	{
		CXXMemory::write<BYTE>(hProcess_, lpAddress_, 0);
		CXXMemory::virtualFree(hProcess_, lpAddress_);
		lpAddress_ = NULL;
	}

	if (hasTempHandle_)
	{
		CXXMemory::closeHandle(hProcess_);
	}
}

bool CXXVirtualMemory::isNull() const
{
	return NULL == lpAddress_;
}

bool CXXVirtualMemory::isData(BYTE* data, __int64 size) const
{
	std::unique_ptr<BYTE[]> pBuffer(new BYTE[static_cast<size_t>(size)]());
	if (nullptr == pBuffer.get())
	{
		return false;
	}

	CXXMemory::read(hProcess_, lpAddress_, pBuffer.get(), size);
	bool ret = memcmp(data, pBuffer.get(), static_cast<size_t>(size)) == 0;

	return ret;
}

void CXXVirtualMemory::clear() const
{
	if ((lpAddress_) != NULL && size_ > 0 && hProcess_ != nullptr)
	{
		std::unique_ptr<BYTE[]> pBuffer(new BYTE[static_cast<size_t>(size_)]());
		CXXMemory::write(hProcess_, lpAddress_, pBuffer.get(), static_cast<__int64>(size_));
	}
}

bool CXXVirtualMemory::isValid() const
{
	return (lpAddress_) != NULL;
}
#pragma endregion

#pragma region Search

CXXMemorySearcherPrivate::CXXMemorySearcherPrivate(HANDLE hProcess, const CXXVector<BYTE>& pattern, __int64 baseAddress)
	: hProcess_(hProcess), pattern_(pattern), baseAddress_(baseAddress)
{
	enumerateMemoryRegions();
}

void CXXMemorySearcherPrivate::enumerateMemoryRegions()
{
	const __int64 MAX_ADDRESS = 0x7FFFFFFFFFFF;
	__int64 address = baseAddress_;

	while (address < MAX_ADDRESS)
	{
		MEMORY_BASIC_INFORMATION mbi;
		SIZE_T result = VirtualQueryEx(hProcess_, (LPCVOID)address, &mbi, sizeof(mbi));
		if (result == 0)
		{
			address += 0x1000; // 跳过页面大小
			continue;
		}

		// 只处理可读的内存区域，排除 PAGE_GUARD
		if (mbi.State == MEM_COMMIT &&
			(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE)) &&
			!(mbi.Protect & PAGE_GUARD))
		{
			MemoryRegion region = { (__int64)mbi.BaseAddress ,(__int64)mbi.RegionSize };
			memRegions_.push_back(region);
		}

		address = (__int64)mbi.BaseAddress + mbi.RegionSize;
	}
}

bool CXXMemorySearcherPrivate::search(CXXVector<__int64>& foundAddresses)
{
	// 确定线程数量
	unsigned int numThreads = std::thread::hardware_concurrency();
	if (numThreads == 0) numThreads = 4;

	CXXVector<std::future<CXXVector<__int64>>> futures;

	size_t totalRegions = static_cast<size_t>(memRegions_.size());
	size_t regionsPerThread = totalRegions / numThreads;
	size_t extra = totalRegions % numThreads;

	auto it = memRegions_.begin();
	for (unsigned int i = 0; i < numThreads; ++i)
	{
		size_t count = regionsPerThread + (i < extra ? 1 : 0);
		CXXVector<MemoryRegion> regions(it, it + count);
		it += count;

		// 启动异步任务
		futures.push_back(std::async(std::launch::async, &CXXMemorySearcherPrivate::searchInRegions, this, regions));
	}

	// 收集结果
	for (auto& future : futures)
	{
		CXXVector<__int64> result = future.get();
		foundAddresses.append(result);
	}

	return !foundAddresses.empty();
}

CXXVector<__int64> CXXMemorySearcherPrivate::searchInRegions(const CXXVector<MemoryRegion>& regions)
{
	CXXVector<__int64> foundAddresses;
	const SIZE_T MAX_CHUNK_SIZE = 0x100000; // 1 MB
	SIZE_T patternSize = static_cast<size_t>(pattern_.size());
	SIZE_T overlap = (patternSize > 1) ? (patternSize - 1) : 0;

	for (const auto& region : regions)
	{
		__int64 regionBase = region.BaseAddress;
		__int64 regionSize = region.RegionSize;
		__int64 offset = 0;

		while (offset < regionSize)
		{
			__int64 readOffset;
			SIZE_T chunkSize;

			if (offset == 0)
			{
				readOffset = offset;
				chunkSize = (SIZE_T)cxx::min((__int64)MAX_CHUNK_SIZE, regionSize - offset);
			}
			else
			{
				readOffset = offset - overlap;
				chunkSize = (SIZE_T)cxx::min((__int64)MAX_CHUNK_SIZE + static_cast<__int64>(overlap), regionSize - readOffset);
			}

			CXXVector<BYTE> buffer(chunkSize);
			SIZE_T bytesRead = 0;
			if (ReadProcessMemory(hProcess_, (LPCVOID)(regionBase + readOffset), buffer.data(), chunkSize, &bytesRead))
			{
				auto it = buffer.begin();
				while (it != buffer.end())
				{
					it = std::search(it, buffer.end(), pattern_.begin(), pattern_.end());
					if (it != buffer.end())
					{
						__int64 matchOffset = std::distance(buffer.begin(), it);
						foundAddresses.push_back(regionBase + readOffset + matchOffset);
						++it; // 移动到下一个位置
					}
				}
			}

			offset += MAX_CHUNK_SIZE;
		}
	}
	return foundAddresses;
}

bool CXXMemory::find(HANDLE hProcess, CXXVector<BYTE> input, __int64 baseAddress, CXXVector<__int64>* output)
{
	CXXMemorySearcherPrivate searcher(hProcess, input, baseAddress);
	CXXVector<__int64> foundAddresses;
	bool found = searcher.search(foundAddresses);

	// 将结果存储在输出向量中
	if (output)
	{
		*output = std::move(foundAddresses);
	}

	return found;
}
#pragma endregion
