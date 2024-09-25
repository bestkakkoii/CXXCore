函数 整型 入口函数()
	炫文本 炫文本
	// 转 UTF8
	CXX_AUTO utf8str = 炫文本.到UTF8()

	// 转 std::string 依据默认编码
	CXX_AUTO sstr = 炫文本.到STDSTRING()

	// 转 std::wstring
	CXX_AUTO wsstr = 炫文本.到STDWSTRING()

	// 转本地ANSI (根据当前系统ACP)
	CXX_AUTO local = 炫文本.到本地编码()

	// 所有编码器都是全局静态,每个ACP只会存在最多一个实例

	// 可以使用ACP取编码器
	炫文本编码器 *繁体编码器 = 炫文本编码器::编码取文本编码器(950);

	// ANSI 到 UNICODE
	CXX_AUTO big5Unicode = 繁体编码器->到十六位万国码(A"")

	// UNICODE 到 ANSI
	CXX_AUTO big5 = 繁体编码器->到指定编码("")

	// 也可以使用名称取编码器
	炫文本编码器 *简体编码器 = 炫文本编码器::编码名称取文本编码器("GB2312")

	// ANSI 到 UNICODE
	CXX_AUTO gb2312Unicode = 简体编码器->到十六位万国码(A"")

	// UNICODE 到 ANSI
	CXX_AUTO gb2312 = 简体编码器->到指定编码("")

	返回 0
