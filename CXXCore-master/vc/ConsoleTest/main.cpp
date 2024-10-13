#pragma execution_character_set("utf-8")
#include <../module_cxxcore.h>
#include <../module_sub_cxzip.h>

int main()
{
	CXXZip::compress(L"D:\\Users\\bestkakkoii\\Desktop\\202410112035", L"D:\\123.zip");

	CXXZip::uncompress(L"D:\\123.zip", L"D:\\新增資料夾");


	cxx::pauseConsole();

	return 0;
}
