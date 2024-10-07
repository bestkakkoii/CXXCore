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

#ifndef CXXTHREAD_H_
//@隐藏{
#define CXXTHREAD_H_
//@隐藏}



#include <cxxglobal.h>
#include <cxxvector.h>
#include <cxxhash.h>
#include <cxxatomic.h>
#include <cxxmutex.h>
#include <cxxstring.h>
#include <cxxvariant.h>

#include <windows.h>
#include <future>
#include <memory>
#include <mutex>
#include <deque>
#include <stdio.h>

//@别名 炫
namespace cxx
{
	//@分组{［线程安全］.炫　线程类

	//@别名 任务结果状态
	enum FutureStatus
		//@隐藏{
		: __int64
		//@隐藏}
	{
		FutureReady,   //@别名 任务完成
		FutureTimeout, //@别名 任务超时
		FutureDeferred //@别名 任务延迟
	};

	//@别名 线程优先级
	enum ThreadPriority
		//@隐藏{
		: __int64
		//@隐藏}
	{
		ThreadIdel = THREAD_PRIORITY_IDLE,					//@别名 空闲
		ThreadLowest = THREAD_PRIORITY_LOWEST,				//@别名 最低
		ThreadBelowNormal = THREAD_PRIORITY_BELOW_NORMAL,	//@别名 低于正常
		ThreadNormal = THREAD_PRIORITY_NORMAL,				//@别名 正常
		ThreadAboveNormal = THREAD_PRIORITY_ABOVE_NORMAL,	//@别名 高于正常
		ThreadHighest = THREAD_PRIORITY_HIGHEST,			//@别名 最高
		ThreadTimeCritical = 15							    //@别名 即时
	};

	//@别名 线程消息类型
	enum ThreadMessageType
		//@隐藏{
		: __int64
		//@隐藏}
	{
		ThreadMessageStoped,             //@别名 线程停止
		ThreadMessageStarted,            //@别名 线程开始
	};

	//@分组}

};

//@分组{［线程安全］.炫　线程类

//@备注 预封装索引自身管理器
//@别名 炫索引器
class CXXIndexer
{
public:
	CXXIndexer(__int64 index)
		: index_(index)
	{}

	//@备注 设置索引
	//@参数 index 索引
	//@返回 无
	//@别名 置索引(索引)
	void setIndex(__int64 index)
	{
		index_ = index;
	}

	//@备注 获取索引
	//@参数 无
	//@返回 索引
	//@别名 索引()
	__int64 index() const { return index_; }

	//@备注 设置自定义属性值
	//@参数 value 自定义属性值
	//@返回 无
	//@别名 置属性(值)
	void setProperty(const CXXVariant& value)
	{
		property_ = value;
	}

	//@备注 获取自定义属性值
	//@参数 无
	//@返回 属性
	//@别名 属性()
	CXXVariant property() const { return property_; }

private:
	CXXVariant property_;
	CXXAtomicInt64 index_ = -1;

};

//@隐藏{
template<typename T>
class CXXFuture;
//@隐藏}

//@备注 预封装的线程基类，包含常用的编号、运行状态、暂停、中断等功能
//@别名 炫线程基类
class CXXThreadBase : public CXXIndexer
{
public:
	//@备注 纯虚函数，需要自己继承用作于线程运行的函数
	//@参数 无
	//@返回 无
	//@别名 运行()
	virtual void run() = 0;

public:
	virtual ~CXXThreadBase();

	//@备注 是否运行中
	//@参数 无
	//@返回 是否运行中
	//@别名 是否运行()
	virtual bool isRunning() const;

	//@备注 请求中断，注意这只是一个线程安全的标志位
	//@参数 无
	//@返回 无
	//@别名 停止()
	virtual void stop();

	//@备注 是否暂停
	//@参数 无
	//@返回 是否暂停
	//@别名 是否暂停()
	virtual bool isPaused() const;

	//@备注 暂停线程
	//@参数 无
	//@返回 无
	//@别名 暂停()
	virtual void paused();

	//@备注 恢复线程
	//@参数 无
	//@返回 无
	//@别名 恢复()
	virtual void resume();


public:
	//@备注 主动置线程运行状态
	//@参数 isRunning 是否运行
	//@返回 无
	//@别名 置运行状态(是否运行)
	virtual void setRunning(bool isRunning);

	//@备注 主动在线程内部检查暂停标志，如果为真则阻塞线程
	//@参数 无
	//@返回 无
	//@别名 检查暂停()
	virtual void checkPaused();

	//@备注 是否中断，线程内部检查用
	//@参数 无
	//@返回 是否中断
	//@别名 检查中断()
	virtual bool checkStop() const;

	/*@声明
	//@备注 设置索引
	//@参数 index 索引
	//@返回 无
	//@别名 置索引(索引)
	void setIndex(__int64 index);

	//@备注 获取索引
	//@参数 无
	//@返回 索引
	//@别名 索引()
	__int64 index() const;
	*/
private:
	//@隐藏{
	CXXAtomicBool isInterruptionRequested_ = false;
	CXXAtomicBool isRunning_ = false;
	CXXAtomicBool isPaused_ = false;

	mutable std::mutex mutex_;
	mutable std::condition_variable pausedCondition_;
	//@隐藏}

};

//@备注 线程类，包含线程的创建、销毁、暂停、中断、优先级等功能
//@别名 炫线程
class CXXThread : public CXXIndexer
{
public:
	//@备注 构造函数
	//@参数 index 自定義線程編號
	CXXThread(__int64 index = -1)
		: CXXIndexer(index)
		, running_(false)
	{
	}

	//@隐藏{
	virtual ~CXXThread()
	{
		if (running_.load(std::memory_order_acquire))
		{
			stop();
		}
	}
	//@隐藏}

	//@备注 开始线程
	//@参数 priority 线程优先级 默认正常
	//@返回 无
	//@别名 开始(优先级)
	void start(cxx::ThreadPriority priority = cxx::ThreadNormal);

	//@隐藏{
	template<typename Function, typename... Args>
	void moveToThread(Function&& func, Args&&... args)
	{
		if (running_.load(std::memory_order_acquire))
		{
			return;
		}

		auto task = std::bind(std::forward<Function>(func), std::forward<Args>(args)...);
		std::unique_lock<std::mutex> lock(mutex_);
		tasks_.emplace_back(std::move(task));
		condition_.notify_one();
	}

	template<typename Class, typename Function, typename... Args>
	void moveToThread(Function Class::* func, Class* classptr, Args&&... args)
	{
		if (running_.load(std::memory_order_acquire))
		{
			return;
		}

		auto task = std::bind(func, classptr, std::forward<Args>(args)...);
		std::unique_lock<std::mutex> lock(mutex_);
		tasks_.emplace_back(std::move(task));
		condition_.notify_one();
	}
	//@隐藏}

	/*@声明
	//@备注 将函数移动到线程
	//@参数 func 函数指针
	//@参数 args 函数参数 1-9个
	//@返回 无
	//@别名 移动到线程(func, args)
	template<typename Function, typename Args>
	void moveToThread(Function func, Args args);

	//@备注 将成员函数移动到线程
	//@参数 func 成员函数指针
	//@参数 classptr 类实例指针
	//@参数 args 成员函数参数 1-9个
	//@返回 无
	//@别名 移动到线程(class_func, class_ptr, args)
	template<typename Class, typename Function, typename Args>
	void moveToThread(Function func, Args args);
	*/

	//@备注 停止线程
	//@参数 无
	//@返回 无
	//@别名 停止()
	void stop();

	//@备注 等待线程完成
	//@参数 无
	//@返回 无
	//@别名 等待()
	void wait();

	//@备注 取线程编号
	//@参数 无
	//@返回 线程编号
	//@别名 编号()
	__int64 id() const;

	//@备注 取线程句柄
	//@参数 无
	//@返回 线程句柄
	//@别名 句柄()
	HANDLE handle() const;

	//@备注 是否运行中
	//@参数 无
	//@返回 是否运行中
	//@别名 是否运行()
	bool isRunning() const;

	//@备注 用于设置线程开始、停止时发送的消息编号
	// WPARAM: 线程类实例指针 LPARAM: 自定义线程编号
	//@参数 message 消息编号
	//@返回 无
	//@别名 置消息(消息)
	void setMessage(cxx::ThreadMessageType type, unsigned __int64 message)
	{
		messageHandlers_[type] = message;
	}

	//@备注 用于设置停止后自动发送停止讯号时接收讯号的窗口句柄
	//@参数 parentWindow 窗口句柄
	//@返回 无
	//@别名 置父窗口(窗口)
	void setParentWindow(HWND parentWindow)
	{
		parentWindow_ = parentWindow;
	}

	//@备注 用于设置停止后自动发送停止讯号时接收讯号的线程编号
	//@参数 parentThreadId 线程编号
	//@返回 无
	//@别名 置父线程(线程)
	void setParentThreadId(unsigned __int64 parentThreadId)
	{
		parentThreadId_ = parentThreadId;
	}


	/*@声明
	//@备注 设置索引
	//@参数 index 索引
	//@返回 无
	//@别名 置索引(索引)
	void setIndex(__int64 index);

	//@备注 获取索引
	//@参数 无
	//@返回 索引
	//@别名 索引()
	__int64 index() const;

	//@备注 设置自定义属性值
	//@参数 value 自定义属性值
	//@返回 无
	//@别名 置属性(值)
	void setProperty(const CXXVariant& value);

	//@备注 获取自定义属性值
	//@参数 无
	//@返回 属性
	//@别名 属性()
	__int64 property() const;

	//@备注 设置自定义属性值
	//@参数 value 自定义属性值
	//@返回 无
	//@别名 置属性(值)
	void setProperty(const CXXVariant& value);

	//@备注 获取自定义属性值
	//@参数 无
	//@返回 属性
	//@别名 属性()
	__int64 property() const;
	*/

public:
	//@备注 用于设置线程优先级
	//@参数 priority 优先级
	//@返回 无
	//@别名 置优先级(优先级)
	static void setThreadPriority(cxx::ThreadPriority priority)
	{
		std::ignore = SetThreadPriority(GetCurrentThread(), static_cast<int>(priority));
	}

	//@备注 让出CPU时间片一段指定的时间，但要注意在时间结束后并不一定会立即恢復线程执行，这取决于系统的调度器
	//@参数 毫秒
	//@参数 是否阻塞, 默认为阻塞(在UI线程中使用时请设置为true)
	//@返回 无
	//@别名 延时(毫秒, 是否阻塞)
	static void sleep(UINT ms, bool isBlocking = true);

	//@备注 立即让出CPU时间片，但要注意不一定会立即恢复线程执行，这取决于系统的调度器
	//@返回 无
	//@别名 让行()
	static void yield();

	//@备注 取当前线程ID
	//@返回 线程ID
	//@别名 当前编号()
	static __int64 currentId();

protected:
	//@隐藏{
	virtual void run();
	//@隐藏}

private:
	//@隐藏{
	static void sleepNoBlock(UINT nWaitInMsecs);

	void exec();

	void postMessage(cxx::ThreadMessageType type);
	//@隐藏}

private:
	//@隐藏{
	std::thread thread_;
	std::atomic_bool running_;
	std::mutex mutex_;
	std::condition_variable condition_;
	std::deque<std::function<void()>> tasks_;

	std::unordered_map <__int64, unsigned __int64> messageHandlers_ = {
		{ cxx::ThreadMessageType::ThreadMessageStoped, WM_USER + 1 },
		{ cxx::ThreadMessageType::ThreadMessageStarted, WM_USER + 2 },
	};

	HWND parentWindow_ = CXX_NULLPTR;
	unsigned __int64 parentThreadId_ = NULL;

	cxx::ThreadPriority priority_ = cxx::ThreadPriority::ThreadNormal;

	unsigned __int64 threadId_ = NULL;
	HANDLE handle_ = CXX_NULLPTR;
	//@隐藏}
};

//@隐藏{
class CXXAsync
{
public:
	CXXAsync() = default;
	virtual ~CXXAsync() = default;

	// std::result_of is deprecated in C++17 and removed in C++20
	template<typename Function, typename... Args>
	static auto run(Function&& func, Args&&... args) -> std::future<std::invoke_result_t<Function, Args...>>
	{
		using ResultType = std::invoke_result_t<Function, Args...>;

		return std::future<ResultType>(std::async(std::launch::async, std::forward<Function>(func), std::forward<Args>(args)...));
	}

	template<typename ClassType, typename Function, typename... Args>
	static auto run(Function ClassType::* func, ClassType* classptr, Args&&... args) -> std::future<std::invoke_result_t<Function, ClassType*, Args...>>
	{
		using ResultType = std::invoke_result_t<Function, ClassType*, Args...>;

		return std::async(
			std::launch::async,
			[classptr, func](Args&&... args)->ResultType
			{
				return (classptr->*func)(std::forward<Args>(args)...);
			},
			std::forward<Args>(args)...);
	}
};
//@隐藏}

/*@声明

//@备注 用于执行单一异步任务并返回结果
//@别名 炫异步
class CXXAsync
{
public:
	CXXAsync() = default;
	~CXXAsync() = default;

	//@备注 用于执行单一异步任务
	// CXXAsync::run(funcPtr, 1, 2, 3, 4, 5, 6, 7, 8, 9);
	//@参数 函数指针
	//@参数 函数参数可以是多个参数最多9个
	//@返回 任务结果
	//@别名 执行(func, args)
	template<typename Function, typename Args>
	static T run(Function func, Args args);

	//@备注 用于执行成员函数的异步任务
	// CXXAsync::run(class::funcPtr, class_ptr, 1, 2, 3, 4, 5, 6, 7, 8, 9);
	//@参数 func 成员函数指针
	//@参数 classptr 类实例指针
	//@参数 args 成员函数的参数
	//@返回 任务结果
	//@别名 执行(class_func, class_ptr, args)
	template<typename Function, typename ClassType, typename Args>
	static T run(Function * func, ClassType* classptr, Args args);
};

*/

//@备注 用于管理 炫异步(CXXAsync) 任务结果
//@别名 炫任务结果
template<typename T>
class CXXFuture
{
public:
	CXXFuture(std::future<T>&& fut)
		: futurePtr(std::make_unique<std::future<T>>(std::move(fut)))
	{}

	//@隐藏{

	CXXFuture(CXXFuture<T>&& other) noexcept
		: futurePtr(std::move(other.futurePtr))
		, result_(other.result_)
		, isResultTaken_(other.isResultTaken_.load(std::memory_order_acquire))
	{}


	CXXFuture<T>& operator=(CXXFuture<T>&& other) noexcept
	{
		if (this != &other)
		{
			futurePtr = std::move(other.futurePtr);
			result_ = other.result_;
			isResultTaken_.store(other.isResultTaken_.load(std::memory_order_acquire), std::memory_order_release);
		}
		return *this;
	}

	virtual ~CXXFuture() = default;
	//@隐藏}

	//@备注 获取任务结果
	//@参数 无
	//@返回 任务结果
	//@别名 结果()
	T result()
	{
		//防止多次调用
		if (isResultTaken_.load(std::memory_order_acquire) || CXX_NULLPTR == futurePtr)
		{
			return result_;
		}

		result_ = futurePtr->std::future<T>::get();
		isResultTaken_.store(true, std::memory_order_release);
		return result_;
	}

	//@隐藏{
	operator std::future<T>& () const { return *futurePtr; }
	//@隐藏}

	//@备注 等待任务完成
	//@参数 无
	//@返回 无
	//@别名 等待()
	void wait()
	{
		if (CXX_NULLPTR == futurePtr)
		{
			return;
		}

		futurePtr->std::future<T>::wait();
	}

	//@备注 等待任务完成
	//@参数 n 等待时间(毫秒)
	//@返回 任务状态
	//@别名 等待至(毫秒)
	cxx::FutureStatus wait(size_t n)
	{
		if (CXX_NULLPTR == futurePtr)
		{
			return cxx::FutureStatus::FutureDeferred;
		}

		return static_cast<cxx::FutureStatus>(futurePtr->std::future<T>::wait_for(std::chrono::microseconds(n)));
	}

	//@备注 是否有效
	//@参数 无
	//@返回 是否有效
	//@别名 是否有效()
	bool isValid() const
	{
		if (CXX_NULLPTR == futurePtr)
		{
			return false;
		}

		return futurePtr->std::future<T>::valid();
	}

	//@备注 是否运行中
	//@参数 无
	//@返回 是否运行中
	//@别名 是否运行()
	bool isRunning() const
	{
		if (CXX_NULLPTR == futurePtr)
		{
			return false;
		}

		return futurePtr->std::future<T>::wait_for(std::chrono::seconds(0)) == std::future_status::timeout;
	}

	//@备注 是否完成
	//@参数 无
	//@返回 是否完成
	//@别名 是否完成()
	bool isFinished() const
	{
		if (CXX_NULLPTR == futurePtr)
		{
			return false;
		}

		return futurePtr->std::future<T>::wait_for(std::chrono::seconds(0)) == std::future_status::ready;
	}

private:
	//@隐藏{
	std::unique_ptr<std::future<T>> futurePtr;
	T result_;
	std::atomic_bool isResultTaken_ = false;
	//@隐藏}
};


//@备注 用于管理多个 炫异步(CXXAsync) 任务的多个 炫任务结果(CXXFuture<T>)
//@别名 炫任务结果管理器
template<typename T>
class CXXFutureSynchronizer
{
public:
	//@别名 是否包含(索引)
	bool contains(__int64 index)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return futures_.find(index) != futures_.end();
	}

	//@备注 插入一个CXFuture<T>
	//@参数 future future<T>
	//@返回 无
	//@别名 插入(编号, 任务结果)
	void insert(__int64 index, CXXFuture<T>&& now_future)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		futures_.insert(std::make_pair(index, std::make_unique<CXXFuture<T>>(std::move(now_future))));
	}

	//@备注 等待所有任务完成
	//@参数 无
	//@返回 无
	//@别名 等待完成()
	void waitForFinished()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		for (const auto& fut : futures_)
		{
			if (CXX_NULLPTR == fut.second)
			{
				continue;
			}

			fut.second->wait();
		}
	}

	//@备注 获取所有任务结果
	//@参数 无
	//@返回 任务结果
	//@别名 结果()
	CXXVector<T> results()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		CXXVector<T> results;
		size_t size = futures_.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (CXX_NULLPTR == futures_[i])
			{
				continue;
			}

			results.append(futures_[i]->result());
		}
		return results;
	}

	//@备注 清除所有任务
	//@参数 无
	//@返回 无
	//@别名 清空()
	void clear()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		futures_.clear();
	}

	//@备注 获取任务数量
	//@参数 无
	//@返回 任务数量
	//@别名 大小()
	size_t size() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return futures_.size();
	}

	//@备注 获取指定任务
	//@参数 index 任务索引
	//@参数 ref_future_ptr 引用任务指针
	//@返回 成功返回true 失败返回false
	//@别名 取出任务指针(索引, 引用任务指针)
	bool takeFutureAt(__int64 index, CXXFuture<T>** pfuture_ptr)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		auto it = futures_.find(index);
		if (it != futures_.end())
		{
			// Take the future
			std::unique_ptr<CXXFuture<T>> fut = std::move(it->second);
			futures_.erase(it);

			if (pfuture_ptr != CXX_NULLPTR)
			{
				*pfuture_ptr = fut.release();
				return true;
			}
		}
		return false;
	}

private:
	//@隐藏{
	mutable std::mutex mutex_;
	std::unordered_map<__int64, std::unique_ptr<CXXFuture<T>>> futures_;
	//@隐藏}
};

//@备注 快速打开线程
#define CXX_ASYNC_RUN(func, ...) (CXXAsync::run(func, __VA_ARGS__)) //@别名 炫_执行线程(func, args)

//@分组}

#endif // CXXTHREAD_H_