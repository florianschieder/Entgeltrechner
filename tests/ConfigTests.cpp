#include "CppUnitTest.h"

#include "../src/Core/Config.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EntgeltrechnerTests
{
    TEST_CLASS(ConfigTests)
    {
    public:
        TEST_METHOD(testGetDefaultValue)
        {
            auto cfg = Config(L"Entgeltrechner", L"Unittests");

            Assert::AreEqual(false, cfg.get_config_item(L"foo", false));
            Assert::AreEqual(true, cfg.get_config_item(L"foo", true));
            Assert::AreEqual(1337, cfg.get_config_item(L"foo", 1337));
            Assert::AreEqual(std::string("foo"),
                             cfg.get_config_item(L"foo", std::string("foo")));
            Assert::AreEqual(std::wstring(L"foo"),
                             cfg.get_config_item(L"foo", std::wstring(L"foo")));
        }

        TEST_METHOD(testCanStoreBoolean)
        {
            auto cfg = Config(L"Entgeltrechner", L"Unittests");

            cfg.set_config_item(L"foo", true);
            Assert::AreEqual(true, cfg.get_config_item(L"foo", false));
            cfg.drop_config_item(L"foo");
            Assert::AreEqual(false, cfg.get_config_item(L"foo", false));
        }

        TEST_METHOD(testCanStoreInt)
        {
            auto cfg = Config(L"Entgeltrechner", L"Unittests");

            cfg.set_config_item(L"foo", 1337);
            Assert::AreEqual(1337, cfg.get_config_item(L"foo", 0));
            cfg.drop_config_item(L"foo");
            Assert::AreEqual(0, cfg.get_config_item(L"foo", 0));
        }

        TEST_METHOD(testCanStoreStr)
        {
            auto cfg = Config(L"Entgeltrechner", L"Unittests");

            cfg.set_config_item(L"foo", std::string("bar"));
            Assert::AreEqual(std::string("bar"),
                             cfg.get_config_item(L"foo", std::string("")));
            cfg.drop_config_item(L"foo");
            Assert::AreEqual(std::string("foo"),
                             cfg.get_config_item(L"foo", std::string("foo")));
        }

        TEST_METHOD(testCanStoreWStr)
        {
            auto cfg = Config(L"Entgeltrechner", L"Unittests");

            cfg.set_config_item(L"foo", std::wstring(L"bar"));
            Assert::AreEqual(std::wstring(L"bar"),
                             cfg.get_config_item(L"foo", std::wstring(L"")));
            cfg.drop_config_item(L"foo");
            Assert::AreEqual(std::wstring(L"foo"),
                             cfg.get_config_item(L"foo", std::wstring(L"foo")));
        }
    };
}