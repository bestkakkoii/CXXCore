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
#ifndef CXXMEMORY_H_
//@隐藏{
#define CXXMEMORY_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxstring.h>
#include <cxxvector.h>
#include <cxxscopedpointer.h>

#include <windows.h> 

//@分组{ ［工具］.炫　内存

//@隐藏{
class CXXMemoryPrivate;
//@隐藏}

//@别名 炫内存
class CXXMemory
{
public:
	CXXMemory();

	CXXMemory(__int64 processId, UINT64 baseAddress = 0);

	CXXMemory(HANDLE hProcess, UINT64 baseAddress = 0);

	virtual ~CXXMemory();

	//@备注 静态:读取目标进程中指定大小的内存至当前进程内存中(32到32, 64到64, 64到32)
	//@参数 hProcess 目标进程句柄
	//@参数 baseAddress 目标进程中的地址
	//@参数 size 要读取的字节大小
	//@参数 buffer 用于存放读取内容的缓冲区指针
	//@返回 成功返回true，失败返回false
	//@别名 读(进程句柄, 地址, 大小, 缓冲区指针)
	template<typename Arg>
	static bool read(HANDLE hProcess, UINT64 baseAddress, Arg* buffer, __int64 size);

	//@备注 静态:读取目标进程中指定地址的字服串并以指定编码解析返回utf16le字串(32到32, 64到64, 64到32)
	//@参数 hProcess 目标进程句柄
	//@参数 baseAddress 目标进程中的地址
	//@参数 size 要读取的字节大小
	//@参数 acp ANSI文本ACP码(-1 为 本地编码, 0 为 unicode)
	//@返回 成功返回utf16le字串，失败返回空字串
	//@别名 读(进程句柄, 地址, 大小, 编码)
	static CXXString read(HANDLE hProcess, UINT64 baseAddress, __int64 size, __int64 acp = -1);

	//@备注 静态:根据指定模板类型读取目标进程中指定地址的内存(32到32, 64到64, 64到32)
	//@参数 hProcess 目标进程句柄
	//@参数 baseAddress 目标进程中的地址
	//@返回 成功返回指定类型的数值，失败返回0
	//@别名 读(进程句柄, 地址)
	template<typename T
		//@隐藏{
		, typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_pointer_v<T>>
		//@隐藏}
	>
	static T read(HANDLE hProcess, UINT64 baseAddress);

	//@备注 静态:写入当前进程内存中的指定大小的内容至目标进程中(32到32, 64到64, 64到32)
	//@参数 hProcess 目标进程句柄
	//@参数 baseAddress 目标进程中的地址
	//@参数 buffer 用于存放写入内容的缓冲区指针
	//@参数 size 要写入的字节大小
	//@返回 成功返回true，失败返回false
	//@别名 写(进程句柄, 地址, 缓冲区指针, 大小)
	template<typename Arg>
	static bool write(HANDLE hProcess, UINT64 baseAddress, Arg* buffer, __int64 size);

	//@备注 静态:写入当前进程内存中的指定字串至目标进程中(32到32, 64到64, 64到32)
	//@参数 hProcess 目标进程句柄
	//@参数 baseAddress 目标进程中的地址
	//@参数 str 要写入的字串
	//@参数 acp ANSI文本ACP码(-1 为 本地编码, 0 为 unicode)
	//@返回 成功返回true，失败返回false
	//@别名 写(进程句柄, 地址, 编码, 字串)
	static bool write(HANDLE hProcess, UINT64 baseAddress, const CXXString& str, __int64 acp = -1, __int64* pSize = nullptr);

	//@备注 静态:根据指定模板类型写入指定数值至目标进程中(32到32, 64到64, 64到32)
	//@参数 hProcess 目标进程句柄
	//@参数 baseAddress 目标进程中的地址
	//@参数 data 要写入的数值
	//@返回 成功返回true，失败返回false
	//@别名 写(进程句柄, 地址, 数值)
	template<typename T
		//@隐藏{
		, typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_pointer_v<T>>
		//@隐藏}
	>
	static bool write(HANDLE hProcess, UINT64 baseAddress, T data);

	//@备注 静态:获取目标进程中指定模块的入口地址(32进程取32进程, 64进程取64进程)
	//@参数 processId 目标进程编号
	//@参数 moduleName 模块名
	//@返回 成功返回模块入口地址，失败返回0
	//@别名 远程取模块地址(进程编号, 模块名)
	static UINT64 getRemoteModuleHandle(__int64 processId, const CXXString& moduleName);

	//@备注 静态:获取目标进程中指定模块的入口地址(32进程取32进程, 64进程取64进程, 64进程取32进程)
	//@参数 hProcess 目标进程句柄
	//@参数 moduleName 模块名
	//@返回 成功返回模块入口地址，失败返回0
	//@别名 远程取模块地址(进程句柄, 模块名)
	static UINT64 getRemoteModuleHandle(HANDLE hProcess, const CXXString& moduleName);

	//@备注 静态:获取目标进程中指定模块内函数地址(32进程取32进程, 64进程取64进程, 64进程取32进程)
	//@参数 processId 目标进程编号
	//@参数 moduleName 模块名
	//@参数 FuncName 函数名
	//@返回 成功返回函数地址，失败返回0
	//@别名 远程取模块函数地址(进程句柄, 模块名, 函数名)
	static UINT64 getProcAddress(__int64 processId, const CXXString& moduleName, const CXXString& FuncName);

	//@备注 静态:获取目标进程中指定模块内函数地址(32进程取32进程, 64进程取64进程, 64进程取32进程)
	//@参数 hProcess 目标进程句柄
	//@参数 moduleName 模块名
	//@参数 FuncName 函数名
	//@返回 成功返回函数地址，失败返回0
	//@别名 远程取模块函数地址(进程句柄, 模块名, 函数名)
	static UINT64 getProcAddress(HANDLE hProcess, const CXXString& moduleName, const CXXString& FuncName);

	//@备注 静态:创建远程线程(32或64)
	//@参数 processId 目标进程编号
	//@参数 startRoutine 远程线程函数地址
	//@参数 argument 远程线程函数参数
	//@返回 成功返回远程线程句柄，失败返回0
	//@别名 远程创建线程(进程句柄, 远程线程函数地址, 远程线程函数参数)
	static HANDLE createRemoteThread(__int64 processId, UINT64 startRoutine, void* argument);

	//@备注 静态:创建远程线程(32或64)
	//@参数 hProcess 目标进程句柄
	//@参数 startRoutine 远程线程函数地址
	//@参数 argument 远程线程函数参数
	//@返回 成功返回远程线程句柄，失败返回0
	//@别名 远程创建线程(进程句柄, 远程线程函数地址, 远程线程函数参数)
	static HANDLE createRemoteThread(HANDLE hProcess, UINT64 startRoutine, void* argument);

	//@备注 静态:複製句柄(32或64)
	//@参数 hSourceProcessHandle 源进程句柄
	//@参数 hSourceHandle 源句柄
	//@参数 hTargetProcessHandle 目标进程句柄
	//@参数 dwOptions 选项
	//@返回 成功返回複製后的句柄，失败返回0
	//@别名 远程複製句柄(源进程句柄, 源句柄, 目标进程句柄, 选项)
	static HANDLE duplicateHandle(HANDLE hSourceProcessHandle, HANDLE hSourceHandle, HANDLE hTargetProcessHandle, __int64 dwOptions);

	//@备注 静态:释放目标进程中指定地址的内存(32到32, 64到64, 64到32, 32到64)
	//@参数 hProcess 目标进程句柄
	//@参数 baseAddress 目标进程中的地址
	//@返回 成功返回true，失败返回false
	//@别名 远程释放(进程句柄, 地址)
	static bool virtualFree(HANDLE hProcess, UINT64 baseAddress);

	//@备注 静态:申请目标进程中指定大小的内存(32到32, 64到64, 64到32, 32到64)
	//@参数 hProcess 目标进程句柄
	//@参数 size 要申请的字节大小
	//@返回 成功返回申请到的内存地址，失败返回0
	//@别名 远程申请(进程句柄, 大小)
	static UINT64 virtualAlloc(HANDLE hProcess, __int64 size);

	//@备注 静态:申请目标进程中指定大小的内存并写入指定编码的字串(32到32, 64到64, 64到32)
	//@参数 hProcess 目标进程句柄
	//@参数 str 要写入的字串
	//@参数 acp ANSI文本ACP码(-1 为 本地编码, 0 为 unicode)
	//@参数 size 成功后确实申请到的大小
	//@返回 成功返回申请到的内存地址，失败返回0
	//@别名 远程申请(进程句柄, 编码, 字串)
	static UINT64 virtualAlloc(HANDLE hProcess, const CXXString& str, __int64 acp = -1, UINT64* size = CXX_NULLPTR);

	//@备注 静态:释放目标进程中闲置的内存(32或64)
	//@参数 hProcess 目标进程句柄，如果为负数则释放当前进程中的闲置的内存
	//@返回 无返回值
	//@别名 释放(进程句柄)
	static void freeUnuseMemory(HANDLE hProcess);

	//@备注 静态:注入指定dll至目标进程中(win7 或以上 32 到 32 或 64 到 64)
	//@参数 dwProcessId 目标进程ID
	//@参数 hProcess 目标进程句柄
	//@参数 dllPath dll路径
	//@参数 phDllModule 用于存放注入后dll模块的句柄
	//@参数 hWnd 目标进程主窗口句柄(可选)
	//@返回 成功返回true，失败返回false
	//@别名 注入(进程ID, 进程句柄, dll路径, dll模块句柄, 目标窗口句柄)
	static bool inject(__int64 dwProcessId, HANDLE hProcess, CXXString dllPath, HMODULE* phDllModule, HWND hWnd = CXX_NULLPTR);

	//@备注 静态:注入指定dll至目标进程中(win10 或以上 32 到 32 或 64 到 32)
	//@参数 dwProcessId 目标进程ID
	//@参数 hProcess 目标进程句柄
	//@参数 dllPath dll路径
	//@参数 phDllModule 用于存放注入后dll模块的句柄
	//@参数 hWnd 目标进程主窗口句柄(可选)
	//@返回 成功返回true，失败返回false
	//@别名 注入64到32(进程ID, 进程句柄, dll路径, dll模块句柄, 目标窗口句柄)
	static bool inject64To32(__int64 dwProcessId, HANDLE hProcess, CXXString dllPath, HMODULE* phDllModule, HWND hWnd = CXX_NULLPTR);//兼容64位注入32位

	//@备注 静态:枚举包含指定模块的进程编号(32或64)
	//@参数 pprocesses 容器指针，用于存放枚举到的进程编号
	//@参数 moduleName 指定需要存在的模块名(可选)
	//@参数 withoutModuleName 指定需要排除的模块名(可选)
	//@返回 成功返回true，失败返回false
	//@别名 枚举进程(进程编号容器, 模块名, 排除的模块名)
	static bool enumProcess(CXXVector<__int64>* pprocesses, const CXXString& moduleName = {}, const CXXString& withoutModuleName = {});

	//@备注 静态:根据进程编号判断进程是否存在(32或64)
	//@参数 dwProcessId 进程编号
	//@返回 存在返回true，不存在返回false
	//@别名 进程是否存在(进程编号)
	static bool isProcessExist(__int64 dwProcessId);

	//@备注 静态:提升指定句柄的指定权限(32或64)
	//@参数 hProcess 目标进程句柄
	//@参数 SE 权限名(可选)
	//@返回 成功返回true，失败返回false
	//@别名 进程提权(进程句柄, 权限名)
	static bool enablePrivilege(HANDLE hProcess, const CXXString& SE = SE_DEBUG_NAME);

	//@备注 静态:打开进程(32或64)
	//@参数 dwProcessId 进程编号
	//@返回 成功返回进程句柄，失败返回0
	//@别名 打开句柄(进程编号)
	static HANDLE openProcess(__int64 dwProcess);

	//@备注 静态:关闭句柄(32或64)
	//@参数 handle 句柄
	//@返回 成功返回true，失败返回false
	//@别名 关闭句柄(句柄)
	static bool closeHandle(HANDLE handle);

	//@备注 静态:通过进程编号获取窗口句柄(32或64)
	//@参数 dwProcessId 进程编号, 如果为-1则获取当前进程首个顶级窗口句柄
	//@返回 成功返回窗口句柄，失败返回0
	//@别名 取窗口句柄(进程编号)
	static HWND getWindowHandle(__int64 dwProcessId);

	//@备注 静态:通过窗口句柄获取进程编号(32或64)
	//@参数 hWnd 窗口句柄
	//@返回 成功返回进程编号，失败返回0
	//@别名 取进程编号(窗口句柄)
	static __int64 getProcessId(HWND hWnd);

	//@备注 静态:包含页保护操作的将自身进程指定的缓冲区填充为指定字节值(32或64)
	//@参数 buffer 用于填充的缓冲区指针
	//@参数 size 要填充的字节大小
	//@参数 value 填充的值(0-255)
	//@返回 返回目标缓冲区指针
	//@别名 填充(缓冲区指针, 大小, 值)
	static void* fill(void* buffer, __int64 size, BYTE value);

	//@备注 静态:包含页保护操作的将自身进程指定的缓冲区填充为0(32或64)
	//@参数 buffer 用于填充的缓冲区指针
	//@参数 size 要填充的字节大小
	//@参数 value 填充的值(0-255)
	//@返回 返回目标缓冲区指针
	//@别名 重置(缓冲区指针, 大小, 值)
	static void reset(void* buffer, __int64 size);

	//@备注 静态:包含页保护操作的清空自身进程的内存(32或64)
	//@参数 buffer 用于清空的缓冲区指针 (填充0)
	//@参数 size 要清空的字节大小
	//@返回 返回目标缓冲区指针
	//@别名 清空(缓冲区指针, 大小)
	static void* clear(void* buffer, __int64 size);

	//@备注 静态:包含页保护操作的複製自身进程的内存(32或64)
	//@参数 dst 目标缓冲区指针
	//@参数 src 源缓冲区指针
	//@参数 size 要複製的字节大小
	//@返回 返回目标缓冲区指针
	//@别名 複製(目标缓冲区指针, 源缓冲区指针, 大小)
	template<class T, class T2>
	static void* copy(T* dis, T2* src, __int64 size);

	//@备注 静态:包含页保护操作的移动自身进程的内存(32或64) [推荐]
	//@参数 dis 目标缓冲区指针
	//@参数 src 源缓冲区指针
	//@参数 size 要移动的字节大小
	//@返回 返回目标缓冲区指针
	//@别名 移动(目标缓冲区指针, 源缓冲区指针, 大小)
	template<class T, class T2>
	static void* move(T* dis, T2* src, __int64 size);

	//@备注 静态:複製自身进程的内存(32或64)  [不推荐，建议使用move]
	//@参数 dst 目标缓冲区指针
	//@参数 src 源缓冲区指针
	//@参数 size 要複製的字节大小
	//@参数 offset 偏移量
	//@返回 返回目标缓冲区指针
	//@别名 複製(目标缓冲区指针, 源缓冲区指针, 大小, 偏移量)
	static void* copy(void* dst, const void* src, __int64 size, UINT64 offset);

	//@备注 静态:比较自身进程的内存(32或64)
	//@参数 dst 目标缓冲区指针
	//@参数 src 源缓冲区指针
	//@参数 size 要比较的字节大小
	//@返回 相同返回0，不同返回非0
	//@别名 比较(目标缓冲区指针, 源缓冲区指针, 大小)
	static __int64 compare(const void* dst, const void* src, __int64 size);

	//@备注 静态:取自身进程类函数地址(32或64)
	//@参数 addr 用于存放函数地址的指针
	//@参数 f 函数指针
	//@返回 无返回值
	//@别名 取类函数地址(函数地址指针, 函数指针)
	template <class ToType, class FromType>
	static void getFuncAddr(ToType* addr, FromType f);

	//@备注 静态:HOOK函数(32或64)
	//@参数 pfnHookFunc 要 CALL 或 JMP 的函数或地址
	//@参数 bOri 要写入HOOK的地址
	//@参数 bOld 用于保存原始数据的BYTE数组，取决于写入地址原始汇编占多大
	//@参数 bNew 用于写入跳转或CALL的BYTE数组要预先填好需要填充的0x90或0xE8 0xE9
	//@参数 nByteSize bOld bNew数组的大小
	//@参数 offest 有时候跳转目标地址前面可能会有其他东西会需要跳过则需要偏移，大部分时候为 0
	//@返回 无返回值
	//@别名 钩(函数或地址, 要写入的地址, 原始数据BYTE数组, 跳转或CALL的BYTE数组, BYTE数组大小, 偏移量)
	template<class T, class T2>
	static void detour(T pfnHookFunc, __int64 bOri, T2* bOld, T2* bNew, const __int64 nByteSize, const __int64 offest);

	//@备注 静态:取消HOOK(32或64)
	//@参数 ori 要还原的地址段
	//@参数 oldBytes 备份用的BYTE数组指针
	//@参数 size BYTE数组大小
	//@返回 无返回值
	//@别名 取消钩(地址, 备份用的BYTE数组指针, BYTE数组大小)
	template<class T>
	static void undetour(T ori, BYTE* oldBytes, __int64 size);

	//@备注 静态:多线程查找二进制数据(32或64)
	//@参数 hProcess 目标进程句柄
	//@参数 input 要查找的二进制数据字节集
	//@参数 baseAddress 查找的起始地址
	//@参数 output 用于存放查找到的所有地址的容器指针
	//@返回 成功返回true，失败返回false
	//@别名 查找(进程句柄, 二进制数据字节集, 起始地址, 输出容器指针)
	static bool find(HANDLE hProcess, CXXVector<BYTE> input, __int64 baseAddress, CXXVector<__int64>* output);

public:
	//@备注 使用指定进程编号来打开进程句柄
	//@参数 processId 目标进程编号
	//@返回 成功返回true，失败返回false
	//@别名 打开(进程编号)
	bool open(__int64 processId);

	//@备注 设置指定进程句柄
	//@参数 h 目标进程句柄
	//@返回 无
	//@别名 置句柄(进程句柄)
	void setHandle(HANDLE h);

	//@备注 设置指定地址
	//@参数 baseAddress 目标进程中的地址
	//@返回 无
	//@别名 置地址(地址)
	void setBaseAddress(UINT64 baseAddress);

	//@备注 从已保存的缓存数据恢复到预设的地址
	//@参数 无
	//@返回 成功返回true，失败返回false
	//@别名 恢复()
	bool restore();

	//@备注 将之前写入的数据重新写入到预设的地址
	//@参数 无
	//@返回 成功返回true，失败返回false
	//@别名 重写()
	bool rewrite();

	//@备注 读取目标进程中指定大小的内存至当前进程内存中(32到32, 64到64, 64到32)
	//@参数 baseAddress 目标进程中的地址
	//@参数 size 要读取的字节大小
	//@参数 buffer 用于存放读取内容的缓冲区指针
	//@返回 成功返回true，失败返回false
	//@别名 读(进程句柄, 地址, 大小, 缓冲区指针)
	template<typename Arg>
	bool read(UINT64 baseAddress, Arg* buffer, __int64 size);

	//@备注 读取目标进程中指定地址的字服串并以指定编码解析返回utf16le字串(32到32, 64到64, 64到32)
	//@参数 baseAddress 目标进程中的地址
	//@参数 size 要读取的字节大小
	//@参数 acp ANSI文本ACP码(-1 为 本地编码, 0 为 unicode)
	//@返回 成功返回utf16le字串，失败返回空字串
	//@别名 读(进程句柄, 地址, 大小, 编码)
	CXXString read(UINT64 baseAddress, __int64 size, __int64 acp = -1);

	//@备注 根据指定模板类型读取目标进程中指定地址的内存(32到32, 64到64, 64到32)
	//@参数 baseAddress 目标进程中的地址
	//@返回 成功返回指定类型的数值，失败返回0
	//@别名 读(进程句柄, 地址)
	template<typename T
		//@隐藏{
		, typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_pointer_v<T>>
		//@隐藏}
	>
	T read(UINT64 baseAddress);

	//@备注 写入当前进程内存中的指定大小的内容至目标进程中(32到32, 64到64, 64到32)
	//@参数 baseAddress 目标进程中的地址
	//@参数 buffer 用于存放写入内容的缓冲区指针
	//@参数 size 要写入的字节大小
	//@返回 成功返回true，失败返回false
	//@别名 写(进程句柄, 地址, 缓冲区指针, 大小)
	template<typename Arg>
	bool write(UINT64 baseAddress, Arg* buffer, __int64 size);

	//@备注 写入当前进程内存中的指定字串至目标进程中(32到32, 64到64, 64到32)
	//@参数 baseAddress 目标进程中的地址
	//@参数 str 要写入的字串
	//@参数 acp ANSI文本ACP码(-1 为 本地编码, 0 为 unicode)
	//@返回 成功返回true，失败返回false
	//@别名 写(进程句柄, 地址, 编码, 字串)
	bool write(UINT64 baseAddress, const CXXString& str, __int64 acp = -1);

	//@备注 写入当前进程内存中的指定字串至目标进程中(32到32, 64到64, 64到32)
	//@参数 baseAddress 目标进程中的地址
	//@参数 data 要写入的数值
	//@返回 成功返回true，失败返回false
	//@别名 写(进程句柄, 地址, 编码, 字串)
	template<typename T>
	bool write(UINT64 baseAddress, T data);

	//@备注 读取目标进程中指定大小的内存至当前进程内存中(32到32, 64到64, 64到32)
	//@参数 baseAddress 目标进程中的地址
	//@参数 size 要读取的字节大小
	//@参数 buffer 用于存放读取内容的缓冲区指针
	//@返回 成功返回true，失败返回false
	//@别名 读(进程句柄, 地址, 大小, 缓冲区指针)
	template<typename Arg>
	bool read(__int64 size, Arg* buffer);

	//@备注 读取目标进程中指定地址的字服串并以指定编码解析返回utf16le字串(32到32, 64到64, 64到32)
	//@参数 baseAddress 目标进程中的地址
	//@参数 size 要读取的字节大小
	//@参数 acp ANSI文本ACP码(-1 为 本地编码, 0 为 unicode)
	//@返回 成功返回utf16le字串，失败返回空字串
	//@别名 读(进程句柄, 地址, 大小, 编码)
	CXXString read(__int64 size, __int64 acp = -1);

	//@备注 根据指定模板类型读取目标进程中指定地址的内存(32到32, 64到64, 64到32)
	//@参数 baseAddress 目标进程中的地址
	//@返回 成功返回指定类型的数值，失败返回0
	//@别名 读(进程句柄, 地址)
	template<typename T
		//@隐藏{
		, typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_pointer_v<T>>
		//@隐藏}
	>
	T read();

	//@备注 写入当前进程内存中的指定大小的内容至目标进程中(32到32, 64到64, 64到32)
	// 会自动备份到缓存中，可调用restore()恢复或rewrite()重写
	//@参数 baseAddress 目标进程中的地址
	//@参数 buffer 用于存放写入内容的缓冲区指针
	//@参数 size 要写入的字节大小
	//@返回 成功返回true，失败返回false
	//@别名 写(进程句柄, 地址, 缓冲区指针, 大小)
	template<typename Arg>
	bool write(Arg* buffer, __int64 size);

	//@备注 写入当前进程内存中的指定字串至目标进程中(32到32, 64到64, 64到32)
	// 会自动备份到缓存中，可调用restore()恢复或rewrite()重写
	//@参数 baseAddress 目标进程中的地址
	//@参数 acp ANSI文本ACP码(-1 为 本地编码, 0 为 unicode)
	//@参数 str 要写入的字串
	//@返回 成功返回true，失败返回false
	//@别名 写(进程句柄, 地址, 编码, 字串)
	bool write(const CXXString& str, __int64 acp = -1);

	//@备注 根据指定模板类型写入指定数值至目标进程中(32到32, 64到64, 64到32)
	// 会自动备份到缓存中，可调用restore()恢复或rewrite()重写
	//@参数 baseAddress 目标进程中的地址
	//@参数 data 要写入的数值
	//@返回 成功返回true，失败返回false
	//@别名 写(进程句柄, 地址, 数值)
	template<typename T
		//@隐藏{
		, typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_pointer_v<T>>
		//@隐藏}
	>
	bool write(T data);

	//@备注 静态:创建远程线程(32或64)
	//@参数 startRoutine 远程线程函数地址 (正长整型)
	//@参数 argument 远程线程函数参数
	//@返回 成功返回远程线程句柄，失败返回0
	//@别名 远程创建线程(进程句柄, 远程线程函数地址, 远程线程函数参数)
	HANDLE createRemoteThread(UINT64 startRoutine, void* argument);
private:
	CXX_DECLARE_PRIVATE(CXXMemory);
	CXXScopedPointer<CXXMemoryPrivate> d_ptr;
};

//@别名 炫远程虚拟内存
class CXXVirtualMemory
{
public:
	CXXVirtualMemory() = default;

	//@参数 进程编号
	//@参数 大小
	//@参数 是否自动释放
	CXXVirtualMemory(__int64 processId, __int64 size, bool autoClear);

	//@参数 进程编号
	//@参数 字串
	//@参数 编码
	//@参数 是否自动释放
	CXXVirtualMemory(__int64 processId, const CXXString& str, __int64 acp, bool autoClear);

	//@参数 进程句柄
	//@参数 大小
	//@参数 是否自动释放
	CXXVirtualMemory(HANDLE h, __int64 size, bool autoClear);

	//@参数 进程句柄
	//@参数 字串
	//@参数 编码
	//@参数 是否自动释放
	CXXVirtualMemory(HANDLE h, const CXXString& str, __int64 acp, bool autoClear);

	//@参数 炫远程虚拟内存
	CXXVirtualMemory(const CXXVirtualMemory& other);

	virtual ~CXXVirtualMemory();

	//@备注 此内存地址是否为空
	//@返回 是返回true，否返回false
	//@别名 是否为空()
	bool isNull() const;

	//@备注 提供指定内存指针，大小，并比较其数据是否与远程内存相同
	//@参数 data 当前进程内存指针
	//@参数 size 参数1的内存大小
	//@返回 相同返回true，不同返回false
	//@别名 是否相同(当前进程内存指针, 大小)
	bool isData(BYTE* data, __int64 size) const;

	//@备注 将远程内存置0 (依据申请大小清0)
	//@返回 无返回值
	//@别名 清空()
	void clear() const;

	//@备注 判断远程内存是否有效
	//@返回 有效返回true，无效返回false
	//@别名 是否有效()
	bool isValid() const;

	//@隐藏{
	CXXVirtualMemory(CXXVirtualMemory&& other) noexcept;

	CXXVirtualMemory& operator=(CXXVirtualMemory&& other) noexcept;

	template<typename T>
	operator T* () const
	{
		return reinterpret_cast<T*>(lpAddress_);
	}

	template<typename T>
	operator T() const
	{
		return (T)(lpAddress_);
	}

	CXXVirtualMemory& operator=(__int64 other);

	CXXVirtualMemory* operator&();

	CXXVirtualMemory& operator=(const CXXVirtualMemory& other);

	friend constexpr inline bool operator==(const CXXVirtualMemory& p1, const CXXVirtualMemory& p2)
	{
		return p1.lpAddress_ == p2.lpAddress_;
	}

	friend constexpr inline bool operator!=(const CXXVirtualMemory& p1, const CXXVirtualMemory& p2)
	{
		return p1.lpAddress_ != p2.lpAddress_;
	}
	//@隐藏}

private:

	//@隐藏{
	bool autoClear_ = false;
	UINT64 lpAddress_ = NULL;
	UINT64 size_ = 0;
	HANDLE hProcess_ = CXX_NULLPTR;
	bool hasTempHandle_ = false;
	//@隐藏}
};

//@分组}



#endif // CXXMEMORY_H_