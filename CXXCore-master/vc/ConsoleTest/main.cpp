#pragma execution_character_set("utf-8")
#include <cxxcore.h>

struct Test
{
	int a = 0;
	int b = 1;
};

int main()
{
	CXXDebug() << CXXString(L"AAAAAa");
	cxxPrint() << CXXString(L"AAAAAa");

	cxx::pauseConsole();

	return 0;
}
