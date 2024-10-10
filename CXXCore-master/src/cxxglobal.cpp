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

#include <cxxglobal.h>

#include <cxxstring.h>
#include <cxxvector.h>
#include <cxxprocess.h>

#include <memory>
#include <sstream>
#include <fstream>
#include <string>

#include <winver.h>
#include <psapi.h>
#include <comdef.h>
#include <wbemidl.h>
#include <winnt.h>
#include <thread>
#include <shlwapi.h>
#include <algorithm> 
#include <crtdbg.h>

namespace cxx
{
	//@备注 用途: 主动引发程序致命错误并弹出消息框显示错误信息
	//@参数 当前文件名: 传入__FILE__宏
	//@参数 当前行号 传入__LINE__宏
	//@参数 message 要输出的信息
	//@返回 无
	//@别名 致命错误信息(文件名,行号,信息)
	void message_fatal(const wchar_t* contextFileL, size_t line, const std::wstring& message)
	{
		using namespace MINT;

#if defined(CXX_CC_MSVC) && defined(_DEBUG) && defined(_CRT_ERROR)
		// we probably should let the compiler do this for us, by declaring QMessageLogContext::file to
		// be const wchar_t * in the first place, but the #ifdefery above is very complex  and we
		// wouldn't be able to change it later on...

		// get the current report mode
		int reportMode = _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_WNDW);
		_CrtSetReportMode(_CRT_ERROR, reportMode);
		int ret = _CrtDbgReportW(_CRT_ERROR, contextFileL, static_cast<int>(line), _CRT_WIDE("debug"),
			reinterpret_cast<const wchar_t*>(message.c_str()));
		if ((ret == 0) && (reportMode & _CRTDBG_MODE_WNDW))
		{
			return; // ignore
		}
		else if (ret == 1)
		{
			_CrtDbgBreak();
		}
#else
		std::ignore = contextFileL;
		std::ignore = line;
		std::ignore = message;
#endif

#ifdef CXX_OS_WIN
		// std::abort() in the MSVC runtime will call _exit(3) if the abort
		// behavior is _WRITE_ABORT_MSG - see also _set_abort_behavior(). This is
		// the default for a debug-mode build of the runtime. Worse, MinGW's
		// std::abort() implementation (in msvcrt.dll) is basically a call to
		// _exit(3) too. Unfortunately, _exit() and _Exit() *do* run the static
		// destructors of objects in DLLs, a violation of the C++ standard (see
		// [support.start.term]). So we bypass std::abort() and directly
		// terminate the application.
#  if defined(CXX_CC_MSVC) && !defined(CXX_CC_INTEL)
		if (IsProcessorFeaturePresent(PF_FASTFAIL_AVAILABLE))
		{
			__fastfail(FAST_FAIL_FATAL_APP_EXIT);
		}
#  else
		RaiseFailFastException(CXX_NULLPTR, CXX_NULLPTR, 0);
#  endif
		// Fallback
		NtTerminateProcess(GetCurrentProcess(), STATUS_FATAL_APP_EXIT);
		// Tell the compiler the application has stopped.
		__assume(0);
#else // !CXX_OS_WIN
		std::abort();
#endif
	}

	//@备注 将数值保存到进程环境变量中
	//@参数 name 环境变量名
	//@参数 value 环境变量值
	//@返回 是否成功
	//@别名 置环境变量(名称,值)
	bool putenv(const wchar_t* name, const wchar_t* value)
	{
		if (name == CXX_NULLPTR || value == CXX_NULLPTR)
		{
			return false;
		}

		std::wstring env = name;
		env += L"=";
		env += value;

		return _wputenv_s(name, env.c_str()) == 0;
	}

	//@备注 获取进程环境变量的值
	//@参数 name 环境变量名
	//@返回 环境变量值
	//@别名 取环境变量(名称)
	CXXString getenv(const wchar_t* name)
	{
		if (name == CXX_NULLPTR)
		{
			return L"";
		}

		constexpr size_t MAX_ENV = 32767;
		std::unique_ptr<wchar_t[]> buffer(new wchar_t[MAX_ENV]);
		size_t size = 0;
		if (_wgetenv_s(&size, buffer.get(), MAX_ENV, name) != 0)
		{
			return L"";
		}

		//remove "%(name)="
		std::wstring env(buffer.get());
		if (env.find(L"=") != std::wstring::npos)
		{
			env = env.substr(env.find(L"=") + 1);
		}

		return env;
	}

	//@备注 设置系统环境变量
	//@参数 name 环境变量名
	//@参数 value 环境变量值
	//@返回 是否成功
	//@别名 置系统变量(名称,值)
	bool putsysenv(const wchar_t* name, const wchar_t* value)
	{
		if (name == CXX_NULLPTR || value == CXX_NULLPTR)
		{
			return false;
		}

		std::wstring env = name;
		env += L"=";
		env += value;

		return SetEnvironmentVariableW(name, value) == TRUE;
	}

	//@备注 取系统变量的值
	//@参数 name 环境变量名
	//@返回 变量值
	//@别名 取系统变量(名称)
	CXXString getsysenv(const wchar_t* name)
	{
		if (name == CXX_NULLPTR)
		{
			return L"";
		}

		constexpr size_t MAX_ENV = 32767;
		std::unique_ptr<wchar_t[]> buffer(new wchar_t[MAX_ENV]);
		if (GetEnvironmentVariableW(name, buffer.get(), MAX_ENV) == 0)
		{
			return L"";
		}

		return buffer.get();
	}

	//@备注 异步执行bat文件, 即便当前进程退出, 也不会影响bat文件的执行
	//@参数 path bat文件路径
	//@参数 data bat文件内容
	//@返回 是否成功
	//@别名 异步执行批处理(路径,内容)
	bool asyncRunBat(const CXXString& path, const CXXString& data)
	{
		std::wstring raw = L"@chcp 65001\n" + std::wstring(data);

		//WINAPI check if file exist, then rmeove it 
		if (PathFileExistsW(path) == TRUE)
		{
			DeleteFileW(path);
		}

		//WINAPI 转UTF8
		int sizePath = WideCharToMultiByte(CP_UTF8, 0, path, -1, CXX_NULLPTR, 0, CXX_NULLPTR, CXX_NULLPTR);
		std::unique_ptr<char[]> utf8Path(new char[sizePath]);
		WideCharToMultiByte(CP_UTF8, 0, path, -1, utf8Path.get(), sizePath, CXX_NULLPTR, CXX_NULLPTR);

		int sizeRaw = WideCharToMultiByte(CP_UTF8, 0, raw.c_str(), -1, CXX_NULLPTR, 0, CXX_NULLPTR, CXX_NULLPTR);
		std::unique_ptr<char[]> utf8Raw(new char[sizeRaw]);
		WideCharToMultiByte(CP_UTF8, 0, raw.c_str(), -1, utf8Raw.get(), sizeRaw, CXX_NULLPTR, CXX_NULLPTR);

		//std open as TRUNCATE
		std::ofstream file(utf8Path.get(), std::ios::binary | std::ios::trunc);
		if (!file.is_open())
		{
			return false;
		}

		// write to file
		file << utf8Raw;
		file.flush(); // flush to file
		file.close();

		HINSTANCE hInstance = ShellExecuteW(CXX_NULLPTR, L"open", path, CXX_NULLPTR, CXX_NULLPTR, SW_HIDE);

		return hInstance > reinterpret_cast<HINSTANCE>(32);
	}

	//@备注 获取当前进程已加载的所有模块名称
	//@返回 模块名称列表
	//@别名 取模块名称列表()
	CXXVector<CXXString> getLibraryPaths()
	{
		CXXVector<CXXString> result;
		HANDLE hProcess = GetCurrentProcess();
		HMODULE hModules[1024] = {};
		DWORD cbNeeded;
		TCHAR szModName[MAX_PATH] = {};
		if (K32EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded) == FALSE)
		{
			return result;
		}

		for (size_t i = 0; i < cbNeeded / sizeof(HMODULE); i++)
		{
			if (K32GetModuleFileNameExW(hProcess, hModules[i], szModName, sizeof(szModName) / sizeof(TCHAR)) == 0)
			{
				continue;
			}

			std::wstring path(szModName);
			if (path.find(L"/") != std::wstring::npos)
			{
				std::replace(path.begin(), path.end(), L'/', L'\\');
			}

			size_t last = path.find_last_of(L"\\");
			if (last != std::wstring::npos)
			{
				path = path.substr(last + 1);
			}

			result.append(path);
		}
		return result;
	}

	//@备注 获取硬件序列号
	//@参数 硬件类型
	//@返回 序列号列表
	//@别名 取硬件序列号(硬件类型)
	CXXVector<CXXString> getHardwareSerialNumber(HardwareType type)
	{
		CXXVector<CXXString> serialNumber;
		HRESULT hres;

		hres = CoInitializeEx(0, COINIT_MULTITHREADED);
		if (FAILED(hres))
		{
			return serialNumber;
		}

		hres = CoInitializeSecurity(
			CXX_NULLPTR,
			-1,                          // COM 身份验证
			CXX_NULLPTR,                   // 身份验证服务
			CXX_NULLPTR,                   // 保留
			RPC_C_AUTHN_LEVEL_DEFAULT,   // 默认身份验证 
			RPC_C_IMP_LEVEL_IMPERSONATE, // 默认模拟 
			CXX_NULLPTR,                   // 身份验证信息
			EOAC_NONE,                   // 附加功能
			CXX_NULLPTR                    // 保留
		);

		if (FAILED(hres))
		{
			CoUninitialize();
			return serialNumber;
		}

		IWbemLocator* pLoc = CXX_NULLPTR;

		hres = CoCreateInstance(
			CLSID_WbemLocator,
			0,
			CLSCTX_INPROC_SERVER,
			IID_IWbemLocator, (LPVOID*)&pLoc);

		if (FAILED(hres))
		{
			CoUninitialize();
			return serialNumber;
		}

		IWbemServices* pSvc = CXX_NULLPTR;

		// connect to WMI
		hres = pLoc->ConnectServer(
			_bstr_t(L"ROOT\\CIMV2"), // WMI 命名空间
			CXX_NULLPTR,               // 用户名
			CXX_NULLPTR,               // 密码
			CXX_NULLPTR,               // 语言区域设置
			NULL,                    // 安全标志
			CXX_NULLPTR,               // 权限
			CXX_NULLPTR,               // 上下文对象 
			&pSvc                    // IWbemServices 代理
		);

		if (FAILED(hres))
		{
			pLoc->Release();
			CoUninitialize();
			return serialNumber;
		}

		hres = CoSetProxyBlanket(
			pSvc,                        // 代理
			RPC_C_AUTHN_WINNT,           // 身份验证服务
			RPC_C_AUTHZ_NONE,            // 授权服务
			CXX_NULLPTR,                   // 服务器主体名称 
			RPC_C_AUTHN_LEVEL_CALL,      // 身份验证级别
			RPC_C_IMP_LEVEL_IMPERSONATE, // 模拟级别
			CXX_NULLPTR,                   // 身份验证信息
			EOAC_NONE                    // 代理功能
		);

		if (FAILED(hres))
		{
			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
			return serialNumber;
		}

		IEnumWbemClassObject* pEnumerator = CXX_NULLPTR;
		std::wstring queryLanguage = L"WQL";
		std::wstring queryString;
		std::vector<std::wstring> key = {};

		switch (type)
		{
		case HardwareType::BIOS:
			queryString = L"SELECT SerialNumber FROM Win32_BIOS";
			key.push_back(L"SerialNumber");
			break;
		case HardwareType::Motherboard:
			queryString = L"SELECT SerialNumber FROM Win32_BaseBoard";
			key.push_back(L"SerialNumber");
			break;
		case HardwareType::HardDrive:
			//名称 和 编号
			queryString = L"SELECT SerialNumber FROM Win32_DiskDrive";
			key.push_back(L"SerialNumber");
			break;
		case HardwareType::CPU:
			queryString = L"SELECT ProcessorId FROM Win32_Processor";
			key.push_back(L"ProcessorId");
			break;
		case HardwareType::GraphicsCard:
			queryString = L"SELECT DeviceID, Name FROM Win32_VideoController";
			key.push_back(L"DeviceID");
			key.push_back(L"Name");
			break;
		case HardwareType::NetworkAdapter:
			queryString = L"SELECT Caption, MACAddress FROM Win32_NetworkAdapter";
			key.push_back(L"Caption");
			key.push_back(L"MACAddress");
			break;
		case HardwareType::PhysicalMemory:
			queryString = L"SELECT Capacity, Speed, Manufacturer FROM Win32_PhysicalMemory";
			key.push_back(L"Capacity");
			key.push_back(L"Speed");
			key.push_back(L"Manufacturer");
			break;
		case HardwareType::SoundDevice:
			queryString = L"SELECT Name, Manufacturer FROM Win32_SoundDevice";
			key.push_back(L"Name");
			key.push_back(L"Manufacturer");
			break;
		case HardwareType::OperatingSystem:
			queryString = L"SELECT Caption, Version, Manufacturer FROM Win32_OperatingSystem";
			key.push_back(L"Caption");
			key.push_back(L"Version");
			key.push_back(L"Manufacturer");
			break;
		default:
			break;
		}

		hres = pSvc->ExecQuery(
			bstr_t(queryLanguage.c_str()),
			bstr_t(queryString.c_str()),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			CXX_NULLPTR,
			&pEnumerator);

		if (FAILED(hres))
		{
			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
			return serialNumber;
		}

		IWbemClassObject* pclsObj = CXX_NULLPTR;
		ULONG uReturn = 0;

		while (pEnumerator)
		{
			HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

			if (0 == uReturn)
			{
				break;
			}

			for (const auto& k : key)
			{
				std::wstring value;
				VARIANT vtProp;
				VariantInit(&vtProp);
				hr = pclsObj->Get(k.c_str(), 0, &vtProp, 0, 0);
				if (SUCCEEDED(hr))
				{
					if (vtProp.vt == VT_BSTR)
					{
						value = vtProp.bstrVal;
					}
					else if (vtProp.vt == VT_I4)
					{
						value = std::to_wstring(vtProp.lVal);
					}
					else if (vtProp.vt == VT_UI4)
					{
						value = std::to_wstring(vtProp.ulVal);
					}
					else if (vtProp.vt == VT_I8)
					{
						value = std::to_wstring(vtProp.llVal);
					}
					else if (vtProp.vt == VT_UI8)
					{
						value = std::to_wstring(vtProp.ullVal);
					}
					else if (vtProp.vt == VT_R8)
					{
						value = std::to_wstring(vtProp.dblVal);
					}
					else if (vtProp.vt == VT_R4)
					{
						value = std::to_wstring(vtProp.fltVal);
					}
					else if (vtProp.vt == VT_BOOL)
					{
						value = vtProp.boolVal ? L"true" : L"false";
					}
					else if (vtProp.vt == VT_DATE)
					{
						value = std::to_wstring(vtProp.date);
					}
					else if (vtProp.vt == VT_NULL)
					{
						value = L"";
					}
					else
					{
						value = L"";
					}
				}
				VariantClear(&vtProp);

				if (static_cast<__int64>(value.size()) > 0)
				{
					serialNumber.append(value);
				}
			}

			pclsObj->Release();
		}

		pEnumerator->Release();
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();

		return serialNumber;
	}

	CXXString commandLine()
	{
		return GetCommandLineW();
	}

	bool isProcess64Bit(__int64 processId)
	{
		bool is64BitProcess = false;

		HANDLE hProcess = NULL;
		if (0 == processId)
		{
			hProcess = GetCurrentProcess();
		}
		else
		{
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, static_cast<DWORD>(processId));
			if (NULL == hProcess)
			{
				std::wcerr << L"无法打开PID为 " << processId << L" 的进程。错误码：" << GetLastError() << std::endl;
				return false;
			}
		}

		USHORT processMachine = 0, nativeMachine = 0;

		// 直接使用 IsWow64Process2 函数，无需动态获取
		if (IsWow64Process2(hProcess, &processMachine, &nativeMachine))
		{
			if (IMAGE_FILE_MACHINE_AMD64 == nativeMachine || IMAGE_FILE_MACHINE_ARM64 == nativeMachine)
			{
				// 在64位系统上
				is64BitProcess = (IMAGE_FILE_MACHINE_AMD64 == processMachine || IMAGE_FILE_MACHINE_ARM64 == processMachine);
			}
			else
			{
				// 在32位系统上
				is64BitProcess = false;
			}
		}
		else
		{
			// IsWow64Process2 不可用，回退到 IsWow64Process
			BOOL isWow64 = FALSE;
			if (IsWow64Process(hProcess, &isWow64))
			{
				SYSTEM_INFO si = { 0 };
				GetNativeSystemInfo(&si);
				if (PROCESSOR_ARCHITECTURE_AMD64 == si.wProcessorArchitecture || PROCESSOR_ARCHITECTURE_ARM64 == si.wProcessorArchitecture)
				{
					// 在64位系统上
					is64BitProcess = !isWow64; // 如果不是WOW64进程，则为64位进程
				}
				else
				{
					// 在32位系统上
					is64BitProcess = false;
				}
			}
			else
			{
				is64BitProcess = false;
			}
		}

		if (0 != processId && NULL != hProcess)
		{
			MINT::NtClose(hProcess);
		}

		return is64BitProcess ? true : false;
	}

	bool sendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, __int64* result, __int64 timeout, __int64 flags)
	{
		DWORD_PTR dwResult = 0;
		LRESULT hr = SendMessageTimeoutW(hWnd, Msg, wParam, lParam, static_cast<UINT>(flags), static_cast<UINT>(timeout), &dwResult);
		if (result != CXX_NULLPTR)
		{
			*result = dwResult;
		}

		return hr == ERROR_SUCCESS;
	}

	bool postMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		return PostMessageW(hWnd, Msg, wParam, lParam) == TRUE;
	}

	bool sendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, __int64* result, __int64 timeout, __int64 flags)
	{
		DWORD_PTR dwResult = 0;
		LRESULT hr = SendMessageTimeoutA(hWnd, Msg, wParam, lParam, static_cast<UINT>(flags), static_cast<UINT>(timeout), &dwResult);
		if (result != CXX_NULLPTR)
		{
			*result = dwResult;
		}

		return hr == ERROR_SUCCESS;
	}

	bool postMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		return PostMessageA(hWnd, Msg, wParam, lParam) == TRUE;
	}

	LRESULT callWindowProc(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		return CallWindowProcW(lpPrevWndFunc, hWnd, Msg, wParam, lParam);
	}

	LRESULT callWindowProcA(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		return CallWindowProcA(lpPrevWndFunc, hWnd, Msg, wParam, lParam);
	}

	bool messageBox(const CXXString& text, const CXXString& caption, HWND parentHWnd, UINT type, __int64* result, __int64 timeout)
	{
		if (timeout == -1)
		{
			return MessageBoxW(parentHWnd, text.c_str(), caption.c_str(), type) == IDOK;
		}

		// MessageBoxTimeoutW from user32.dll
		HMODULE hUser32 = GetModuleHandleW(L"user32.dll");
		if (hUser32 == CXX_NULLPTR)
		{
			return false;
		}

		using MessageBoxTimeoutW_t = int(WINAPI*)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType, WORD wLanguageId, DWORD dwMilliseconds, PDWORD_PTR dwResult);

		DWORD_PTR dwResult = 0;
		LRESULT hr = -1;
		MessageBoxTimeoutW_t MessageBoxTimeoutW = reinterpret_cast<MessageBoxTimeoutW_t>(GetProcAddress(hUser32, "MessageBoxTimeoutW"));

		if (MessageBoxTimeoutW != CXX_NULLPTR)
		{
			hr = MessageBoxTimeoutW(parentHWnd, text.c_str(), caption.c_str(), type, 0, static_cast<DWORD>(timeout), &dwResult);

			if (result != CXX_NULLPTR)
			{
				*result = dwResult;
			}
		}

		return hr == IDOK;
	}

	bool shellExecute(const CXXString& operation, const wchar_t* file, const wchar_t* parameters, const wchar_t* directory, int showCmd, HWND parentHwnd)
	{
		return ShellExecuteW(parentHwnd, operation.c_str(), file, parameters, directory, showCmd) > reinterpret_cast<HINSTANCE>(32);
	}

	void exit(UINT exitCode)
	{
		::exit(exitCode);
	}

	bool terminateProcess(HANDLE hProcess, UINT exitCode)
	{
		if (hProcess == CXX_NULLPTR)
		{
			TerminateProcess(GetCurrentProcess(), exitCode);
			return false;
		}

		return TerminateProcess(hProcess, exitCode) == TRUE;
	}
}