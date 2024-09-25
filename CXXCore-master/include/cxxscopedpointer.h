// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef CXXSCOPEDPOINTER_H_
//@隐藏{
#define CXXSCOPEDPOINTER_H_
//@隐藏}

#include <cxxglobal.h>

#include <stdlib.h>



//@分组{［内存安全］.炫　作用域指针类

//@隐藏{
template <typename T>
struct CXXScopedPointerDeleter
{
	//@备注 清理指针内存
	//@参数 pointer 需要清理的指针
	static inline void cleanup(T* pointer) noexcept
	{
		// Enforce a complete type.
		typedef char IsIncompleteType[sizeof(T) ? 1 : -1];
		(void)sizeof(IsIncompleteType);

		delete pointer;
	}

	//@备注 调用清理函数，清理指针
	//@参数 pointer 需要清理的指针
	void operator()(T* pointer) const noexcept
	{
		cleanup(pointer);
	}
};

template <typename T>
struct CXXScopedPointerArrayDeleter
{
	//@备注 清理数组指针内存
	//@参数 pointer 需要清理的数组指针
	static inline void cleanup(T* pointer) noexcept
	{
		typedef char IsIncompleteType[sizeof(T) ? 1 : -1];
		(void)sizeof(IsIncompleteType);

		delete[] pointer;
	}

	//@备注 调用清理函数，清理数组指针
	//@参数 pointer 需要清理的数组指针
	void operator()(T* pointer) const noexcept
	{
		cleanup(pointer);
	}
};

struct CXXScopedPointerPodDeleter
{
	//@备注 清理通过 malloc 分配的内存
	//@参数 pointer 需要清理的指针
	static inline void cleanup(void* pointer) noexcept
	{
		free(pointer);
	}

	//@备注 调用清理函数，清理内存
	//@参数 pointer 需要清理的指针
	void operator()(void* pointer) const noexcept
	{
		cleanup(pointer);
	}
};

//@隐藏}

//@备注 智能指针类，它负责在作用域结束时自动释放一般指针。
//@别名 炫作用域指针
template <typename T, typename Cleanup
	//@隐藏{
	= CXXScopedPointerDeleter<T>
	//@隐藏}
>
class CXXScopedPointer
{
public:
	//@备注 构造函数，用于初始化指针
	//@参数 p 初始化的指针
	CXX_NODISCARD explicit CXXScopedPointer(T* p = nullptr)
		//@隐藏{
		noexcept
		: d(p)
		//@隐藏}
	{}

	//@备注 析构函数，自动清理指针
	~CXXScopedPointer()
	{
		T* oldD = this->d;
		Cleanup::cleanup(oldD);
	}

	//@备注 获取指针对象的引用
	//@返回 引用类型的指针对象
	T& operator*() const
	{
		CXX_ASSERT(d);
		return *d;
	}

	//@备注 通过箭头符号访问指针的成员
	//@返回 指针类型的对象
	T* operator->() const noexcept
	{
		return d;
	}

	//@备注 判断指针是否为空
	//@返回 布尔值，指针为空时返回 true，否则返回 false
	bool operator!() const noexcept
	{
		return !d;
	}

	//@备注 判断指针是否有效（非空）
	//@返回 布尔值，指针有效时返回 true，否则返回 false
	explicit operator bool() const
	{
		return !isNull();
	}

	//@备注 获取指针对象
	//@返回 指针类型的对象
	//@别名 地址()
	T* data() const noexcept
	{
		return d;
	}

	//@备注 获取指针对象
	//@返回 指针类型的对象
	//@别名 取()
	T* get() const noexcept
	{
		return d;
	}

	//@备注 判断指针是否为空
	//@返回 布尔值，指针为空时返回 true，否则返回 false
	//@别名 是否空()
	bool isNull() const noexcept
	{
		return !d;
	}

	//@备注 重置指针对象并释放旧对象内存
	//@参数 other 新的指针对象
	//@别名 重置(新指针)
	void reset(T* other = nullptr)
		//@隐藏{
		noexcept(noexcept(Cleanup::cleanup(std::declval<T*>())))
		//@隐藏}
	{
		if (d == other)
			return;
		T* oldD = std::exchange(d, other);
		Cleanup::cleanup(oldD);
	}

	//@隐藏{
	typedef T* pointer;

	friend bool operator==(const CXXScopedPointer<T, Cleanup>& lhs, const CXXScopedPointer<T, Cleanup>& rhs) noexcept
	{
		return lhs.data() == rhs.data();
	}

	friend bool operator!=(const CXXScopedPointer<T, Cleanup>& lhs, const CXXScopedPointer<T, Cleanup>& rhs) noexcept
	{
		return lhs.data() != rhs.data();
	}

	friend bool operator==(const CXXScopedPointer<T, Cleanup>& lhs, std::nullptr_t) noexcept
	{
		return lhs.isNull();
	}

	friend bool operator==(std::nullptr_t, const CXXScopedPointer<T, Cleanup>& rhs) noexcept
	{
		return rhs.isNull();
	}

	friend bool operator!=(const CXXScopedPointer<T, Cleanup>& lhs, std::nullptr_t) noexcept
	{
		return !lhs.isNull();
	}

	friend bool operator!=(std::nullptr_t, const CXXScopedPointer<T, Cleanup>& rhs) noexcept
	{
		return !rhs.isNull();
	}
	//@隐藏}

protected:
	//@隐藏{
	T* d;
	//@隐藏}

private:
	//@隐藏{
	CXX_DISABLE_COPY_MOVE(CXXScopedPointer);
	//@隐藏}
};

//@备注 智能指针类，它负责在作用域结束时自动释放"数组"指针。
//@别名 炫作用域数组指针
template <typename T, typename Cleanup
	//@隐藏{
	= CXXScopedPointerArrayDeleter<T>
	//@隐藏}
>
class CXXScopedArrayPointer
	//@隐藏{
	: public CXXScopedPointer<T, Cleanup>
	//@隐藏}
{
private:
	//@隐藏{
	template <typename Ptr>
	using if_same_type = typename std::enable_if<std::is_same<typename std::remove_cv<T>::type, Ptr>::value, bool>::type;
	//@隐藏}

public:
	//@备注 默认构造函数，初始化指针为空
	CXX_NODISCARD inline CXXScopedArrayPointer()
		//@隐藏{
		: CXXScopedPointer<T, Cleanup>(nullptr)
		//@隐藏}
	{}

	//@备注 析构函数，自动释放数组指针
	inline ~CXXScopedArrayPointer() = default;

	//@备注 构造函数，初始化数组指针
	//@参数 p 初始化的数组指针
	template <typename D
		//@隐藏{
		, if_same_type<D> = true
		//@隐藏}
	>
	CXX_NODISCARD explicit CXXScopedArrayPointer(D* p)
		//@隐藏{
		: CXXScopedPointer<T, Cleanup>(p)
		//@隐藏}
	{}

	//@备注 重载下标运算符，获取数组元素
	//@参数 i 数组索引
	//@返回 数组元素的引用
	T& operator[](__int64 i)
	{
		return this->d[i];
	}

	//@备注 重载下标运算符，获取数组元素
	//@参数 i 数组索引
	//@返回 数组元素的常量引用
	const T& operator[](__int64 i) const
	{
		return this->d[i];
	}

private:
	explicit inline CXXScopedArrayPointer(void*)
	{}

private:
	//@隐藏{
	CXX_DISABLE_COPY_MOVE(CXXScopedArrayPointer);
	//@隐藏}
};

//@分组}

#endif // CXXSCOPEDPOINTER_H_
