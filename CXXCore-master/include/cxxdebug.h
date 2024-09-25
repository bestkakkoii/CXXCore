#pragma once
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
#ifndef CXXDEBUG_H_
//@隐藏{
#define CXXDEBUG_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxvector.h>
#include <cxxstring.h>
#include <cxxscopedpointer.h>

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <mutex>
#include <regex>
#include <vector>
#include <list>
#include <tuple>
#include <map>
#include <unordered_map>
#include <utility>

#include <../3rdparty/magic_enum/magic_enum.hpp>

//@别名 炫
namespace cxx
{
	//@分组{［调试］.炫　调试类
	// 
	//@备注 输出级别枚举
	//@别名 调试标誌
	enum DebugOutputLevel
		//@隐藏{
		: __int64
		//@隐藏}
	{
		//@备注 白色 
		//@别名 无
		None = 0,
		//@备注 绿色 
		//@别名 资讯
		Info,
		//@备注 深绿色 
		//@别名 调试
		Debug,
		//@备注 深黄色 
		//@别名 警告
		Warn,
		//@备注 红色 
		//@别名 错误
		Error,
		//@备注 深红色 
		//@别名 异常
		Fatal,
		//@隐藏{
		Nothing,
		//@隐藏}
	};

	// 常用控制台颜色的 ANSI 转义序列代码
	//@别名 控制台颜色
	enum ConsoleColor
		//@隐藏{
		: USHORT
		//@隐藏}
	{
		Black = 30,         //@别名 黑色
		Red = 31,           //@别名 红色
		Green = 32,         //@别名 绿色
		Yellow = 33,        //@别名 黄色
		Blue = 34,          //@别名 蓝色
		Magenta = 35,       //@别名 洋红色
		Cyan = 36,          //@别名 青色
		White = 37,         //@别名 白色
		BrightBlack = 90,   //@别名 亮黑色
		BrightRed = 91,     //@别名 亮红色
		BrightGreen = 92,   //@别名 亮绿色
		BrightYellow = 93,  //@别名 亮黄色
		BrightBlue = 94,    //@别名 亮蓝色
		BrightMagenta = 95, //@别名 亮洋红色
		BrightCyan = 96,    //@别名 亮青色
		BrightWhite = 97    //@别名 亮白色
	};

	static std::istream& cin = std::cin;		//@别名 控制台输入A
	static std::ostream& cout = std::cout;		//@别名 控制台输出A
	static std::ostream& cerr = std::cerr;		//@别名 控制台错误A
	static std::ostream& clog = std::clog;		//@别名 控制台日志A

	static std::wistream& wcin = std::wcin;		//@别名 控制台输入
	static std::wostream& wcout = std::wcout;	//@别名 控制台输出
	static std::wostream& wcerr = std::wcerr;	//@别名 控制台错误
	static std::wostream& wclog = std::wclog;	//@别名 控制台日志

	//@隐藏{
	class NextLine
	{
	public:
		const char data_ = '\n';
	public:
		operator const char& () const { return data_; }
	};
	//@隐藏}

	static NextLine endl; //@别名 控制台换行

	//@备注 清空当前控制台流内的所有数据
	//@别名 控制台清空()
	void clearConsole();

	//@别名 控制台暂停()
	void pauseConsole();

	//@别名 置控制台UTF8()
	void setConsoleUtf8();

	//@别名 置控制台UTF16()
	void setConsoleUtf16();

	//@备注 system
	//@别名 控制台命令(命令)
	__int64 system(const CXXString& command);
	//@分组}
}

//@分组{［调试］.炫　调试类

//@隐藏{
class CXXDebugPrivate;
//@隐藏}

//@隐藏{
using CXXMessageHandler = std::function<bool(__int64 flags, const CXXString& text)>;
//@隐藏}

/*@声明
//@备注 CXXMessageHandler 炫消息处理回调，可以用于自订写入日誌文件...等
//@参数 flags 调试标誌
//@参数 text 调试信息
//@返回 逻辑型 返回 true 则不会打印到调试窗口
//@别名 炫消息处理回调(调试标誌, 调试信息)
typedef bool(__cdecl* CXXMessageHandler)(__int64 flags, const CXXString& text);
*/

//@别名 炫调试
class CXXDebug
{
public:
	//@备注 使用方法: 
	// 调试() << 輸出內容 << 輸出內容2 << ..... ;
	//@返回 无
	CXXDebug();

	//@备注 使用方法: 
	// 调试(调试标誌) << 輸出內容 << 輸出內容2 << ..... ;
	//@参数 调试标誌
	//@返回 无
	CXXDebug(cxx::DebugOutputLevel flags);

	//@隐藏{
	CXXDebug(cxx::DebugOutputLevel flags, const wchar_t* contextFile, size_t line);
	CXXDebug(const wchar_t* contextFile, size_t line);
	virtual ~CXXDebug();
	static void setCodePage();
	//@隐藏}

	//@备注 无引号模式，使用方式:
	// 调试(调试标誌).noquote() << 輸出內容 << 輸出內容2 << ..... ;
	//@参数 无
	//@返回 當前调试对象
	//@别名 无修饰()
	CXXDebug& noquote();

	//@备注 无输出模式，使用方式:
	// 调试(调试标誌).raw() << 輸出內容 << 輸出內容2 << ..... ;
	//@参数 无
	//@返回 原始()
	//@别名 裸()
	CXXDebug& raw();

	//@备注 取出将任意变量类名
	//@参数 无
	//@返回 类名
	//@别名 类名()
	template<typename T>
	static CXXString typeName(T value)
	{
		return CXXString::fromUtf8(typeid(value).name());
	}

	//@备注 将两个CXDebug对象的数据流交换
	//@别名 交换(对象)
	void swap(CXXDebug& other);

	//@备注 转为字符串返回，使用方式:
	// CXXDebug cxxDebugObject;
	// cxxDebugObject << "Hello World";
	// CXXString str = CXXDebug::toString(cxxDebugObject);
	// 
	//@参数 可變參數
	//@返回 格式化后的字符串
	//@别名 到文本(可變參數)
	template <typename T>
	static CXXString toString(T&& object)
	{
		CXXDebug debug;
		debug << std::forward<T>(object);
		return debug.currentStream().str();
	}

	//@参数 可變參數
	//@返回 格式化后的字符串
	//@别名 到文本(可變參數)
	CXXString toString();

public: // static
	//@备注 设置调试信息前缀 使用方式: debug::setPrefix("{time}, {pid}, {tid}");
	//@参数 前缀字符串
	//@返回 无
	//@别名 置前缀(void)
	static void setPrefix(const CXXString& prefix);

	//@备注 设置调试信息后缀 使用方式: debug::setSuffix("{time}, {pid}, {tid}");
	//@参数 后缀字符串
	//@返回 无
	//@别名 置后缀(void)
	static void setSuffix(const CXXString& suffix);

	//@备注 格式参数: hh:mm:ss.zzz ap | yyyy/MM/dd | hh:MM:ss | hh:MM | hh | MM | ss | zzz | ap | ddd | dddd | MMMM | MMM
	//@参数 无
	//@返回 无
	//@别名 置时间格式(格式)
	static void setTimeFormat(const CXXString& format);

	//@别名 置启用ANSI转义序列(是否启用)
	static void setANSIColorEnabled(bool enabled);

	//@备注 添加自定义格式化 如: 添加自订格式("{哈哈哈}", 12345) 最终输出时 所有的 {哈哈哈} 都会自动替换成 12345
	//要注意数值只能接受 文本型/字符型/整型/双浮点型/浮点型
	//@参数 无
	//@返回 无
	//@别名 加自订格式(字符串格式, 数值)
	template <typename T>
	static void addCustomFormat(const CXXString& format, const T& value)
	{
		static const std::wregex reg(LR"(\{([a-zA-Z0-9\u4e00-\u9fa5]+)\})");
		std::wsmatch match;
		if (std::regex_search(format.toStdWString(), match, reg))
		{
			std::wostringstream ss;
			ss << T;
			customFormatHashRef().insert(std::pair<std::wstring, std::wstring>(format.toStdWString(), ss.str()));
		}
	}

	//@备注 添加自定义颜色格式化，替换时末尾会自动补上ESC[0m
	// 假设设置为 CXXDebug::addColorCustomFormat("{哈哈哈}", A"\033[35m")
	// 则实际输出时 所有 {哈哈哈} 都会替换成 "\033[35m哈哈哈\033[0m" 控制台中显示为紫色
	//@参数 格式 如 "{哈哈哈}"
	//@参数 要使用的ANSI转义序列
	//@返回 无
	//@别名 加自订颜色格式(字符串格式, 数值)
	static void addColorCustomFormat(const CXXString& format, const char* color);

	//@备注 添加自定义格式化 如: 添加自订格式("{哈哈哈}", 12345) 最终输出时 所有的 {哈哈哈} 都会自动替换成 12345
	//要注意数值只能接受 文本型/字符型/整型/双浮点型/浮点型
	//@参数 格式 如 "{指定字串}"
	//@参数 要替换的整数值
	//@返回 无
	//@别名 加自订格式(字符串格式, 数值)
	static void addCustomFormat(const CXXString& format, int value);

	//@备注 添加自定义格式化 如: 添加自订格式("{哈哈哈}", 12345) 最终输出时 所有的 {哈哈哈} 都会自动替换成 12345
	//要注意数值只能接受 文本型/字符型/整型/双浮点型/浮点型
	//@参数 格式 如 "{指定字串}"
	//@参数 要替换的正整数值
	//@返回 无
	//@别名 加自订格式(字符串格式, 数值)
	static void addCustomFormat(const CXXString& format, UINT value);

	//@备注 添加自定义格式化 如: 添加自订格式("{哈哈哈}", 12345) 最终输出时 所有的 {哈哈哈} 都会自动替换成 12345
	//要注意数值只能接受 文本型/字符型/整型/双浮点型/浮点型
	//@参数 格式 如 "{指定字串}"
	//@参数 要替换的浮点数值
	//@返回 无
	//@别名 加自订格式(字符串格式, 数值)
	static void addCustomFormat(const CXXString& format, float value);

	//@备注 添加自定义格式化 如: 添加自订格式("{哈哈哈}", 12345) 最终输出时 所有的 {哈哈哈} 都会自动替换成 12345
	//要注意数值只能接受 文本型/字符型/整型/双浮点型/浮点型
	//@参数 格式 如 "{指定字串}"
	//@参数 要替换的双精度浮点数值
	//@返回 无
	//@别名 加自订格式(字符串格式, 数值)
	static void addCustomFormat(const CXXString& format, double value);

	//@备注 添加自定义格式化 如: 添加自订格式("{哈哈哈}", 12345) 最终输出时 所有的 {哈哈哈} 都会自动替换成 12345
	//要注意数值只能接受 文本型/字符型/整型/双浮点型/浮点型
	//@参数 格式 如 "{指定字串}"
	//@参数 要替换的字符串A
	//@返回 无
	//@别名 加自订格式(字符串格式, 数值)
	static void addCustomFormat(const CXXString& format, const char* value);

	//@备注 添加自定义格式化 如: 添加自订格式("{哈哈哈}", 12345) 最终输出时 所有的 {哈哈哈} 都会自动替换成 12345
	//要注意数值只能接受 文本型/字符型/整型/双浮点型/浮点型
	//@参数 格式 如 "{指定字串}"
	//@参数 要替换的文本
	//@返回 无
	//@别名 加自订格式(字符串格式, 数值)
	static void addCustomFormat(const CXXString& format, const wchar_t* value);

	//@备注 创建一个新的控制台窗口，并将输入输出流导向新的控制台
	//@参数 无
	//@返回 无
	//@别名 建控制台()
	static HWND createConsole();

	//@备注 安装自订消息处理回调，所有输出在实际打印之前，都会先调用此回调函数，如果返回 true 则不会打印
	// typedef bool(__cdecl* CXXMessageHandler)(__int64 flags, const CXXString& text);
	//@参数 全局或静态类消息处理回调函数指针
	//@返回 无
	//@别名 装消息处理器(回调函数指针)
	static void installMessageHandler(CXXMessageHandler handler);

	//@备注 安装自订消息处理回调，所有输出在实际打印之前，都会先调用此回调函数，如果返回 true 则不会打印
	// typedef bool(__cdecl* CXXMessageHandler)(__int64 flags, const CXXString& text);
	//@参数 类消息处理回调函数指针
	//@参数 类实例指针
	//@返回 无
	//@别名 装消息处理器(回调函数指针, 类实例指针)
	template<typename T, typename U>
	static void installMessageHandler(T&& t, U* u)
	{
		installMessageHandler(std::forward<T>(t), std::forward<U>(u));
	}

#if _WIN32_WINNT > _WIN32_WINNT_WIN7
#if NTDDI_VERSION > NTDDI_WIN7
	//@备注 安装SEH异常处理調，当异常发生时会自动输出Minidump( .dmp )文件，可继续执行的异常，会自动继续执行
	//@参数 无
	//@返回 无
	//@别名 安装SEH异常处理()
	static void installExceptionHandleSEH();

	//@备注 安装VEH异常处理，当异常发生时会自动输出Minidump( .dmp )文件，可继续执行的异常，会自动继续执行
	//@参数 表示调用处理程序的顺序。如果参数非零，则该处理程序将首先被调用。如果参数为零，则该处理程序将最后被调用
	//@返回 无
	//@别名 装VEH异常处理(顺序)
	static void installExceptionHandleVEH(ULONG first = 1);
#endif
#endif


public: // operator override
	//@隐藏{
	template<typename T>
	inline CXXDebug& operator>>(T& value)
	{
		CXX_D(CXXDebug);

		if (!isNoOutput())
		{
			std::wcin >> value;
		}
		return *this;
	}

	CXXDebug& operator>>(bool& value);
	CXXDebug& operator>>(short& value);
	CXXDebug& operator>>(USHORT& value);
	CXXDebug& operator>>(int& value);
	CXXDebug& operator>>(UINT& value);
	CXXDebug& operator>>(__int64& value);
	CXXDebug& operator>>(UINT64& value);
	CXXDebug& operator>>(long& value);
	CXXDebug& operator>>(ULONG& value);
	CXXDebug& operator>>(float& value);
	CXXDebug& operator>>(double& value);
	CXXDebug& operator>>(CXXString& value);
	CXXDebug& operator>>(wchar_t& value);
	CXXDebug& operator>>(char& value);
	CXXDebug& operator>>(BYTE& value);

	template <typename T, typename std::enable_if<std::is_enum<typename std::decay<T>::type>::value>::type* = nullptr>
	CXXDebug& operator<<(T value)
	{
		using DecayedT = typename std::decay<T>::type;
		std::wostringstream ss;
		ss << CXXString::fromUtf8(std::string(magic_enum::enum_name(value)).c_str())
			<< L"(" << static_cast<std::underlying_type_t<DecayedT>>(value) << L")";
		return *this << ss.str();
	}

	//template <typename T>
	//CXXDebug& operator<<(T value)
	//{
	//	return *this << L"Unsupported type: " << typeid(value).name();
	//}

	CXXDebug& operator<<(bool t);
	CXXDebug& operator<<(BYTE t);
	CXXDebug& operator<<(char t);
	CXXDebug& operator<<(short t);
	CXXDebug& operator<<(USHORT t);
	CXXDebug& operator<<(int t);
	CXXDebug& operator<<(UINT t);
	CXXDebug& operator<<(__int64 t);
	CXXDebug& operator<<(UINT64 t);
	CXXDebug& operator<<(long t);
	CXXDebug& operator<<(ULONG t);
	CXXDebug& operator<<(wchar_t t);
	CXXDebug& operator<<(const char* value);
	CXXDebug& operator<<(const wchar_t* value);
	CXXDebug& operator<<(float value);
	CXXDebug& operator<<(double value);
	CXXDebug& operator<<(const CXXString& value);

#ifdef CXXATOMIC_H_
	template<typename T>
	CXXDebug& operator<<(CXXAtomic<T> t)
	{
		if (!isNoQuote())
		{
			currentStream() << L"CXXAtomic(" << t.load() << L")";
		}
		else
		{
			currentStream() << t.load();
		}

		return maybeSpace();
	}
#endif

	template<typename T>
	CXXDebug& operator<<(T* value)
	{
		if (!isNoQuote())
		{
			std::wostringstream ss;
			ss << L"0x" << std::hex << reinterpret_cast<uintptr_t>(value);
			currentStream() << ss.str();
		}
		else
		{
			currentStream() << reinterpret_cast<UINT64>(value);
		}

		return maybeSpace();
	}

	template<typename T>
	CXXDebug& operator<<(const CXXVector<T>& value)
	{
		return printSequentialContainer(L"CXXVector", value);
	}

#ifdef CXXPOINT_H_
	CXXDebug& operator<<(const CXXPoint& value)
	{
		this->noquote() << L"CXXPoint(" << value.x() << L',' << value.y() << L')';

		return maybeSpace();
	}

	CXXDebug& operator<<(const CXXPointF& value)
	{
		this->noquote() << L"CXXPointF(" << value.x() << L',' << value.y() << L')';

		return maybeSpace();
	}
#endif

	CXXDebug& operator<<(const BYTE* value);

	// STL compatible
	template <typename T, typename Alloc>
	CXXDebug& operator<<(const std::vector<T, Alloc>& value)
	{
		return printSequentialContainer(L"std::vector", value);
	}

	template <typename T, typename Alloc>
	CXXDebug& operator<<(const std::list<T, Alloc>& value)
	{
		return printSequentialContainer(L"std::list", value);
	}

	template <typename Key, typename T, typename Compare, typename Alloc>
	CXXDebug& operator<<(const std::map<Key, T, Compare, Alloc>& value)
	{
		return printSequentialContainer(L"std::map", value); // yes, sequential: *it is std::pair
	}

	template <typename Key, typename T, typename Compare, typename Alloc>
	CXXDebug& operator<<(const std::multimap<Key, T, Compare, Alloc>& value)
	{
		return printSequentialContainer(L"std::multimap", value); // yes, sequential: *it is std::pair
	}

	template <class T1, class T2>
	CXXDebug& operator<<(const std::pair<T1, T2>& value)
	{
		this->noquote() << L"std::pair(" << value.first << L',' << value.second << L')';
		return maybeSpace();
	}

	template <typename... Types>
	CXXDebug& operator<<(const std::tuple<Types...>& value)
	{
		this->noquote() << L"std::tuple(";
		printTupleElements(*this, value);  // 調用遞歸函數展開 tuple
		*this << L')';
		return maybeSpace();
	}

	//@隐藏}

private:
	//@隐藏{
	// 遞歸地將 tuple 的元素打印出來
	template <std::size_t Index = 0, typename... Types>
	typename std::enable_if<Index == sizeof...(Types), void>::type
		printTupleElements(CXXDebug& debug, const std::tuple<Types...>& tuple)
	{
		// 結束條件，不執行任何操作
	}

	template <std::size_t Index = 0, typename... Types>
	typename std::enable_if < Index < sizeof...(Types), void>::type
		printTupleElements(CXXDebug& debug, const std::tuple<Types...>& tuple)
	{
		// 打印當前元素
		if (Index > 0)
			debug << L","; // 元素之間添加逗號
		debug << std::get<Index>(tuple);

		// 遞歸打印下一個元素
		printTupleElements<Index + 1>(debug, tuple);
	}

	std::wostringstream& currentStream();

	bool isNoQuote() const;

	bool isNoOutput() const;

	CXXDebug& maybeSpace();

	static std::unordered_map<std::wstring, std::wstring>& customFormatHashRef();

	template <typename SequentialContainer>
	inline CXXDebug& printSequentialContainer(const wchar_t* which, const SequentialContainer& c)
	{
		typename SequentialContainer::const_iterator it = c.begin();
		typename SequentialContainer::const_iterator end = c.end();


		this->noquote() << which << L'(';

		if (it != end)
		{
			this->noquote() << *it;
			++it;
		}
		while (it != end)
		{
			this->noquote() << L',' << *it;
			++it;
		}

		this->noquote() << L')';

		return maybeSpace();
	}
	//@隐藏}
private:
	CXX_DECLARE_PRIVATE(CXXDebug);
	CXXScopedPointer<CXXDebugPrivate> d_ptr;
};

//@别名 控制台颜色
enum ConsoleColor
	//@隐藏{
	: WORD
	//@隐藏}
{
	// 前景色
	Black = 0,
	DarkBlue = FOREGROUND_BLUE,
	DarkGreen = FOREGROUND_GREEN,
	DarkCyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
	DarkRed = FOREGROUND_RED,
	DarkMagenta = FOREGROUND_RED | FOREGROUND_BLUE,
	DarkYellow = FOREGROUND_RED | FOREGROUND_GREEN,
	DarkGray = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	Gray = FOREGROUND_INTENSITY,
	Blue = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
	Green = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	Cyan = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
	Red = FOREGROUND_INTENSITY | FOREGROUND_RED,
	Magenta = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
	Yellow = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
	White = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	// 背景色
	BackgroundBlack = 0,
	BackgroundDarkBlue = BACKGROUND_BLUE,
	BackgroundDarkGreen = BACKGROUND_GREEN,
	BackgroundDarkCyan = BACKGROUND_GREEN | BACKGROUND_BLUE,
	BackgroundDarkRed = BACKGROUND_RED,
	BackgroundDarkMagenta = BACKGROUND_RED | BACKGROUND_BLUE,
	BackgroundDarkYellow = BACKGROUND_RED | BACKGROUND_GREEN,
	BackgroundDarkGray = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	BackgroundGray = BACKGROUND_INTENSITY,
	BackgroundBlue = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
	BackgroundGreen = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
	BackgroundCyan = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
	BackgroundRed = BACKGROUND_INTENSITY | BACKGROUND_RED,
	BackgroundMagenta = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
	BackgroundYellow = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
	BackgroundWhite = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
};

#include <iostream>
#include <iomanip>
#include <string>
#include <windows.h>  // For USHORT type

//@别名 炫控制台进度条
class CXXConsoleProgressBar
{
public:
	CXXConsoleProgressBar() {}

	CXXConsoleProgressBar(__int64 width);

	CXXConsoleProgressBar(__int64 width, wchar_t fillChar);

	CXXConsoleProgressBar(__int64 width, wchar_t fillChar, bool showPercentage);

	//@备注 设置填充字符
	//@别名 置填充字符(宽字符)
	void setFillCharacter(wchar_t newFillChar)
	{
		fillChar_ = newFillChar;
	}

	//@备注 控制是否显示数字百分比
	//@别名 置显示百分比(是否显示)
	void setShowPercentage(bool show)
	{
		showPercentage_ = show;
	}

	//@备注 设置进度条颜色
	//@别名 置颜色(进度)
	void setColor(USHORT color)
	{
		color_ = color;
	}

	//@备注 设置后外框字符
	//@别名 置前后框字符(宽字符)
	void setBorderCharacter(wchar_t newFrontBorderChar, wchar_t newBackBorderChar)
	{
		backBorderChar_ = newBackBorderChar;
		frontBorderChar_ = newFrontBorderChar;
	}

	//@备注 设置外框颜色
	//@别名 置框颜色(颜色)
	void setBorderColor(USHORT color)
	{
		borderColor_ = color;
	}

	//@备注 更新进度 双精度浮点数 0.01相当于 1%
	//@别名 更新(进度)
	void update(double progressPercentage) const;

private:
	size_t width_ = 50;  // 进度条的宽度
	wchar_t fillChar_ = L'#'; // 用于填充进度条的字符
	wchar_t frontBorderChar_ = L'['; // 进度条前外框字符
	wchar_t backBorderChar_ = L']'; // 进度条后外框字符
	bool showPercentage_ = true; // 是否在末尾显示百分比
	USHORT color_ = cxx::BrightGreen;  // ANSI Magenta 的颜色代码
	USHORT borderColor_ = cxx::White;  // 默认外框颜色为白色 (ANSI 白色代码)
};


//@备注 打印一般信息(白色)包含函数名、行号 用法: cxxPrint << "信息"
//@别名 炫打印
#ifndef cxxPrint
#ifndef CXX_NO_DEBUG_OUTPUT
#define cxxPrint() CXXDebug(cxx::DebugOutputLevel::None, CXX_FUNCTION, __LINE__).noquote()
#else
#define cxxPrint() while (false) CXXDebug(cxx::DebugOutputLevel::Nothing)
#endif
#endif

//@备注 打印调试信息(绿色)包含函数名、行号 用法: cxxDebug << "调试信息"
//@别名 炫输出
#ifndef cxxDebug
#ifndef CXX_NO_DEBUG_OUTPUT
#define cxxDebug() CXXDebug(cxx::DebugOutputLevel::Debug, CXX_FUNCTION, __LINE__).noquote()
#else
#define cxxDebug() while (false) CXXDebug(cxx::DebugOutputLevel::Nothing)
#endif
#endif

//@备注 打印信息(蓝色)包含函数名、行号 用法: cxxInfo << "信息"
//@别名 炫信息
#ifndef cxxInfo
#ifndef CXX_NO_INFO_OUTPUT
#define cxxInfo() CXXDebug(cxx::DebugOutputLevel::Info, CXX_FUNCTION, __LINE__).noquote()
#else
#define cxxInfo() while (false) CXXDebug(cxx::DebugOutputLevel::Nothing)
#endif
#endif

//@备注 打印警告调试信息(橘色)包含函数名、行号 用法: cxxWarn << "警告信息"
//@别名 炫警告
#ifndef cxxWarn
#ifndef CXX_NO_WARN_OUTPUT
#define cxxWarn() CXXDebug(cxx::DebugOutputLevel::Warn, CXX_FUNCTION, __LINE__).noquote()
#else
#define cxxWarn() while (false) CXXDebug(cxx::DebugOutputLevel::Nothing)
#endif
#endif

//@备注 打印错误调试信息(红色)包含函数名、行号 用法: cxxError << "错误信息"
//@别名 炫错误
#ifndef cxxError
#ifndef CXX_NO_ERROR_OUTPUT
#define cxxError() CXXDebug(cxx::DebugOutputLevel::Error, CXX_FUNCTION, __LINE__).noquote()
#else
#define cxxError() while (false) CXXDebug(cxx::DebugOutputLevel::Nothing)
#endif
#endif

//@备注 打印致命错误调试信息(深红色)包含函数名、行号，注意：此函数会主动引发程序崩溃并弹出消息框显示错误信息 用法: cxxFatal << "致命错误信息"
//@别名 炫致命错误
#ifndef cxxFatal
#define cxxFatal() CXXDebug(CXX_FUNCTION, __LINE__).noquote() << CXX_FUNCTION << L"@" << __LINE__ << L":"
#endif

//@分组}

#endif // CXXDEBUG_H_