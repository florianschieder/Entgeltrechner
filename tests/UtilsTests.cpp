#include "CppUnitTest.h"

#include "../src/Core/Utils.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EntgeltrechnerTests
{
    TEST_CLASS(UtilsTests)
    {
    private:
        _CrtMemState previousState;

    public:
        TEST_METHOD_INITIALIZE(setUp)
        {
            _CrtMemCheckpoint(&this->previousState);
        }

        TEST_METHOD_CLEANUP(tearDown)
        {
            _CrtMemState currentState, stateDiff;
            _CrtMemCheckpoint(&currentState);
            int diffResult = _CrtMemDifference(&stateDiff,
                                               &this->previousState,
                                               &currentState);
            if (diffResult) {
                Assert::Fail(L"memory leak(s) detected.");
            }
        }
        TEST_METHOD(testAnsiToWideString)
        {
            const auto actual = ansiToWideString("foobar");
            const auto expected = std::wstring(L"foobar");

            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(testWideToAnsiString)
        {
            const auto actual = wideToAnsiString(L"foobar");
            const auto expected = std::string("foobar");

            Assert::AreEqual(expected, actual);
        }
    };
}