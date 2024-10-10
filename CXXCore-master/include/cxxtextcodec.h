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
#ifndef CXXTEXTCODEC_H_
//@隐藏{
#define CXXTEXTCODEC_H_
//@隐藏}



#include <cstdint>
#include <string>
#include <windows.h>
#include <mutex>
#include <atomic>
#include <vector>
#include <unordered_map>
#include <locale>
#include <cxxglobal.h>
#include <cxxstring.h>
#include <cxxscopedpointer.h>

//@分组{［文本］.炫　文本编码类

//@别名 编码页
enum ActiveCodePage
	//@隐藏{
	: UINT
	//@隐藏}
{
	US_ASCII = 20127, //@别名 美国标准代码
	ISO_8859_1 = 28591, //@别名 西欧字符集
	ISO_8859_2 = 28592, //@别名 中欧字符集
	ISO_8859_3 = 28593, //@别名 南欧字符集
	ISO_8859_4 = 28594, //@别名 北欧字符集
	ISO_8859_5 = 28595, //@别名 拉丁_西里尔字符集
	ISO_8859_6 = 28596, //@别名 拉丁_阿拉伯字符集
	ISO_8859_7 = 28597, //@别名 拉丁_希腊字符集
	ISO_8859_8 = 28598, //@别名 拉丁_希伯来字符集
	ISO_8859_9 = 28599, //@别名 拉丁_土耳其字符集
	ISO_8859_10 = 28600, //@别名 北欧字符集_2
	SHIFT_JIS = 932, //@别名 日文假名字符集
	EUC_JP = 20932, //@别名 日文扩展字符集
	EUC_KR = 949, //@别名 韩文扩展字符集
	ISO_2022_KR = 50225, //@别名 韩文邮件和新闻字符集
	ISO_2022_JP = 50220, //@别名 日文邮件和新闻扩展字符集
	ISO_2022_JP_2 = 50222, //@别名 日文邮件和新闻扩展字符集2
	GB2312 = 936, //@别名 简体中文字符集
	UTF_8 = 65001, //@别名 八位万国码
	GB18030 = 54936, //@别名 中文国家标准
	GBK = GB18030,
	BIG5 = 950,  //@别名 繁体中文字符集
	CP851 = 851, //@别名 DOS西欧字符集
};

//@别名 编码地图标志
enum MapFlag
	//@隐藏{
	: UINT
	//@隐藏}
{
	MapFlagByteRev = LCMAP_BYTEREV, //@别名 字节反转
	MapFlagFullWidth = LCMAP_FULLWIDTH, //@别名 全角
	MapFlagHalfWidth = LCMAP_HALFWIDTH, //@别名 半角
	MapFlagHiragana = LCMAP_HIRAGANA, //@别名 平假名
	MapFlagKatakana = LCMAP_KATAKANA, //@别名 片假名
	MapFlagLinguisticCasing = LCMAP_LINGUISTIC_CASING, //@别名 语言大小写
	MapFlagLowerCase = LCMAP_LOWERCASE, //@别名 小写
	MapFlagHash = LCMAP_HASH, //@别名 哈希
	MapFlagSimplifiedChinese = LCMAP_SIMPLIFIED_CHINESE, //@别名 简体中文
	MapFlagSortHandle = LCMAP_SORTHANDLE, //@别名 排序句柄
	MapFlagSortKey = LCMAP_SORTKEY, //@别名 排序键
	MapFlagTitleCase = LCMAP_TITLECASE, //@别名 标题大小写
	MapFlagTraditionalChinese = LCMAP_TRADITIONAL_CHINESE, //@别名 繁体中文
	MapFlagUpperCase = LCMAP_UPPERCASE, //@别名 大写
};

//@隐藏{
class CXXString;
class CXXTextCodecPrivate;
//@隐藏}

//@备注 转换文本编码
//@别名 炫文本编码器
class CXXTextCodec
{
public:
	CXXTextCodec();
	explicit CXXTextCodec(__int64 acp);
	virtual ~CXXTextCodec() = default;

	//@备注 从ACP获取文本编码器的实例指针
	//@参数 ACP
	//@返回 文本编码器的实例指针
	//@别名 编码取文本编码器(编码号码)
	static CXXTextCodec* codecFromACP(__int64 acp);

	//@备注 从编码名称获取文本编码器的实例指针
	//@参数 编码名称
	//@返回 文本编码器的实例指针
	//@别名 编码名称取文本编码器(编码名称)
	static CXXTextCodec* codecFromName(const CXXString& name);

	//@备注 将指定ACP的文本转换为Unicode
	//@参数 来源文本
	//@返回 Unicode文本
	//@别名 到十六位万国码(指定编码的文本)
	CXXString toUnicode(const std::string& input) const;

	//@备注 将指Unicode文本转换为指定ACP的文本
	//@参数 来源文本
	//@返回 指定ACP的文本
	//@别名 到指定编码(十六位万国码文本)
	std::string fromUnicode(const CXXString& input) const;

	//@备注 用于进行字符串映射或生成字符串的排序键。
	// 这个函数可以对输入的字符串进行各种转换，
	// 比如大小写转换、全角/半角转换、汉字简繁转换等，或者是生成用于字符串比较的排序键
	//@参数 映射标志集合
	//@参数 输入文本
	//@返回 映射后的文本
	//@别名 映射文本(映射标志集合, 输入文本)
	CXXString mapString(UINT dwMapFlags, const CXXString& input) const;

private:
	//@隐藏{
	CXX_DECLARE_PRIVATE(CXXTextCodec);
	CXXScopedPointer<CXXTextCodecPrivate> d_ptr;
	//@隐藏}
};

//@隐藏{
static const UINT _CXX_CURRENT_ACP = ::GetACP();
//@隐藏}

//@别名 当前文本编码器()
#define CXX_TEXT_CODEC() CXXTextCodec::codecFromACP(_CXX_CURRENT_ACP)

//@分组}

#endif // CXXTEXTCODEC_H_