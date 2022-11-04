#include <chrono>
#include <future>

#include "CppUnitTest.h"

#include "../src/Core/AbstractApplication.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EntgeltrechnerTests
{
    ////////////////////////////// TEST APPS //////////////////////////////

    class BadApplicationThrowingException : public AbstractApplication
    {
    public:
        BadApplicationThrowingException()
            : AbstractApplication(GetModuleHandle(NULL),
                                  L"BadApplicationThrowingException",
                                  FinalizeExceptionMode::NOTHING)
        {
        }

        int run()
        {
            throw std::exception("faulty");
        }
    };

    class BadApplicationThrowingSomeOtherValue : public AbstractApplication
    {
    public:
        BadApplicationThrowingSomeOtherValue()
            : AbstractApplication(GetModuleHandle(NULL),
                                  L"BadApplicationThrowingSomeOtherValue",
                                  FinalizeExceptionMode::NOTHING)
        {
        }

        int run()
        {
            throw 12345;
        }
    };

    class GoodApplication : public AbstractApplication
    {
    public:
        GoodApplication(int exitCode)
            : AbstractApplication(GetModuleHandle(NULL),
                                  L"GoodApplication",
                                  FinalizeExceptionMode::NOTHING)
        {
            this->exitCode = exitCode;
        }

        int run()
        {
            return this->exitCode;
        }

    private:
        int exitCode;
    };

    ////////////////////////////// TESTS //////////////////////////////

    TEST_CLASS(AbstractApplicationTests)
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
        TEST_METHOD(testFaultyApplicationsReturnExitFailure)
        {
            // Ensures an application which throws an exception (or something
            // else) does not cause the entire program to crash but returns
            // EXIT_FAILURE.

            Assert::AreEqual(EXIT_FAILURE,
                             BadApplicationThrowingException().start());
            Assert::AreEqual(EXIT_FAILURE,
                             BadApplicationThrowingSomeOtherValue().start());
        }

        TEST_METHOD(testGoodApplicationPassesThroughExitCodeProperly)
        {
            Assert::AreEqual(EXIT_SUCCESS,
                             GoodApplication(EXIT_SUCCESS).start());
            Assert::AreEqual(54321, GoodApplication(54321).start());
        }
    };
}