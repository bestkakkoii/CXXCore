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

#ifndef CXXGLOBAL_H_
//@隐藏{
#define CXXGLOBAL_H_
//@隐藏}

#pragma execution_character_set("utf-8")

#include <windows.h>
#include <string>
#include <type_traits>
#include <iostream>

#ifndef MINT_USE_SEPARATE_NAMESPACE
//@隐藏{
#define MINT_USE_SEPARATE_NAMESPACE
//@隐藏}
#include <../3rdparty/MINT/MINT.h>
#endif

//@隐藏{
class CXXString;
template<typename T>
class CXXVector;
//@隐藏}

//@隐藏{
#if defined(_MSC_VER)
#define CXX_CC_MSVC
#endif

#if defined(_WIN32) || defined(_WIN64)
#define CXX_OS_WIN
#endif

#if defined(__INTEL_COMPILER)
#define CXX_CC_INTEL
#endif

#if defined(CXX_CC_MSVC)
#if defined(__cplusplus)

#define CXX_COMPILER_STATIC_ASSERT

#endif // __cplusplus
#endif // CXX_CC_MSVC

//@隐藏}

//@分组{［全局］.炫　ＣＰＰ关键字
#pragma region Keyword
//@备注 命名空间开始 "namespace name { "
// 用途: 命名空间开始
//@参数 命名空间名
//@别名 炫_命名空间开始
#define CXX_BEGIN_NAMESPACE(name) namespace name { 

//@备注 命名空间结束 "}"
// 用途: 命名空间结束
//@别名 炫_命名空间结束
#define CXX_END_NAMESPACE }

//@备注 "using namespace name"
// 用途: 使用命名空间
//@参数 命名空间名
//@别名 炫_使用命名空间
#define CXX_USING_NAMESPACE(name) using namespace name

//@备注 "using namespace cxx"
// 不建议使用
// 不建议使用
// 不建议使用
// 不建议使用
// 不建议使用
// 用途: 使用 炫 命名空间
//@别名 置炫扩展命名空间
#define CXX_USE_NAMESPACE using namespace cxx

//@备注 constexpr
// 用途: 常量表达式，编译期间计算
//@别名 炫_常量表达式
#define CXX_CONSTEXPR constexpr

//@备注 ...
// 用途: 可变参数
//@别名 炫_可变参数
#define CXX_ELLIPSIS ...

//@备注 decltype
// 用途: 获取表达式的类型
// 炫_类型(表达式)
//@参数 表达式
//@返回 表达式的类型
//@别名 炫_类型
#define CXX_DECLTYPE(expr) decltype(expr)

//@备注 noexcept  (C++11)
// 用途: 表示函数不会抛出异常
// 炫_不抛出异常
//@别名 炫_不抛出异常
#define CXX_NOEXCEPT noexcept

//@备注 override
// 用途: 表示函数是复盖基类的虚函数
// 炫_复盖
//@别名 炫_复盖
#define CXX_OVERRIDE override

//@备注 final
// 用途: 表示类是最终类，不能被继承
// 炫_最终
//@别名 炫_最终
#define CXX_FINAL final

//@备注 and
// 用途: 逻辑与
// 炫_逻辑与
//@别名 炫_逻辑与
#define CXX_AND and

//@备注 and_eq
// 用途: 位与赋值
// 炫_位与赋值
//@别名 炫_位与赋值
#define CXX_EQ and_eq

//@备注 asm
// 用途: 内联汇编
// 炫_内联汇编
//@别名 炫_内联汇编
#define CXX_ASM __asm

//@备注 bitand
// 用途: 位与
// 炫_位与
//@别名 炫_位与
#define CXX_BITAND bitand

//@备注 bitor
// 用途: 位或
// 炫_位或
//@别名 炫_位或
#define CXX_BITOR bitor

//@备注 compl
// 用途: 按位取反
// 炫_按位取反
//@别名 炫_按位取反
#define CXX_COMPL compl

//@备注 const_cast
// 用途: 常量转换，用于去除const和volatile属性
// 炫_常量转换
// 参数 type 转换后的类型
// 参数 value 要转换的值
//@别名 炫_常量转换
#define CXX_CONST_CAST(type, value) const_cast<type>(value)

//@备注 dynamic_cast
// 用途: 动态转换, 用于将基类指针或引用转换为派生类指针或引用
// 炫_动态转换
// 参数 type 转换后的类型
// 参数 value 要转换的值
//@别名 炫_动态转换
#define CXX_DYNAMIC_CAST(type, value) dynamic_cast<type>(value)

//@备注 explicit
// 用途: 显式构造函数
// 炫_显式
//@别名 炫_显式
#define CXX_EXPLICIT explicit

//@备注 export
// 用途: 导出模板
// 炫_导出
//@别名 炫_导出
#define CXX_EXPORT export

//@备注 extern
// 用途: 用于多文件共享全局变量或函数
// 炫_外部
//@别名 炫_外部
#define CXX_EXTERN_API extern

//@备注 extern
// 用途: 用于多文件共享全局变量
// 炫_外部
//@别名 炫_外部声明
#define CXX_EXTERN(a) extern a

//@备注 占位 {
// 用途: 占位符
// 炫_作用域开始
//@别名 炫_作用域开始
#define CXX_BEGIN_FIELD {

//@备注 占位 }
// 用途: 占位符
// 炫_作用域结束
//@别名 炫_作用域结束
#define CXX_END_FIELD }

//@备注 false
// 用途: 逻辑假
// 炫_假
//@别名 炫_假
#define CXX_FALSE false

//@备注 friend
// 用途: 友元
// 炫_友元
//@别名 炫_友元
#define CXX_FRIEND friend

//@备注 mutable
// 用途: 可变
// 炫_可变
//@别名 炫_可变
#define CXX_MUTABLE mutable

//@备注 namespace
// 用途: 命名空间
// 炫_命名空间
//@别名 炫_命名空间
#define CXX_NAMESPACE namespace

//@备注 not
// 用途: 逻辑非
// 炫_逻辑非
//@别名 炫_逻辑非
#define CXX_NOT not

//@备注 not_eq
// 用途: 位非赋值
// 炫_位非赋值
//@别名 炫_位非赋值
#define CXX_NOT_EQ not_eq

//@备注 operator
// 用途: 运算符
// 炫_运算符
//@别名 炫_运算符
#define CXX_OPERATOR operator

//@备注 or
// 用途: 逻辑或
// 炫_逻辑或
//@别名 炫_逻辑或
#define CXX_OR or

//@备注 or_eq
// 用途: 位或赋值
// 炫_位或赋值
//@别名 炫_位或赋值
#define CXX_OR_EQ or_eq

//@备注 register
// 用途: 寄存器
// 使用方法: register int i = 0;
// 炫_寄存器
//@别名 炫_寄存器
#define CXX_REGISTER register

//@备注 reinterpret_cast
// 用途: 重新解释转换
// 炫_重新解释转换
//@别名 炫_重新解释转换
#define CXX_REINTERPRET_CAST(type, value) reinterpret_cast<type>()

//@备注 signed
// 用途: 有符号
// 炫_有符号
//@别名 炫_有符号
#define CXX_SIGNED signed

//@备注 thread_local (C++11)
// 用途: 线程局部存储(TLS)
// 炫_线程局部存储
//@别名 炫_线程局部存储
#define CXX_THREAD_LOCAL thread_local

//@备注 throw
// 用途: 抛出异常
// 炫_抛出
//@别名 炫_抛出
#define CXX_THROW throw

//@备注 true
// 用途: 逻辑真
// 炫_真
//@别名 炫_真
#define CXX_TRUE true

//@备注 typedef
// 用途: 类型定义
// 炫_类型定义
//@别名 炫_类型定义
#define CXX_TYPEDEF typedef

//@备注 typeid
// 用途: 获取类型信息
// 炫_类型信息
//@别名 炫_类型信息
#define CXX_TYPEID typeid

//@备注 union
// 用途: 联合，union是一种特殊的类，它的所有成员共用同一块内存
// 炫_联合
//@别名 炫_联合
#define CXX_UNION union

//@备注 using
// 用途: 别名 比如 typedef int myint 等同 using myint = int;
// 炫_别名
//@别名 炫_别名
#define CXX_USING using

//@备注 volatile
// 用途: 易失性
// 炫_易失性
//@别名 炫_易失性
#define CXX_VOLATILE volatile

//@备注 xor
// 用途: 逻辑异或
// 炫_逻辑异或
//@别名 炫_逻辑异或
#define CXX_XOR xor

//@备注 xor_eq
// 用途: 位异或赋值
// 炫_位异或赋值
//@别名 炫_位异或赋值
#define CXX_XOR_EQ xor_eq

#if __cplusplus >= 201703L
//@备注 concept (C++20)
// 用途: 概念， 用于模板约束
// 使用方法: template <typename T> concept Integral = std::is_integral<T>::value;
// 炫_概念
//@别名 炫_概念
#define CXX_CONCEPT concept

//@备注 consteval (C++20)
// 用途: 常量表达式函数， 用于在编译期间计算
// 使用方法: consteval int f(int n) { return n * n; }
// 炫_常量表达式函数
//@别名 炫_常
#define CXX_CONSTEVAL consteval

//@备注 constinit (C++20)
// 用途: 常量初始化， 用于初始化常量
// 使用方法: constinit int n = 10;
// 炫_常量初始化
//@别名 炫_常量初始化
#define CXX_CONSTINIT constinit

//@备注 requires (C++20)
// 用途: 要求， 用于模板约束
// 使用方法: template <typename T> requires std::is_integral<T>::value void f(T n) { }
// 炫_要求
//@别名 炫_要求
#define CXX_REQUIRE requires
#endif
#pragma endregion
//@分组}

//@分组{［全局］.炫　ＣＰＰ基本类型
#pragma region BasicType
//@备注 auto
// 用途: 自动类型推导
// 炫_自动
//@别名 炫_自动
#define CXX_AUTO auto

//@备注 constexpr auto
// 用途: 自动类型推导
// 炫_自动
//@别名 炫_常量自动
#define CXX_CONSTEXPR_AUTO constexpr auto

//@备注 枚举开始
// 用途: 创建枚举
// 炫_枚举
//@别名 炫_枚举开始
#define CXX_ENUM_BEGIN enum {

//@备注 枚举结束
// 用途: 创建枚举
// 炫_枚举
//@别名 炫_枚举结束
#define CXX_ENUM_END };

//@备注 nullptr
// 用途: 空指针，在C++11中引入，对于现代C++代码中应该使用nullptr而不是NULL或0
// 炫_空指针
//@别名 炫_空指针
#define CXX_NULLPTR nullptr

typedef unsigned char BYTE;				     //@别名 炫字节型
typedef unsigned char* cxxbyteptr;			 //@别名 炫字节型指针
#if __cplusplus >= 202002L
typedef char8_t cxchar8;					 //@别名 炫字符型U8
#endif
typedef char16_t cxxchar16;					 //@别名 炫字符型U16
typedef char32_t cxxchar32;					 //@别名 炫字符型U32

//@备注 对应 WINAPI 的 HRESULT
//@别名 炫_结果类型
enum CXResultType
	//@隐藏{
	: UINT64
	//@隐藏}
{
	CXX_OK = S_OK,										 //@别名 炫_成功
	CXX_NO_TRUE = S_FALSE,								 //@别名 炫_不为真
	CXX_UNEXPECTED = E_UNEXPECTED,						 //@别名 炫_意外
	CXX_NOTIMPL = E_NOTIMPL,							 //@别名 炫_未实现
	CXX_OUTOFMEMORY = E_OUTOFMEMORY,					 //@别名 炫_内存不足
	CXX_INVALIDARG = E_INVALIDARG,						 //@别名 炫_无效参数
	CXX_NOINTERFACE = E_NOINTERFACE,					 //@别名 炫_无接口
	CXX_POINTER = E_POINTER,							 //@别名 炫_指针
	CXX_HANDLE = E_HANDLE,								 //@别名 炫_句柄
	CXX_ABORT = E_ABORT,								 //@别名 炫_中止
	CXX_FAIL = E_FAIL,									 //@别名 炫_失败
	CXX_ACCESSDENIED = E_ACCESSDENIED,					 //@别名 炫_拒绝访问
	CXX_PENDING = E_PENDING,							 //@别名 炫_挂起
	CXX_BOUNDS = E_BOUNDS,								 //@别名 炫_越界
	CXX_CHANGED_STATE = E_CHANGED_STATE,				 //@别名 炫_改变状态
	CXX_ILLEGAL_STATE_CHANGE = E_ILLEGAL_STATE_CHANGE,   //@别名 炫_非法状态改变
	CXX_ILLEGAL_METHOD_CALL = E_ILLEGAL_METHOD_CALL,	 //@别名 炫_非法方法调用
};

//@备注 if 炫_是否成功(hr)
#define CXX_SUCCESS(hr) (((LRESULT)(hr)) >= 0ULL) //@别名 炫_是否成功

//@备注 if 炫_是否成功(hr)
#define CXX_FAILED(hr) (((LRESULT)(hr)) < 0ULL)   //@别名 炫_是否失败
#pragma endregion

//@分组}

//@别名 炫
namespace cxx
{
	//@分组{［全局］.炫　ＧＬＯＢ全局函数

	//@隐藏{
	using CXXIgnore = std::_Ignore;
	//@隐藏}

		//@隐藏{
	inline constexpr
		//@隐藏}
		// 与 炫_不使用(x) 类似 但 用法如 cxx::ignore = GetTickCount(); 
		//@别名 忽略
		CXXIgnore ignore = {};

	//@隐藏{
	extern
		//@隐藏}
	//@备注 用途: 主动引发程序致命错误并弹出消息框显示错误信息
	//@参数 当前文件名: 传入__FILE__宏
	//@参数 当前行号 传入__LINE__宏
	//@参数 message 要输出的信息
	//@返回 无
	//@别名 致命错误信息(文件名,行号,信息)
		void message_fatal(const wchar_t* contextFileL, size_t line, const std::wstring& message);

	//@隐藏{
	extern
		//@隐藏}
	//@备注 将数值保存到进程环境变量中
	//@参数 name 环境变量名
	//@参数 value 环境变量值
	//@返回 是否成功
	//@别名 置环境变量(名称,值)
		bool putenv(const wchar_t* name, const wchar_t* value);

	//@隐藏{
	extern
		//@隐藏}
	//@备注 获取进程环境变量的值
	//@参数 name 环境变量名
	//@返回 环境变量值
	//@别名 取环境变量(名称)
		CXXString getenv(const wchar_t* name);

	//@隐藏{
	extern
		//@隐藏}
	//@备注 设置系统环境变量
	//@参数 name 环境变量名
	//@参数 value 环境变量值
	//@返回 是否成功
	//@别名 置系统变量(名称,值)
		bool putsysenv(const wchar_t* name, const wchar_t* value);

	//@隐藏{
	extern
		//@隐藏}
	//@备注 取系统变量的值
	//@参数 name 环境变量名
	//@返回 变量值
	//@别名 取系统变量(名称)
		CXXString getsysenv(const wchar_t* name);

	//@隐藏{
	extern
		//@隐藏}
	//@备注 异步执行bat文件, 即便当前进程退出, 也不会影响bat文件的执行
	//@参数 path bat文件路径
	//@参数 data bat文件内容
	//@返回 是否成功
	//@别名 异步批处理(路径,内容)
		bool asyncRunBat(const CXXString& path, const CXXString& data);

	//@隐藏{
	extern
		//@隐藏}
	//@备注 获取当前进程已加载的所有模块名称
	//@返回 模块名称列表
	//@别名 取模块名列表()
		CXXVector<CXXString> getLibraryPaths();

	//@备注 硬件类型
	//@别名 硬件类型
	enum HardwareType
	{
		BIOS,				//@别名 BIOS
		Motherboard,		//@别名 主板
		HardDrive,			//@别名 硬盘
		CPU,				//@别名 处理器
		GraphicsCard,		//@别名 显卡
		NetworkAdapter,		//@别名 网卡
		PhysicalMemory,		//@别名 内存
		SoundDevice,        //@别名 声卡
		OperatingSystem,    //@别名 操作系统
	};

	//@隐藏{
	extern
		//@隐藏}
	//@备注 获取硬件序列号
	//@参数 硬件类型
	//@返回 序列号列表
	//@别名 取硬件序列号(硬件类型)
		CXXVector<CXXString> getHardwareSerialNumber(HardwareType type);

	//@隐藏{
	extern
		//@隐藏}
	//@备注 获取当前进程的命令行
	//@别名 取命令行()
		CXXString commandLine();

	//@隐藏{
	extern
		//@隐藏}
	//@备注 检查当前进程或指定进程是否为64位进程
	//@参数 进程ID
	//@返回 是否为64位进程
	//@别名 是否64位(进程ID)
		bool isProcess64Bit(__int64 processId);

	//@隐藏{
	extern
		//@隐藏}
	//@备注 发送消息到窗口消息过程 (UNICODE)
	//@参数 窗口句柄
	//@参数 消息
	//@参数 wParam
	//@参数 lParam
	//@参数 接收返回值的指针 (可选)
	//@参数 超时时间 (可选)
	//@参数 标志 (可选)
	//@返回 是否成功
	//@别名 发送消息(窗口句柄,消息,wParam,lParam,返回值指针,超时时间,标志)
		bool sendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, __int64* result = nullptr, __int64 timeout = -1, __int64 flags = SMTO_NORMAL);

	//@隐藏{
	extern
		//@隐藏}
	//@备注 提交消息到窗口消息队列 (UNICODE)
	//@参数 窗口句柄
	//@参数 消息
	//@参数 wParam
	//@参数 lParam
	//@返回 是否成功
	//@别名 提交消息(窗口句柄,消息,wParam,lParam)
		bool postMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	//@隐藏{
	extern
		//@隐藏}
	//@备注 发送消息到窗口消息过程 (ANSI)
	//@参数 窗口句柄
	//@参数 消息
	//@参数 wParam
	//@参数 lParam
	//@参数 接收返回值的指针 (可选)
	//@参数 超时时间 (可选)
	//@参数 标志 (可选)
	//@返回 是否成功
	//@别名 发送消息A(窗口句柄,消息,wParam,lParam,返回值指针,超时时间,标志)
		bool sendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, __int64* result = nullptr, __int64 timeout = -1, __int64 flags = SMTO_NORMAL);

	//@隐藏{
	extern
		//@隐藏}
	//@备注 提交消息到窗口消息队列 (ANSI)
	//@参数 窗口句柄
	//@参数 消息
	//@参数 wParam
	//@参数 lParam
	//@返回 是否成功
	//@别名 提交消息A(窗口句柄,消息,wParam,lParam)
		bool postMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	//@隐藏{
	extern
		//@隐藏}
	//@备注 调用窗口消息过程 (UNICODE)
	//@参数 窗口消息过程指针
	//@参数 窗口句柄
	//@参数 消息
	//@参数 wParam
	//@参数 lParam
	//@返回 返回值
	//@别名 调用窗口过程(窗口消息过程指针,窗口句柄,消息,wParam,lParam)
		LRESULT callWindowProc(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	//@隐藏{
	extern
		//@隐藏}
	//@备注 调用窗口消息过程 (ANSI)
	//@参数 窗口消息过程指针
	//@参数 窗口句柄
	//@参数 消息
	//@参数 wParam
	//@参数 lParam
	//@返回 返回值
	//@别名 调用窗口过程A(窗口消息过程指针,窗口句柄,消息,wParam,lParam)
		LRESULT callWindowProcA(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	//@隐藏{
	extern
		//@隐藏}
	//@备注 系统消息框
	//@参数 文本
	//@参数 标题
	//@参数 父窗口句柄
	//@参数 类型
	//@参数 返回值指针
	//@参数 超时时间
	//@返回 是否成功
	//@别名 消息框(文本,标题,父窗口句柄,类型,返回值指针,超时时间)
		bool messageBox(const CXXString& text, const CXXString& caption, HWND parentHWnd = nullptr, UINT type = MB_OK, __int64* result = 0, __int64 timeout = -1);

	//@隐藏{
	extern
		//@隐藏}
	//@备注 此操作为委托系统操作，与当前进程无关
	//@参数 operation 操作
	// edit: 编辑
	// explore: 浏览
	// find: 查找
	// open: 打开
	// print: 打印
	// runas: 以管理员身份运行
	//@参数 file 文件
	//@参数 parameters 参数
	//@参数 directory 目录
	//@参数 showCmd 显示方式
	//@参数 parentHwnd 父窗口句柄
	//@返回 是否成功
	//@别名 执行(操作,文件,参数,目录,显示方式,父窗口句柄)
		bool shellExecute(const CXXString& operation, const wchar_t* file = {}, const wchar_t* parameters = {}, const wchar_t* directory = {}, int showCmd = SW_SHOWNORMAL, HWND parentHwnd = nullptr);

	//@隐藏{
	extern
		//@隐藏}
	//@备注 退出进程
	//@参数 exitCode 退出码
	//@返回 无
	//@别名 退出(退出码)
		void exit(UINT exitCode = 0);

	//@隐藏{
	extern
		//@隐藏}
	//@备注 强制终止进程
	//@参数 hProcess 进程句柄 (可选)
	//@参数 exitCode 退出码 (可选)
	//@返回 是否成功
	//@别名 终止进程(进程句柄,退出码)
		bool terminateProcess(HANDLE hProcess = nullptr, UINT exitCode = 0);

	//@分组}
}

//@分组}

//@分组{［全局］.炫　ＭＳＶＣ宏
#pragma region MSVCMacro
//@备注 用途: 当函数返回值或任意变量未使用时，可以使用此宏来消除编译器警告
// 炫_不使用(变量)
//@别名 炫_未使用
#  define CXX_UNUSED(x) (void)x;

#  ifdef __cplusplus
#  ifdef CXX_COMPILER_STATIC_ASSERT
//@备注 static_assert(bool(Condition), #Condition)
// 用于在编译期间检查一个条件是否为真
// 炫_静态断言(条件)
//@参数 Condition 条件
//@别名 炫_静态断言
#  define CXX_STATIC_ASSERT(Condition) static_assert(bool(Condition), #Condition)

//@备注 static_assert(bool(Condition), Message)
// 用于在编译期间检查一个条件是否为真
// 炫_静态断言(条件,信息)
//@参数 Condition 条件
//@参数 Message 信息
//@别名 炫_静态断言文本
#  define CXX_STATIC_ASSERT_X(Condition, Message) static_assert(bool(Condition), Message)
#  endif
#  endif

//#  define CXX_CC_MSVC_NET
//@备注 inline
// 用于标记一个函数是可以尝试内联的
// 炫_内联
//@别名 炫_内联
#  define CXX_OUTOFLINE_TEMPLATE inline
//#  define CXX_COMPILER_MANGLES_RETURN_TYPE

//@备注 __FUNCSIG__
// 用于获取函数签名
// 炫_函数信息
//@别名 炫_函数信息
#  define CXX_FUNCSIG CXXString::fromUtf8(__FUNCSIG__)

//@备注 __FUNCTION__
// 用于获取函数名
// 炫_函数名
//@别名 炫_函数名
#  define CXX_FUNCTION CXXString::fromUtf8(__FUNCTION__)

//@备注 __FILE__
// 用于获取当前文件名
// 炫_文件名
//@别名 炫_文件名
#  define CXX_FILE CXXString::fromUtf8(__FILE__)

//@备注 __LINE__
// 用于获取当前行号
// 炫_行号
//@别名 炫_行号
#  define CXX_LINE __LINE__

//@备注 __DATE__
// 用于获取编译日期
// 炫_日期
//@别名 炫_日期
#  define CXX_DATE __DATE__

//@备注 __TIME__
// 用于获取编译时间
// 炫_时间
//@别名 炫_时间
#  define CXX_TIME __TIME__

//@备注 __COUNTER__
// 用于获取一个递增的计数器
// 炫_计数器
//@别名 炫_计数器
#  define CXX_COUNTER __COUNTER__

//@备注 _WIN32
// 用于判断编译环境是否为Windows 32位
// 炫_是否为Win32
//@别名 炫_是否为Win32
#  define CXX_WIN32 _WIN32

//@备注 _WIN64
// 用于判断编译环境是否为Windows 64位
// 炫_是否为Win64
//@别名 炫_是否为Win64
#  define CXX_WIN64 _WIN64

//@备注 _M_IX86
// 用于判断编译环境是否为x86
// 炫_是否为x86
//@别名 炫_是否为x86
#  define CXX_X86 _M_IX86

//@备注 _M_X64
// 用于判断编译环境是否为x64
// 炫_是否为x64
//@别名 炫_是否为x64
#  define CXX_X64 _M_X64

//@备注 _M_ARM
// 用于判断编译环境是否为ARM
// 炫_是否为ARM
//@别名 炫_是否为ARM
#  define CXX_ARM _M_ARM

//@备注 _M_ARM64
// 用于判断编译环境是否为ARM64
// 炫_是否为ARM64
//@别名 炫_是否为ARM64
#  define CXX_ARM64 _M_ARM64

//@备注 判断是否为调试模式
// 炫_是否为调试模式
//@别名 炫_是否为调试模式
#  define CXX_DEBUG _DEBUG

//@备注 判断是否为发布模式
// 炫_是否为发布模式
//@别名 炫_是否为发布模式
#  define CXX_RELEASE NDEBUG

//@备注 __alignof(type)
// 用于获取类型的对齐方式
// 炫_对齐方式(类型)
//@参数 type 类型
//@返回 对齐方式
//@别名 炫_对齐方式
#  define CXX_ALIGNOF(type) __alignof(type)

//@备注 __declspec(align(n))
// 用于标记一个变量的对齐方式
// 炫_对齐(n)
//@参数 n 对齐方式
//@别名 炫_对齐
#  define CXX_DECL_ALIGN(n) __declspec(align(n))

//@备注 __assume(expr)
// 用于标记一个条件永远为真
// 炫_假设(条件)
//@参数 expr 条件
//@别名 炫_假设
#  define CXX_ASSUME_IMPL(expr) __assume(expr)

//@备注 __assume(0)
// 用于假设一个条件永远为假
// 炫_永远为假
//@别名 炫_永远为假
#  define CXX_UNREACHABLE_IMPL() __assume(0)

//@备注 __declspec(noreturn)
// 用于标记一个函数永远不会返回
// 炫_永远不返回
//@别名 炫_永远不返回
#  define CXX_NORETURN __declspec(noreturn)

//@备注 __declspec(deprecated)
// 用于标记一个函数或类已经过时，不建议使用
// 炫_过时
//@别名 炫_过时
#  define CXX_DEPRECATED __declspec(deprecated)
#  ifndef CXX_CC_CLANG
//@备注 __declspec(deprecated(text))
// 用于标记一个函数或类已经过时，不建议使用
// 炫_过时(文本)
//@参数 text 过时文本
//@别名 炫_过时文本
#  define CXX_DEPRECATED_X(text) __declspec(deprecated(text))
#  endif
//@备注 __declspec(dllexport)
// 用于标记一个函数或类是导出的
// 炫_导出
//@别名 炫_动态库导出
#  define CXX_DECL_EXPORT __declspec(dllexport)
//@备注 __declspec(dllimport)
// 用于标记一个函数或类是导入的
// 炫_导入
//@别名 炫_动态库导入
#  define CXX_DECL_IMPORT __declspec(dllimport)

//@备注 stdext::make_unchecked_array_iterator(x)
// 用于创建一个指向数组的迭代器，该迭代器不会检查数组的边界
// 炫_创建不检查数组迭代器(数组)
//@参数 x 数组
//@返回 数组迭代器
//@别名 炫_创建不检查数组迭代器
#  define CXX_MAKE_UNCHECKED_ARRAY_ITERATOR(x) stdext::make_unchecked_array_iterator(x) // Since _MSC_VER >= 1800

//@备注 stdext::make_checked_array_iterator(x, size_t(N))
// 用于创建一个指向数组的迭代器，该迭代器会检查数组的边界
// 炫_创建检查数组迭代器(数组,元素个数)
//@参数 x 数组
//@参数 N 元素个数
//@返回 数组迭代器
//@别名 炫_创建检查数组迭代器
#  define CXX_MAKE_CHECKED_ARRAY_ITERATOR(x, N) stdext::make_checked_array_iterator(x, size_t(N)) // Since _MSC_VER >= 1500

//@备注 (sizeof(arr) / sizeof(arr[0]))
// 用于计算数组元素个数
// CXX_COUNT_OF(数组)
//@别名 炫_计算数组元素个数
#  define CXX_COUNT_OF(arr) (sizeof(arr) / sizeof(arr[0]))

#  ifndef CXX_DEPRECATED_VARIABLE
//@备注 CXX_DEPRECATED
// 用于标记一个变量已经过时，不建议使用
// 炫_过时变量
//@别名 炫_过时变量
#  define CXX_DEPRECATED_VARIABLE CXX_DEPRECATED
#  endif

#  ifndef CXX_DEPRECATED_CONSTRUCTOR
//@备注 CXX_DEPRECATED
// 用于标记一个构造函数已经过时，不建议使用
// 炫_过时构造函数
//@别名 炫_过时构造函数
#  define CXX_DEPRECATED_CONSTRUCTOR CXX_DEPRECATED
#  endif

#  ifdef CXX_NO_DEPRECATED // 当不需要过时的宏时，定义CXX_NO_DEPRECATED 来禁用它
#  undef CXX_DEPRECATED
#  undef CXX_DEPRECATED_X
#  undef CXX_DEPRECATED_VARIABLE
#  undef CXX_DEPRECATED_CONSTRUCTOR
#  endif

#ifdef CXX_CC_MSVC
//@备注 __declspec(noinline)
// 用途: 禁止内联
//@别名 炫_禁止内联
#  define CXX_NEVER_INLINE __declspec(noinline)

//@备注 __forceinline
// 用途: 强制内联
//@别名 炫_强制内联
#  define CXX_ALWAYS_INLINE __forceinline
#elif defined(CXX_CC_GNU)
//@备注 __attribute__((noinline))
// 用于禁止内联
// 炫_禁止内联
//@别名 炫_禁止内联
#  define CXX_NEVER_INLINE __attribute__((noinline))

//@备注 __attribute__((always_inline))
// 用于强制内联
// 炫_强制内联
//@别名 炫_强制内联
#  define CXX_ALWAYS_INLINE inline __attribute__((always_inline))
#endif

//@备注 do { try { expr; } catch (...) { std::terminate(); } } while (false)
// 用于在发生异常时终止程序
// 炫_异常终止(expr)
//@参数 expr 表达式
//@别名 炫_异常终止
#define CXX_TERMINATE_ON_EXCEPTION(expr) do { try { expr; } catch (...) { std::terminate(); } } while (false)

#if __cplusplus >= 201703L
//@备注  [[fallthrough]]
// 用途: 标记一个switch分支是有意的穿透
// 炫_穿透
//@别名 炫_穿透
#define CXX_FALLTHROUGH [[fallthrough]]

//@备注   [[nodiscard]]
// 用途: 标记一个函数当返回值未使用时会产生警告
// 炫_不使用返回值警告
//@别名 炫_不使用返回值警告
#define CXX_NODISCARD [[nodiscard]]
#else
//@备注   (当前非C++17 忽略此宏)
//@别名 炫_穿透
#define CXX_FALLTHROUGH
//@备注   (当前非C++17 忽略此宏)
//@别名 炫_不使用返回值警告
#define CXX_NODISCARD
#endif // __cplusplus >= 201703L

#if __cplusplus > 201703L
//@备注   [[likely]]
// 用途: 标记一个条件是有可能为真的
// 炫_可能为真
//@别名 炫_可能为真
#define CXX_LIKELY [[likely]]

//@备注   [[unlikely]]
// 用途: 标记一个条件是有可能为假的
// 炫_可能为假
//@别名 炫_可能为假
#define  CXX_UNLIKELY [[unlikely]]

//备注   [[nodiscard("reason")]]
// 用途: 标记一个函数当返回值未使用时会产生警告
// 炫_不使用返回值警告文本(原因)
//@参数 原因
//@别名 炫_不使用返回值警告文本(原因)
#define CXX_NODISCARD_X(reason) [[nodiscard(reason)]]
#else
//@备注   (当前非C++20 忽略此宏)
//@别名 炫_可能为真
#define CXX_LIKELY
//@备注   (当前非C++20 忽略此宏)
//@别名 炫_可能为假
#define CXX_UNLIKELY
//@备注   (当前非C++20 忽略此宏)
//@别名 炫_不使用返回值警告文本(原因)
#define CXX_NODISCARD_X(reason) (void)reason;
#endif // __cplusplus > 201703L

//@备注   [[carries_dependency]]
// 用途: 标记一个变量是有依赖的
// 炫_依赖
//@别名 炫_依赖
#define CXX_CARRIES_DEPENDENCY [[carries_dependency]]

//@备注 
// Class(const Class &) = delete; 
// Class &operator=(const Class &) = delete;
//@别名 炫_禁止拷贝
#define CXX_DISABLE_COPY(Class) Class(const Class &) = delete; Class &operator=(const Class &) = delete;

//@备注
// Class(Class &&) = delete;
// Class &operator=(Class &&) = delete;
//@别名 炫_禁止移动
#define CXX_DISABLE_MOVE(Class) Class(Class &&) = delete; Class &operator=(Class &&) = delete;

//@备注 
// CXX_DISABLE_COPY(Class)
// CXX_DISABLE_MOVE(Class)
// public: static Class& getInstance() { static Class instance;return instance; }
//@别名 炫_禁止拷贝和移动
#define CXX_DISABLE_COPY_MOVE(Class) CXX_DISABLE_COPY(Class) CXX_DISABLE_MOVE(Class) public: static Class& getInstance() { static Class instance;return instance; }

#ifndef CXX_INSTANCE
//@备注 Class& g_##Class = Class::getInstance()
//@别名 炫_取单例(类名)
#define CXX_INSTANCE(Class) Class& g_##Class = Class::getInstance()
#endif

//@隐藏{
#define CXX_WARNING_PUSH
#define CXX_WARNING_POP
#define CXX_WARNING_DISABLE_GCC (warnings)

template <typename T>
inline T* cxxGetPtrHelper(T* ptr)
{
	return ptr;
}

template <typename Ptr>
inline auto cxxGetPtrHelper(Ptr& ptr) -> decltype(ptr.operator->())
{
	return ptr.operator->();
}

#define _CXX_DECLARE_PRIVATE(Class) \
    inline Class##Private* d_func() \
    { return reinterpret_cast<Class##Private *>(cxxGetPtrHelper(d_ptr)); } \
    inline const Class##Private* d_func() const \
    { return reinterpret_cast<const Class##Private *>(cxxGetPtrHelper(d_ptr)); } \
    friend class Class##Private;

#define _CXX_DECLARE_PRIVATE_D(Dptr, Class) \
    inline Class##Private* d_func() \
    { return reinterpret_cast<Class##Private *>(cxxGetPtrHelper(Dptr)); } \
    inline const Class##Private* d_func() const \
    { return reinterpret_cast<const Class##Private *>(cxxGetPtrHelper(Dptr)); } \

#define _CXX_DECLARE_PUBLIC(Class)                                    \
    inline Class* q_func() { return static_cast<Class *>(q_ptr); } \
    inline const Class* q_func() const { return static_cast<const Class *>(q_ptr); } \
    friend class Class;

//@隐藏}

//@别名 炫_声明私有类
#define CXX_DECLARE_PRIVATE(Class) _CXX_DECLARE_PRIVATE(Class)
//@别名 炫_声明公开类
#define CXX_DECLARE_PUBLIC(Class) _CXX_DECLARE_PUBLIC(Class)
//@别名 炫_声明私有指针
#define CXX_D(Class) Class##Private * const d = d_func()
//@别名 炫_声明公开指针
#define CXX_Q(Class) Class * const q = q_func()

//@隐藏{
template<typename Enum> class CXXFlags;
//@隐藏}

//@备注 typedef CXXFlags<Enum> Flags;
// 用途: 将枚举值转换为位标誌
// 炫_定义位标誌(位标誌名,枚举名)
//@参数 Flags 位标誌名
//@参数 Enum 枚举名
//@别名 炫_定义位标誌(位标誌名,枚举名)
#define CXX_DECLARE_FLAGS(Flags, Enum) typedef CXXFlags<Enum> Flags

#ifdef _DEBUG
//@备注 cxx::message_fatal(__FILEW__, __LINE__, L"ASSERT: " + std::wstring(L#cond)); } while (false)
// 用途: 断言一个条件是否为真，如果条件为假则弹出消息框显示错误信息(注意非调试模式下不会弹出消息框)
// 炫_断言(条件)
//@参数 cond 条件
//@别名 炫_断言(条件)
#define CXX_ASSERT( cond ) do { if (!(cond)) cxx::message_fatal(__FILEW__, __LINE__, L"ASSERT: " + std::wstring(L#cond)); } while (false)

//@备注 do { if (!(cond)) cxx::message_fatal(__FILEW__, __LINE__, what); } while (false)
// 用途: 断言一个条件是否为真，如果条件为假则弹出消息框显示错误信息(注意非调试模式下不会弹出消息框)
// 用法: 举例来说 CXX_ASSERT_X(1 == 2, "main", "1不等于2")
//@参数 cond 条件
//@参数 what 信息
//@别名 炫_断言讯息(条件,位置,信息)
#define CXX_ASSERT_X(cond, what) do { if (!(cond)) cxx::message_fatal(__FILEW__, __LINE__, std::wstring(what)); } while (false)
#else
//@备注 do { } while (false)
// 用途: 断言一个条件是否为真，如果条件为假则弹出消息框显示错误信息(注意非调试模式下不会弹出消息框)
// 炫_断言(条件)
//@参数 cond 条件
//@别名 炫_断言(条件)
#define CXX_ASSERT(cond) do { } while (false)
//@备注 do { } while (false)
// 用途: 断言一个条件是否为真，如果条件为假则弹出消息框显示错误信息(注意非调试模式下不会弹出消息框)
// 用法: 举例来说 CXX_ASSERT_X(1 == 2, "main", "1不等于2")
//@参数 cond 条件
//@参数 where 位置
//@参数 what 信息
//@别名 炫_断言讯息(条件,位置,信息)
#define CXX_ASSERT_X(cond, what) do { } while (false)
#endif

//@备注 do { if (!(cond)) cxx::message_fatal(__FILEW__, __LINE__, L"ASSUME: " + std::wstring(L#cond)); } while (false)
// 用途: 断言一个条件是否为真，如果条件为假则弹出消息框显示错误信息(注意非调试模式也会弹出消息框)
// 炫_假设断言(条件)
//@参数 cond 条件
//@别名 炫_假设断言(条件)
#define CXX_ASSUME(cond) do { if (!(cond)) cxx::message_fatal(__FILEW__, __LINE__, L"ASSUME: " + std::wstring(L#cond)); } while (false)
#pragma endregion
//@分组}

//@分组{［全局］.炫　ＣＰＰ标准模板库［部分］
#pragma region STL
//@备注 std::move(var)
// 用途: 将一个变量转换为右值
// 炫_转移(变量)
//@参数 var 变量
//@返回 右值
//@别名 炫_转移
#define CXX_MOVE(var) std::move(var)

//@备注 std::move_backward
// 用途: 将一个区间的值移动到另一个区间
// 炫_向后转移(区间1,区间2)
//@参数 区间1
//@参数 区间2
// @返回 
//@别名 炫_向后转移
#define CXX_MOVE_BACKWARD(s, d) std::move_backward(std::begin(s), std::end(s), std::end(d))

//@备注 std::forward<decltype(var)>(var)
// 用途: 完美转发
// 炫_完美转发(变量)
//@参数 var 变量
//@返回 完美转发
//@别名 炫_完美转发
#define CXX_FORWARD_X(var) std::forward<decltype(var)>(var)

//@备注 std::swap(a, b)
// 用途: 交换两个变量的值
// 炫_交换(变量1,变量2)
//@参数 a 变量1
//@参数 b 变量2
//@别名 炫_交换
#define CXX_SWAP(a, b) std::swap(a, b)

//@备注 swap_ranges
// 用途: 交换两个区间的值
// 炫_交换区间(区间1,区间2)
//@参数 区间1
//@参数 区间2
//@别名 炫_交换区间
#define CXX_SWAP_RANGES(range1, range2) std::swap_ranges(range1, range2)

//@备注 std::begin
// 用途: 获取容器的起始迭代器
// 炫_容器起始迭代器(容器)
//@参数 容器
//@返回 起始迭代器
//@别名 炫_容器起始迭代器
#define CXX_BEGIN(container) std::begin(container)

//@备注 std::end
// 用途: 获取容器的结束迭代器
// 炫_容器结束迭代器(容器)
//@参数 容器
//@返回 结束迭代器
//@别名 炫_容器结束迭代器
#define CXX_END(container) std::end(container)

//@备注 std::next
// 用途: 获取迭代器的下一个迭代器
// 炫_下一个迭代器(迭代器)
//@参数 迭代器
//@返回 下一个迭代器
//@别名 炫_下一个迭代器
#define CXX_NEXT(iterator) std::next(iterator)

//@备注 std::prev
// 用途: 获取迭代器的上一个迭代器
// 炫_上一个迭代器(迭代器)
//@参数 迭代器
//@返回 上一个迭代器
//@别名 炫_上一个迭代器
#define CXX_PREV(iterator) std::prev(iterator)

//@备注 std::tie
// 用途: 创建一个tuple
// 炫_创建元组(变量1,变量2)
//@参数 变量1
//@参数 变量2
//@返回 元组
//@别名 炫_创建元组
#define CXX_TIE(var1, var2) std::tie(var1, var2)

//@备注 std::sort
// 用途: 对容器进行排序
// 炫_排序(容器)
//@参数 容器
//@别名 炫_排序
#define CXX_SORT(container) std::sort(std::begin(container), std::end(container))

//@备注 std::reverse
// 用途: 对容器进行反转
// 炫_反转(容器)
//@参数 容器
//@别名 炫_反转
#define CXX_REVERSE(container) std::reverse(std::begin(container), std::end(container))

//@备注 std::unique
// 用途: 对容器进行去重
// 炫_去重(容器)
//@参数 容器
//@别名 炫_去重
#define CXX_UNIQUE(container) std::unique(std::begin(container), std::end(container))

//@备注 std::distance
// 用途: 计算两个迭代器之间的距离
// 炫_距离(迭代器1,迭代器2)
//@参数 迭代器1
//@参数 迭代器2
//@返回 距离
//@别名 炫_距离
#define CXX_DISTANCE(iterator1, iterator2) std::distance(iterator1, iterator2)

//@备注 std::find
// 用途: 在容器中查找指定元素
// 炫_查找(容器,元素)
//@参数 容器
//@参数 元素
//@返回 迭代器
//@别名 炫_查找
#define CXX_FIND(container, value) std::find(std::begin(container), std::end(container), value)

//@备注 std::find_if
// 用途: 在容器中查找满足条件的元素
// 炫_查找满足条件(容器,条件)
//@参数 容器
//@参数 条件
//@返回 迭代器
//@别名 炫_查找满足条件
#define CXX_FIND_IF(container, pred) std::find_if(std::begin(container), std::end(container), pred)

//@备注 std::find_if_not
// 用途: 在容器中查找不满足条件的元素
// 炫_查找不满足条件(容器,条件)
//@参数 容器
//@参数 条件
//@返回 迭代器
//@别名 炫_查找不满足条件
#define CXX_FIND_IF_NOT(container, pred) std::find_if_not(std::begin(container), std::end(container), pred)

//@备注 std::count
// 用途: 计算容器中指定元素的个数
// 炫_计数(容器,元素)
//@参数 容器
//@参数 元素
//@返回 个数
//@别名 炫_计数
#define CXX_COUNT(container, value) std::count(std::begin(container), std::end(container), value)

//@备注 std::count_if
// 用途: 计算容器中满足条件的元素的个数
// 炫_计数满足条件(容器,条件)
//@参数 容器
//@参数 条件
//@返回 个数
//@别名 炫_计数满足条件
#define CXX_COUNT_IF(container, pred) std::count_if(std::begin(container), std::end(container), pred)

//@备注 std::all_of
// 用途: 判断容器中所有元素是否都满足条件
// 炫_所有满足条件(容器,条件)
//@参数 容器
//@参数 条件
//@返回 是否满足
//@别名 炫_所有满足条件
#define CXX_ALL_OF(container, pred) std::all_of(std::begin(container), std::end(container), pred)

//@备注 std::any_of
// 用途: 判断容器中是否有元素满足条件
// 炫_有满足条件(容器,条件)
//@参数 容器
//@参数 条件
//@返回 是否满足
//@别名 炫_有满足条件
#define CXX_ANY_OF(container, pred) std::any_of(std::begin(container), std::end(container), pred)

//@备注 std::none_of
// 用途: 判断容器中是否没有元素满足条件
// 炫_没有满足条件(容器,条件)
//@参数 容器
//@参数 条件
//@返回 是否满足
//@别名 炫_没有满足条件
#define CXX_NONE_OF(container, pred) std::none_of(std::begin(container), std::end(container), pred)

//@备注 std::for_each
// 用途: 对容器中的每个元素执行指定操作
// 炫_对每个元素(容器,操作)
//@参数 容器
//@参数 操作
//@别名 炫_对每个元素
#define CXX_FOR_EACH(container, func) std::for_each(std::begin(container), std::end(container), func)

//@备注 std::transform
// 用途: 对容器中的每个元素执行指定操作，并将结果存储到另一个容器中
// 炫_转换(容器1,容器2,操作)
//@参数 容器1
//@参数 容器2
//@参数 操作
//@别名 炫_转换
#define CXX_TRANSFORM(container1, container2, func) std::transform(std::begin(container1), std::end(container1), std::begin(container2), func)

//@备注 std::copy
// 用途: 将容器中的元素复制到另一个容器中
// 炫_复制(容器1,容器2)
//@参数 容器1
//@参数 容器2
//@别名 炫_复制
#define CXX_COPY(container1, container2) std::copy(std::begin(container1), std::end(container1), std::begin(container2))

//@备注 std::copy_if
// 用途: 将容器中满足条件的元素复制到另一个容器中
// 炫_复制满足条件(容器1,容器2,条件)
//@参数 容器1
//@参数 容器2
//@参数 条件
//@别名 炫_复制满足条件
#define CXX_COPY_IF(container1, container2, pred) std::copy_if(std::begin(container1), std::end(container1), std::begin(container2), pred)

//@备注 std::copy_n
// 用途: 将容器中的前n个元素复制到另一个容器中
// 炫_复制n个(容器1,容器2,n)
//@参数 容器1
//@参数 容器2
//@参数 n
//@别名 炫_复制n个
#define CXX_COPY_N(container1, container2, n) std::copy_n(std::begin(container1), n, std::begin(container2))

//@备注 std::copy_backward
// 用途: 将容器中的元素逆向复制到另一个容器中
// 炫_逆向复制(容器1,容器2)
//@参数 容器1
//@参数 容器2
//@别名 炫_逆向复制
#define CXX_COPY_BACKWARD(container1, container2) std::copy_backward(std::begin(container1), std::end(container1), std::end(container2))

//@备注 std::fill
// 用途: 将容器中的元素填充为指定值
// 炫_填充(容器,值)
//@参数 容器
//@参数 值
//@别名 炫_填充
#define CXX_FILL(container, value) std::fill(std::begin(container), std::end(container), value)

//@备注 std::fill_n
// 用途: 将容器中的前n个元素填充为指定值
// 炫_填充n个(容器,值,n)
//@参数 容器
//@参数 值
//@参数 n
//@别名 炫_填充n个
#define CXX_FILL_N(container, value, n) std::fill_n(std::begin(container), n, value)

//@备注 std::generate
// 用途: 用生成器生成容器中的元素
// 炫_生成(容器,生成器)
//@参数 容器
//@参数 生成器
//@别名 炫_生成
#define CXX_GENERATE(container, gen) std::generate(std::begin(container), std::end(container), gen)

//@备注 std::generate_n
// 用途: 用生成器生成容器中的前n个元素
// 炫_生成n个(容器,生成器,n)
//@参数 容器
//@参数 生成器
//@参数 n
//@别名 炫_生成n个
#define CXX_GENERATE_N(container, gen, n) std::generate_n(std::begin(container), n, gen)

//@备注 std::remove
// 用途: 移除容器中的指定元素
// 炫_移除(容器,元素)
//@参数 容器
//@参数 元素
//@别名 炫_移除
#define CXX_REMOVE(container, value) std::remove(std::begin(container), std::end(container), value)

//@备注 std::remove_if
// 用途: 移除容器中满足条件的元素
// 炫_移除满足条件(容器,条件)
//@参数 容器
//@参数 条件
//@别名 炫_移除满足条件
#define CXX_REMOVE_IF(container, pred) std::remove_if(std::begin(container), std::end(container), pred)

//@备注 std::remove_copy
// 用途: 将容器中的元素复制到另一个容器中，并移除指定元素
// 炫_复制移除(容器1,容器2,元素)
//@参数 容器1
//@参数 容器2
//@参数 元素
//@别名 炫_复制移除
#define CXX_REMOVE_COPY(container1, container2, value) std::remove_copy(std::begin(container1), std::end(container1), std::begin(container2), value)

//@备注 std::remove_copy_if
// 用途: 将容器中的元素复制到另一个容器中，并移除满足条件的元素
// 炫_复制移除满足条件(容器1,容器2,条件)
//@参数 容器1
//@参数 容器2
//@参数 条件
//@别名 炫_复制移除满足条件
#define CXX_REMOVE_COPY_IF(container1, container2, pred) std::remove_copy_if(std::begin(container1), std::end(container1), std::begin(container2), pred)

//@备注 std::remove_copy_n
// 用途: 将容器中的前n个元素复制到另一个容器中，并移除指定元素
// 炫_复制移除n个(容器1,容器2,元素,n)
//@参数 容器1
//@参数 容器2
//@参数 元素
//@参数 n
//@别名 炫_复制移除n个
#define CXX_REMOVE_COPY_N(container1, container2, value, n) std::remove_copy_n(std::begin(container1), n, std::begin(container2), value)

//@备注 std::replace
// 用途: 将容器中的指定元素替换为另一个元素
// 炫_替换(容器,元素,新元素)
//@参数 容器
//@参数 元素
//@参数 新元素
//@别名 炫_替换
#define CXX_REPLACE(container, value, new_value) std::replace(std::begin(container), std::end(container), value, new_value)

//@备注 std::replace_if
// 用途: 将容器中满足条件的元素替换为另一个元素
// 炫_替换满足条件(容器,条件,新元素)
//@参数 容器
//@参数 条件
//@参数 新元素
//@别名 炫_替换满足条件
#define CXX_REPLACE_IF(container, pred, new_value) std::replace_if(std::begin(container), std::end(container), pred, new_value)

//@备注 std::replace_copy
// 用途: 将容器中的元素复制到另一个容器中，并替换指定元素
// 炫_复制替换(容器1,容器2,元素,新元素)
//@参数 容器1
//@参数 容器2
//@参数 元素
//@参数 新元素
//@别名 炫_复制替换
#define CXX_REPLACE_COPY(container1, container2, value, new_value) std::replace_copy(std::begin(container1), std::end(container1), std::begin(container2), value, new_value)

//@备注 std::replace_copy_if
// 用途: 将容器中的元素复制到另一个容器中，并替换满足条件的元素
// 炫_复制替换满足条件(容器1,容器2,条件,新元素)
//@参数 容器1
//@参数 容器2
//@参数 条件
//@参数 新元素
//@别名 炫_复制替换满足条件
#define CXX_REPLACE_COPY_IF(container1, container2, pred, new_value) std::replace_copy_if(std::begin(container1), std::end(container1), std::begin(container2), pred, new_value)

//@备注 std::replace_copy_n
// 用途: 将容器中的前n个元素复制到另一个容器中，并替换指定元素
// 炫_复制替换n个(容器1,容器2,元素,新元素,n)
//@参数 容器1
//@参数 容器2
//@参数 元素
//@参数 新元素
//@参数 n
//@别名 炫_复制替换n个
#define CXX_REPLACE_COPY_N(container1, container2, value, new_value, n) std::replace_copy_n(std::begin(container1), n, std::begin(container2), value, new_value)

//@备注 std::reverse_copy
// 用途: 将容器中的元素复制到另一个容器中，并反转
// 炫_复制反转(容器1,容器2)
//@参数 容器1
//@参数 容器2
//@别名 炫_复制反转
#define CXX_REVERSE_COPY(container1, container2) std::reverse_copy(std::begin(container1), std::end(container1), std::begin(container2))

//@备注 std::rotate
// 用途: 将容器中的元素进行旋转
// 炫_旋转(容器,位置)
//@参数 容器
//@参数 位置
//@别名 炫_旋转
#define CXX_ROTATE(container, middle) std::rotate(std::begin(container), middle, std::end(container))

//@备注 std::rotate_copy
// 用途: 将容器中的元素复制到另一个容器中，并旋转
// 炫_复制旋转(容器1,容器2,位置)
//@参数 容器1
//@参数 容器2
//@参数 位置
//@别名 炫_复制旋转
#define CXX_ROTATE_COPY(container1, container2, middle) std::rotate_copy(std::begin(container1), middle, std::end(container1), std::begin(container2))

//@备注 std::random_shuffle
// 用途: 对容器中的元素进行随机排列
// 炫_随机排列(容器)
//@参数 容器
//@别名 炫_随机排列
#define CXX_RANDOM_SHUFFLE(container) std::random_shuffle(std::begin(container), std::end(container))

//@备注 std::shuffle
// 用途: 对容器中的元素进行随机排列
// 炫_随机排列(容器,生成器)
//@参数 容器
//@参数 生成器
//@别名 炫_洗牌
#define CXX_SHUFFLE(container, gen) std::shuffle(std::begin(container), std::end(container), gen)

//@备注 std::min_element
// 用途: 获取容器中的最小元素
// 炫_最小元素(容器)
//@参数 容器
//@返回 最小元素
//@别名 炫_最小元素
#define CXX_MIN_ELEMENT(container) std::min_element(std::begin(container), std::end(container))

//@备注 std::max_element
// 用途: 获取容器中的最大元素
// 炫_最大元素(容器)
//@参数 容器
//@返回 最大元素
//@别名 炫_最大元素
#define CXX_MAX_ELEMENT(container) std::max_element(std::begin(container), std::end(container))

//@备注 std::minmax_element
// 用途: 获取容器中的最小和最大元素
// 炫_最小最大元素(容器)
//@参数 容器
//@返回 最小和最大元素
//@别名 炫_最小最大元素
#define CXX_MINMAX_ELEMENT(container) std::minmax_element(std::begin(container), std::end(container))

//@备注 std::is_sorted
// 用途: 判断容器是否已经排序
// 炫_已排序(容器)
//@参数 容器
//@返回 是否已排序
//@别名 炫_已排序
#define CXX_IS_SORTED(container) std::is_sorted(std::begin(container), std::end(container))

//@备注 std::is_sorted_until
// 用途: 获取容器中的第一个未排序的元素
// 炫_未排序元素(容器)
//@参数 容器
//@返回 未排序元素
//@别名 炫_未排序元素
#define CXX_IS_SORTED_UNTIL(container) std::is_sorted_until(std::begin(container), std::end(container))

//@备注 std::nth_element
// 用途: 对容器中的元素进行部分排序
// 炫_部分排序(容器,位置)
//@参数 容器
//@参数 位置
//@别名 炫_有限排序
#define CXX_NTH_ELEMENT(container, nth) std::nth_element(std::begin(container), nth, std::end(container))

//@备注 std::partial_sort
// 用途: 对容器中的元素进行部分排序
// 炫_部分排序(容器,位置)
//@参数 容器
//@参数 位置
//@别名 炫_部分排序
#define CXX_PARTIAL_SORT(container, nth) std::partial_sort(std::begin(container), nth, std::end(container))

//@备注 std::partial_sort_copy
// 用途: 将容器中的元素复制到另一个容器中，并进行部分排序
// 炫_复制部分排序(容器1,容器2,位置)
//@参数 容器1
//@参数 容器2
//@参数 位置
//@别名 炫_复制部分排序
#define CXX_PARTIAL_SORT_COPY(container1, container2, nth) std::partial_sort_copy(std::begin(container1), std::end(container1), std::begin(container2), std::end(container2), nth)

//@备注 std::is_heap
// 用途: 判断容器是否已经是一个堆
// 炫_已堆(容器)
//@参数 容器
//@返回 是否已堆
//@别名 炫_已堆
#define CXX_IS_HEAP(container) std::is_heap(std::begin(container), std::end(container))

//@备注 std::is_heap_until
// 用途: 获取容器中的第一个不满足堆的元素
// 炫_未堆元素(容器)
//@参数 容器
//@返回 未堆元素
//@别名 炫_未堆元素
#define CXX_IS_HEAP_UNTIL(container) std::is_heap_until(std::begin(container), std::end(container))

//@备注 std::make_heap
// 用途: 创建一个堆
// 炫_创建堆(容器)
//@参数 容器
//@别名 炫_创建堆
#define CXX_MAKE_HEAP(container) std::make_heap(std::begin(container), std::end(container))

//@备注 std::push_heap
// 用途: 将一个元素推入堆
// 炫_推入堆(容器)
//@参数 容器
//@别名 炫_推入堆
#define CXX_PUSH_HEAP(container) std::push_heap(std::begin(container), std::end(container))

//@备注 std::pop_heap
// 用途: 将堆顶元素弹出堆
// 炫_弹出堆(容器)
//@参数 容器
//@别名 炫_弹出堆
#define CXX_POP_HEAP(container) std::pop_heap(std::begin(container), std::end(container))

//@备注 std::sort_heap
// 用途: 对堆进行排序
// 炫_排序堆(容器)
//@参数 容器
//@别名 炫_排序堆
#define CXX_SORT_HEAP(container) std::sort_heap(std::begin(container), std::end(container))

//@备注 std::is_permutation
// 用途: 判断两个容器是否是排列
// 炫_是排列(容器1,容器2)
//@参数 容器1
//@参数 容器2
//@返回 是否是排列
//@别名 炫_是排列
#define CXX_IS_PERMUTATION(container1, container2) std::is_permutation(std::begin(container1), std::end(container1), std::begin(container2))

//@备注 std::next_permutation
// 用途: 获取容器的下一个排列
// 炫_下一个排列(容器)
//@参数 容器
//@返回 是否有下一个排列
//@别名 炫_下一个排列
#define CXX_NEXT_PERMUTATION(container) std::next_permutation(std::begin(container), std::end(container))

//@备注 std::prev_permutation
// 用途: 获取容器的上一个排列
// 炫_上一个排列(容器)
//@参数 容器
//@返回 是否有上一个排列
//@别名 炫_上一个排列
#define CXX_PREV_PERMUTATION(container) std::prev_permutation(std::begin(container), std::end(container))

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

//@备注 std::min
// 用途: 获取两个值中的最小值
// 炫_最小(值1,值2)
//@参数 值1
//@参数 值2
//@返回 最小值
//@别名 炫_最小
#define CXX_MIN(value1, value2) std::min(value1, value2)

//@备注 std::max
// 用途: 获取两个值中的最大值
// 炫_最大(值1,值2)
//@参数 值1
//@参数 值2
//@返回 最大值
//@别名 炫_最大
#define CXX_MAX(value1, value2) std::max(value1, value2)

//@备注  is_void
// 用途: 检查类型是否为void
// 炫_是空
//@参数 變量
//@返回 是否为void
//@别名 炫_是否空
#define CXX_IS_VOID(type) std::is_void<decltype(type)>::value

//@备注  is_null_pointer
// 用途: 检查类型是否为std::nullptr_t
// 炫_是空指针
//@参数 變量
//@返回 是否为std::nullptr_t
//@别名 炫_是否空指针
#define CXX_IS_NULLPTR(type) std::is_null_pointer<decltype(type)>::value

//@备注  is_integral
// 用途: 检查类型是否为整型
// 炫_是否整型
//@参数 變量
//@返回 是否为整数类型
//@别名 炫_是否整数
#define CXX_IS_INTEGRAL(type) std::is_integral<decltype(type)>::value

//@备注  is_floating_point
// 用途: 检查类型是否为浮点类型
// 炫_是否浮点
//@参数 變量
//@返回 是否为浮点类型
//@别名 炫_是否浮点
#define CXX_IS_FLOATING_POINT(type) std::is_floating_point<decltype(type)>::value

//@备注  is_array
// 用途: 检查类型是否为数组类型
// 炫_是否数组
//@参数 變量
//@返回 是否为数组类型
//@别名 炫_是否数组
#define CXX_IS_ARRAY(type) std::is_array<decltype(type)>::value

//@备注  is_enum
// 用途: 检查类型是否为枚举类型
// 炫_是否枚举
//@参数 變量
//@返回 是否为枚举类型
//@别名 炫_是否枚举
#define CXX_IS_ENUM(type) std::is_enum<decltype(type)>::value

//@备注  is_union
// 用途: 检查类型是否为联合类型
// 炫_是否联合
//@参数 變量
//@返回 是否为联合类型
//@别名 炫_是否联合
#define CXX_IS_UNION(type) std::is_union<decltype(type)>::value

//@备注  is_class
// 用途: 检查类型是否为类类型
// 炫_是否类
//@参数 變量
//@返回 是否为类类型
//@别名 炫_是否类
#define CXX_IS_CLASS(type) std::is_class<decltype(type)>::value

//@备注  is_function
// 用途: 检查类型是否为函数类型
// 炫_是否函数
//@参数 變量
//@返回 是否为函数类型
//@别名 炫_是否函数
#define CXX_IS_FUNCTION(type) std::is_function<decltype(type)>::value

//@备注  is_pointer
// 用途: 检查类型是否为指针类型
// 炫_是否指针
//@参数 變量
//@返回 是否为指针类型
//@别名 炫_是否指针
#define CXX_IS_POINTER(type) std::is_pointer<decltype(type)>::value

//@备注  is_lvalue_reference
// 用途: 检查类型是否为左值引用类型
// 炫_是否左值引用
//@参数 變量
//@返回 是否为左值引用类型
//@别名 炫_是否左值引用
#define CXX_IS_LVALUE_REFERENCE(type) std::is_lvalue_reference<decltype(type)>::value

//@备注  is_rvalue_reference
// 用途: 检查类型是否为右值引用类型
// 炫_是否右值引用
//@参数 變量
//@返回 是否为右值引用类型
//@别名 炫_是否右值引用
#define CXX_IS_RVALUE_REFERENCE(type) std::is_rvalue_reference<decltype(type)>::value

//@备注  is_member_object_pointer
// 用途: 检查类型是否为成员对象指针类型
// 炫_是否成员对象指针
//@参数 變量
//@返回 是否为成员对象指针类型
//@别名 炫_是否成员对象指针
#define CXX_IS_MEMBER_OBJECT_POINTER(type) std::is_member_object_pointer<decltype(type)>::value

//@备注  is_member_function_pointer
// 用途: 检查类型是否为成员函数指针类型
// 炫_是否成员函数指针
//@参数 變量
//@返回 是否为成员函数指针类型
//@别名 炫_是否成员函数指针
#define CXX_IS_MEMBER_FUNCTION_POINTER(type) std::is_member_function_pointer<decltype(type)>::value

//@备注  is_fundamental
// 用途: 检查类型是否为基本类型
// 炫_是否基本
//@参数 變量
//@返回 是否为基本类型
//@别名 炫_是否基本
#define CXX_IS_FUNDAMENTAL(type) std::is_fundamental<decltype(type)>::value

//@备注  is_arithmetic
// 用途: 检查类型是否为算术类型
// 炫_是否算术
//@参数 變量
//@返回 是否为算术类型
//@别名 炫_是否算术
#define CXX_IS_ARITHMETIC(type) std::is_arithmetic<decltype(type)>::value

//@备注  is_scalar
// 用途: 检查类型是否为标量类型
// 炫_是否标量
//@参数 變量
//@返回 是否为标量类型
//@别名 炫_是否标量
#define CXX_IS_SCALAR(type) std::is_scalar<decltype(type)>::value

//@备注  is_object
// 用途: 检查类型是否为对象类型
// 炫_是否对象
//@参数 變量
//@返回 是否为对象类型
//@别名 炫_是否对象
#define CXX_IS_OBJECT(type) std::is_object<decltype(type)>::value

//@备注  is_compound
// 用途: 检查类型是否为复合类型
// 炫_是否复合
//@参数 變量
//@返回 是否为复合类型
//@别名 炫_是否复合
#define CXX_IS_COMPOUND(type) std::is_compound<decltype(type)>::value

//@备注  is_reference
// 用途: 检查类型是否为引用类型
// 炫_是否引用
//@参数 變量
//@返回 是否为引用类型
//@别名 炫_是否引用
#define CXX_IS_REFERENCE(type) std::is_reference<decltype(type)>::value

//@备注  is_member_pointer
// 用途: 检查类型是否为成员指针类型
// 炫_是否成员指针
//@参数 變量
//@返回 是否为成员指针类型
//@别名 炫_是否成员指针
#define CXX_IS_MEMBER_POINTER(type) std::is_member_pointer<decltype(type)>::value

//@备注  is_const
// 用途: 检查类型是否为const类型
// 炫_是否常量
//@参数 變量
//@返回 是否为const类型
//@别名 炫_是否常量
#define CXX_IS_CONST(type) std::is_const<decltype(type)>::value

//@备注  is_volatile
// 用途: 检查类型是否为volatile类型
// 炫_是否易失
//@参数 變量
//@返回 是否为volatile类型
//@别名 炫_是否易失
#define CXX_IS_VOLATILE(type) std::is_volatile<decltype(type)>::value

//@备注  is_trivial
// 用途: 检查类型是否为平凡类型
// 炫_是否平凡
//@参数 變量
//@返回 是否为平凡类型
//@别名 炫_是否平凡
#define CXX_IS_TRIVIAL(type) std::is_trivial<decltype(type)>::value

//@备注  is_trivially_copyable
// 用途: 检查类型是否为平凡可复制类型
// 炫_是否平凡可复制
//@参数 變量
//@返回 是否为平凡可复制类型
//@别名 炫_是否平凡可复制
#define CXX_IS_TRIVIALLY_COPYABLE(type) std::is_trivially_copyable<decltype(type)>::value

//@备注  is_standard_layout
// 用途: 检查类型是否为标准布局类型
// 炫_是否标准布局
//@参数 變量
//@返回 是否为标准布局类型
//@别名 炫_是否标准布局
#define CXX_IS_STANDARD_LAYOUT(type) std::is_standard_layout<decltype(type)>::value

//@备注  has_unique_object_representations
// 用途: 检查类型是否具有唯一对象表示
// 炫_是否唯一对象表示
//@参数 變量
//@返回 是否具有唯一对象表示
//@别名 炫_是否唯一对象表示
#define CXX_HAS_UNIQUE_OBJECT_REPRESENTATIONS(type) std::has_unique_object_representations<decltype(type)>::value

//@备注  is_empty
// 用途: 检查类型是否为空类型
// 炫_是否空
//@参数 變量
//@返回 是否为空类型
//@别名 炫_是否为空
#define CXX_IS_EMPTY(type) std::is_empty<decltype(type)>::value

//@备注  is_polymorphic
// 用途: 检查类型是否为多态类型
// 炫_是否多态
//@参数 變量
//@返回 是否为多态类型
//@别名 炫_是否多态
#define CXX_IS_POLYMORPHIC(type) std::is_polymorphic<decltype(type)>::value

//@备注  is_abstract
// 用途: 检查类型是否为抽象类型
// 炫_是否抽象
//@参数 變量
//@返回 是否为抽象类型
//@别名 炫_是否抽象
#define CXX_IS_ABSTRACT(type) std::is_abstract<decltype(type)>::value

//@备注  is_final
// 用途: 检查类型是否为最终类型
// 炫_是否最终
//@参数 變量
//@返回 是否为最终类型
//@别名 炫_是否最终
#define CXX_IS_FINAL(type) std::is_final<decltype(type)>::value

//@备注  is_aggregate
// 用途: 检查类型是否为聚合类型
// 炫_是否聚合
//@参数 變量
//@返回 是否为聚合类型
//@别名 炫_是否聚合
#define CXX_IS_AGGREGATE(type) std::is_aggregate<decltype(type)>::value

#if __cplusplus >= 202300L
//@备注  is_implicit_lifetime
// 用途: 检查类型是否为隐式生命周期类型
// 炫_是否隐式生命周期
//@参数 變量
//@返回 是否为隐式生命周期类型
//@别名 炫_是否隐式生命周期
#define CXX_IS_IMPLICIT_LIFETIME(type) std::is_implicit_lifetime<decltype(type)>::value
#endif

//@备注  is_signed
// 用途: 检查类型是否为有符号类型
// 炫_是否有符号
//@参数 變量
//@返回 是否为有符号类型
//@别名 炫_是否有符号
#define CXX_IS_SIGNED(type) std::is_signed<decltype(type)>::value

//@备注  is_unsigned
// 用途: 检查类型是否为无符号类型
// 炫_是否无符号
//@参数 變量
//@返回 是否为无符号类型
//@别名 炫_是否无符号
#define CXX_IS_UNSIGNED(type) std::is_unsigned<decltype(type)>::value

#if __cplusplus >= 201703L
//@备注  is_bounded_array
// 用途: 检查类型是否为有界数组类型
// 炫_是否有界数组
//@参数 變量
//@返回 是否为有界数组类型
//@别名 炫_是否有界数组
#define CXX_IS_BOUNDED_ARRAY(type) std::is_bounded_array<decltype(type)>::value

//@备注  is_unbounded_array
// 用途: 检查类型是否为无界数组类型
// 炫_是否无界数组
//@参数 變量
//@返回 是否为无界数组类型
//@别名 炫_是否无界数组
#define CXX_IS_UNBOUNDED_ARRAY(type) std::is_unbounded_array<decltype(type)>::value
#endif

#if __cplusplus >= 202300L
//@备注  is_scoped_enum
// 用途: 检查类型是否为作用域枚举类型
// 炫_是否作用域枚举
//@参数 變量
//@返回 是否为作用域枚举类型
//@别名 炫_是否作用域枚举
#define CXX_IS_SCOPED_ENUM(type) std::is_scoped_enum<decltype(type)>::value
#endif
#pragma endregion
//@分组}

//@隐藏{
class CXXFlag
{
public:
	constexpr inline CXXFlag(__int64 value) noexcept : i(value)
	{}

	constexpr inline operator __int64() const noexcept { return i; }

private:
	__int64 i = 0LL;
};

template<typename Enum>
class CXXFlags
{
	CXX_STATIC_ASSERT_X((std::is_enum<Enum>::value), "CXXFlags is only usable on enumeration types.");

	//template <typename E> friend std::ostream& operator<<(std::ostream& o, CXXFlags<E> fs) { return o << fs.i; }
	//template <typename E> friend std::wostream& operator<<(std::wostream& o, CXXFlags<E> fs) { return o << fs.i; }
public:

	// see above for MSVC
	// the definition below is too complex for qdoc
	typedef __int64 Int;

	typedef Enum enum_type;
	// compiler-generated copy/move ctor/assignment operators are fine!
	constexpr inline CXXFlags() noexcept
		: i(0)
	{}

	constexpr inline CXXFlags(Enum flags) noexcept
		: i(Int(flags))
	{}

	constexpr inline CXXFlags(CXXFlag flag) noexcept
		: i(flag)
	{}

	constexpr inline CXXFlags(UINT64 flags) noexcept
		: i(Int(flags))
	{}

	constexpr inline CXXFlags(std::initializer_list<Enum> flags) noexcept
		: i(initializer_list_helper(flags.begin(), flags.end()))
	{}

	constexpr inline CXXFlags& operator&=(long mask) noexcept { i &= mask; return *this; }
	constexpr inline CXXFlags& operator&=(int mask) noexcept { i &= mask; return *this; }
	constexpr inline CXXFlags& operator&=(UINT mask) noexcept { i &= mask; return *this; }
	constexpr inline CXXFlags& operator&=(__int64 mask) noexcept { i &= mask; return *this; }
	constexpr inline CXXFlags& operator&=(UINT64 mask) noexcept { i &= mask; return *this; }
	constexpr inline CXXFlags& operator&=(Enum mask) noexcept { i &= Int(mask); return *this; }
	constexpr inline CXXFlags& operator|=(CXXFlags other) noexcept { i |= other.i; return *this; }
	constexpr inline CXXFlags& operator|=(Enum other) noexcept { i |= Int(other); return *this; }
	constexpr inline CXXFlags& operator^=(CXXFlags other) noexcept { i ^= other.i; return *this; }
	constexpr inline CXXFlags& operator^=(Enum other) noexcept { i ^= Int(other); return *this; }

	constexpr inline operator Int() const noexcept { return i; }

	constexpr inline CXXFlags operator|(CXXFlags other) const noexcept { return CXXFlags(CXXFlag(i | other.i)); }
	constexpr inline CXXFlags operator|(Enum other) const noexcept { return CXXFlags(CXXFlag(i | Int(other))); }
	constexpr inline CXXFlags operator^(CXXFlags other) const noexcept { return CXXFlags(CXXFlag(i ^ other.i)); }
	constexpr inline CXXFlags operator^(Enum other) const noexcept { return CXXFlags(CXXFlag(i ^ Int(other))); }
	constexpr inline CXXFlags operator&(int mask) const noexcept { return CXXFlags(CXXFlag(i & mask)); }
	constexpr inline CXXFlags operator&(long mask) const noexcept { return CXXFlags(CXXFlag(i & mask)); }
	constexpr inline CXXFlags operator&(UINT mask) const noexcept { return CXXFlags(CXXFlag(i & mask)); }
	constexpr inline CXXFlags operator&(__int64 mask) const noexcept { return CXXFlags(CXXFlag(i & mask)); }
	constexpr inline CXXFlags operator&(UINT64 mask) const noexcept { return CXXFlags(CXXFlag(i & mask)); }
	constexpr inline CXXFlags operator&(Enum other) const noexcept { return CXXFlags(CXXFlag(i & Int(other))); }
	constexpr inline CXXFlags operator~() const noexcept { return CXXFlags(CXXFlag(~i)); }

	constexpr inline bool operator!() const noexcept { return !i; }

	constexpr inline bool testFlag(Enum flag) const noexcept { return (i & Int(flag)) == Int(flag) && (Int(flag) != 0 || i == Int(flag)); }

	constexpr inline CXXFlags& setFlag(Enum flag, bool on = true) noexcept
	{
		return on ? (*this |= flag) : (*this &= ~Int(flag));
	}

private:
	constexpr static inline Int initializer_list_helper(typename std::initializer_list<Enum>::const_iterator it,
		typename std::initializer_list<Enum>::const_iterator end)
		noexcept
	{
		return (it == end ? Int(0) : (Int(*it) | initializer_list_helper(it + 1, end)));
	}

private:
	Int i;
};
//@隐藏}

//@别名 炫
namespace cxx
{
	//@分组{［全局］.炫　ＷＩＮ窗口标志枚举类

	//@启用枚举前缀

	//@备注 WindowType
	// 用途: 窗口类型
	// 窗口类型
	//@别名 窗口类型
	enum WindowType
	{
		Window = 0x00000001,
		Dialog = 0x00000002 | Window,

		WindowType_Mask = 0x000000ff,
		MSWindowsFixedSizeDialogHint = 0x00000100,
		MSWindowsOwnDC = 0x00000200,
		FramelessWindowHint = 0x00000800,
		WindowTitleHint = 0x00001000,
		WindowSystemMenuHint = 0x00002000,
		WindowMinimizeButtonHint = 0x00004000,
		WindowMaximizeButtonHint = 0x00008000,
		WindowMinMaxButtonsHint = WindowMinimizeButtonHint | WindowMaximizeButtonHint,
		WindowContextHelpButtonHint = 0x00010000,
		WindowShadeButtonHint = 0x00020000,
		WindowStaysOnTopHint = 0x00040000,
		WindowTransparentForInput = 0x00080000,
		WindowOverridesSystemGestures = 0x00100000,
		WindowDoesNotAcceptFocus = 0x00200000,
		MaximizeUsingFullscreenGeometryHint = 0x00400000,

		CustomizeWindowHint = 0x02000000,
		WindowStaysOnBottomHint = 0x04000000,
		WindowCloseButtonHint = 0x08000000,
		BypassGraphicsProxyWidget = 0x20000000,
		NoDropShadowWindowHint = 0x40000000,
		WindowFullscreenButtonHint = 0x80000000,

		Default = Window | WindowTitleHint | WindowSystemMenuHint | WindowMinimizeButtonHint | WindowMaximizeButtonHint
	};
	//@禁用枚举前缀

	//@分组}

	//@隐藏{
	CXX_DECLARE_FLAGS(WindowFlags, WindowType);
	//@隐藏}

#pragma region Enums
	//@分组{［其他］.炫　ＣＯＭ组件类

	//@别名 线程套间类型
	enum ThreadApartmentType
		//@隐藏{
		: __int64
		//@隐藏}
	{
		//@备注 全局單例
		//@别名 多线程套间
		MTA = 0x0,
		//@备注 每个线程都必須要有自己的COM对象
		//@别名 单线程套间
		STA = 0x2
	};

	//@分组}

	//@分组{［文本］.炫　加密类

	//@别名 哈希算法类型
	enum HashAlgorithm
	{
		MD2,
		MD4,
		MD5,
		SHA1,
		SHA256,
		SHA384,
		SHA512
	};

	//@别名 加密算法类型
	enum CryptAlgorithm
	{
		RSA,
		RSA_SIGN,
		DH,
		DSA,
		RC2,
		RC4,
		AES,
		DES,
		DESX,
		TripleDES,
		TripleDES_112,
		AES_GMAC,
		AES_CMAC,
		ECDSA_P256,
		ECDSA_P384,
		ECDSA_P521,
		ECDH_P256,
		ECDH_P384,
		ECDH_P521,
		RNG_FIPS186_DSA,
		RNG_DUAL_EC
	};

	//@别名 链模式
	enum ChainMode
	{
		NA,
		CBC,
		ECB,
		CFB,
		CCM,
		GCM
	};

	//@分组}

	//@分组{［其他］.炫　进程类

	//@别名 进程错误
	enum ProcessError
	{
		FailedToStart,  //@别名 进程启动失败
		Crashed,		//@别名 进程崩溃
		Timedout,		//@别名 进程超时
		WriteError,		//@别名 进程写入错误
		ReadError, 		//@别名 进程读取错误
		UnknownError	//@别名 进程未知错误
	};

	//@别名 进程状态
	enum ProcessState
	{
		NotRunning,		//@别名 未运行
		Starting, 		//@别名 启动中
		Running,		//@别名 运行中
	};

	//@别名 退出状态
	enum ExitStatus
	{
		NormalExit,		//@别名 正常退出
		CrashExit,		//@别名 崩溃退出
	};

	//@别名 进程流打开模式
	enum ProcessOpenModeFlag
	{
		ProcessNotOpen = 0x0000,	//@别名 进程流未打开
		ProcessReadOnly = 0x0001,	//@别名 进程流只读
		ProcessWriteOnly = 0x0002,	//@别名 进程流只写
		//@别名 进程流读写
		ProcessReadWrite = ProcessReadOnly | ProcessWriteOnly
	};

	//@隐藏{
	CXX_DECLARE_FLAGS(ProcessOpenMode, ProcessOpenModeFlag);
	//@隐藏}

	//@分组}

	//@分组{［其他］.炫　推送通知类

	// @别名 推送通知排版类型
	enum WinToastType
	{
		ImageAndText01, //@别名 图片和文本01
		ImageAndText02, //@别名 图片和文本02
		ImageAndText03, //@别名 图片和文本03
		ImageAndText04, //@别名 图片和文本04
		Text01,         //@别名 文本01
		Text02,         //@别名 文本02
		Text03,         //@别名 文本03
		Text04          //@别名 文本04
	};

	//@别名 推送通知音频
	enum WinToastAudio
	{
		DefaultSound,   //@别名 默认声音
		IM,				//@别名 IM
		Mail,			//@别名 邮件
		Reminder,		//@别名 提醒
		SMS,			//@别名 短信
		Alarm,			//@别名 闹钟
		Alarm2,			//@别名 闹钟2
		Alarm3,			//@别名 闹钟3
		Alarm4,			//@别名 闹钟4
		Alarm5,			//@别名 闹钟5
		Alarm6,			//@别名 闹钟6
		Alarm7,			//@别名 闹钟7
		Alarm8,			//@别名 闹钟8
		Alarm9,			//@别名 闹钟9
		Alarm10,		//@别名 闹钟10
		Call,			//@别名 电话
		Call1,			//@别名 电话1
		Call2,			//@别名 电话2
		Call3,			//@别名 电话3
		Call4,			//@别名 电话4
		Call5,			//@别名 电话5
		Call6,			//@别名 电话6
		Call7,			//@别名 电话7
		Call8,			//@别名 电话8
		Call9,			//@别名 电话9
		Call10,			//@别名 电话10
	};

	//@别名 推送通知音频选项
	enum AudioOption
	{
		AudioDefault = 0,    //@别名 默认
		AudioSilent,		 //@别名 静音
		AudioLoop,			 //@别名 循环
	};

	//@别名 推送通知裁剪
	enum CropHint
	{
		Square, //@别名 正方形
		Circle, //@别名 圆形
	};

	//@别名 推送通知文本框
	enum TextField
	{
		FirstLine = 0,  //@别名 第一行
		SecondLine,		//@别名 第二行
		ThirdLine,		//@别名 第三行
	};

	//@别名 推送通知结果
	enum WinToastResult
	{
		UserCanceled,		//@别名 用户取消
		ApplicationHidden,	//@别名 应用程序隐藏
		TimedOut,			//@别名 超时
		Successed,			//@别名 成功
		Failured			//@别名 失败
	};

	//@分组}

//@分组{［工具］.炫　随机数引擎类

	//@别名 随机数生成引擎类型
	enum RandomGeneratorsType
	{
		//@备注 普通随机数生成引擎 std::default_random_engine
		//优点:
		// 通用性好，适用于大多数需要随机数的场景。
		// 实现依赖于编译器，可能会自动选择最适合当前平台的实现。
		//缺点 :
		// 因为是默认引擎，其具体实现可能在不同的编译器和平台间有所不同，导致难以在不同环境下保持结果的一致性。
		// 可能不是对于所有用途都有最佳的性能或随机性。
		Regular = 0, //@别名 普通

		//@备注 MinSTD 最小标准(Minimum standard)随机数生成引擎 std::minstd_rand 或 std::minstd_rand0
		//优点:
		// 算法简单，速度快，占用资源少。对于基本的随机数需求足够好
		//缺点 :
		// 随机性较差，周期较短，不适合用于需要高质量随机数的场景。
		MinSTD = 1,//@别名 最小标准

		//@备注 基于 Mersenne Twister(梅森旋转算法) 的随机数生成引擎 std::mt19937 或 std::mt19937_64
		// 19937 是指它的周期长度为 2^19937 - 1
		//优点:
		// 高质量随机数，周期长，适用于大多数需要高质量随机数的场景。
		//缺点 :
		// 占用资源多，速度慢，不适合用于对性能要求较高的场景。
		Mt19937 = 2,//@别名 梅森旋转
	};

	//@别名 随机数分布类型
	enum RandomDistributionType
	{
		//@备注 均匀分布 std::uniform_int_distribution 或 std::uniform_real_distribution
		Uniform = 0,//@别名 均匀

		//@备注 std::bernoulli_distribution 伯努利分布
		Bernoulli = 1,//@别名 伯努利

		//@备注 std::binomial_distribution 二项分布
		Binomial = 2,//@别名 二项

		//@备注 std::negative_binomial_distribution 负二项分布
		NegativeBinomial = 3, //@别名 负二项

		//@备注 std::geometric_distribution 几何分布
		Geometric = 4, //@别名 几何

		//@备注 std::poisson_distribution 泊松分布
		Poisson = 5, //@别名 泊松

		//@备注 std::exponential_distribution 指数分布
		Exponential = 6, //@别名 指数

		//@备注 std::gamma_distribution 伽马分布
		Gamma = 7, //@别名 伽马

		//@备注 std::weibull_distribution 威布尔分布
		Weibull = 8, //@别名 威布尔

		//@备注 std::extreme_value_distribution 极值分布
		ExtremeValue = 9, //@别名 极值

		//@备注 std::normal_distribution 正态分布
		Normal = 10, //@别名 正态

		//@备注 std::lognormal_distribution 对数正态分布
		Lognormal = 11, //@别名 对数正态

		//@备注 std::chi_squared_distribution 卡方分布
		ChiSquared = 12, //@别名 卡方

		//@备注 std::cauchy_distribution 柯西分布
		Cauchy = 13, //@别名 柯西

		//@备注 std::fisher_f_distribution F分布
		FisherF = 14, //@别名 F

		//@备注 std::student_t_distribution t分布
		StudentT = 15, //@别名 t

		//@备注 std::discrete_distribution 离散分布
		Discrete = 16, //@别名 离散

		//@备注 std::piecewise_constant_distribution 分段常数分布
		PiecewiseConstant = 17, //@别名 分段常数

		//@备注 std::piecewise_linear_distribution 分段线性分布
		PiecewiseLinear = 18 //@别名 分段线性
	};

	//@分组}

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

	//@分组{［文本］.炫　配置类（ＩＮＩ／注册表）

	//@别名 配置格式
	enum SettingFormat
	{
		InvalidFormat = 0, //@别名 无效格式

		IniFormat, //@别名 INI格式
		RegistryFormat, //@别名 注册表格式
	};

	//@分组}

	//@分组{［线程安全］.炫　线程共享锁类

	//@别名 递归模式
	enum RecursionMode
	{
		Recursive, //@别名 递归
		NonRecursive //@别名 非递归
	};

	//@分组}

//@分组{［内存安全］.炫　共享内存类

	//@备注 共享内存访问模式枚举
	//@别名 共享内存访问模式
	enum SharedMemoryAccessMode
	{
		SharedMemoryReadOnly,
		SharedMemoryReadWrite
	};

	//@备注 共享内存错误枚举
	//@别名 共享内存错误
	enum SharedMemoryError
	{
		SharedMemoryNoError,
		SharedMemoryPermissionDenied,
		SharedMemoryInvalidSize,
		SharedMemoryKeyError,
		SharedMemoryAlreadyExists,
		SharedMemoryNotFound,
		SharedMemoryUnknownError
	};

	//@备注 共享内存版本切换
	//@别名 共享内存版本
	enum SharedMemoryVersion
	{
		SharedMemoryANSI,
		SharedMemoryUnicode
	};

	//@分组}

	//@分组{ ［文件／文件夹］.炫　ＪＳＯＮ类


	//@别名 JSON格式
	enum JsonFormat
	{
		JsonIndented, //@别名 缩进
		JsonCompact   //@别名 紧凑
	};

	//@分组}

//@分组{［文件／文件夹］.炫　文件对话框类


	//@别名 文件对话框接受模式
	enum FileDialogAcceptMode
		//@隐藏{
		: __int64
		//@隐藏}
	{
		//@备註 打开文件
		OpenFile = 0, //@别名 打开文件

		//@备註 打开目录
		OpenDirectory, //@别名 打开文件夹

		//@备註 保存文件
		SaveFile, //@别名 保存文件
	};

	//@别名 文件对话框标籤
	enum FileDialogLabel
		//@隐藏{
		: __int64
		//@隐藏}
	{
		//@备註 确定按钮标签
		OkButton, //@别名 确定按钮

		//@备註 文件名标签
		FileName, //@别名 文件名
	};

	//@别名 文件对话框选项
	enum FileDialogOption
		//@隐藏{
		: __int64
		//@隐藏}
	{
		//@备註 (0x2) 启用复盖文件时的提示，如果用户选择一个已经存在的文件
		OverwritePrompt = 0x2, //@别名 启用复盖文件提示

		//@备註 (0x4) 仅显示在文件类型筛选器中指定的文件类型，不显示所有文件
		StrictFileTypes = 0x4, //@别名 严格文件类型

		//@备註 (0x8) 不允许改变当前目录
		NoChangeDirectory = 0x8, //@别名 禁止改变目录

		//@备註 (0x20) 允许用户选择文件夹而不是文件
		PickFolders = 0x20, //@别名 允许选择文件夹

		//@备註 (0x40) 强制使用文件系统对话框，而不是自定义对话框
		ForceFileSystem = 0x40, //@别名 强制文件系统对话框

		//@备註 (0x80) 允许选择所有非存储项目，不仅限于文件系统项目
		AllNonStorageItems = 0x80, //@别名 允许所有非存储项

		//@备註 (0x100) 不验证用户输入的文件名或路径
		NoValidate = 0x100, //@别名 禁止验证

		//@备註 (0x200) 允许用户选择多个文件
		AllowMultiSelect = 0x200, //@别名 允许多选

		//@备註 (0x800) 路径必须存在
		PathMustExist = 0x800, //@别名 路径必须存在

		//@备註 (0x1000) 文件必须存在
		FileMustExist = 0x1000, //@别名 文件必须存在

		//@备註 (0x2000) 显示创建文件的提示，当用户指定的文件不存在时
		CreatePrompt = 0x2000, //@别名 创建文件提示

		//@备註 (0x4000) 文件对话框应该检查是否有其他应用程序已经打开了所选文件
		ShareAware = 0x4000, //@别名 共享感知

		//@备註 (0x8000) 不允许选择只读文件
		NoReadOnlyReturn = 0x8000, //@别名 禁止只读返回

		//@备註 (0x10000) 不测试文件的创建能力
		NoTestFileCreate = 0x10000, //@别名 禁止测试文件创建

		//@备註 (0x20000) 隐藏最近访问的位置
		HideMRUPlaces = 0x20000, //@别名 隐藏最近访问位置

		//@备註 (0x40000) 隐藏固定的位置
		HidePinnedPlaces = 0x40000, //@别名 隐藏固定位置

		//@备註 (0x100000) 不解析shell链接
		NoDereferenceLinks = 0x100000, //@别名 禁止解析链接

		//@备註 (0x200000) 需要与确定文件名按钮进行交互
		OkButtonNeedsInteraction = 0x200000, //@别名 需要确定文件名按钮交互

		//@备註 (0x2000000) 不将文件添加到最近文件列表中
		DontAddToRecent = 0x2000000, //@别名 不添加到最近文件列表

		//@备註 (0x10000000) 强制显示隐藏文件和文件夹
		ForceShowHidden = 0x10000000, //@别名 强制显示隐藏文件

		//@备註 (0x20000000) 默认情况下不以最小模式打开
		DefaultNoMinimode = 0x20000000, //@别名 默认不以最小模式打开

		//@备註 (0x40000000) 强制显示预览面板
		ForcePreviewPaneOn = 0x40000000, //@别名 强制显示预览面板

		//@备註 (0x80000000) 支持可流式传输的项目
		SupportStreamableItems = 0x80000000, //@别名 支持可流式传输项目

		//@备註 默认打开文件
		// 复盖提示(0x2) | 严格文件类型(0x4) | 路径必须存在(0x8000) 的组合
		DefaultOpenFile = OverwritePrompt | StrictFileTypes | PathMustExist,  //@别名 默认打开文件

		//@备註 默认保存文件
		// 复盖提示(0x2) | 文件必须存在(0x1000) | 严格文件类型(0x4) | 路径必须存在(0x8000) 的组合
		DefaultSaveFile = OverwritePrompt | FileMustExist | StrictFileTypes | PathMustExist, //@别名 默认保存文件

		//@备註 默认打开文件夹，包括文件夹选择和路径必须存在
		// 允许选择文件夹(0x20) | 路径必须存在(0x8000) 的组合
		DefaultOpenDirectory = PickFolders | PathMustExist, //@别名 默认打开文件夹
	};

	//@隐藏{
	CXX_DECLARE_FLAGS(FileDialogOptions, FileDialogOption);
	//@隐藏}
#pragma endregion


}

#endif // CXXGLOBAL_H_