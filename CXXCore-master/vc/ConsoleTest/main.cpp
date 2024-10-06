#pragma execution_character_set("utf-8")
#include <cxxcore.h>

struct Test
{
	int a = 0;
	int b = 1;
};

int main()
{
	int a = 0;
	CXXDebug() << &a;
	cxxPrint() << CXXString(L"AAAAAa");

	cxx::pauseConsole();

	return 0;
}
