#include <../module_cxxcore.h>

int main()
{
	CXXString testStr = L"aaa|bbb|ccc||ddd";

	CXXStringList testList = testStr.split(L"|", cxx::SkipEmptyParts);

	for (const CXXString& item : testList)
	{
		cxxDebug() << item;
	}

	cxx::pauseConsole();

	return 0;
}
