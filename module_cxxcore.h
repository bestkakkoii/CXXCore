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

#ifndef MODULE_CXX_H_
//@隐藏{
#define MODULE_CXX_H_
//@隐藏}

//@模块名称 炫扩展模块
//@版本     炫扩展-核心模块
//@日期     2024-09-23
//@作者     Bestkakkoii
//@依赖     "module_cxxcore.h"
//@QQ       1642687802
//@默认安装位置 炫扩展\
//@包含目录 @当前模块路径 "CXXCore-master\include"
//@包含目录 @当前模块路径 "CXXCore-master\3rdparty"

//@模块备注 
// > ### 专为炫语言设计的开源模块，遵循GPL-3.0开源协议，亦可提供给任意CPP17或以上的项目使用。
// 
// ***免责声明: 本模块只用作技术研究，不针对任何使用场景，用本模块制作的任何软件和本人无关，也不代表任何炫彩官方立场。***
// 
// # 基本介绍:
// - 本模块着重于线程安全和低调用难度，为了补充炫语言核心模块的不足而存在。
// - 不依赖于炫语言核心模块，但包含其所有功能， ╮(•ω •)╭ (干掉他~!
// 
// - 本模块必须位于模块列表的第一个位置，以确保环境相关的设置能应用到所有模块。
// - 本模块必须位于模块列表的第一个位置，以确保环境相关的设置能应用到所有模块。
// - 本模块必须位于模块列表的第一个位置，以确保环境相关的设置能应用到所有模块。
// 
// * 环境配置 -> 请查看 `@当前模块路径/README.md` *
// * 历史更新日誌 -> 请查看 `@当前模块路径/CHANGELOG.md` *
// * 书写规范     -> 请查看 `@当前模块路径/CXXCore-master/.gitee/PULL_REQUEST_TEMPLATE/PULL_REQUEST_TEMPLATE.zh-CN.md` *
// * 清理无用缓存文件或已生成的二进制文件 -> 请查看 `@当前模块路径/CleanCache.ps1` *
// ***.MD文件请用记事本、VS Code或任意文本编辑器打开(UTF-8编码)***
// 
// # 适用环境
// - **操作系统**: Windows 10 1903 (10.0.18362) 或更高版本
// - **SDK 版本**: Windows SDK 10.0.18362 或以上
// - **编译器**: MSVC [Visual Studio 2019 16.11.x 或 Visual Studio 2022 17.x 及以上]
// - **C++ 标准**: ISO C++17 标准或更高
// - **C 标准**: ISO C17 (2018) 标准
// - **强制源代码编码/运行环境编码**: UTF-8
// - **炫文本默认编码**: UTF-16LE
// 
// # 最新更新日志:
// #### [2024-09-25]()
//   * 新增 增加字节集和文本数组相关的更多操作函数
//   * 新增 炫分词器，用于文本分词，可以随时操作分词规则
//
//@模块信息结束

//@模块集合

#if 0
#define CXX_STATIC
#endif

#ifdef CXX_STATIC
#ifdef _WIN64
#ifdef _DEBUG
//@lib "lib\CXXCorex64d.lib"
#else
//@lib "lib\CXXCorex64.lib"
#endif
#else
#ifdef _DEBUG
//@lib "lib\CXXCored.lib"
#else
//@lib "lib\CXXCore.lib"
#endif
#endif
#endif

#ifndef CXX_STATIC
//@src "CXXCore-master\src\cxxcore.cpp"
//@src "CXXCore-master\src\cxxglobal.cpp"
//@src "CXXCore-master\src\cxxmap.cpp"
//@src "CXXCore-master\src\cxxvector.cpp"
//@src "CXXCore-master\src\cxxvariant.cpp"
//@src "CXXCore-master\src\cxxelapsedtimer.cpp"
//@src "CXXCore-master\src\cxxscopedpointer.cpp"
//@src "CXXCore-master\src\cxxstring.cpp"
//@src "CXXCore-master\src\cxxregex.cpp"
//@src "CXXCore-master\src\cxxtextcodec.cpp"
//@src "CXXCore-master\src\cxxdir.cpp"
//@src "CXXCore-master\src\cxxfile.cpp"
//@src "CXXCore-master\src\cxxthread.cpp"
//@src "CXXCore-master\src\cxxdebug.cpp"
#endif

//@解析头文件
#include "CXXCore-master\include\cxxcore.h"

#endif // MODULE_CXX_H_
