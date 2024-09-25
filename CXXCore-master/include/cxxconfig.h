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
#ifndef CXXCONFIG_H_
//@隐藏{
#define CXXCONFIG_H_
//@隐藏}

//@隐藏{
#pragma execution_character_set("utf-8")

#include <sdkddkver.h>

/** Transform expression \a _exp_ to string format. */
#define __AUX_STR_EXP(_exp_)                    #_exp_
/** Transform \a _exp_ to string format. */
#define ___AUX_STR(_exp_)                       __AUX_STR_EXP(_exp_)
/** location file name and file line */
#define __LOCATION_STR                          __FILE__ "("  ___AUX_STR(__LINE__) ")"
/** define warning message throw */
#define throw_warning(_code_, _message_)        message(__LOCATION_STR ": warning C" ___AUX_STR(_code_) ": " _message_)
#define throw_error(_level_, _code_, _message_) message(__LOCATION_STR ":" _level_ " error C" ___AUX_STR(_code_) ": " _message_)

/* CX module only supports Windows operating systems. */
#if !defined(_WIN32) && !defined(_WIN64)
#define CXX_ERROR_OS_NOT_SUPPORTED
#pragma throw_error("fatal", 10001, "炫扩展模块仅支持 Windows 操作系统。")
#endif

/* CX module only supports MSVC Compiler. */
#if !defined(_MSC_VER)
#define CXX_ERROR_COMPILER_NOT_SUPPORTED
#pragma throw_error("fatal", 10002, "炫扩展模块仅支持 MSVC 编译器。")
/* CX module only supports MSVC 2019 or above. */
#elif _MSC_VER < 1920
#define CXX_ERROR_COMPILER_VERSION_NOT_SUPPORTED
#pragma throw_error("fatal", 10003, "炫扩展模块仅支持 MSVC 2019 或以上的编译器。")
#endif

/* CX module requires Windows SDK to be defined. */
#if !defined(_WIN32_WINNT)
#define CXX_ERROR_WINSDK_NOT_DEFINED
#pragma throw_error("fatal", 10004, "炫扩展模块需要定义 Windows SDK 版本。")
/* CX module only supports Windows SDK version 10 or above. */
#elif _WIN32_WINNT < 0x0A00
#define CXX_ERROR_WINSDK_VERSION_NOT_SUPPORTED
#pragma throw_error("fatal", 10005, "炫扩展模块仅支持 Windows SDK 为 10 或以上版本")
#endif

/*
199711L 表示 C++98 或 C++03 标准
201103L 表示 C++11 标准
201402L 表示 C++14 标准
201703L 表示 C++17 标准
202002L 表示 C++20 标准

C++ 17 or newer
*/
#if _MSVC_LANG < 201703L
#define CXX_ERROR_CPP_STANDARD_NOT_SUPPORTED
#pragma throw_error("fatal", 10006, "炫扩展模块仅支持 C++17 或以上的标准。如果已切换版本，但还是出现错误请在C++编译选项添加 `/Zc:__cplusplus` 指令来更新")
#endif

#if defined(CXX_ERROR_OS_NOT_SUPPORTED) || \
	defined(CXX_ERROR_COMPILER_NOT_SUPPORTED) || \
	defined(CXX_ERROR_COMPILER_VERSION_NOT_SUPPORTED) || \
	defined(CXX_ERROR_WINSDK_NOT_DEFINED) || \
	defined(CXX_ERROR_WINSDK_VERSION_NOT_SUPPORTED) || \
	defined(CXX_ERROR_CPP_STANDARD_NOT_SUPPORTED)
#error "因为上述错误，炫扩展模块无法继续编译。\
请在项目视图->对项目名点击右键->属性-> \
选择炫语言->SDK改成Windows10->平台工具集改成VS2019或更高"
#endif

//@隐藏}

#endif // CXCONFIG_H_