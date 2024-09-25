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

#ifndef MODULE_CXXMEMORYSAFE_H_
//@隐藏{
#define MODULE_CXXMEMORYSAFE_H_
//@隐藏}

//@模块名称 炫扩展模块-内存安全
//@版本     炫扩展-子模块
//@日期     请参考炫扩展-核心模块
//@作者     Bestkakkoii
//@依赖     "module_cxxcore.h"
//@QQ       1642687802
//@默认安装位置 炫扩展\
//@包含目录 @当前模块路径 "CXXCore-master\include"
//@包含目录 @当前模块路径 "CXXCore-master\3rdparty"

//@模块备注 > ### 专为炫语言设计的开源模块，遵循GPL-3.0开源协议，亦可提供给任意CPP17或以上的项目使用。
// 
// ***免责声明: 本模块只用作技术研究，不针对任何使用场景，用本模块制作的任何软件和本人无关，也不代表任何炫彩官方立场。*** 
// 本模块是一个内存安全的封装，可以智能的管理内存或句柄(HANDLE)的分配和释放。亦或是使用内存池来提高内存的分配效率。
//@模块信息结束

//@模块集合

#ifndef CXX_STATIC
//@src "CXXCore-master\src\cxxcleanuphandler.cpp"
//@src "CXXCore-master\src\cxxmemorypool.cpp"
//@src "CXXCore-master\src\cxxscopedguard.cpp"
//@src "CXXCore-master\src\cxxscopedhandle.cpp"
//@src "CXXCore-master\src\cxxsharedmemory.cpp"
//@src "CXXCore-master\src\cxxsemaphore.cpp"
#endif

//@解析头文件
#include "CXXCore-master\include\cxxmemorylib.h"

#endif // MODULE_CXX_H_
