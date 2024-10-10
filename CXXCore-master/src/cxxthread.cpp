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

#include <cxxthread.h>

CXXThreadBase::~CXXThreadBase() {}

bool CXXThreadBase::isRunning() const
{
	return isRunning_;
}

void CXXThreadBase::stop() { resume(); isInterruptionRequested_ = true; }

bool CXXThreadBase::isPaused() const
{
	return isPaused_;
}

void CXXThreadBase::paused()
{
	if (!isPaused())
		return;

	isPaused_ = true;
}

void CXXThreadBase::resume()
{
	if (!isPaused())
		return;

	{
		std::unique_lock<std::mutex> lock(mutex_);
		isPaused_ = false;
	}

	pausedCondition_.notify_all();
}

void CXXThreadBase::setRunning(bool isRunning)
{
	isRunning_ = isRunning;
}

void CXXThreadBase::checkPaused()
{
	std::unique_lock<std::mutex> lock(mutex_);
	if (isPaused_)
		pausedCondition_.wait(lock);
}

bool CXXThreadBase::checkStop() const
{
	return isInterruptionRequested_;
}

void CXXThread::start(cxx::ThreadPriority priority)
{
	if (running_.load(std::memory_order_acquire))
	{
		return;
	}

	priority_ = priority;
	running_.store(true, std::memory_order_release);
	thread_ = std::thread(&CXXThread::run, this);
}

void CXXThread::stop()
{
	running_.store(false, std::memory_order_release);
	condition_.notify_all();
}

void CXXThread::wait()
{
	if (thread_.joinable())
	{
		thread_.join();
	}
}

__int64 CXXThread::id() const
{
	return threadId_;
}

HANDLE CXXThread::handle() const
{
	return handle_;
}

bool CXXThread::isRunning() const
{
	return running_.load(std::memory_order_acquire);
}

void CXXThread::run()
{
	postMessage(cxx::ThreadMessageType::ThreadMessageStarted);
	setThreadPriority(priority_);
	threadId_ = GetCurrentThreadId();
	handle_ = thread_.native_handle();
	exec();
	postMessage(cxx::ThreadMessageType::ThreadMessageStoped);
}

void CXXThread::exec()
{
	while (running_.load(std::memory_order_acquire))
	{
		std::function<void()> task;

		{
			std::unique_lock<std::mutex> lock(mutex_);

			if (tasks_.empty())
				break;

			condition_.wait(lock, [this]
				{
					return !tasks_.empty() || !running_.load(std::memory_order_acquire);
				}
			);

			if (!running_.load(std::memory_order_acquire))
				break;

			task = std::move(tasks_.front());
			tasks_.pop_front();
		}

		task();
	}
}

void CXXThread::postMessage(cxx::ThreadMessageType type)
{
	PostMessageW(parentWindow_, static_cast<UINT>(type), reinterpret_cast<WPARAM>(this), static_cast<LPARAM>(CXXIndexer::index()));
	PostThreadMessageW(static_cast<DWORD>(parentThreadId_), static_cast<UINT>(type), reinterpret_cast<WPARAM>(this), static_cast<LPARAM>(CXXIndexer::index()));
}

void CXXThread::sleepNoBlock(UINT nWaitInMsecs)
{
	auto startTime = std::chrono::high_resolution_clock::now();
	MSG message;

	constexpr int timeCheckInterval = 5;
	int checkCounter = 0;

	for (;;)
	{
		if (PeekMessageW(&message, CXX_NULLPTR, NULL, NULL, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&message);
			DispatchMessageW(&message);
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}

		if (++checkCounter >= timeCheckInterval)
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
			if (elapsed >= nWaitInMsecs)
				break;

			checkCounter = 0;
		}

		std::this_thread::yield();
	}
}


//@备注 让出CPU时间片一段指定的时间，但要注意在时间结束后并不一定会立即恢復线程执行，这取决于系统的调度器
//@参数 毫秒
//@参数 是否阻塞, 默认为阻塞(在UI线程中使用时请设置为true)
//@返回 无
//@别名 延时(毫秒, 是否阻塞)
void CXXThread::sleep(UINT ms, bool isBlocking)
{
	if (!isBlocking)
	{
#if defined(CXX_OS_WIN)
		sleepNoBlock(ms);
		return;
#endif
	}

#if defined(CXX_OS_WIN)
	Sleep(ms);
#else
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#endif
}

//@备注 立即让出CPU时间片，但要注意不一定会立即恢复线程执行，这取决于系统的调度器
//@返回 无
//@别名 让行()
void CXXThread::yield()
{
#if defined(CXX_OS_WIN)
	SwitchToThread();
#else
	std::this_thread::yield();
#endif
}

//@备注 取当前线程ID
//@返回 线程ID
//@别名 当前编号()
__int64 CXXThread::currentId()
{
#if defined(CXX_OS_WIN)
	return GetCurrentThreadId();
#else
	return std::hash<std::thread::id>()(std::this_thread::get_id());
#endif
}

//@隐藏{
template class CXXFuture<bool>;
template class CXXFuture<BYTE>;
template class CXXFuture<char>;
template class CXXFuture<wchar_t>;
template class CXXFuture<short>;
template class CXXFuture<USHORT>;
template class CXXFuture<int>;
template class CXXFuture<UINT>;
template class CXXFuture<__int64>;
template class CXXFuture<UINT64>;
template class CXXFuture<float>;
template class CXXFuture<double>;
template class CXXFuture<CXXString>;
template class CXXFuture<std::string>;
//@隐藏}

//@隐藏{
template class CXXFutureSynchronizer<bool>;
template class CXXFutureSynchronizer<BYTE>;
template class CXXFutureSynchronizer<char>;
template class CXXFutureSynchronizer<wchar_t>;
template class CXXFutureSynchronizer<short>;
template class CXXFutureSynchronizer<USHORT>;
template class CXXFutureSynchronizer<int>;
template class CXXFutureSynchronizer<UINT>;
template class CXXFutureSynchronizer<__int64>;
template class CXXFutureSynchronizer<UINT64>;
template class CXXFutureSynchronizer<float>;
template class CXXFutureSynchronizer<double>;
template class CXXFutureSynchronizer<CXXString>;
template class CXXFutureSynchronizer<std::string>;
//@隐藏}
