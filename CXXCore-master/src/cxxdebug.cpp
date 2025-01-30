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
#include <pch.h>

#include <cxxdebug_p.h>
#include <cxxtextcodec.h>

#include <chrono>
#include <ctime>
#include <io.h>
#include <fcntl.h>
#include <locale>
#include <stdexcept>
#include <memory>

std::wstring CXXDebugPrivate::prefixFormat_ = L"{time} | {level} | {tid}";
std::wstring CXXDebugPrivate::suffixFormat_ = L"";
std::wstring CXXDebugPrivate::timeFormat_ = L"yyyy/MM/dd hh:mm:ss.zzz ap";
std::wstring CXXDebugPrivate::levelString_ = L"{NORMAL}";
std::unordered_map<std::wstring, std::wstring> CXXDebugPrivate::customFormatHash_;
std::unordered_map<std::wstring, std::string> CXXDebugPrivate::customColorFormatHash_;
bool CXXDebugPrivate::ansiColorEnabled_ = true;

CXXMessageHandler CXXDebugPrivate::messageHandler_ = CXX_NULLPTR;

__int64 CXXDebugPrivate::currentCodePage_ = CXXDebugPrivate::ANSI;

namespace cxx_debug_private
{
	int _oldStdInMode = 0;
	int _oldStdOutMode = 0;
	int _oldStdErrMode = 0;

	namespace ansi
	{
		constexpr const char* RESET = "\033[0m";			// 重置
		constexpr const char* RED = "\033[31m";				// 紅色
		constexpr const char* GREEN = "\033[32m";			// 綠色
		constexpr const char* YELLOW = "\033[33m";			// 黃色
		constexpr const char* BLUE = "\033[34m";			// 藍色
		constexpr const char* MAGENTA = "\033[35m";			// 紫色
		constexpr const char* CYAN = "\033[36m";			// 青色
		constexpr const char* WHITE = "\033[37m";			// 白色
		constexpr const char* LIGHT_BLACK = "\033[90m";		// 亮黑（灰）
		constexpr const char* LIGHT_RED = "\033[91m";		// 亮紅
		constexpr const char* LIGHT_GREEN = "\033[92m";		// 亮綠
		constexpr const char* LIGHT_YELLOW = "\033[93m";	// 亮黃
		constexpr const char* LIGHT_BLUE = "\033[94m";		// 亮藍
		constexpr const char* LIGHT_MAGENTA = "\033[95m";	// 亮品紅
		constexpr const char* LIGHT_CYAN = "\033[96m";		// 亮青
		constexpr const char* LIGHT_WHITE = "\033[97m";		// 亮白
		constexpr const char* BOLD = "\033[1m";				// 粗體

		static bool enableANSIColors()
		{
			// 獲取標準輸出句柄
			HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
			if (hOut == INVALID_HANDLE_VALUE)
			{
				return false;
			}

			DWORD dwMode = 0;
			if (FALSE == GetConsoleMode(hOut, &dwMode))
			{
				return false;
			}

			if (dwMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)
			{
				return true;
			}

			// 啟用 ANSI 轉義序列支持
			dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			if (FALSE == SetConsoleMode(hOut, dwMode))
			{
				return false;
			}

			return true;
		}
	}
}

CXXDebugPrivate::CXXDebugPrivate(CXXDebug* q, cxx::DebugOutputLevel flags)
	: q_ptr(q)
	, flags_(flags)
	, hConsole_(GetStdHandle(STD_OUTPUT_HANDLE))
{
	if (flags_ == cxx::DebugOutputLevel::Nothing)
	{
		noOutput_ = true;
	}

	WORD COLOR = 0;
	switch (flags)
	{
	case cxx::DebugOutputLevel::None:
	{
		COLOR = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		levelString_ = L"{NORMAL}";

		break;
	}
	case cxx::DebugOutputLevel::Info:
	{
		COLOR = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		levelString_ = L"{INFO}";

		break;
	}
	case cxx::DebugOutputLevel::Debug:
	{
		COLOR = FOREGROUND_GREEN;
		levelString_ = L"{DEBUG}";

		break;
	}
	case cxx::DebugOutputLevel::Warn:
	{
		COLOR = FOREGROUND_RED | FOREGROUND_GREEN;
		levelString_ = L"{WARN}";

		break;
	}
	case cxx::DebugOutputLevel::Error:
	{
		COLOR = FOREGROUND_RED | FOREGROUND_INTENSITY;
		levelString_ = L"{ERROR}";

		break;
	}
	case cxx::DebugOutputLevel::Fatal:
	{
		COLOR = FOREGROUND_RED;
		levelString_ = L"{FATAL}";
		break;
	}
	}

	setConsoleTextColor(COLOR);

	static bool init = false;
	if (!init)
	{
		init = true;
		CXXDebug::addColorCustomFormat(L"{NORMAL}", cxx_debug_private::ansi::WHITE);
		CXXDebug::addColorCustomFormat(L"{INFO}", cxx_debug_private::ansi::BLUE);
		CXXDebug::addColorCustomFormat(L"{DEBUG}", cxx_debug_private::ansi::GREEN);
		CXXDebug::addColorCustomFormat(L"{WARN}", cxx_debug_private::ansi::YELLOW);
		CXXDebug::addColorCustomFormat(L"{ERROR}", cxx_debug_private::ansi::RED);
		CXXDebug::addColorCustomFormat(L"{FATAL}", cxx_debug_private::ansi::LIGHT_RED);

		cxx_debug_private::ansi::enableANSIColors();
	}
}

CXXDebugPrivate::~CXXDebugPrivate()
{

}

CXXDebug& CXXDebugPrivate::maybeSpace()
{
	// if not end with ' ' , add a space
	std::wstring str = outputBuffer_.str();
	if (!str.empty() && str.back() != L' ')
	{
		outputBuffer_ << L" ";
	}

	return *q_ptr;
}

std::wstring CXXDebugPrivate::getCurrentTimeString() const
{
	static _DateTimePrivate dt;
	std::wstring formattedTime = dt.format(timeFormat_);
	return formattedTime.c_str();
}

void CXXDebugPrivate::setConsoleTextColor(WORD color) const
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole_, &csbi);
	SetConsoleTextAttribute(hConsole_, (csbi.wAttributes & BACKGROUND_COLOR_MASK) | color);
}

std::wstring CXXDebugPrivate::replaceAll(const std::wstring& input, const std::wstring& search, const std::wstring& replace)
{
	if (search.empty())  // 防止无限循环，如果 search 为空直接返回原始字符串
	{
		return input;
	}

	std::wstring output = input;
	size_t pos = 0;

	// 使用 for(;;) 循环代替 while
	for (;;)
	{
		// 查找 search 字符串在 output 中的位置
		pos = output.find(search, pos);
		if (pos == std::wstring::npos)  // 如果没找到，跳出循环
		{
			break;
		}

		// 进行替换
		output.replace(pos, search.length(), replace);

		// 更新位置，跳过已替换的部分
		pos += replace.length();
	}

	return output;
}

std::string::size_type CXXDebugPrivate::findSubstring(const std::string& input, const std::string& search, std::string::size_type startPos, std::string::size_type* endPos)
{
	std::string::size_type inputLen = input.length();
	std::string::size_type searchLen = search.length();

	// 检查边界条件
	if (searchLen == 0 || inputLen == 0 || searchLen > inputLen || startPos >= inputLen)
	{
		return std::string::npos;  // 如果无效输入，返回 npos
	}

	// 遍历从 startPos 开始的输入字符串
	for (std::string::size_type i = startPos; i <= inputLen - searchLen; ++i)
	{
		bool matched = true;

		// 逐字节匹配搜索词
		for (std::string::size_type j = 0; j < searchLen; ++j)
		{
			if (input[i + j] == '\0' || search[j] == '\0')
			{
				break;
			}

			if (input[i + j] != search[j])
			{
				matched = false;  // 发现不匹配，跳出内部循环
				break;
			}
		}

		// 如果找到完全匹配的子字符串
		if (matched)
		{
			if (endPos) // 如果 endPos 不为 nullptr，则设置结束位置
			{
				*endPos = i + searchLen;
			}
			return i;  // 返回匹配的起始位置
		}
	}

	return std::string::npos;  // 如果没有找到，返回 npos
}

bool CXXDebugPrivate::subStringReplace(std::string& input, const std::string& search, const std::string& replace)
{
	std::string::size_type pos = 0;
	std::string::size_type endPos = 0;
	size_t count = 0;

	// 循环查找待替换的字符串
	for (;;)
	{
		pos = findSubstring(input, search, pos, &endPos);
		if (std::string::npos == pos)
		{
			break;
		}

		// 移除找到的子字符串
		input.erase(pos, search.size());  // 移除指定长度的子串

		// 插入替换字符串
		input.insert(pos, replace);  // 在原来的位置插入替换的字符串

		// 更新位置，跳过当前替换的部分，避免无限循环
		pos += replace.size();
		++count;
	}

	return count > 0;
}

bool CXXDebugPrivate::replaceAllToColor(const std::string& input, const std::string& search, const std::string& colorStr, std::string& output)
{
	// [{customText}] ->  [colorStr + customText + ansi::RESET]
	std::string _input = input;

	std::string _prereplace = search;

	std::string _colorStr = colorStr;
	std::string _reset = cxx_debug_private::ansi::RESET;

	if (_prereplace.front() == '{' && _prereplace[_prereplace.size() - 1] == '}')
	{
		_prereplace = _prereplace.substr(1, _prereplace.size() - 2);
	}

	std::string newString = _colorStr + _prereplace + _reset;

	bool ok = subStringReplace(_input, search, std::move(newString));
	if (ok)
	{
		output = std::move(_input);  // 使用 std::move 赋值
	}

	return ok;
}

bool CXXDebugPrivate::replaceCustomColorFormat(std::wstring& text) const
{
	if (!ansiColorEnabled_)
	{
		return false;
	}

	SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	std::string newStr = CXXString(text).toUtf8();
	for (auto it = customColorFormatHash_.begin(); it != customColorFormatHash_.end(); ++it)
	{
		std::ignore = replaceAllToColor(newStr, CXXString(it->first).toUtf8(), it->second, newStr);
	}

	auto findAndRelace = [](std::string& src, const std::string& replacefrom, const std::string& replaceTo)->void
		{
			size_t replaceToSize = replacefrom.size();
			size_t pos = std::string::npos;
			for (;;)
			{
				pos = src.find(replacefrom);
				if (pos == std::string::npos)
				{
					break;
				}

				src.replace(pos, replaceToSize, replaceTo);
				pos += replaceToSize;
			}
		};

	findAndRelace(newStr, "$GREEN", cxx_debug_private::ansi::GREEN);
	findAndRelace(newStr, "$LIGHT_GREEN", cxx_debug_private::ansi::LIGHT_GREEN);
	findAndRelace(newStr, "$RESET", cxx_debug_private::ansi::RESET);

	CXXString str = CXXString::fromUtf8(newStr.c_str());
	std::wcout << str.c_str() << std::endl;

	return true;
}

void CXXDebugPrivate::replacePrefixOrSuffix(const std::wstring& text, std::wstring& dst, bool forceNoColor) const
{
	std::wstring _prefix = text;

	std::wstring time = getCurrentTimeString();
	std::wstring tid = std::to_wstring(GetCurrentThreadId());
	std::wstring pid = std::to_wstring(GetCurrentProcessId());

	if (ansiColorEnabled_ && !forceNoColor)
	{
		_prefix = replaceAll(_prefix, L"{level}", levelString_);

		const std::wstring _reset = L"$RESET";

		{
			const std::wstring _colorStr = L"$LIGHT_GREEN";

			const std::wstring coloredTID = _colorStr + tid + _reset;

			_prefix = replaceAll(_prefix, L"{tid}", coloredTID);

			const std::wstring coloredPID = _colorStr + pid + _reset;

			_prefix = replaceAll(_prefix, L"{pid}", coloredPID);
		}


		{
			const std::wstring _colorStr = L"$GREEN";

			// 确保拼接后的字符串正确
			const std::wstring coloredTime = _colorStr + time + _reset;

			// 只替换 {time} 部分，而不是整个字符串
			dst = replaceAll(_prefix, L"{time}", coloredTime);
		}
	}
	else
	{
		levelString_.erase(std::remove(levelString_.begin(), levelString_.end(), L'{'), levelString_.end());
		levelString_.erase(std::remove(levelString_.begin(), levelString_.end(), L'}'), levelString_.end());
		_prefix = replaceAll(_prefix, L"{level}", levelString_);
		_prefix = replaceAll(_prefix, L"{tid}", tid);
		_prefix = replaceAll(_prefix, L"{pid}", std::to_wstring(GetCurrentProcessId()));
		dst = replaceAll(_prefix, L"{time}", time);
	}
}

void CXXDebugPrivate::replaceCustomFormat(std::wstring& text) const
{
	for (auto it = customFormatHash_.begin(); it != customFormatHash_.end(); ++it)
	{
		text = replaceAll(text, it->first.c_str(), it->second.c_str());
	}
}

CXXDebug::CXXDebug()
	: d_ptr(new CXXDebugPrivate(this, cxx::DebugOutputLevel::None))
{
}

CXXDebug::CXXDebug(cxx::DebugOutputLevel flags)
	: d_ptr(new CXXDebugPrivate(this, flags))
{
}

CXXDebug::CXXDebug(cxx::DebugOutputLevel flags, const wchar_t* contextFile, size_t line)
	: d_ptr(new CXXDebugPrivate(this, flags))
{
	CXX_D(CXXDebug);

	d->currentLine_ = line;
	d->currentFile_ = contextFile;
}

CXXDebug::CXXDebug(const wchar_t* contextFile, size_t line)
	: d_ptr(new CXXDebugPrivate(this, cxx::DebugOutputLevel::Fatal))

{
	CXX_D(CXXDebug);

	d->currentLine_ = line;
	d->currentFile_ = contextFile;
}

CXXDebug::~CXXDebug()
{
	CXX_D(CXXDebug);

	if (d->noOutput_)
	{
		return;
	}

#ifndef CXX_NO_DEBUG_OUTPUT
	// 前缀替换新数值
	d->replacePrefixOrSuffix(d->prefixFormat_, d->prefix_);
	d->replacePrefixOrSuffix(d->prefixFormat_, d->prefixOriginal_, true);

	// 后缀替换新数值
	d->replacePrefixOrSuffix(d->suffixFormat_, d->suffix_);
	d->replacePrefixOrSuffix(d->suffixFormat_, d->suffixOriginal_, true);


	// 添加前缀
	std::wstring outputString;
	std::wstring outputStringOriginal;
	if (static_cast<__int64>(d->prefix_.size()) > 0)
	{
		outputString = (d->prefix_ + L" | ");
		outputStringOriginal = (d->prefixOriginal_ + L" | ");
	}

	// 添加行号、文件名
	if (!d->currentFile_.empty())
	{
		outputString += L" ";
		outputString += d->currentFile_;
		outputString += L" @ ";
		outputString += std::to_wstring(d->currentLine_);
		outputString += L" : ";

		outputStringOriginal += L" ";
		outputStringOriginal += d->currentFile_;
		outputStringOriginal += L" @ ";
		outputStringOriginal += std::to_wstring(d->currentLine_);
		outputStringOriginal += L" : ";
	}

	// 添加输出内容
	outputString += d->outputBuffer_.str();

	// 添加原始输出内容
	outputStringOriginal += d->outputBuffer_.str();

	// 替换自定义格式
	d->replaceCustomFormat(outputString);
	d->replaceCustomFormat(outputStringOriginal);

	// 添加后缀
	if (static_cast<__int64>(d->suffix_.size()) > 0)
	{
		outputString.append(d->suffix_);
		outputStringOriginal.append(d->suffixOriginal_);
	}

	// 调用消息处理器
	bool bret = false;
	if (d->messageHandler_ != CXX_NULLPTR)
	{
		bret = d->messageHandler_(d->flags_, CXXString(outputStringOriginal));
		if (bret)
		{
			return;
		}
	}

	// 添加换行符
	if (outputString.back() != L'\n')
	{
		outputString += L"\n";
	}

	if (outputStringOriginal.back() != L'\n')
	{
		outputStringOriginal += L"\n";
	}

	// 输出到IDE调试窗口
	OutputDebugStringW(outputStringOriginal.c_str());

	do
	{
		if (cxx::DebugOutputLevel::Fatal == d->flags_)
		{
			// 输出到标准错误
			std::wcerr << outputString;

			// 弹出系统消息框
			cxx::message_fatal(d->currentFile_.data(), d->currentLine_, outputString);

			break;
		}

		if (!d->replaceCustomColorFormat(outputString))
		{
			if (cxx::DebugOutputLevel::Error != d->flags_)
			{
				// 输出到标准错误
				std::wcerr << outputString;
			}
			else
			{
				// 输出到标准输出
				std::wcout << outputString;
			}
		}
	} while (false);

	// 重置颜色
	SetConsoleTextAttribute(d->hConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif
}

std::wostringstream& CXXDebug::currentStream()
{
	CXX_D(CXXDebug);
	return d->outputBuffer_;
}

bool CXXDebug::isNoQuote() const
{
	CXX_D(const CXXDebug);
	return d->noQuote_;

}

CXXDebug& CXXDebug::maybeSpace()
{
	CXX_D(CXXDebug);
	return d->maybeSpace();
}

std::unordered_map<std::wstring, std::wstring>& CXXDebug::customFormatHashRef()
{
	return CXXDebugPrivate::customFormatHash_;
}

bool CXXDebug::isNoOutput() const
{
	CXX_D(const CXXDebug);
	return d->noOutput_;
}

void CXXDebug::setCodePage()
{
	static bool isSet = false;

	if (isSet)
	{
		return;
	}

	isSet = true;

	setlocale(LC_ALL, "en_US.UTF-8");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	//#ifdef CXX_FORCE_UTF8
	//cxx_debug_private::_oldStdInMode = _setmode(_fileno(stdin), _O_U8TEXT);
	//cxx_debug_private::_oldStdOutMode = _setmode(_fileno(stdout), _O_U8TEXT);
	//cxx_debug_private::_oldStdErrMode = _setmode(_fileno(stderr), _O_U8TEXT);
	//	currentCodePage_ = UTF8;
	//#else
	cxx_debug_private::_oldStdInMode = _setmode(_fileno(stdin), _O_WTEXT);
	cxx_debug_private::_oldStdOutMode = _setmode(_fileno(stdout), _O_WTEXT);
	cxx_debug_private::_oldStdErrMode = _setmode(_fileno(stderr), _O_WTEXT);
	//#endif
}

CXXDebug& CXXDebug::noquote()
{
	CXX_D(CXXDebug);

	d->noQuote_ = true;
	return *this;
}

CXXDebug& CXXDebug::raw()
{
	CXX_D(CXXDebug);

	d->isRaw_ = true;
	return *this;
}

void CXXDebug::swap(CXXDebug& other)
{
	CXX_D(CXXDebug);

	std::swap(d->outputBuffer_, other.d_ptr->outputBuffer_);
}

CXXString CXXDebug::toString()
{
	CXX_D(CXXDebug);
	return d->outputBuffer_.str();
}

//@备注 设置调试信息前缀 使用方式: debug::setPrefix("{time}, {pid}, {tid}");
//@参数 前缀字符串
//@返回 无
//@别名 置前缀(void)
void CXXDebug::setPrefix(const CXXString& prefix)
{
	CXXDebugPrivate::prefixFormat_ = prefix;
}

//@备注 设置调试信息后缀 使用方式: debug::setSuffix("{time}, {pid}, {tid}");
//@参数 后缀字符串
//@返回 无
//@别名 置后缀(void)
void CXXDebug::setSuffix(const CXXString& suffix)
{
	CXXDebugPrivate::suffixFormat_ = suffix;
}

//@备注 格式参数: hh:mm:ss.zzz ap | yyyy/MM/dd | hh:MM:ss | hh:MM | hh | MM | ss | zzz | ap | ddd | dddd | MMMM | MMM
//@参数 无
//@返回 无
//@别名 创建控制台(是否分离)
void CXXDebug::setTimeFormat(const CXXString& format)
{
	CXXDebugPrivate::timeFormat_ = format;
}

//@别名 置启用ANSI转义序列(是否启用)
void CXXDebug::setANSIColorEnabled(bool enabled)
{
	CXXDebugPrivate::ansiColorEnabled_ = enabled;
}

void CXXDebug::addColorCustomFormat(const CXXString& format, const char* color)
{
	CXXDebugPrivate::customColorFormatHash_.insert(std::pair<std::wstring, std::string>(format.toStdWString(), color));
}

//@备注 添加自定义格式化 如: 添加自订格式("{哈哈哈}", 12345) 最终输出时 所有的 {哈哈哈} 都会自动替换成 12345
//要注意数值只能接受 文本型/字符型/整型/双浮点型/浮点型
//@参数 无
//@返回 无
//@别名 加自订格式(字符串格式, 数值)
void CXXDebug::addCustomFormat(const CXXString& format, int value)
{
	static const std::wregex reg(LR"(\{([a-zA-Z0-9\u4e00-\u9fa5]+)\})");
	std::wsmatch match;
	std::wstring wformat = format.toStdWString();
	if (std::regex_search(wformat, match, reg))
	{
		std::wostringstream ss;
		ss << std::to_wstring(value);
		CXXDebugPrivate::customFormatHash_.insert(std::pair<std::wstring, std::wstring>(wformat, ss.str()));
	}
}

//@备注 添加自定义格式化 如: 添加自订格式("{哈哈哈}", 12345) 最终输出时 所有的 {哈哈哈} 都会自动替换成 12345
//要注意数值只能接受 文本型/字符型/整型/双浮点型/浮点型
//@参数 无
//@返回 无
//@别名 加自订格式(字符串格式, 数值)
void CXXDebug::addCustomFormat(const CXXString& format, UINT value)
{
	static const std::wregex reg(LR"(\{([a-zA-Z0-9\u4e00-\u9fa5]+)\})");
	std::wsmatch match;
	std::wstring wformat = format.toStdWString();
	if (std::regex_search(wformat, match, reg))
	{
		std::wostringstream ss;
		ss << std::to_wstring(value);
		CXXDebugPrivate::customFormatHash_.insert(std::pair<std::wstring, std::wstring>(wformat, ss.str()));
	}
}

//@备注 添加自定义格式化 如: 添加自订格式("{哈哈哈}", 12345) 最终输出时 所有的 {哈哈哈} 都会自动替换成 12345
//要注意数值只能接受 文本型/字符型/整型/双浮点型/浮点型
//@参数 无
//@返回 无
//@别名 加自订格式(字符串格式, 数值)
void CXXDebug::addCustomFormat(const CXXString& format, float value)
{
	static const std::wregex reg(LR"(\{([a-zA-Z0-9\u4e00-\u9fa5]+)\})");
	std::wsmatch match;
	std::wstring wformat = format.toStdWString();
	if (std::regex_search(wformat, match, reg))
	{
		std::wostringstream ss;
		ss << std::fixed << std::setprecision(static_cast<std::streamsize>(std::numeric_limits<float>::digits10) + 1) << value << L'f';
		CXXDebugPrivate::customFormatHash_.insert(std::pair<std::wstring, std::wstring>(wformat, ss.str()));
	}
}

//@备注 添加自定义格式化 如: 添加自订格式("{哈哈哈}", 12345) 最终输出时 所有的 {哈哈哈} 都会自动替换成 12345
//要注意数值只能接受 文本型/字符型/整型/双浮点型/浮点型
//@参数 无
//@返回 无
//@别名 加自订格式(字符串格式, 数值)
void CXXDebug::addCustomFormat(const CXXString& format, double value)
{
	static const std::wregex reg(LR"(\{([a-zA-Z0-9\u4e00-\u9fa5]+)\})");
	std::wsmatch match;
	std::wstring wformat = format.toStdWString();
	if (std::regex_search(wformat, match, reg))
	{
		std::wostringstream ss;
		ss << std::fixed << std::setprecision(static_cast<std::streamsize>(std::numeric_limits<float>::digits10) + 1) << value;
		CXXDebugPrivate::customFormatHash_.insert(std::pair<std::wstring, std::wstring>(wformat, ss.str()));
	}
}

//@备注 添加自定义格式化 如: 添加自订格式("{哈哈哈}", 12345) 最终输出时 所有的 {哈哈哈} 都会自动替换成 12345
//要注意数值只能接受 文本型/字符型/整型/双浮点型/浮点型
//@参数 无
//@返回 无
//@别名 加自订格式(字符串格式, 数值)
void CXXDebug::addCustomFormat(const CXXString& format, const char* value)
{
	static const std::wregex reg(LR"(\{([a-zA-Z0-9\u4e00-\u9fa5]+)\})");
	std::wsmatch match;
	std::wstring wformat = format.toStdWString();
	if (std::regex_search(wformat, match, reg))
	{
		std::wostringstream ss;
		ss << CXXString::fromUtf8(value).toStdWString();
		CXXDebugPrivate::customFormatHash_.insert(std::pair<std::wstring, std::wstring>(wformat, ss.str()));
	}
}

//@备注 添加自定义格式化 如: 添加自订格式("{哈哈哈}", 12345) 最终输出时 所有的 {哈哈哈} 都会自动替换成 12345
//要注意数值只能接受 文本型/字符型/整型/双浮点型/浮点型
//@参数 无
//@返回 无
//@别名 加自订格式(字符串格式, 数值)
void CXXDebug::addCustomFormat(const CXXString& format, const wchar_t* value)
{
	static const std::wregex reg(LR"(\{([a-zA-Z0-9\u4e00-\u9fa5]+)\})");
	std::wsmatch match;
	std::wstring wformat = format.toStdWString();
	if (std::regex_search(wformat, match, reg))
	{
		std::wostringstream ss;
		ss << std::wstring(value);
		CXXDebugPrivate::customFormatHash_.insert(std::pair<std::wstring, std::wstring>(wformat, ss.str()));
	}
}

HWND CXXDebug::createConsole()
{
	if (FALSE == AllocConsole())
	{
		return CXX_NULLPTR;
	}

	FILE* f_dummy;
	freopen_s(&f_dummy, "CONOUT$", "w", stdout);
	freopen_s(&f_dummy, "CONOUT$", "w", stderr);
	freopen_s(&f_dummy, "CONIN$", "r", stdin);

	std::cout.clear();
	std::clog.clear();
	std::cerr.clear();
	std::cin.clear();

	HANDLE h_con_out = CreateFileW(TEXT("CONOUT$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, CXX_NULLPTR, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, CXX_NULLPTR);
	HANDLE h_con_in = CreateFileW(TEXT("CONIN$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, CXX_NULLPTR, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, CXX_NULLPTR);
	SetStdHandle(STD_OUTPUT_HANDLE, h_con_out);
	SetStdHandle(STD_ERROR_HANDLE, h_con_out);
	SetStdHandle(STD_INPUT_HANDLE, h_con_in);

	std::wcout.clear();
	std::wclog.clear();
	std::wcerr.clear();
	std::wcin.clear();

	SetConsoleCtrlHandler(CXX_NULLPTR, FALSE);

	return GetConsoleWindow();
}

//@备注 安装自订消息处理回调，所有输出在实际打印之前，都会先调用此回调函数，如果返回false则不会打印
//      typedef BOOL(__stdcall* CXMessageHandler)(DebugFlags flags, const CXXString& text);
//@参数 消息处理回调函数
//@返回 无
//@别名 安装消息处理器(消息处理器)
void CXXDebug::installMessageHandler(CXXMessageHandler handler)
{
	CXXDebugPrivate::messageHandler_ = handler;
}

CXXDebug& CXXDebug::operator<<(bool value)
{
	CXX_D(CXXDebug);

	d->outputBuffer_ << (value ? L"true" : L"false");

	return d->maybeSpace();
}

CXXDebug& CXXDebug::operator<<(BYTE value)
{
	CXX_D(CXXDebug);

	if (!d->noQuote_)
	{
		d->outputBuffer_ << value << L"b";
	}
	else
	{
		d->outputBuffer_ << value;
	}

	return d->maybeSpace();
}

CXXDebug& CXXDebug::operator<<(char value)
{
	CXX_D(CXXDebug);

	char s[2] = { value, 0 };
	std::wstring data(CXXTextCodec::codecFromName(L"UTF-8")->toUnicode(s));

	if (d->isRaw_)
	{
		d->outputBuffer_ << data;
	}
	else if (!d->noQuote_)
	{
		d->outputBuffer_ << L"a'" << data << L"'";
	}
	else
	{
		d->outputBuffer_ << data;
	}

	return d->maybeSpace();
}

CXXDebug& CXXDebug::operator<<(short t)
{
	CXX_D(CXXDebug);

	if (!d->noQuote_)
	{
		d->outputBuffer_ << CXXString::number(t).toStdWString() << L"i16";
	}
	else
	{
		d->outputBuffer_ << CXXString::number(t).toStdWString();
	}

	return d->maybeSpace();
}

CXXDebug& CXXDebug::operator<<(USHORT t)
{
	CXX_D(CXXDebug);
	if (!d->noQuote_)
	{
		d->outputBuffer_ << CXXString::number(t).toStdWString() << L"ui16";
	}
	else
	{
		d->outputBuffer_ << CXXString::number(t).toStdWString();
	}

	return d->maybeSpace();
}

CXXDebug& CXXDebug::operator<<(int t)
{
	CXX_D(CXXDebug);

	if (!d->noQuote_)
	{
		d->outputBuffer_ << CXXString::number(t).toStdWString() << L"i32";
	}
	else
	{
		d->outputBuffer_ << CXXString::number(t).toStdWString();
	}

	return d->maybeSpace();
}

CXXDebug& CXXDebug::operator<<(UINT t)
{
	CXX_D(CXXDebug);

	if (!d->noQuote_)
	{
		d->outputBuffer_ << CXXString::number(t).toStdWString() << L"ui32";
	}
	else
	{
		d->outputBuffer_ << CXXString::number(t).toStdWString();
	}

	return d->maybeSpace();
}

CXXDebug& CXXDebug::operator<<(__int64 t)
{
	CXX_D(CXXDebug);

	if (!d->noQuote_)
	{
		d->outputBuffer_ << CXXString::number(t).toStdWString() << L"i64";
	}
	else
	{
		d->outputBuffer_ << CXXString::number(t).toStdWString();
	}

	return d->maybeSpace();
}

CXXDebug& CXXDebug::operator<<(UINT64 t)
{
	CXX_D(CXXDebug);

	if (!d->noQuote_)
	{
		d->outputBuffer_ << CXXString::number(t).toStdWString() << L"ui64";
	}
	else
	{
		d->outputBuffer_ << CXXString::number(t).toStdWString();
	}

	return d->maybeSpace();
}

CXXDebug& CXXDebug::operator<<(long t)
{
	CXX_D(CXXDebug);

	if (!d->noQuote_)
	{
		d->outputBuffer_ << CXXString::number(t).toStdWString() << L"L";
	}
	else
	{
		d->outputBuffer_ << CXXString::number(t).toStdWString();
	}

	return d->maybeSpace();
}

CXXDebug& CXXDebug::operator<<(ULONG t)
{
	CXX_D(CXXDebug);

	if (!d->noQuote_)
	{
		d->outputBuffer_ << CXXString::number(t).toStdWString() << L"UL";
	}
	else
	{
		d->outputBuffer_ << CXXString::number(t).toStdWString();
	}

	return d->maybeSpace();
}

CXXDebug& CXXDebug::operator<<(wchar_t t)
{
	CXX_D(CXXDebug);

	if (d->isRaw_)
	{
		std::wstring w;
		w.push_back(t);
		d->outputBuffer_ << w;
		return d->maybeSpace();
	}
	else if (!d->noQuote_)
	{
		d->outputBuffer_ << L"w'" << t << L"'";
	}
	else
	{
		d->outputBuffer_ << t;
	}

	return d->maybeSpace();
}

CXXDebug& CXXDebug::operator<<(const wchar_t* value)
{
	CXX_D(CXXDebug);

	if (d->isRaw_)
	{
		d->outputBuffer_ << value;
	}
	else if (!d->noQuote_)
	{
		d->outputBuffer_ << L"W\"" << value << L"\"";
	}
	else
	{
		d->outputBuffer_ << value;
	}

	return d->maybeSpace();
}

CXXDebug& CXXDebug::operator<<(const char* value)
{
	CXX_D(CXXDebug);

	std::wstring data(CXXTextCodec::codecFromName(L"UTF-8")->toUnicode(value));

	if (d->isRaw_)
	{
		d->outputBuffer_ << data;
	}
	else if (!d->noQuote_)
	{
		d->outputBuffer_ << L"A\"" << data << L"\"";
	}
	else
	{
		d->outputBuffer_ << data;
	}

	return d->maybeSpace();
}

CXXDebug& CXXDebug::operator<<(float value)
{
	CXX_D(CXXDebug);

	if (!d->noQuote_)
	{
		d->outputBuffer_ << std::fixed << std::setprecision(static_cast<std::streamsize>(std::numeric_limits<float>::digits10) + 1) << value << L'f';
	}
	else
	{
		d->outputBuffer_ << std::fixed << std::setprecision(static_cast<std::streamsize>(std::numeric_limits<float>::digits10) + 1) << value;
	}

	return d->maybeSpace();
}

CXXDebug& CXXDebug::operator<<(double value)
{
	CXX_D(CXXDebug);

	d->outputBuffer_ << std::fixed << std::setprecision(static_cast<std::streamsize>(std::numeric_limits<float>::digits10) + 1) << value;

	return d->maybeSpace();
}

CXXDebug& CXXDebug::operator<<(const CXXString& value)
{
	CXX_D(CXXDebug);

	if (d->isRaw_)
	{
		d->outputBuffer_ << value.toStdWString();
	}
	else if (!d->noQuote_)
	{
		d->outputBuffer_ << L"\"" << value.toStdWString() << L"\"";
	}
	else
	{
		d->outputBuffer_ << value.toStdWString();
	}

	return d->maybeSpace();
}

CXXDebug& CXXDebug::operator<<(const BYTE* value)
{
	CXX_D(CXXDebug);

	std::wostringstream ss;
	for (int i = 0; value[i] != 0; ++i)
	{
		ss << L"\\x" << std::hex << std::setw(2) << std::setfill(L'0') << static_cast<BYTE>(value[i]);
	}

	d->outputBuffer_ << ss.str();

	return d->maybeSpace();
}

#if _WIN32_WINNT > _WIN32_WINNT_WIN7
#if NTDDI_VERSION > NTDDI_WIN7
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
static LONG CALLBACK MinidumpCallback(PEXCEPTION_POINTERS pException)
{
	do
	{
		if (!pException)
		{
			break;
		}

		auto pathFileExists = [](const wchar_t* name)->BOOL
			{
				DWORD dwAttrib = GetFileAttributesW(name);
				return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
			};

		if (!pathFileExists(L"dump"))
		{
			CreateDirectoryW(L"dump", CXX_NULLPTR);
		}

		wchar_t pszFileName[MAX_PATH] = {};
		SYSTEMTIME stLocalTime = {};
		GetLocalTime(&stLocalTime);
		swprintf_s(pszFileName, L"dump\\%04d%02d%02d_%02d%02d%02d.dmp",
			stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
			stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond);

		HANDLE hDumpFile = CreateFileW(pszFileName, GENERIC_WRITE, NULL, CXX_NULLPTR, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, CXX_NULLPTR);
		if (hDumpFile == INVALID_HANDLE_VALUE)
		{
			break;
		}

		MINIDUMP_EXCEPTION_INFORMATION dumpInfo = {};
		dumpInfo.ExceptionPointers = pException;
		dumpInfo.ThreadId = GetCurrentThreadId();
		dumpInfo.ClientPointers = TRUE;

		MiniDumpWriteDump(
			GetCurrentProcess(),
			GetCurrentProcessId(),
			hDumpFile,
			(MINIDUMP_TYPE)(
				MiniDumpNormal
				| MiniDumpWithFullMemory
				| MiniDumpWithHandleData
				| MiniDumpWithThreadInfo
				| MiniDumpWithUnloadedModules
				| MiniDumpWithProcessThreadData
				),
			&dumpInfo,
			CXX_NULLPTR,
			CXX_NULLPTR
		);

		if (pException->ExceptionRecord->ExceptionFlags == EXCEPTION_NONCONTINUABLE)
		{
			std::wostringstream wss;
			wss << L"A fatal error occured, \r\n";
			wss << L"sorry but we have to terminate this program.\r\n";
			wss << L"please send minidump to developer.\r\n";
			wss << L"Basic Infomations:\r\n\r\n";
			wss << L"ExceptionAddress:0x" << std::hex << pException->ExceptionRecord->ExceptionAddress << L"\r\n";
			wss << L"ExceptionFlags:" << (pException->ExceptionRecord->ExceptionFlags == EXCEPTION_NONCONTINUABLE ? L"NON CONTINUEABLE" : L"CONTINUEABLE") << L"\r\n";
			wss << L"ExceptionCode:0x" << std::hex << pException->ExceptionRecord->ExceptionCode << L"\r\n";
			wss << L"NumberParameters:" << std::dec << pException->ExceptionRecord->NumberParameters << L"\r\n";

			ShellExecuteW(CXX_NULLPTR, L"open", L"dump", CXX_NULLPTR, CXX_NULLPTR, SW_SHOWNORMAL);
			MessageBoxW(CXX_NULLPTR, wss.str().c_str(), L"Fatal Error", MB_OK | MB_ICONERROR);

			return EXCEPTION_EXECUTE_HANDLER;
		}
		else
		{
			std::wostringstream wss;
			wss << L"A warning error occured, \r\n";
			wss << L"Basic Infomations:\r\n\r\n";
			wss << L"ExceptionAddress:0x" << std::hex << pException->ExceptionRecord->ExceptionAddress << L"\r\n";
			wss << L"ExceptionFlags:" << (pException->ExceptionRecord->ExceptionFlags == EXCEPTION_NONCONTINUABLE ? L"NON CONTINUEABLE" : L"CONTINUEABLE") << L"\r\n";
			wss << L"ExceptionCode:0x" << std::hex << pException->ExceptionRecord->ExceptionCode << L"\r\n";
			wss << L"NumberParameters:" << std::dec << pException->ExceptionRecord->NumberParameters << L"\r\n";

			ShellExecuteW(CXX_NULLPTR, L"open", L"dump", CXX_NULLPTR, CXX_NULLPTR, SW_SHOWNORMAL);
		}
	} while (false);

	return EXCEPTION_CONTINUE_SEARCH;
}

void CXXDebug::installExceptionHandleSEH()
{
	if (IsDebuggerPresent())
	{
		return;
	}

	static bool installed = false;
	if (installed)
	{
		return;
	}

	installed = true;

	SetUnhandledExceptionFilter(MinidumpCallback);
}

void CXXDebug::installExceptionHandleVEH(ULONG first)
{
	if (IsDebuggerPresent())
	{
		return;
	}

	static bool installed = false;
	if (installed)
	{
		return;
	}

	installed = true;

	AddVectoredExceptionHandler(first, MinidumpCallback);
}
#endif
#endif

void cxx::clearConsole()
{
	std::system("cls");
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacterW(hConsole, L' ', dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(hConsole, coordScreen);
}

void cxx::pauseConsole()
{
	std::system("pause");
}

void cxx::setConsoleUtf8()
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	setlocale(LC_ALL, "en_US.UTF-8");
	cxx_debug_private::_oldStdInMode = _setmode(_fileno(stdin), _O_U8TEXT);
	cxx_debug_private::_oldStdOutMode = _setmode(_fileno(stdout), _O_U8TEXT);
	cxx_debug_private::_oldStdErrMode = _setmode(_fileno(stderr), _O_U8TEXT);
}

void cxx::setConsoleUtf16()
{
	cxx_debug_private::_oldStdInMode = _setmode(_fileno(stdin), _O_U16TEXT);
	cxx_debug_private::_oldStdOutMode = _setmode(_fileno(stdout), _O_U16TEXT);
	cxx_debug_private::_oldStdErrMode = _setmode(_fileno(stderr), _O_U16TEXT);
}

__int64 cxx::system(const CXXString& command)
{
	return std::system(reinterpret_cast<char*>(command.toUtf8().data()));
}

CXXDebug& CXXDebug::operator>>(bool& value)
{
	CXX_D(CXXDebug);

	d->noOutput_ = true;

	if (cxx_debug_private::_oldStdInMode != _O_WTEXT)
	{
		std::wcin >> value;
	}
	else
	{
		std::cin >> value;
	}

	return *this;
}

CXXDebug& CXXDebug::operator>>(short& value)
{
	CXX_D(CXXDebug);

	d->noOutput_ = true;

	if (cxx_debug_private::_oldStdInMode != _O_WTEXT)
	{
		std::wcin >> value;
	}
	else
	{
		std::cin >> value;
	}

	return *this;
}

CXXDebug& CXXDebug::operator>>(USHORT& value)
{
	CXX_D(CXXDebug);

	d->noOutput_ = true;

	if (cxx_debug_private::_oldStdInMode != _O_WTEXT)
	{
		std::wcin >> value;
	}
	else
	{
		std::cin >> value;
	}

	return *this;
}

CXXDebug& CXXDebug::operator>>(int& value)
{
	CXX_D(CXXDebug);

	d->noOutput_ = true;

	if (cxx_debug_private::_oldStdInMode != _O_WTEXT)
	{
		std::wcin >> value;
	}
	else
	{
		std::cin >> value;
	}

	return *this;
}

CXXDebug& CXXDebug::operator>>(UINT& value)
{
	CXX_D(CXXDebug);

	d->noOutput_ = true;

	if (cxx_debug_private::_oldStdInMode != _O_WTEXT)
	{
		std::wcin >> value;
	}
	else
	{
		std::cin >> value;
	}

	return *this;
}

CXXDebug& CXXDebug::operator>>(__int64& value)
{
	CXX_D(CXXDebug);

	d->noOutput_ = true;

	if (cxx_debug_private::_oldStdInMode != _O_WTEXT)
	{
		std::wcin >> value;
	}
	else
	{
		std::cin >> value;
	}

	return *this;
}

CXXDebug& CXXDebug::operator>>(UINT64& value)
{
	CXX_D(CXXDebug);

	d->noOutput_ = true;

	if (cxx_debug_private::_oldStdInMode != _O_WTEXT)
	{
		std::wcin >> value;
	}
	else
	{
		std::cin >> value;
	}

	return *this;
}

CXXDebug& CXXDebug::operator>>(long& value)
{
	CXX_D(CXXDebug);

	d->noOutput_ = true;

	if (cxx_debug_private::_oldStdInMode != _O_WTEXT)
	{
		std::wcin >> value;
	}
	else
	{
		std::cin >> value;
	}

	return *this;
}

CXXDebug& CXXDebug::operator>>(ULONG& value)
{
	CXX_D(CXXDebug);

	d->noOutput_ = true;

	if (cxx_debug_private::_oldStdInMode != _O_WTEXT)
	{
		std::wcin >> value;
	}
	else
	{
		std::cin >> value;
	}

	return *this;
}

CXXDebug& CXXDebug::operator>>(float& value)
{
	CXX_D(CXXDebug);

	d->noOutput_ = true;

	if (cxx_debug_private::_oldStdInMode != _O_WTEXT)
	{
		std::wcin >> value;
	}
	else
	{
		std::cin >> value;
	}

	return *this;
}

CXXDebug& CXXDebug::operator>>(double& value)
{
	CXX_D(CXXDebug);

	d->noOutput_ = true;

	if (cxx_debug_private::_oldStdInMode != _O_WTEXT)
	{
		std::wcin >> value;
	}
	else
	{
		std::cin >> value;
	}

	return *this;
}

CXXDebug& CXXDebug::operator>>(CXXString& value)
{
	CXX_D(CXXDebug);

	d->noOutput_ = true;

	if (cxx_debug_private::_oldStdInMode != _O_WTEXT)
	{
		std::wstring w;
		std::wcin >> w; value = w[0];
	}
	else
	{
		std::string a;
		std::cin >> a;
		value = CXXString::fromUtf8(a.c_str());
	}

	return *this;
}

CXXDebug& CXXDebug::operator>>(wchar_t& value)
{
	CXX_D(CXXDebug);

	d->noOutput_ = true;

	if (cxx_debug_private::_oldStdInMode != _O_WTEXT)
	{
		std::wstring w;
		std::wcin >> w; value = w[0];
	}
	else
	{
		std::string a;
		std::cin >> a;
		value = CXXString::fromUtf8(a.c_str()).front();
	}
	return *this;
}

CXXDebug& CXXDebug::operator>>(char& value)
{
	CXX_D(CXXDebug);

	d->noOutput_ = true;

	if (cxx_debug_private::_oldStdInMode != _O_WTEXT)
	{
		std::wstring w;
		std::wcin >> w;
		value = (char)w[0];
	}
	else
	{
		std::string a;
		std::cin >> a;
		value = (char)a.c_str()[0];
	}

	return *this;
}

CXXDebug& CXXDebug::operator>>(BYTE& value)
{
	CXX_D(CXXDebug);

	d->noOutput_ = true;

	if (cxx_debug_private::_oldStdInMode != _O_WTEXT)
	{
		short s = 0;
		std::wcin >> s;
		value = (BYTE)(s);
	}
	else
	{
		short s = 0;
		std::cin >> s;
		value = (BYTE)(s);
	}

	return *this;
}


CXXConsoleProgressBar::CXXConsoleProgressBar(__int64 width)
	: width_(static_cast<size_t>(width))
{
	cxx_debug_private::ansi::enableANSIColors();
}

CXXConsoleProgressBar::CXXConsoleProgressBar(__int64 width, wchar_t fillChar)
	: width_(static_cast<size_t>(width))
	, fillChar_(fillChar)
{
	cxx_debug_private::ansi::enableANSIColors();
}

CXXConsoleProgressBar::CXXConsoleProgressBar(__int64 width, wchar_t fillChar, bool showPercentage)
	: width_(static_cast<size_t>(width))
	, fillChar_(fillChar)
	, showPercentage_(showPercentage)
{
	cxx_debug_private::ansi::enableANSIColors();
}

//@备注 更新进度 双精度浮点数 0.01相当于 1%
//@别名 更新(进度)
void CXXConsoleProgressBar::update(double progressPercentage) const
{
	size_t filledLength = static_cast<size_t>(progressPercentage * width_);
	std::wstring progressBar(filledLength, fillChar_);
	std::wstring progressString = std::to_wstring(static_cast<size_t>(progressPercentage * 100.0)) + L"%";

	// ANSI 转义序列：隐藏光标
	std::wcout << L"\033[?25l";

	// 返回行首并清空行
	std::wcout << L"\r" << std::flush;

	// 显示外框颜色
	std::wcout << L"\033[" << borderColor_ << L"m";
	std::wcout << frontBorderChar_; // 输出前外框

	// ANSI 转义序列：设置颜色
	std::wcout << L"\033[" << color_ << L"m";

	// 输出进度条内容
	std::wcout << progressBar << std::wstring(width_ - filledLength, L' ');

	// 恢复外框颜色并输出后外框
	std::wcout << L"\033[" << borderColor_ << L"m";
	std::wcout << backBorderChar_;

	// 显示百分比
	if (showPercentage_)
	{
		std::wcout << L" " << progressString;
	}

	// ANSI 转义序列：恢复默认颜色和显示光标
	std::wcout << L"\033[0m";  // 恢复默认颜色
	std::wcout << L"\033[?25h";  // 显示光标

	// 若进度为 100%，换行
	if (progressPercentage == 1.0)
	{
		std::wcout << L"\r\n" << std::flush;
	}

	// 确保输出立即刷新
	std::wcout << std::flush;
}