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
//@版本     看日期就好，版本不重要
//@日期     2024-09-18
//@作者     Bestkakkoii
//@依赖     "module_cxx.h"
//@QQ       1642687802
//@默认安装位置 炫扩展\
//@包含目录 @当前模块路径 "CXXCore-master\include"
//@包含目录 @当前模块路径 "CXXCore-master\3rdparty"

//@模块备注 
//> ### 专门设计给炫语言使用的开源模块，亦可提供给任意CPP17或以上的项目使用。
//***特别声明: 此模块仅本人兴趣製作，不代表炫彩官方立场。***
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
// [VS2022下载地址](https://c2rsetup.officeapps.live.com/c2r/downloadVS.aspx?sku=community&channel=Release&version=VS2022&source=VSLandingPage&cid=2030:4e2e553ab57c41b0bae7ef375d74555a)
//
// [Windows SDK 下载地址](https://go.microsoft.com/fwlink/?linkid=2272523)
//
// # 测试环境
// - **操作系统**: Windows 11 23H2
// - **SDK 版本**: Windows SDK 10.0.26100.0
// - **编译器**: MSVC [Visual Studio 2022 17.11.3]
// - **C++ 标准**: ISO C++17 标准 (/std:C++17)
// - **C 标准**: ISO C17 (2018) 标准 (/std:c17)
// - **系统编码**: CP950 (Big5)
//
// # 基本介绍:
// - 本模块着重于线程安全和低调用难度
// - 次要考量运行速度  ╮(•ω •)╭
//
// * 环境配置 -> 请查看 ./README.md *
// * 历史更新日誌 -> 请查看 `@当前模块路径/CHANGELOG.md` *
// * 书写规范     -> 请查看 `@当前模块路径/CXXCore-master/.gitee/PULL_REQUEST_TEMPLATE/PULL_REQUEST_TEMPLATE.zh-CN.md` *
// * 清理无用缓存文件或已生成的二进制文件 -> 请查看 `@当前模块路径/CleanCache.ps1` *
//
// # 更新日志:
// 
//#### [2024-09-19]()
//  * 修改 将绝大部份模版的显式声明类型移除
//  * 修改 将大多数不必要的 inline 定义移入cpp减少编译后体积
//  * 修改 因SDK兼容问题取消预编译静态库，可以自行编译。请查看 `@当前模块路径/CXXCore-master/vs/CXXCore.sln`
//  * 修改 重新整理目录结构
//
//#### [2024-09-18]()
//  * 修正 修正许多警告、代码错误
//  * 修改 改成静态库的方式，请至群文件下载，放置于 `@当前模块路径/CXXCore-master/lib` 下
//    * 如果不想使用静态库请把所有 `*.h` 中 `//@source` 改成 `//@src`
//    * 并把所有 `@当前模块路径/CXXCore-master/module_cxx.h` 中的 `@lib` 注释掉
//    * 需要 `MT` 或 `MTd` 静态库 请自行使用 `VS2022` 打开 `@当前模块路径/CXXCore-master/vs/CXXCore.sln` 编译
//
//#### [2024-09-16]()
//  * 新增 炫调试增加枚举输出实际名称、增加 typeName 可以获取任意变量的文本类型名称
//  * 新增 炫线程基类 炫线程 增加 索引、置索引的声明，并添加 属性、置属性 用于添加自订数据给线程实例
//  * 新增 炫内存(CXXMemory), 炫远程虚拟内存(CXXVirtualMemory) 提供本地、远程内存相关的操作
//  * 新增 cxxmath.h 提供大量基于 STL 的数学函数(原文无别名)、和常量如PI 位于炫命名空间
//    * 函数 cxx::min cxx::max cxx::abs 与核心模块的宏定义冲突 改为 minc maxc absc
//  * 新增 增加若干全局函数 :
//    * 取命令行, 是否64位, 退出, 终止进程
//    * 发送消息, 提交消息, 调用窗口过程, 消息框, 执行
//  * 未来可能新增 CXXFile 增加根据指定条件或筛选器递归查找文件、枚举文件、查找文件内容
//  * 未来可能新增 CXXDir 增加指定条件或筛选器递归查找目录、枚举目录
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
//@src "CXXCore-master\src\cxxatomic.cpp"
//@src "CXXCore-master\src\cxxcache.cpp"
//@src "CXXCore-master\src\cxxcleanuphandler.cpp"
//@src "CXXCore-master\src\cxxcom.cpp"
//@src "CXXCore-master\src\cxxcontiguouscache.cpp"
//@src "CXXCore-master\src\cxxcrypto.cpp"
//@src "CXXCore-master\src\cxxdate.cpp"
//@src "CXXCore-master\src\cxxdatetime.cpp"
//@src "CXXCore-master\src\cxxdebug.cpp"
//@src "CXXCore-master\src\cxxdir.cpp"
//@src "CXXCore-master\src\cxxelapsedtimer.cpp"
//@src "CXXCore-master\src\cxxfile.cpp"
//@src "CXXCore-master\src\cxxfiledialog.cpp"
//@src "CXXCore-master\src\cxxhash.cpp"
//@src "CXXCore-master\src\cxxmap.cpp"
//@src "CXXCore-master\src\cxxjson.cpp"
//@src "CXXCore-master\src\cxxlibrary.cpp"
//@src "CXXCore-master\src\cxxmemory.cpp"
//@src "CXXCore-master\src\cxxmemorypool.cpp"
//@src "CXXCore-master\src\cxxmutex.cpp"
//@src "CXXCore-master\src\cxxmutexlocker.cpp"
//@src "CXXCore-master\src\cxxpair.cpp"
//@src "CXXCore-master\src\cxxpoint.cpp"
//@src "CXXCore-master\src\cxxprocess.cpp"
//@src "CXXCore-master\src\cxxqueue.cpp"
//@src "CXXCore-master\src\cxxrandom.cpp"
//@src "CXXCore-master\src\cxxreadwritelock.cpp"
//@src "CXXCore-master\src\cxxregex.cpp"
//@src "CXXCore-master\src\cxxscopedguard.cpp"
//@src "CXXCore-master\src\cxxscopedhandle.cpp"
//@src "CXXCore-master\src\cxxscopedpointer.cpp"
//@src "CXXCore-master\src\cxxset.cpp"
//@src "CXXCore-master\src\cxxsetting.cpp"
//@src "CXXCore-master\src\cxxsharedmemory.cpp"
//@src "CXXCore-master\src\cxxstack.cpp"
//@src "CXXCore-master\src\cxxstring.cpp"
//@src "CXXCore-master\src\cxxtextcodec.cpp"
//@src "CXXCore-master\src\cxxthread.cpp"
//@src "CXXCore-master\src\cxxtime.cpp"
//@src "CXXCore-master\src\cxxvariant.cpp"
//@src "CXXCore-master\src\cxxvector.cpp"
//@src "CXXCore-master\src\cxxwintoast.cpp"
//@src "CXXCore-master\src\cxxapplication.cpp"
//@src "CXXCore-master\3rdparty\wintoastlib\src\wintoastlib.cpp"
#endif

//@解析头文件
#include "CXXCore-master\include\cxx.h"

#endif // MODULE_CXX_H_
