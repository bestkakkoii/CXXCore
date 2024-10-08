﻿静态 函数 逻辑型 调试输出处理回调(长整型 flags, const 炫文本& text)
	炫文件 文件("我的日志.log", 炫::写入 | 炫::附加文件尾, true)
	如果 炫::错误 == flags || 炫::异常 == flags
		文件.写(text)

	返回 假 // 如果返回真则不输出到调试窗口和控制台

函数 整型 入口函数()
	// 炫调试可以接收任意C / CPP基本类型、炫扩展自订类型、STL常用容器
	逻辑型c o = false
	字节型 a = 1
	短整型 b = 2
	整型 c = 3
	长整型 d = 4
	const 字符型 *e = "wchar_t*"
	const 字符型A *f = A"char*"
	const 字符型 g ='w'
	字符型A h = A'c'
	浮点型 i = 5.01f
	双浮点型 j = 6.02
	炫文本 文本 = "哈哈哈"
	炫动态数组<整型> vec = {1, 2, 3, 4};
	炫文本数组 strList = {"恭", "喜", "发", "财"}; // 等同于 炫动态数组<炫文本>

	炫调试::装消息处理器(&调试输出处理回调)

	炫调试(炫::资讯) << o;
	炫调试(炫::调试) << a;
	炫调试(炫::无) << b;
	炫调试(炫::错误) << c;
	炫调试() << d;
	炫调试() << e;
	炫调试() << f;
	炫调试() << g;
	炫调试() << h;
	炫调试() << i;
	炫调试() << j;
	炫调试() << 文本;
	炫调试() << vec;
	炫调试() << strList;
	炫调试() << &j;

	// 无修饰
	炫输出() << o;
	炫输出() << a;
	炫输出() << b;
	炫输出() << c;
	炫输出() << d;
	炫输出() << e;
	炫输出() << f;
	炫输出() << g;
	炫输出() << h;
	炫输出() << i;
	炫打印() << j;
	炫信息() << 文本;
	炫错误() << vec;
	炫错误() << strList;
	炫打印() << &j;

	// 单行多参数
	炫信息() << "哈哈哈" << 666 << A"嘿嘿嘿" << 123.321

	// 多行多参数
	炫信息() \
	<< "哈哈哈" \
	<< 666  \
	<< A"嘿嘿嘿" \
	<< 123.321
	炫线程::延时(2000)

	炫控制台进度条 进度条
	进度条.置颜色(炫::亮绿色)
	进度条.置填充字符('|');

	变量循环 (双浮点型 progress = 0; progress < 100; progress++)
		进度条.更新(progress / 100.0)
		炫线程::延时(50)
	进度条.更新(1.0)

	炫::控制台暂停()
	返回 0
