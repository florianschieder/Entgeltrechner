#include <Windows.h>

#include "CppUnitTest.h"

#include "../src/Hibernations/InsurancePercentageHibernation.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EntgeltrechnerTests
{
    TEST_CLASS(InsurancePercentageHibernationTests)
    {
    private:
        sqlite3 *dbHandle;
        InsurancePercentageHibernation *hib;
        _CrtMemState previousState;

    public:
        TEST_METHOD_INITIALIZE(setUp)
        {
            _CrtMemCheckpoint(&this->previousState);
            sqlite3_open(":memory:", &this->dbHandle);
            this->hib = new InsurancePercentageHibernation(this->dbHandle);

            auto result = sqlite3_exec(
                this->dbHandle,
                "\
				INSERT INTO `insurance_percentage` \
				  (`type`, `employerShare`, `employeeShare`, `year`) \
				VALUES \
				  (0, 0.15, 0.15, 2020), \
				  (1, 0.16, 0.16, 2020), \
				  (2, 0.17, 0.17, 2020), \
				  (3, 0.18, 0.18, 2020), \
				  (4, 0.19, 0.19, 2020), \
				  \
				  (0, 0.25, 0.255, 2021), \
				  (1, 0.26, 0.264, 2021), \
				  (2, 0.27, 0.253, 2021), \
				  (3, 0.28, 0.282, 2021), \
				  (4, 0.29, 0.291, 2021), \
				  \
				  (0, 0.35, 0.351, 2022), \
				  (1, 0.36, 0.362, 2022), \
				  (2, 0.37, 0.373, 2022), \
				  (3, 0.38, 0.384, 2022), \
				  (4, 0.39, 0.395, 2022);",
                [](void *, int, char **, char **) -> int { return 0; },
                NULL,
                NULL);
            auto msg = sqlite3_errmsg(this->dbHandle);
            Assert::AreEqual(SQLITE_OK, result);
        }

        TEST_METHOD_CLEANUP(tearDown)
        {
            sqlite3_close(this->dbHandle);
            delete this->hib;

            _CrtMemState currentState, stateDiff;
            _CrtMemCheckpoint(&currentState);
            int diffResult = _CrtMemDifference(&stateDiff,
                                               &this->previousState,
                                               &currentState);
            if (diffResult) {
                Assert::Fail(L"memory leak(s) detected.");
            }
        }

        TEST_METHOD(testEmptyResultSet)
        {
            InsurancePercentageFilters filters = {2019, std::nullopt};
            Assert::AreEqual(static_cast<size_t>(0),
                             this->hib->loadMany(filters).size());
        }

        TEST_METHOD(testBreaksOnInsufficientFilters)
        {
            InsurancePercentageFilters filters = {std::nullopt, std::nullopt};

            auto raisedException = false;
            try {
                this->hib->loadMany(filters);
            }
            catch (const InsufficientFiltersException &ex) {
                UNREFERENCED_PARAMETER(ex);
                raisedException = true;
            }
            Assert::IsTrue(raisedException);
        }

        TEST_METHOD(testCanFilterByYear)
        {
            InsurancePercentageFilters filters = {std::nullopt, std::nullopt};

            for (const auto &year : {2020, 2021, 2022}) {

                const auto &failureText =
                    std::format(L"failed for year {}", year);

                filters.year = year;
                auto result = this->hib->loadMany(filters);
                Assert::AreEqual(static_cast<size_t>(5),
                                 result.size(),
                                 failureText.c_str());

                for (const auto &record : result) {
                    Assert::AreEqual(year, record.year);
                }
            }
        }

        TEST_METHOD(testCanFilterByType)
        {
            InsurancePercentageFilters filters = {std::nullopt, std::nullopt};

            auto types = {
                InsuranceType::ARBEITSLOSENVERSICHERUNG,
                InsuranceType::KRANKENVERSICHERUNG,
                InsuranceType::PFLEGEVERSICHERUNG,
                InsuranceType::RENTENVERSICHERUNG,
                InsuranceType::UNFALLVERSICHERUNG,
            };
            for (const auto &type : types) {

                const auto &failureText =
                    std::format(L"failed for type {}", static_cast<int>(type));

                filters.type = type;
                auto result = this->hib->loadMany(filters);
                Assert::AreEqual(static_cast<size_t>(3),
                                 result.size(),
                                 failureText.c_str());

                for (const auto &record : result) {
                    Assert::AreEqual(static_cast<int>(type),
                                     static_cast<int>(record.type));
                }
            }
        }

        TEST_METHOD(testLoadsModel)
        {
            InsurancePercentageFilters filters = {
                2021,
                InsuranceType::RENTENVERSICHERUNG};
            auto result = this->hib->loadMany(filters);
            Assert::AreEqual(static_cast<size_t>(1), result.size());

            const auto &item = result[0];
            Assert::AreEqual(
                static_cast<int>(InsuranceType::RENTENVERSICHERUNG),
                static_cast<int>(item.type));
            Assert::AreEqual(0.27, item.employerShare);
            Assert::AreEqual(0.253, item.employeeShare);
            Assert::AreEqual(2021, item.year);
        }

        TEST_METHOD(testSavesModel)
        {
            InsurancePercentageFilters filters = {
                2025,
                InsuranceType::RENTENVERSICHERUNG};
            auto resultBefore = this->hib->loadMany(filters);
            Assert::AreEqual(static_cast<size_t>(0), resultBefore.size());

            InsurancePercentage newItem = {
                InsuranceType::RENTENVERSICHERUNG,
                0.1234,
                0.4321,
                2025,
            };
            this->hib->save(newItem);

            auto resultAfter = this->hib->loadMany(filters);
            Assert::AreEqual(static_cast<size_t>(1), resultAfter.size());

            const auto &item = resultAfter[0];
            Assert::AreEqual(
                static_cast<int>(InsuranceType::RENTENVERSICHERUNG),
                static_cast<int>(item.type));
            Assert::AreEqual(0.1234, item.employerShare);
            Assert::AreEqual(0.4321, item.employeeShare);
            Assert::AreEqual(2025, item.year);
        }

        TEST_METHOD(testUpdatesModel)
        {
            InsurancePercentageFilters filters = {
                2021,
                InsuranceType::RENTENVERSICHERUNG};

            auto resultBefore = this->hib->loadMany(filters);
            Assert::AreEqual(static_cast<size_t>(1), resultBefore.size());

            auto &itemBefore = resultBefore[0];
            Assert::AreEqual(0.27, itemBefore.employerShare);
            Assert::AreEqual(0.253, itemBefore.employeeShare);

            itemBefore.employerShare = 0.254;
            this->hib->save(itemBefore);

            auto resultAfter = this->hib->loadMany(filters);
            Assert::AreEqual(static_cast<size_t>(1), resultAfter.size());

            auto &itemAfter = resultAfter[0];
            Assert::AreEqual(0.254, itemAfter.employerShare);
            Assert::AreEqual(0.253, itemAfter.employeeShare);
        }

        TEST_METHOD(testCannotDeliverSingleItemIfThereAreMore)
        {
            InsurancePercentageFilters filters = {2021, std::nullopt};

            auto raisedException = false;
            try {
                this->hib->loadOne(filters);
            }
            catch (const MultipleObjectsReturnedException &ex) {
                Assert::AreEqual("multiple objects returned: 5", ex.what());
                raisedException = true;
            }
            Assert::IsTrue(raisedException);
        }

        TEST_METHOD(testLoadsSingleModel)
        {
            InsurancePercentageFilters filters = {
                2021,
                InsuranceType::RENTENVERSICHERUNG};
            auto item = this->hib->loadOne(filters);

            Assert::AreEqual(
                static_cast<int>(InsuranceType::RENTENVERSICHERUNG),
                static_cast<int>(item.type));
            Assert::AreEqual(0.27, item.employerShare);
            Assert::AreEqual(0.253, item.employeeShare);
            Assert::AreEqual(2021, item.year);
        }
    };
}