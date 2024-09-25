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
#ifndef CXXFILEDIALOG_P_H_
#define CXXFILEDIALOG_P_H_

#include <cxxfiledialog.h>
#include <windows.h>


class CXXFileDialogPrivate
{
public:
	explicit CXXFileDialogPrivate(CXXFileDialog* q);

	CXXFileDialogPrivate(CXXFileDialog* q, cxx::FileDialogAcceptMode mode,
		HWND parent,
		const CXXString& caption,
		const CXXString& directory);

	void setDialog(IFileDialog* pDialog);

	HRESULT create();

	HRESULT setNameFilters();

	HRESULT handleFilesOrDirectoriesSelection();

	HRESULT handleSaveFileSelection();

	void clear();

private:
	IFileDialog* pDialog_ = CXX_NULLPTR;
	IFileOpenDialog* pOpenDialog_ = CXX_NULLPTR;
	IFileSaveDialog* pSaveDialog_ = CXX_NULLPTR;
	HWND parent_ = CXX_NULLPTR;

	cxx::FileDialogAcceptMode fileMode_ = cxx::FileDialogAcceptMode::OpenFile;
	cxx::FileDialogOptions options_ = cxx::FileDialogOption::DefaultOpenFile;

	std::wstring directory_;
	std::wstring caption_;
	static CXXVector<std::wstring> history_;

	CXXVector<std::wstring> nameFilters_ = { L"Any Files|*.*" };
	CXXVector<std::wstring> selectedFiles_;


private:
	CXX_DECLARE_PUBLIC(CXXFileDialog);
	CXXFileDialog* q_ptr = CXX_NULLPTR;
};

#endif // CXXFILEDIALOG_P_H_