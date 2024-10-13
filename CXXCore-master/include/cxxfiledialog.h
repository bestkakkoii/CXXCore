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

#ifndef CXXFILEDIALOG_H_
//@隐藏{
#define CXXFILEDIALOG_H_
//@隐藏}




#include <cxxglobal.h>
#include <cxxvector.h>
#include <cxxstring.h>
#include <cxxscopedpointer.h>

#include <shobjidl.h> 

//@别名 炫
namespace cxx
{



} // namespace cxx

//@分组{［文件／文件夹］.炫　文件对话框类

//@备注 创建打开文件对话框
//@参数 目录 起始目录(選填)
//@参数 标题 对话框标题(選填)
//@参数 父窗口 父窗口句柄(選填)
//@返回 已选择的文件
//@别名 炫创建打开文件对话框(目录, 标题, 父窗口)
extern CXXVector<CXXString> cxxCreateOpenFileDialog(const CXXString& directory = L"", const CXXString& caption = L"", HWND parent = CXX_NULLPTR);

//@备注 创建打开文件夹对话框
//@参数 目录 起始目录(選填)
//@参数 标题 对话框标题(選填)
//@参数 父窗口 父窗口句柄(選填)
//@返回 已选择的目录
//@别名 炫创建打开文件夹对话框(目录, 标题, 父窗口)
extern CXXVector<CXXString> cxxCreateOpenDirectoryDialog(const CXXString& directory = L"", const CXXString& caption = L"", HWND parent = CXX_NULLPTR);

//@备注 创建保存文件对话框
//@参数 目录 起始目录(選填)
//@参数 标题 对话框标题(選填)
//@参数 父窗口 父窗口句柄(選填)
//@返回 已选择的文件
//@别名 炫创建保存文件对话框(目录, 标题, 父窗口)
extern CXXVector<CXXString> cxxCreateSaveFileDialog(const CXXString& directory = L"", const CXXString& caption = L"", HWND parent = CXX_NULLPTR);

//@隐藏{
class CXXFileDialogPrivate;
//@隐藏}

//@别名 炫文件对话框
//@备注 基于微软 COM 创建文件、目录、保存的系统对话框
class CXXFileDialog
{
public:
	//@备注 构造
	//@参数 选项属性枚举值组合
	//@参数 父窗口句柄(选填)
	//@参数 标题(选填)
	//@参数 目录(选填)
	//@返回 无
	//@别名 构造(选项, 父窗口, 标题, 目录)
	explicit CXXFileDialog(cxx::FileDialogAcceptMode mode,
		HWND parent = CXX_NULLPTR,
		const CXXString& caption = L"",
		const CXXString& directory = L"");

	//@备注 构造
	//@参数 选项属性枚举值组合
	//@参数 父窗口句柄(选填)
	//@返回 无
	//@别名 构造(选项, 父窗口)
	CXXFileDialog(__int64 mode, HWND parent);

	virtual	~CXXFileDialog();

	//@备注 是否有效
	//@返回 true 有效, false 无效
	//@别名 是否有效()
	bool isValid() const;

	//@备注 执行
	//@返回 true 成功, false 失败
	//@别名 执行()
	bool exec();

	//@备注 获取父窗口句柄
	//@返回 父窗口句柄
	//@别名 父窗口()
	HWND parent() const;

	//@备注 获取目录
	//@返回 目录
	//@别名 目录()
	CXXString directory() const;

	//@备注 获取文件模式
	//@返回 文件模式
	//@别名 模式()
	cxx::FileDialogAcceptMode fileMode() const;

	//@备注 获取历史纪录
	//@返回 历史纪录
	//@别名 纪录()
	CXXVector<CXXString> history() const;

	//@备注 获取过滤器 
	//@返回 过滤器
	//@别名 过滤器()
	CXXVector<CXXString> nameFilters() const;

	//@备注 获取选项
	//@返回 选项
	//@别名 选项()
	cxx::FileDialogOptions options() const;

	//@备注 设置过滤器 格式: "文本文件|*.txt" 或 "所有文件|*.*"
	//@参数 单个过滤器
	//@返回 无
	//@别名 置过滤器(过滤格式字符串)
	void setFilter(const CXXString& filter);

	//@备注 设置过滤器 格式: { "文本文件|*.txt", "所有文件|*.*" }
	//@参数 多个过滤器
	//@返回 无
	//@别名 置过滤器()
	void setFilter(const CXXVector<CXXString>& filters);

	//@备注 获取已选择的文件或目录
	//@返回 已选择的文件或目录
	//@别名 已选择()
	CXXVector<CXXString> selectedFiles() const;

	//@备注 设置起始目录
	//@参数 目录
	//@返回 无
	//@别名 置目录(目录路径)
	void setDirectory(const CXXString& directory);

	//@备注 设置文件模式
	//@参数 文件模式
	//@返回 无
	//@别名 置模式(文件模式)
	void setFileMode(cxx::FileDialogAcceptMode mode);

	//@备注 设置选项
	//@参数 单个选项
	//@参数 是否启用(选填:默认为true)
	//@返回 无
	//@别名 置选项(选项, 是否启用)
	void setOption(cxx::FileDialogOption option, bool on = true);

	//@备注 设置选项
	//@参数 多个选项按位或运算组合
	//@返回 无
	//@别名 置多选项(多选项)
	void setOptions(cxx::FileDialogOptions options);

	//@备注 设置窗口标题
	//@参数 标题
	//@返回 无
	//@别名 置标题(标题)
	void setWindowTitle(const CXXString& title);

	//@备注 设置预设文件名
	//@参数 文件名
	//@返回 无
	//@别名 置文件名(文件名)
	void setDefaultFileName(const CXXString& filename);

	//@备注 设置预设目录
	//@参数 目录
	//@返回 无
	//@别名 置目录(目录路径)
	void setDefaultFolder(const CXXString& folder);

	//@备注 设置标籤文字
	//@参数 标籤类别
	//@参数 文字
	//@返回 无
	//@别名 置标籤文本(标籤, 文字)
	void setLabelText(cxx::FileDialogLabel label, const CXXString& text);

	//@备注 设置要保存的文件的初始位置。
	// 使用该方法可以将用户打开的文件的路径设置为保存对话框的初始位置。
	// 这可以用于让用户选择要复盖的文件或指定默认保存位置。
	//@参数 filePath 要保存的文件的路径。
	//@返回 无
	//@别名 置另存目录(文件路径)
	void setSaveAsItem(const CXXString& filePath);

	//@备注 设置默认扩展名
	//@参数 扩展名
	//@返回 无
	//@别名 置默认扩展名(扩展名)
	void setDefaultExtension(const CXXString& extension);

private:
	//@隐藏{
	CXX_DECLARE_PRIVATE(CXXFileDialog);
	CXXScopedPointer<CXXFileDialogPrivate> d_ptr;
	//@隐藏}
};





//@分组}

#endif // CXXFILEDIALOG_H_