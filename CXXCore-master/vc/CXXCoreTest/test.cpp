#include "CppUnitTest.h"
#include "cxxstring.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<CXXString>(const CXXString& str)
			{
				return str.toStdWString();
			}
		}
	}
}


namespace CXXStringTests
{
	TEST_CLASS(CXXStringTests)
	{
	public:

		TEST_METHOD(Constructor_Default)
		{
			CXXString str;
			Assert::IsTrue(str.isEmpty());
		}

		TEST_METHOD(Constructor_Copy)
		{
			CXXString str1(L"Hello");
			CXXString str2(str1);
			Assert::AreEqual(str1, str2);
		}

		TEST_METHOD(Constructor_WString)
		{
			std::wstring wstr = L"Hello";
			CXXString str(wstr);
			Assert::AreEqual(wstr.c_str(), str.c_str());
		}

		TEST_METHOD(Constructor_WCharPtr)
		{
			const wchar_t* wstr = L"Hello";
			CXXString str(wstr);
			Assert::AreEqual(wstr, str.c_str());
		}

		TEST_METHOD(Constructor_WChar)
		{
			wchar_t wch = L'H';
			CXXString str(wch);
			Assert::AreEqual(L"H", str.c_str());
		}

		TEST_METHOD(Constructor_Move)
		{
			CXXString str1(L"Hello");
			CXXString str2(std::move(str1));
			Assert::AreEqual(L"Hello", str2.c_str());
			Assert::IsTrue(str1.isEmpty());
		}

		TEST_METHOD(Operator_Assign)
		{
			CXXString str1(L"Hello");
			CXXString str2;
			str2 = str1;
			Assert::AreEqual(str1, str2);
		}

		TEST_METHOD(Operator_Assign_WCharPtr)
		{
			const wchar_t* wstr = L"Hello";
			CXXString str;
			str = wstr;
			Assert::AreEqual(wstr, str.c_str());
		}

		TEST_METHOD(Operator_Assign_Move)
		{
			CXXString str1(L"Hello");
			CXXString str2;
			str2 = std::move(str1);
			Assert::AreEqual(L"Hello", str2.c_str());
			Assert::IsTrue(str1.isEmpty());
		}

		TEST_METHOD(Operator_Equal)
		{
			CXXString str1(L"Hello");
			CXXString str2(L"Hello");
			Assert::IsTrue(str1 == str2);
		}

		TEST_METHOD(Operator_NotEqual)
		{
			CXXString str1(L"Hello");
			CXXString str2(L"World");
			Assert::IsTrue(str1 != str2);
		}

		TEST_METHOD(Operator_Greater)
		{
			CXXString str1(L"World");
			CXXString str2(L"Hello");
			Assert::IsTrue(str1 > str2);
		}

		TEST_METHOD(Operator_Less)
		{
			CXXString str1(L"Hello");
			CXXString str2(L"World");
			Assert::IsTrue(str1 < str2);
		}

		TEST_METHOD(Operator_Index)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L'H', str[0]);
		}

		TEST_METHOD(Swap)
		{
			CXXString str1(L"Hello");
			CXXString str2(L"World");
			str1.swap(str2);
			Assert::AreEqual(L"World", str1.c_str());
			Assert::AreEqual(L"Hello", str2.c_str());
		}

		TEST_METHOD(Size)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(static_cast<__int64>(5), str.size());
		}

		TEST_METHOD(Count)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(static_cast<__int64>(5), str.count());
		}

		TEST_METHOD(Length)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(static_cast<__int64>(5), str.length());
		}

		TEST_METHOD(IsEmpty)
		{
			CXXString str;
			Assert::IsTrue(str.isEmpty());
		}

		TEST_METHOD(Resize)
		{
			CXXString str(L"Hello");
			str.resize(3);
			Assert::AreEqual(L"Hel", str.c_str());
		}

		TEST_METHOD(Resize_FillChar)
		{
			CXXString str(L"Hello");
			str.resize(8, L'x');
			Assert::AreEqual(L"Helloxxx", str.c_str());
		}

		TEST_METHOD(Fill)
		{
			CXXString str(L"Hello");
			str.fill(L'x', 3);
			Assert::AreEqual(L"xxx", str.c_str());
		}

		TEST_METHOD(Truncate)
		{
			CXXString str(L"Hello");
			str.truncate(3);
			Assert::AreEqual(L"Hel", str.c_str());
		}

		TEST_METHOD(Chop)
		{
			CXXString str(L"Hello");
			str.chop(2);
			Assert::AreEqual(L"Hel", str.c_str());
		}

		TEST_METHOD(Capacity)
		{
			CXXString str(L"Hello");
			Assert::IsTrue(str.capacity() >= str.size());
		}

		TEST_METHOD(Reserve)
		{
			CXXString str(L"Hello");
			str.reserve(10);
			Assert::IsTrue(str.capacity() >= 10);
		}

		TEST_METHOD(Squeeze)
		{
			CXXString str(L"Hello");
			str.reserve(10);
			str.squeeze();
			Assert::IsTrue(str.capacity() >= str.size());
		}

		TEST_METHOD(Get)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L"Hello", str.get());
		}

		TEST_METHOD(Data)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L"Hello", str.data());
		}

		TEST_METHOD(ConstData)
		{
			const CXXString str(L"Hello");
			Assert::AreEqual(L"Hello", str.constData());
		}

		TEST_METHOD(CStr)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L"Hello", str.c_str());
		}

		TEST_METHOD(UShort)
		{
			CXXString str(L"Hello");
			USHORT* ushort = str.ushort();
			Assert::AreEqual(static_cast<USHORT>(L'H'), ushort[0]);
		}

		TEST_METHOD(Clear)
		{
			CXXString str(L"Hello");
			str.clear();
			Assert::IsTrue(str.isEmpty());
		}

		TEST_METHOD(At)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L'H', str.at(0).c_str()[0]);
		}

		TEST_METHOD(Value)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L'H', str.value(0).c_str()[0]);
		}

		TEST_METHOD(Front)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L'H', str.front());
		}

		TEST_METHOD(Back)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L'o', str.back());
		}

		TEST_METHOD(First)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L'H', str.first());
		}

		TEST_METHOD(Last)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L'o', str.last());
		}

		TEST_METHOD(LastIndexOf)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(static_cast<__int64>(4), str.lastIndexOf(L"o"));
		}

		TEST_METHOD(Contains)
		{
			CXXString str(L"Hello");
			Assert::IsTrue(str.contains(L"ell"));
		}

		TEST_METHOD(Count_Char)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(static_cast<__int64>(2), str.count(L"l"));
		}

		TEST_METHOD(IndexOf)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(static_cast<__int64>(1), str.indexOf(L"e"));
		}

		TEST_METHOD(Left)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L"Hel", str.left(3).c_str());
		}

		TEST_METHOD(Right)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L"llo", str.right(3).c_str());
		}

		TEST_METHOD(Mid)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L"ell", str.mid(1, 3).c_str());
		}

		TEST_METHOD(Chopped)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L"Hel", str.chopped(2).c_str());
		}

		TEST_METHOD(StartsWith)
		{
			CXXString str(L"Hello");
			Assert::IsTrue(str.startsWith(L"He"));
		}

		TEST_METHOD(EndsWith)
		{
			CXXString str(L"Hello");
			Assert::IsTrue(str.endsWith(L"lo"));
		}

		TEST_METHOD(IsUpper)
		{
			CXXString str(L"HELLO");
			Assert::IsTrue(str.isUpper());
		}

		TEST_METHOD(IsLower)
		{
			CXXString str(L"hello");
			Assert::IsTrue(str.isLower());
		}

		TEST_METHOD(LeftJustified)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L"Hello   ", str.leftJustified(8, L' ').c_str());
		}

		TEST_METHOD(RightJustified)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L"   Hello", str.rightJustified(8, L' ').c_str());
		}

		TEST_METHOD(ToLower)
		{
			CXXString str(L"HELLO");
			Assert::AreEqual(L"hello", str.toLower().c_str());
		}

		TEST_METHOD(ToUpper)
		{
			CXXString str(L"hello");
			Assert::AreEqual(L"HELLO", str.toUpper().c_str());
		}

		TEST_METHOD(ToHalf)
		{
			// Assuming toHalf converts full-width characters to half-width
			CXXString str(L"Ｈｅｌｌｏ");
			Assert::AreEqual(L"Hello", str.toHalf().c_str());
		}

		TEST_METHOD(ToFull)
		{
			// Assuming toFull converts half-width characters to full-width
			CXXString str(L"Hello");
			Assert::AreEqual(L"Ｈｅｌｌｏ", str.toFull().c_str());
		}

		TEST_METHOD(Trimmed)
		{
			CXXString str(L"  Hello  ");
			Assert::AreEqual(L"Hello", str.trimmed().c_str());
		}

		TEST_METHOD(Simplified)
		{
			CXXString str(L"  Hello   World  ");
			Assert::AreEqual(L"Hello World", str.simplified().c_str());
		}

		TEST_METHOD(ToHtmlEscaped)
		{
			CXXString str(L"<Hello>");
			Assert::AreEqual(L"&lt;&#72;&#101;&#108;&#108;&#111;&gt;", str.toHtmlEscaped().c_str());
		}

		TEST_METHOD(ToUrl)
		{
			CXXString str(L"Hello World");
			Assert::AreEqual(L"Hello%20World", str.toUrl().c_str());
		}

		TEST_METHOD(Insert)
		{
			CXXString str(L"Hello");
			str.insert(3, L"p");
			Assert::AreEqual(L"Helplo", str.c_str());
		}

		TEST_METHOD(Append)
		{
			CXXString str(L"Hello");
			str.append(L" World");
			Assert::AreEqual(L"Hello World", str.c_str());
		}

		TEST_METHOD(Prepend)
		{
			CXXString str(L"World");
			str.prepend(L"Hello ");
			Assert::AreEqual(L"Hello World", str.c_str());
		}

		TEST_METHOD(Operator_PlusEqual)
		{
			CXXString str(L"Hello");
			str += L" World";
			Assert::AreEqual(L"Hello World", str.c_str());
		}

		TEST_METHOD(Operator_Plus)
		{
			CXXString str1(L"Hello");
			CXXString str2 = str1 + L" World";
			Assert::AreEqual(L"Hello World", str2.c_str());
		}

		TEST_METHOD(Remove)
		{
			CXXString str(L"Hello World");
			str.remove(5, 6);
			Assert::AreEqual(L"Hello", str.c_str());
		}

		TEST_METHOD(Replace)
		{
			CXXString str(L"Hello World");
			str.replace(6, 5, L"Universe");
			Assert::AreEqual(L"Hello Universe", str.c_str());
		}

		TEST_METHOD(Format)
		{
			CXXString str(L"%1 %2 %3");
			str = str.format(123, L"hello", 3.14);
			Assert::AreEqual(L"123 hello 3.140", str.c_str());
		}

		TEST_METHOD(Split)
		{
			CXXString str(L"Hello,World");
			auto result = str.split(L",");
			Assert::AreEqual(static_cast<__int64>(2), result.size());
			Assert::AreEqual(L"Hello", result[0].c_str());
			Assert::AreEqual(L"World", result[1].c_str());
		}

		TEST_METHOD(Repeated)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L"HelloHelloHello", str.repeated(3).c_str());
		}

		TEST_METHOD(ToHexString)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L"48656c6c6f", str.toHexString().toLower().c_str());
		}

		TEST_METHOD(ToBase64)
		{
			CXXString str(L"Hello");
			Assert::AreEqual(L"SGVsbG8=", str.toBase64().c_str());
		}

		TEST_METHOD(ToUtf8)
		{
			CXXString str(L"Hello");
			Assert::AreEqual("Hello", str.toUtf8().c_str());
		}

		TEST_METHOD(ToLocal8Bit)
		{
			CXXString str(L"Hello");
			Assert::AreEqual("Hello", str.toLocal8Bit().c_str());
		}

		TEST_METHOD(ToStdWString)
		{
			CXXString str(L"Hello");
			std::wstring wstr = str.toStdWString();
			Assert::AreEqual(L"Hello", wstr.c_str());
		}

		TEST_METHOD(ToStdString)
		{
			CXXString str(L"Hello");
			std::string sstr = str.toStdString();
			Assert::AreEqual("Hello", sstr.c_str());
		}

		TEST_METHOD(FromUtf8)
		{
			CXXString str = CXXString::fromUtf8("Hello");
			Assert::AreEqual(L"Hello", str.c_str());
		}

		TEST_METHOD(FromLocal8Bit)
		{
			CXXString str = CXXString::fromLocal8Bit("Hello");
			Assert::AreEqual(L"Hello", str.c_str());
		}

		TEST_METHOD(Compare)
		{
			CXXString str1(L"Hello");
			CXXString str2(L"World");
			Assert::IsTrue(CXXString::compare(str1, str2) < 0);
		}

		TEST_METHOD(ToShort)
		{
			CXXString str(L"123");
			bool ok;
			short value = str.toShort(&ok);
			Assert::IsTrue(ok);
			Assert::AreEqual(static_cast<short>(123), value);
		}

		TEST_METHOD(ToInt)
		{
			CXXString str(L"123");
			bool ok;
			int value = str.toInt(&ok);
			Assert::IsTrue(ok);
			Assert::AreEqual(123, value);
		}

		TEST_METHOD(ToLong)
		{
			CXXString str(L"123");
			bool ok;
			long value = str.toLong(&ok);
			Assert::IsTrue(ok);
			Assert::AreEqual(123L, value);
		}

		TEST_METHOD(ToLongLong)
		{
			CXXString str(L"123");
			bool ok;
			__int64 value = str.toLongLong(&ok);
			Assert::IsTrue(ok);
			Assert::AreEqual(static_cast<__int64>(123), value);
		}

		TEST_METHOD(ToFloat)
		{
			CXXString str(L"123.45");
			bool ok;
			float value = str.toFloat(&ok);
			Assert::IsTrue(ok);
			Assert::AreEqual(123.45f, value);
		}

		TEST_METHOD(ToDouble)
		{
			CXXString str(L"123.45");
			bool ok;
			double value = str.toDouble(&ok);
			Assert::IsTrue(ok);
			Assert::AreEqual(123.45, value);
		}

		TEST_METHOD(SetNum_Short)
		{
			CXXString str;
			str.setNum(static_cast<short>(123));
			Assert::AreEqual(L"123", str.c_str());
		}

		TEST_METHOD(SetNum_Int)
		{
			CXXString str;
			str.setNum(123);
			Assert::AreEqual(L"123", str.c_str());
		}

		TEST_METHOD(SetNum_Long)
		{
			CXXString str;
			str.setNum(123L);
			Assert::AreEqual(L"123", str.c_str());
		}

		TEST_METHOD(SetNum_LongLong)
		{
			CXXString str;
			str.setNum(static_cast<__int64>(123));
			Assert::AreEqual(L"123", str.c_str());
		}

		TEST_METHOD(SetNum_Float)
		{
			CXXString str;
			str.setNum(123.45f);
			Assert::AreEqual(L"123.45", str.c_str());
		}

		TEST_METHOD(SetNum_Double)
		{
			CXXString str;
			str.setNum(123.45);
			Assert::AreEqual(L"123.45", str.c_str());
		}

		TEST_METHOD(Number_Short)
		{
			CXXString str = CXXString::number(static_cast<short>(123));
			Assert::AreEqual(L"123", str.c_str());
		}

		TEST_METHOD(Number_Int)
		{
			CXXString str = CXXString::number(123);
			Assert::AreEqual(L"123", str.c_str());
		}

		TEST_METHOD(Number_Long)
		{
			CXXString str = CXXString::number(123L);
			Assert::AreEqual(L"123", str.c_str());
		}

	};
}