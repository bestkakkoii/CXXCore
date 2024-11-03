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
#ifndef CXXASTARDEVICE_H_
//@隐藏{
#define CXXASTARDEVICE_H_
//@隐藏}

#include <cxxscopedpointer.h>
#include <cxxpoint.h>

#include <algorithm>

//@分组{［工具］.炫　算法类


//@隐藏{
class CXXAStarDevicePrivate;
class CXXNode;
//@隐藏}

//@隐藏{
using CXXAStarCallback = std::function<double(const CXXPoint&)>;
//@隐藏}

/*@声明
//@备注 typedef std::function<double(const CXXPoint&)> CXXAStarCallback;
//@参数 const CXXPoint& 点坐标
//@返回 double 成本
//@别名 炫A星通行回调(坐标点)
typedef std::function<double(const CXXPoint&)> CXXAStarCallback;
*/

//@备注 用于2D网格的A*路径寻找，包含随机路线变异、牆体避让
//@别名 炫A星
class CXXAStarDevice
{
public:
	CXXAStarDevice();

	//@参数 width 宽度
	//@参数 height 高度
	CXXAStarDevice(__int64 width, __int64 height);

	virtual ~CXXAStarDevice();

public:
	//@备注 设置寻路回调，提供给算法判断指定点判断是否可通行
	//@参数 callback 回调函数
	//@别名 置通行回调(回调函数指针)
	void setCanpass(const CXXAStarCallback& callback);

	//@备注 设置寻路回调，提供给算法判断指定点判断是否可通行
	//@参数 callback 类成员函数指针
	//@参数 object 类实例指针
	//@别名 置通行回调(回调函数指针, 对象指针)
	template<typename T, typename C>
	void setCanpass(C callback, T* object)
	{
		setCanpass(std::bind(callback, object, std::placeholders::_1));
	}

	//@备注 是否允许斜角8方为true,4方为false
	//@参数 corner 是否允许拐角
	//@别名 置拐角(true)
	void setCorner(bool corner);

	//@备注 初始化地图缓存参数
	//@参数 width 宽度
	//@参数 height 高度
	//@别名 初始化(宽度, 高度)
	void initialize(__int64 width, __int64 height);

	//@备注 执行寻路操作
	//@参数 start 起点
	//@参数 end 终点
	//@参数 pPath 路径用于接收返回路径的动态数组指针
	//@返回 是否找到路径
	//@别名 查找(起点, 终点, 路径)
	bool find(const CXXPoint& start, const CXXPoint& end, CXXVector<CXXPoint>* pPath);

private:
	//@隐藏{

	CXX_DECLARE_PRIVATE(CXXAStarDevice);
	CXXScopedPointer<CXXAStarDevicePrivate> d_ptr;

	//@隐藏}
};

//@分组}

#endif // CXXASTARDEVICE_H_