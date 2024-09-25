
#include <CppUnitTest.h>

#include <cxxcore.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CXModuleTest
{
	using namespace cxx;

	TEST_CLASS(CXXDebugTest)
	{
	public:
		TEST_METHOD(TestDebugOutput)
		{
			{
				CXXDebug debug;
				debug << 1i8;
				Assert::AreEqual(L"1i8 ", debug.toString().c_str());
			}

			{
				CXXDebug debug;
				debug << L"WCHAR*";
				Assert::AreEqual(L"WCHAR* ", debug.toString().c_str());
			}

			{
				CXXDebug debug;
				debug << "CHAR*";
				Assert::AreEqual(L"CHAR* ", debug.toString().c_str());
			}

			{
				CXXDebug debug;
				debug << L'w';
				Assert::AreEqual(L"w ", debug.toString().c_str());
			}

			{
				CXXDebug debug;
				debug << 'a';
				Assert::AreEqual(L"a ", debug.toString().c_str());
			}
		}
	};

	TEST_CLASS(CXXLibraryTest)
	{
	public:
		TEST_METHOD(TestLoad)
		{
			CXXLibrary lib(L"kernel32");
			Assert::IsTrue(lib.isValid());
		}

		TEST_METHOD(TestResolve)
		{
			CXXLibrary lib(L"kernel32");
			Assert::IsTrue(lib.isValid());

			auto proc = lib.resolve(L"GetModuleHandleW");
			Assert::IsNotNull(reinterpret_cast<void*>(proc));
		}

		TEST_METHOD(TestLoadAndResolve)
		{
			CXXLibrary lib(L"kernel32");
			Assert::IsTrue(lib.isValid());

			auto proc = lib.resolve(L"GetModuleHandleW");
			Assert::IsNotNull(reinterpret_cast<void*>(proc));
		}

		TEST_METHOD(TestLoadAndResolveWithParameterAndReturnValue)
		{
			CXXLibrary lib(L"kernel32");
			Assert::IsTrue(lib.isValid());

			HMODULE hModule = CXX_NULLPTR;
			bool result = lib.invoke(CallingConvention::StdCall, &hModule, L"GetModuleHandleW", L"kernel32.dll");
			Assert::IsTrue(result);
			Assert::IsNotNull(hModule);
		}
	};

	TEST_CLASS(CXXWinToastTest)
	{
	public:
		class ToastResultCallbackTest
		{
		public:
			void callback(int result, int activeIndex)
			{
				cxxPrint() << L"result:" << result << L"activeIndex:" << activeIndex;
			}
		};

		TEST_METHOD(TestNotifyInitialize)
		{
			CXXWinToast::setAppName(L"MyWinToast");
			CXXWinToast::setAppUserModelId(L"MyToastLLC", L"MyWinToast", L"MySubToast", L"1.0.0.0");
			Assert::IsTrue(CXXWinToast::initialize());
		}

		TEST_METHOD(TestNotify)
		{
			CXXVariant a;
			ToastResultCallbackTest callback;
			CXXWinToast toast(cxx::Text04);
			toast.setAudio(cxx::SMS);
			toast.setTextField(L"hello world", 0);
			toast.setTextField(L"hello two", 1);
			toast.setTextField(L"hello three", 2);
			toast.addAction(L"oh yes");
			toast.addAction(L"oh no");
			toast.setTimeout(30000);
			toast.setToastResultCallback(&ToastResultCallbackTest::callback, &callback);
			Assert::IsTrue(toast.show());
		}
	};

	TEST_CLASS(CXXProcessTest)
	{
	public:
		class ProcessStdOutCallbackTest
		{
		public:
			void callback(const CXXString& text)
			{
				cxxPrint() << text;
			}
		};

		static void testCreateProcess()
		{
			CXXProcess process;
			CXXVector<CXXString> args;
			args << L"/M";
			ProcessStdOutCallbackTest callback;
			process.setStdOutCallback(&ProcessStdOutCallbackTest::callback, &callback);
			process.start(L"tasklist", args);
			Assert::IsTrue(process.waitForStarted());
		}

		TEST_METHOD(TestCreateProcess)
		{
			testCreateProcess();
			Assert::IsTrue(true);
		}
	};
}
