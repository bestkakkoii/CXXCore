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

#ifndef CXX_H_
//@隐藏{
#define CXX_H_
//@隐藏}


//@分组{［全局］
//@分组}

/* 全局 */
#pragma region Global
//@隐藏{
#include <cxxconfig.h>
//@隐藏}

//@解析头文件
#include <cxxglobal.h>
#pragma endregion

//@分组{［内存安全］
//@分组}

/* 内存安全 */
#pragma region MemorySafe
//@解析头文件
#include <cxxscopedpointer.h>

//@解析头文件
#include <cxxmemorypool.h>

//@解析头文件
#include <cxxscopedhandle.h>

//@解析头文件
#include <cxxsharedmemory.h>

//@解析头文件
#include <cxxcleanuphandler.h>

//@解析头文件
#include <cxxscopedguard.h>

#pragma endregion

//@分组{［容器］
//@分组}

/* 容器 */
#pragma region Container
//@解析头文件
#include <cxxpoint.h>

//@解析头文件
#include <cxxvector.h>

//@解析头文件
#include <cxxqueue.h>

//@解析头文件
#include <cxxstack.h>

//@解析头文件
#include <cxxhash.h>

//@解析头文件
#include <cxxmap.h>

//@解析头文件
#include <cxxcache.h>

//@解析头文件
#include <cxxcontiguouscache.h>

//@解析头文件
#include <cxxpair.h>

//@解析头文件
#include <cxxset.h>

//@解析头文件
#include <cxxvariant.h>
#pragma endregion

//@分组{［工具］
//@分组}

/* 工具 */
#pragma region Utility
//@解析头文件
#include <cxxmath.h>

//@解析头文件
#include <cxxrandom.h>

//@解析头文件
#include <cxxelapsedtimer.h>

//@解析头文件
#include <cxxmemory.h>
#pragma endregion

//@分组{［文本］
//@分组}

/* 文本 */
#pragma region Text
//@解析头文件
#include <cxxstring.h>

//@解析头文件
#include <cxxtextcodec.h>

//@解析头文件
#include <cxxregex.h>

//@解析头文件
#include <cxxsetting.h>

//@解析头文件
#include <cxxcrypto.h>
#pragma endregion

//@分组{［文件／文件夹］

//@分组}

/* 文件/文件夹 */
#pragma region File
//@解析头文件
#include <cxxdir.h>

//@解析头文件
#include <cxxfile.h>

//@解析头文件
#include <cxxfiledialog.h>

//@解析头文件
#include <cxxjson.h>
#pragma endregion

//@分组{［线程安全］
//@分组}

/* 线程/线程安全 */
#pragma region ThreadSafe
//@解析头文件
#include <cxxatomic.h>

//@解析头文件
#include <cxxsemaphore.h>

//@解析头文件
#include <cxxmutex.h>

//@解析头文件
#include <cxxmutexlocker.h>

//@解析头文件
#include <cxxreadwritelock.h>

//@解析头文件
#include <cxxreadlocker.h>

//@解析头文件
#include <cxxwritelocker.h>

//@解析头文件
#include <cxxthread.h>
#pragma endregion

//@分组{［调试］
//@分组}

/* 调试 */
#pragma region Debug

//@解析头文件
#include <cxxdebug.h>
#pragma endregion

//@分组{［其他］
//@分组}

/* 其他 */
#pragma region Misc
//@解析头文件
#include <cxxwintoast.h>

//@解析头文件
#include <cxxprocess.h>

//@解析头文件
#include <cxxapplication.h>

//@解析头文件
#include <cxxcom.h>

//@解析头文件
#include <cxxlibrary.h>

//@解析头文件
#include <cxxtime.h>

//@解析头文件
#include <cxxdate.h>

//@解析头文件
#include <cxxdatetime.h>
#pragma endregion

/* 弃用 */
#pragma region Deprecated
//@隐藏{
/*
#include <cxresource.h>
*/
//@隐藏}

//@隐藏{
#ifdef _MSC_VER
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "dbghelp.lib")
#endif
//@隐藏}

#pragma endregion

#endif // CXX_H_
