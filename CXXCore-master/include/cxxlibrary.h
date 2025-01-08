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
#ifndef CXXLIBRARY_H_
//@隐藏{
#define CXXLIBRARY_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxstring.h>
#include <cxxscopedpointer.h>

#include <string>
#include <mutex>
#include <unordered_map>
#include <atomic>

//@别名 炫
namespace cxx
{

}

//@分组{［其他］.炫　动态库类

//@隐藏{
class CXXLibraryPrivate;
//@隐藏}

//@备注 常用动态库操作
//@别名 炫动态库
class CXXLibrary
{
public:
	//@参数 插件名称(纯名称、相对路径、绝对路径) .dll扩展名可选
	CXXLibrary(const CXXString& libraryName);

	//@隐藏{
	virtual ~CXXLibrary();
	//@隐藏}

	//@备注 重新加载插件
	//@参数 插件名称(纯名称、相对路径、绝对路径) .dll扩展名可选
	//@返回 是否加载成功
	//@别名 载入(插件名称)
	bool load(const CXXString& libraryName);

	//@备注 获取插件名称
	//@返回 插件名称
	//@别名 文件名()
	CXXString fileName() const;

	//@备注 检查库是否有效
	//@返回 是否有效
	//@别名 是否有效()
	bool isValid() const;

	//@备注 调用 GetLastError() 获取错误叙述，如果失败则直接返回错误代码文本
	//@返回 错误信息文本
	//@别名 错误文本()
	CXXString errorString() const;

	//@备注 取模块地址
	//@返回 模块地址
	//@别名 地址()
	HMODULE data() const;

	//@备注 获取函数地址
	//@参数 函数名称
	//@返回 函数地址
	//@别名 解析(函数名称)
	FARPROC resolve(const CXXString& functionName);

	//@隐藏{
	template<typename ReturnType, typename... Args>
	bool invoke(cxx::CallingConvention conv, ReturnType* returnType, const CXXString& functionName, Args... args)
	{
		std::lock_guard<std::mutex> lock(mutex());
		if (!isValid())
		{
			CXX_ASSERT_X(false, std::wstring(L"Library '") + libraryName() + L"' is not loaded");
			return false;
		}

		ReturnType result = ReturnType();
		if (returnType != CXX_NULLPTR)
		{
			*returnType = result;
		}

		std::wstring key = functionName.toStdWString();

		std::unordered_map<std::wstring, FARPROC>& map = usedFunctions();

		auto iter = map.find(key);
		if (iter == map.end())
		{
			FARPROC procAddress = resolveHelper(functionName);
			if (procAddress == CXX_NULLPTR)
			{
				return false;
			}

			map[key] = procAddress;
		}

		switch (conv)
		{
		case cxx::CallingConvention::StdCall:
		{
			auto func = reinterpret_cast<ReturnType(__stdcall*)(Args...)>(map[key]);
			return callFunction(func, returnType, args...);
		}
		case cxx::CallingConvention::Cdecl:
		{
			auto func = reinterpret_cast<ReturnType(__cdecl*)(Args...)>(map[key]);
			return callFunction(func, returnType, args...);
		}
		case cxx::CallingConvention::FastCall:
		{
			auto func = reinterpret_cast<ReturnType(__fastcall*)(Args...)>(map[key]);
			return callFunction(func, returnType, args...);
		}
		case cxx::CallingConvention::ThisCall:
		{
			auto func = reinterpret_cast<ReturnType(__thiscall*)(Args...)>(map[key]);
			return callFunction(func, returnType, args...);
		}
		case cxx::CallingConvention::VectorCall:
		{
			auto func = reinterpret_cast<ReturnType(__vectorcall*)(Args...)>(map[key]);
			return callFunction(func, returnType, args...);
		}
		default:
		{
			break;
		}
		}

		return false;
	}
	//@隐藏}

	/*@声明
	//@备注 调用函数
	//@参数 conv 调用约定枚举值
	//@参数 returnType 接收返回值的变量指针
	//@参数 functionName 函数名称
	//@参数 args... 参数(可变参数)
	//@返回 是否调用成功
	//@别名 调用(调用约定枚举值, 返回值指针, 函数名称, 参数)
	template<typename ReturnType, typename Args>
	bool invoke(CallingConvention conv, ReturnType* returnType, const string& functionName, Args args);
	*/

	//@备注 隐藏插件
	//@别名 隐藏()
	bool hide();

	//@别名 置标志(标志)
	void setFlags(__int64 flags);

	//@别名 置句柄(文件句柄)
	void setHandle(HANDLE handle);

private:
	//@隐藏{
	std::unordered_map<std::wstring, FARPROC>& usedFunctions();

	std::wstring& libraryName();

	FARPROC resolveHelper(const CXXString& functionName);

	std::mutex& mutex();

	std::atomic<unsigned int>& lastError();

	template<typename FuncType, typename ReturnType, typename... Args>
	bool callFunction(FuncType func, ReturnType* returnType, Args... args)
	{
		try
		{
			SetLastError(0);
			ReturnType result = func(args...);
			lastError().store(GetLastError(), std::memory_order_release);
			if (returnType != CXX_NULLPTR)
			{
				*returnType = result;
			}

			return lastError().load() == 0;
		}
		catch (...)
		{
			CXX_ASSERT_X(false, L"Exception occurred during function call");
			return false;
		}
	}
	//@隐藏}
private:
	//@隐藏{
	CXX_DECLARE_PRIVATE(CXXLibrary);
	CXXScopedPointer<CXXLibraryPrivate> d_ptr;
	//@隐藏}
};


//@分组}

#endif // CXXLIBRARY_H_