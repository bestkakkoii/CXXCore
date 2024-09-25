静态 函数 空类型 json输出()
	炫JSON json; // 首先创建一个空的json对象
	json["pi"] = 3.141;
	json["happy"] = true;
	json["name"] = "Niels";
	json["nothing"] = CXX_NULLPTR;
	json["answer"]["everything"] = 42; // 初始化answer对象
	json["list"] = {1, 0, 2}; // 使用列表初始化的方法对"list"数组初始化
	json["object"] = {{A"currency", A"USD"}, {A"value", 42.99}}; // 初始化object对象
	炫文件 文件("test.json", 炫::写入 | 炫::清空文件);
	文件.写(json.到JSON(炫::缩进));
	文件.刷新()

	炫文件夹::打开(炫程序::目录());

函数 整型 入口函数()
	炫文件夹::置当前工作目录(炫程序::目录());

	炫JSON json( \
	"{" \
	"    \"key\": {" \
	"        \"STRING\": \"value\"," \
	"        \"INT\": 123," \
	"        \"BOOL\": true," \
	"        \"DOUBLE\": 3.14," \
	"        \"ARRAY\": [1, 2, 3]," \
	"        \"OBJECT\": {" \
	"            \"key1\": \"value1\"," \
	"            \"key2\": \"value2\"" \
	"        }" \
	"    }" \
	"}");
	
	炫调试() << json.到JSON(炫::缩进);

	json输出()

	炫::控制台暂停()
	返回 0