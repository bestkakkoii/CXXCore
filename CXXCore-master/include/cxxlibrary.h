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
#ifndef CXXLIBRARY_H_
//@隐藏{
#define CXXLIBRARY_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxstring.h>
#include <cxxatomic.h>
#include <cxxscopedpointer.h>

#include <string>
#include <mutex>
#include <unordered_map>

//@别名 炫
namespace cxx
{
	//@分组{［其他］.炫　动态库类

	//@别名 炫調用約定
	enum CallingConvention
	{
		StdCall,	 //@别名 标准调用约定
		Cdecl,		 //@别名 C调用约定
		FastCall,   //@别名 快速调用约定
		ThisCall,   //@别名 类成员调用约定
		VectorCall, //@别名 向量调用约定
	};

	//@别名 炫库标志
	enum LibraryFlags
	{
		//DONT_RESOLVE_DLL_REFERENCES
		DontResolveDllReferences = 0x00000001,		  //@别名 不解析动态库引用
		//LOAD_LIBRARY_AS_DATAFILE
		LoadLibraryAsDataFile = 0x00000002,			  //@别名 作为数据文件载入
		//LOAD_WITH_ALTERED_SEARCH_PATH
		LoadWithAlteredSearchPath = 0x00000008,		  //@别名 使用更改的搜索路径载入
		//LOAD_IGNORE_CODE_AUTHZ_LEVEL
		LoadIgnoreCodeAuthzLevel = 0x00000010,		  //@别名 忽略代码授权级别
		//LOAD_LIBRARY_AS_IMAGE_RESOURCE 
		LoadLibraryAsImageResource = 0x00000020,	  //@别名 作为图像资源载入
		//LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE
		LoadLibraryAsDataFileExclusive = 0x00000040,  //@别名 作为独占数据文件载入
		//LOAD_LIBRARY_REQUIRE_SIGNED_TARGET 
		LoadLibraryRequireSignedTarget = 0x00000080,  //@别名 要求签名目标载入
		//LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR
		LoadLibrarySearchDllLoadDir = 0x00000100,     //@别名 搜索动态库载入目录
		//LOAD_LIBRARY_SEARCH_APPLICATION_DIR
		LoadLibrarySearchApplicationDir = 0x00000200, //@别名 搜索应用程序目录
		//LOAD_LIBRARY_SEARCH_USER_DIRS
		LoadLibrarySearchUserDirs = 0x00000400,		  //@别名 搜索用户目录
		//LOAD_LIBRARY_SEARCH_SYSTEM32
		LoadLibrarySearchSystem32 = 0x00000800,		  //@别名 搜索系统32目录
		//LOAD_LIBRARY_SEARCH_DEFAULT_DIRS
		LoadLibrarySearchDefaultDirs = 0x00001000,	  //@别名 搜索默认目录
		//LOAD_LIBRARY_SAFE_CURRENT_DIRS
		LoadLibrarySafeCurrentDirs = 0x00002000,	  //@别名 安全搜索当前目录
	};

	//@分组}
}

//@分组{［其他］.炫　动态库类

//@隐藏{
class CXXLibraryPrivate;
//@隐藏}

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
		std::unique_lock<std::mutex> lock(mutex());
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

	CXXAtomicUInt32& lastError();

	template<typename FuncType, typename ReturnType, typename... Args>
	bool callFunction(FuncType func, ReturnType* returnType, Args... args)
	{
		try
		{
			SetLastError(0);
			ReturnType result = func(args...);
			lastError() = GetLastError();
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