函数 整型 入口函数()
	炫文本数组 CLSIDs;
	炫文本数组 proIDs;
	如果 (炫组件对象::取信息("scrrun.dll", &CLSIDs, &proIDs))
		炫打印() << "CLSID:" << CLSIDs;
		炫打印() << "ProID:" << proIDs;
	
	// 初始化 COM组件
	炫组件对象 组件;
	如果 (!组件.创建("WScript.Shell"))
		炫错误() << "WScript.Shell COM 对象创建失败";
		返回 1;
	
	// 要执行的 PowerShell 命令
	炫文本 命令 = "powershell.exe -NoProfile -ExecutionPolicy Bypass -Command \"Get-Process | Select-Object -First 5\"";

	// 调用 WScript.Shell 的 Run 方法来执行 PowerShell 命令
	炫组件变体型 结果 = 组件.通用方法("Run", 命令, 0, true); // 参数: 命令, 窗口样式, 是否等待执行完成
	如果 (结果.isNull())
		炫错误() << "PowerShell 命令执行失败";
		返回 1;
	
	炫打印() << "类型: " << 结果.类型名();
	炫打印() << "结果: " << 结果.到整型();
	炫::控制台暂停();
	返回 0
