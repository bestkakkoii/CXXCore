## 更新日志

#### [2024-10-10]()
  * 修改 将 `cxxmemorypool.h` 和 `cxxmemorypool.cpp` 的版权声明从 Box2D 版权声明更改为 GNU General Public License (GPL)。
    * 将 `cxxmemorypool.h` 中的 `#include <cxxglobal.h>` 和 `#include <cstdint>` 替换为 `#include <memory_resource>`。
    * 移除了原本的内存池实现，并改用 C++17 的 `std::pmr::synchronized_pool_resource` 来实现内存池，并将内存池的默认大小设置为 1MB (`MemoryPoolDefaultPoolSize`)。
    * 更新了内存池的优点和缺点说明，并添加了使用方法的註释。
    * 将 `CXXMemoryPool` 类模板化，并修改了内存分配和释放的方法签名，使其支持分配和释放指定数量的对象。
    * 更新了 `CXXScopedMemoryPool` 类，使其与新的 `CXXMemoryPool` 类兼容。
    * 在 `CXXCoreTest.vcxproj.filters` 中，将 `cxxmemorypool` 的过滤器从 `old` 文件夹移动到 `template` 文件夹。

#### [2024-10-07]()
  * 修改 在 `cxxwintoast.h` 中新增 `CXXToastResultCallback` 的 `typedef` 和相关註释。
    * 在 `cxxwritelocker.h` 中将析构函数改为虚拟函数。
    * 在 `cxxcom.cpp` 中将函数名称改为 `get`。
    * 在 `cxxdir.cpp` 中新增目录重命名函数。
    * 在 `cxxfile.cpp` 中新增析构函数清理代码，并更新路径。
    * 在 `cxxfile.cpp` 中对 `map` 和 `unmap` 函数增加互斥锁。
    * 在 `cxxfiledialog.cpp` 中重命名函数并移除重複代码。
    * 在 `cxxmutex.cpp` 中修改参数类型。
    * 在 `cxxscopedhandle.cpp` 中新增获取句柄函数。
    * 在 `cxxstring.cpp` 和 `cxxvariant.cpp` 中更新註释别名。

#### [2024-10-06]()
  * 修改 应对 IDE 总是出现命名空间解析 BUG，将所有命名空间的枚举移至核心模块

#### [2024-09-25]()
  * 新增 增加字节集和文本数组相关的更多操作函数
  * 新增 炫分词器，用于文本分词，可以随时操作分词规则

#### [2024-09-23]()
  * 修正 炫调试输出到调试窗口会包含转义序列的问题，并增加PID和TID控制台内显示颜色
  * 修正 炫配置因拆分模块后缺少头文件无法编译的问题
  * 修改 将模块更加细分，将可能不常用的类单独放入一个模块，以减少编译时间
    * 注意，目前因IDE的原因可能无法正确的跨模块使用相同的命名空间 炫(cxx)

#### [2024-09-21]()
  * 修改 将模块拆分成若干部份:核心和其它多个模块，所有模块都必须基于核心

#### [2024-09-19]()
  * 修改 将绝大部份模版的显式声明类型移除
  * 修改 将大多数不必要的 inline 定义移入cpp减少编译后体积
  * 修改 因SDK兼容问题取消预编译静态库，可以自行编译。请查看 `@当前模块路径/CXXCore-master/vs/CXXCore.sln`
  * 修改 重新整理目录结构

#### [2024-09-18]()
  * 修正 修正许多警告、代码错误
  * 修改 改成静态库的方式，请至群文件下载，放置于 `@当前模块路径/CXXCore-master/lib` 下
    * 如果不想使用静态库请把所有 `*.h` 中 `//@source` 改成 `//@src`
    * 并把所有 `@当前模块路径/CXXCore-master/module_cxx.h` 中的 `@lib` 注释掉
    * 需要 `MT` 或 `MTd` 静态库 请自行使用 `VS2022` 打开 `模@当前模块路径/CXXCore-master/vs/CXXCore.sln` 编译

#### [2024-09-16]()
  * 新增 炫调试增加枚举输出实际名称、增加 typeName 可以获取任意变量的文本类型名称
  * 新增 炫线程基类 炫线程 增加 索引、置索引的声明，并添加 属性、置属性 用于添加自订数据给线程实例
  * 新增 炫内存(CXXMemory), 炫远程虚拟内存(CXXVirtualMemory) 提供本地、远程内存相关的操作
  * 新增 cxxmath.h 提供大量基于 STL 的数学函数 (原文无别名)、和常量如PI 位于炫命名空间
  * 新增 增加若干全局函数:
    * 取命令行, 是否64位, 退出, 终止进程
    * 发送消息, 提交消息, 调用窗口过程, 消息框, 执行
  * 未来可能新增 CXXFile 增加根据指定条件或筛选器递归查找文件、枚举文件、查找文件内容
  * 未来可能新增 CXXDir 增加指定条件或筛选器递归查找目录、枚举目录

#### [2024-09-15]()
  * 新增 炫加密(CXXCrypto) 支持常用文本和字节集对称/非对称加解密，输出文本不可读则自动转换十六进制
  * 新增 炫文本增加 toHexString fromHexString fromLocal8Bit fromUtf8 对字节集、字节指针+大小输入的支持
  * 新增 炫动态数组 建构函数对于迭代器输入的支持
  * 新增 炫文本增加 toBase64 fromBase64
  * 修改 将所有 NULL 可以替换为 nullptr 的作修改
  * 修改 优化哈希值生成速度

#### [2024-09-14]()
  * 新增 JSON 示例
  * 新增 炫时间(CXXTime), 炫日期(CXXData), 炫日期时间(CXXDateTime)
  * 新增 炫作用域守卫(CXXScopedGuard), 炫共享内存(CXXSharedMemory), 炫堆管理器(CXXCleanupHandler)
  * 新增 炫JSON(CXXJson), 炫JSON对象(CXXJsonObject), 炫JSON解析错误(CXXJsonPraseError)
  * 新增 多个容器类 
    * 炫配对(CXXPair), 炫映射(CXXMap), 炫集合(CXXSet), 
    * 炫缓存(CXXCache), 炫连续缓存(CXXContiguousCache)
  * 修改 应对新IDE对于分组作修正
  * 修改 将所有函数中文别名统一规范

#### [2024-09-13]()
  * 新增 一个关于调试输出、控制台进度条的示例
  * 新增 提供一个递归清理所有项目缓存资源的ps脚本
  * 新增 控制台ANSI转义序列 自订颜色输出颜色 仅 WIN10 上系统支持 (支持更多颜色、并且支持同一行多种颜色)
  * 修正 所有示例版本过旧无法编译的问题
  * 修正 函数签名(CXX_FUNCSIG)、函数名(CXX_FUNCTION)、文件名(CXX_FILE)输出乱码的问题
  * 修正 CXXDebug 在 cxx::None 模式下输出空白的问题
  * 修正 CXXElaspedTimer 首次计时不正确的问题
  * 修正 CXXString 移动语义建构时为正确初始化私有类的问题
  * 修正 CXXComObject 一个插值参数错误问题
  * 修正 CXXComObject 多级调用没有正确的返回分派指针，并增加 CXXComVariant::invoke 接口
  * 修改 CXXComObject 别名, CXXComVairant 别名

#### [2024-09-12]()
  * 修改 完善炫进程(CXXProcess) 增加启动进程和分离启动进程、转接输入输出流
  * 修改 添加炫彩 common__.h 预编译头
  * 修改 添加模版显式实例化
  * 修改 添加 CXXHash 对于 CXXPoint, CXXPoint 作为键值的支持
  * 修正 炫推送通知(CXXWinToast) 编译错误问题

#### [2024-09-11]()
  * 新增 炫文件夹(CXXDir)
  * 新增 炫程序(CXXApplication) [尚未完善]
  * 新增 炫进程(CXXProcess)     [尚未完善]
  * 新增 炫推送通知(CXXWinToast)
  * 修改 将剩馀未统一命名规范的类更名
  * 修改 将多个宏更名
  * 修改 修正部分错误
  * 修正 部分模板函数无法编译问题
  * 修正 x32 编译出现大量警告的问题、和其他 x64/x32 警告，现在警告等级4不会再出现警告了

#### [2024-09-02]()
  * 修改 完全移除对 module_base.h (炫彩官方核心) 的依赖，不再支持输入或输出任何炫彩核心内的类型
  * 修改 接续将多个模块更名
  * 修改 应对XCIDE接下来关于命名空间支持的调整

#### [2024-08-25]()
  * 修改 cx::com_obj -> CXXComObject
  * 修改 cx::com_variant -> CXXComVariant
  * 修改 cx::regex -> CXXRegex
  * 修改 cx::regex_match -> CXXRegexMatch
  * 修改 cx::regex_match_iterator -> CXXRegexMatchIterator
  * 修改 cx::variant -> CXXVariant
  * 修改 cx::text_codec -> CXXTextCodec
  * 修改 cx::scoped_handle -> CXXScopedHandle
  * 修改 cx::scoped_ptr -> CXXScopedPointer
  * 修改 cx::scoped_array_ptr -> CXXScopedArrayPointer
  * 修改 cx::string -> CXXString
  * 修改 cx::vec<T> -> CXXVector<T>
  * 修改 cx::atomic<T> -> CXXAtomic<T>
  * 修改 更改部分类名、命名风格、移出 cx 命名空间
  * 修改 结构更改为 pimpl 模式、将绝大部分定义移入源文件、将接口与数据、逻辑分离

#### [2024-08-21]()
  * 公告 后续更新预告:
	  * 将绝大多数的函数定义移入源文件(.cpp)，以减少头文件(.h)的体积
	  * 将接口与数据、逻辑分离到私有类中(pimpl)，减少后续更新对外部接口的影响 例如 CXTest -> CXTestPrivate
	  * 将大部分的类移出命名空间、并更改为新的名称、所有枚举统一併入命名空间，不再使用 CX 前缀，改為 CXX
	  * 所有类将回归使用大驼峰命名法、成员函数、全局函数统一使用小驼峰命名法
	  * 统一使用 allman 风格排版、私有成员变量统一使用 使用 '_' 结尾、禁止省略任何花括号
	  * 其他规范基本参照 Google 风格指南 https:zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/contents.html
  * 修改 不再支持C++14或以下版本

#### [2024-06-21]()
  * 修改 移除所有扩展库自定义的基础类型:
	* 修改 全部改回C++标准库的基础类型 bool char int float short __int64 double wchar_t 
	* 修改 无符号类型则使用 windows.h 中的 UINT UINT64 (因炫彩无法辨识 unsigned 关键字)
    * 自适应32/64无符号整数类型统一改为 size_t
    * 弃用正整型一律改为 __int64

#### [2024-05-07]()
  * 没有做任何改动，仅应对IDE更新导致模块日誌消失的问题

#### [2024-05-03]()
  * 新增 cx::thread 线程 用于更深度的线程创建

#### [2024-04-28]()
  * 修改 暂时移除 cx::resource

#### [2024-04-11]()
  * 修正 为避免CXXString接收 char 或 char* 时出现递归的问题，移除 CXXString 对 char 或 char* 的接收支持
    * 但依然可以使用 CXXString::fromUtf8 或 CXXString::fromLocal8Bit 来接收 char 或 char* 转换成 CXXString

#### [2024-03-31]()
  * 修正 cx::future_synchronizer 编译失败问题

#### [2024-03-30]()
  * 修正 cx::library 缺少調用約定選擇導致棧溢出的問題

#### [2024-03-28]()
  * 新增 cx::com_obj 现在可以设置对象属性值、可以更方便用于调用系统COM比如js或vb
  * 优化变体型可以直接接收基本类型的参数
  * 修改 开始将部份类分离成公/私有类

#### [2024-03-24]()
  * 新增 cx::library 动态库
  * 修正 文本::格式化 置数字的问题
  * 修正 发行版编译问题

#### [2024-03-23]()
  * 修正 cx::async 编译错误
  * 修正 cx::resource 读取问题

#### [2024-03-21]()
  * 新增 增加 cx::resource(炫::资源) 用于读取xcc或xrc资源，可透过结构路径取用资源
    * 资源文件生成器位于 data/XC_module/Bestkakkoii飞/cx/bin/xcc 下

#### [2024-03-14]()
  * 移除 cx::debug 对 codecvt 的依赖
  * 新增 增加 CXXString 对多字节直接输入的支持
  * 修改 现在源代码编码/运行环境编码将强制转换为 utf8 编码
  * 修改 现在开始会逐步将扩展模块内部所有std::string, std::wstring, CXText, CXTextA 替换为使用CXXString

#### [2024-03-12]()
  * 新增 cx::crypto_hash 哈希算法器

#### [2024-03-10]()
  * 新增 CXXString(炫::文本) [还有许多功能没完善，但已经可以完全取代文本型 文本型A]

#### [2024-03-07]()
  * 新增 cx::debug 增加输入指令、控制台命令、控制台进度条类

#### [2024-03-02]()
  * 新增 cx::async 新增宏 炫_执行线程(函数指针, ...)
  * 新增 cx::future 和 cx::future_synchronizer 新增函数
  * 新增 cx::async 添加使用示例
  * 新增 cx::async 新增对C++20的支持(因 std::result_of 在 C++17 被弃用而在 C++20 被移除)

#### [2024-02-26]()
  * 新增 cx::setting(炫::配置) ini配置文件或注册表读写

#### [2024-02-26]()
  * 修正 CXXTextCodec 部份错误
  * 新增 CXXTextCodec 增加字符串映射、使用字符串名称获取编码器、常量代码页枚举..等

#### [2024-02-25]()
  * 修正 cx::futre 和 cx::future_synchronizer 编译错误的问题

#### [2024-02-22]()
  * 新增 cx::com(炫::COM对象)

#### [2024-02-20]()
  * 修正 cx::variant 保存ANSI字符串时析构抛出异常的问题

#### [2024-02-19]()
  * 修改 cx::var(炫::变量) 至 cx::variant(炫::通用型)
  * 修正 cx::variant的toString方法调用后引发异常的问题
  * 修正 cx::random toInt方法在64位下会出现警告的问题

#### [2024-02-09]()
  * 新增 为了兼容炫IDE，改变cx::random模板和函数调用方法

#### [2024-02-08]()
  * 新增 cx::random 随机

#### [2024-02-07]()
  * 新增 cx::variant 通用型
  * 修改 统一类命名

#### [2024-02-05]()
  * 新增 cx::file_dialog 文件对话框
  * 修正 cx::debug 无法正常输出 CXXVector<CXText>的问题

#### [2024-02-04]()
  * 新增 大量C++ STL容器函数
  * 修改 因手残洗掉旧的日志重写一篇