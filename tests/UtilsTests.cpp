#include "CppUnitTest.h"

#include "../src/Core/Utils.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EntgeltrechnerTests
{
    TEST_CLASS(UtilsTests)
    {
    public:
        TEST_METHOD(testAnsiToWideString)
        {
            const auto actual = ansiToWideString("foobar");
            const auto expected = std::wstring(L"foobar");

            Assert::AreEqual(expected, actual);
        }
    };
}