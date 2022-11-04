#include "CppUnitTest.h"

#include "../src/Core/Registry.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EntgeltrechnerTests
{
    TEST_CLASS(RegistryTests)
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

        TEST_METHOD(testCanReadDefaultValue)
        {
            auto key = RegistryKey(HKEY_CURRENT_USER,
                                   L"SOFTWARE",
                                   RegistryKeyMode::Open);
            Assert::AreEqual(1337L,
                             key.loadValue(L"SomeWeirdInexistentKey", 1337));
        }

        TEST_METHOD(testThrowsExceptionIfKeyCannotBeFound)
        {
            auto notFound = false;
            try {
                auto _ = RegistryKey(HKEY_LOCAL_MACHINE,
                                     L"SomeInexistentSubkey",
                                     RegistryKeyMode::Open);
            }
            catch (const RegistryKeyNotFoundException &_) {
                UNREFERENCED_PARAMETER(_);
                notFound = true;
            }
            Assert::IsTrue(notFound);
        }

        TEST_METHOD(testThrowsExceptionIfKeyMayNotBeAccessed)
        {
            // HLKM\SECURITY is a Windows system registry key.
            // One can assume the test always should pass if the test is
            // running under "normal" privileges. If it does not, either
            // the implementation is broken or the test is running on a
            // really weird Windows setup.
            auto inaccessible = false;
            try {
                auto _ = RegistryKey(HKEY_LOCAL_MACHINE,
                                     L"SECURITY",
                                     RegistryKeyMode::Open);
            }
            catch (const RegistryKeyInaccessibleException &_) {
                UNREFERENCED_PARAMETER(_);
                inaccessible = true;
            }
            Assert::IsTrue(inaccessible);
        }

        TEST_METHOD(testReadAndWrite)
        {
            RegistryKey(
                HKEY_CURRENT_USER,
                L"SOFTWARE\\Entgeltrechner\\Unittests\\testReadAndWrite",
                RegistryKeyMode::Create)
                .drop();

            auto key = RegistryKey(
                HKEY_CURRENT_USER,
                L"SOFTWARE\\Entgeltrechner\\Unittests\\testReadAndWrite",
                RegistryKeyMode::Create);

            // (1) long
            Assert::AreEqual(-1L, key.loadValue(L"Foobar", -1));
            key.saveValue(L"Foobar", 1337);
            Assert::AreEqual(1337L, key.loadValue(L"Foobar", -1));
            key.dropValue(L"Foobar");
            Assert::AreEqual(-1L, key.loadValue(L"Foobar", -1));

            // (2) ANSI String
            Assert::AreEqual(std::string(""), key.loadValue(L"Foobar", ""));
            key.saveValue(L"Foobar", "Blubb");
            Assert::AreEqual(std::string("Blubb"),
                             key.loadValue(L"Foobar", ""));
            key.dropValue(L"Foobar");
            Assert::AreEqual(std::string(""), key.loadValue(L"Foobar", ""));

            // (3) Unicode String
            Assert::AreEqual(std::wstring(L""), key.loadValue(L"Foobar", L""));
            key.saveValue(L"Foobar", L"Blubb");
            Assert::AreEqual(std::wstring(L"Blubb"),
                             key.loadValue(L"Foobar", L""));
            key.dropValue(L"Foobar");
            Assert::AreEqual(std::wstring(L""), key.loadValue(L"Foobar", L""));

            // (4) Drop the key and ensure it does not exist anymore
            key.drop();
            auto notFound = false;
            try {
                auto _ = RegistryKey(
                    HKEY_LOCAL_MACHINE,
                    L"SOFTWARE\\Entgeltrechner\\Unittests\\testReadAndWrite",
                    RegistryKeyMode::Open);
            }
            catch (const RegistryKeyNotFoundException &_) {
                UNREFERENCED_PARAMETER(_);
                notFound = true;
            }
            Assert::IsTrue(notFound);
        }
    };
}