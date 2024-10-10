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
#ifndef CXXWINTOAST_H_
//@隐藏{
#define CXXWINTOAST_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxstring.h>
#include <cxxatomic.h>
#include <cxxscopedpointer.h>

#include <functional>
#include <utility>

#include <../3rdparty/wintoastlib/include/wintoastlib.h>

//@别名 炫
namespace cxx
{

}

//@分组{［其他］.炫　推送通知类

//@隐藏{
using CXXToastResultCallback = std::function<void(int state, int actionIndex)>;
//@隐藏}

/*@声明
//@备注 推送通知结果回调
// typedef std::function<void(int state, int actionIndex)> CXXToastResultCallback
//@参数 状态
//@参数 动作索引
//@别名 推送通知结果回调(状态, 动作索引)
typedef std::function<void(int state, int actionIndex)> CXXToastResultCallback;
*/


//@隐藏{
using CXXToastHandlerBase = WinToastLib::IWinToastHandler;

class CXXToastHandler : public CXXToastHandlerBase
{
public:
	CXXToastHandler();

	virtual ~CXXToastHandler();

public:
	//@别名 推送通知已激活()
	void toastActivated() const override final;

	//@别名 推送通知已激活(动作索引)
	void toastActivated(int actionIndex) const override final;

	//@别名 推送通知已关闭(状态)
	void toastDismissed(WinToastDismissalReason state) const override final;

	//@别名 推送通知失败()
	void toastFailed() const override final;
};
//@隐藏}

//@隐藏{
class CXXWinToastPrivate;
//@隐藏}

//@备注 Windows推送通知
//@别名 炫推送通知
class CXXWinToast
{
public:
	//@备注 初始化，必须在使用前调用，全局只需调用一次
	//@参数 无
	//@返回 是否成功
	//@别名 初始化()
	static bool initialize();

	//@参数 无
	CXXWinToast();

	//@参数 排版类型
	explicit CXXWinToast(cxx::WinToastType type);

	//@隐藏{
	virtual ~CXXWinToast();
	//@隐藏}

	//@备注 获取排版类型
	//@参数 无
	//@返回 排版类型
	//@别名 类型()
	cxx::WinToastType type() const;

	//@备注 获取最后一次选中的动作索引
	//@参数 无
	//@返回 动作索引
	//@别名 动作索引()
	__int64 activateIndex() const;

	//@备注 设置图像
	//@参数 图像路径
	//@参数 裁剪类型
	//@返回 无
	//@别名 置图像(路径, 裁剪)
	void setImage(const CXXString& imagePath, cxx::CropHint hint = cxx::Square);

	//@备注 设置焦點圖像
	//@参数 圖像路徑
	//@参数 是否內聯
	//@返回 无
	//@别名 置焦點圖像(路徑, 是否內聯)
	void setHeroImage(const CXXString& imagePath, bool isInline = true);

	//@备注 设置排版类型，必须先于其他设置，否则其他设置会被初始化
	//@参数 排版类型
	//@返回 无
	//@别名 置类型(排版类型)
	void setType(cxx::WinToastType type);

	//@备注 超时时长，默认为最大值，超时后会自动关闭
	//@参数 超时时长(毫秒)
	//@返回 无
	//@别名 置超时(毫秒)
	void setTimeout(__int64 timeout);

	//@备注 设置音频
	//@参数 音频类型
	//@参数 音频选项
	//@返回 无
	//@别名 置音频(音频, 选项)
	void setAudio(cxx::WinToastAudio audio, cxx::AudioOption option = cxx::AudioDefault);

	//@备注 设置文本
	//@参数 文本
	//@参数 文本框索引，必須是0到2之间的整数
	//@返回 无
	//@别名 置文本(文本, 文本框索引)
	void setTextField(const CXXString& text, __int64 fieldIndex);

	//@备注 设置附加文本
	//@参数 附加文本
	//@返回 无
	//@别名 置附加文本(文本)
	void setAttributionText(const CXXString& attributionText);

	//@备注 添加动作
	//@参数 标签(按鈕文本)
	//@返回 无
	//@别名 加动作(标签)
	void addAction(const CXXString& label);

	//@备注 设置推送通知结果回调
	//@参数 回调函数(全局或静态类成员函数)
	//@返回 无
	//@别名 置回调(回调)
	template<typename T>
	void setToastResultCallback(T&& callback)
	{
		toastResultCallbackRef() = std::forward<T>(callback);
	}

	//@备注 设置推送通知结果回调
	//@参数 回调函数(类成员函数)
	//@参数 类实例指针
	//@返回 无
	//@别名 置回调(回调, 实例)
	template<typename T, typename U>
	void setToastResultCallback(T&& callback, U* instance)
	{
		toastResultCallbackRef() = std::bind(std::forward<T>(callback), instance, std::placeholders::_1, std::placeholders::_2);
	}

	//@备注 显示推送通知
	//@参数 无
	//@返回 是否成功
	//@别名 显示()
	bool show() const;

public:
	//@备注 设置应用程序名称
	//@参数 应用程序名称
	//@返回 无
	//@别名 置程序名(名称)
	static void setAppName(const CXXString& appName);

	//@备注 设置应用程序用户模型ID
	//@参数 公司名称
	//@参数 产品名称
	//@参数 子产品
	//@参数 版本信息
	//@返回 无
	//@别名 置信息(公司, 产品, 子产品, 版本)
	static void setAppUserModelId(const CXXString& companyName, const CXXString& productName, const CXXString& subProduct, const CXXString& versionInformation);

private:
	//@隐藏{
	static CXXToastResultCallback& toastResultCallbackRef();

private:
	CXX_DECLARE_PRIVATE(CXXWinToast);
	CXXScopedPointer<CXXWinToastPrivate> d_ptr;

	//@隐藏}
};

//@分组}

#endif // CXXWINTOAST_H_