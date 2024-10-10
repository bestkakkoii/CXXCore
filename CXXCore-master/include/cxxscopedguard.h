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
#ifndef CXXSCOPEGUARD_H_
//@隐藏{
#define CXXSCOPEGUARD_H_
//@隐藏}



#include <functional>
#include <utility>

//@分组{［内存安全］.炫　作用域守卫

//@备注 CXXScopeGuard 类，用于托管一个在当前作用域结束时自动调用的函数。
//       该类支持托管全局函数、静态函数、成员函数以及实例对象的成员函数。
//@别名 炫作用域守卫
template <typename F>
class CXXScopeGuard
{
public:
	//@隐藏{
	explicit
		//@隐藏}
	//@备注 构造函数，接受一个函数对象并托管。此函数将在 CXXScopeGuard 的析构函数中调用。
	//@参数 f 要托管的函数，可以是全局函数、静态函数、成员函数等。
	//@别名 构造 (函数对象)
		CXXScopeGuard(F&& f)
		: func_(std::forward<F>(f)), active_(true)
	{
	}

	//@备注 禁止拷贝构造函数，以防止复制此类
	CXXScopeGuard(const CXXScopeGuard&) = delete;

	//@备注 移动构造函数，允许将一个 CXXScopeGuard 对象移动到另一个对象中
	CXXScopeGuard(CXXScopeGuard&& other) noexcept
		: func_(std::move(other.func_))
		, active_(other.active_)
	{
		other.dismiss();
	}

	//@备注 析构函数，在对象销毁时自动调用托管的函数
	//@别名 析构 ()
	virtual ~CXXScopeGuard()
	{
		if (active_)
		{
			func_();
		}
	}

	//@备注 允许显式取消托管的函数调用，调用此函数后，析构时不会调用托管的函数
	//@别名 取消()
	void dismiss() noexcept
	{
		active_ = false;
	}

	//禁止赋值操作符，以防止无意中复制或重新分配
	CXXScopeGuard& operator=(const CXXScopeGuard&) = delete;

	//@备注 支持托管成员函数，传入实例对象及其成员函数
	//@参数 memFn 实例对象的成员函数指针
	//@参数 obj 实例对象
	//@返回 返回一个 CXXScopeGuard 对象，用于托管指定的成员函数
	//@别名 创建 (对象, 成员函数)
	template <typename Obj, typename MemFn>
	static auto create(MemFn&& memFn, Obj&& obj)
	{
		return CXXScopeGuard([obj = std::forward<Obj>(obj), memFn = std::forward<MemFn>(memFn)]() mutable {
			std::invoke(memFn, obj);
			});
	}

private:
	//@隐藏{
	F func_;      // 被托管的函数对象
	bool active_; // 标识是否允许在析构时调用托管函数
	//@隐藏}
};

//@分组}

#endif // CXXSCOPEGUARD_H
