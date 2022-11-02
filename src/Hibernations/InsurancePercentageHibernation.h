#ifndef HIBERNATIONS_INSURANCE_PERCENTAGE_H_
#define HIBERNATIONS_INSURANCE_PERCENTAGE_H_

#include <cassert>
#include <optional>

#include "../Models/InsurancePercentage.h"
#include "AbstractDBHibernation.h"

#include <sqlite3/sqlite3.h>

struct InsurancePercentageFilters
{
    std::optional<int> year;
    std::optional<InsuranceType> type;
};

// TODO boilerplatish sqlite3 code. outsourcing + unit tests
class InsurancePercentageHibernation :
    public AbstractDBHibernation<InsurancePercentage,
                                 InsurancePercentageFilters>
{
public:
    InsurancePercentageHibernation(sqlite3 *dbHandle)
        : AbstractDBHibernation(dbHandle)
    {
        this->createStructure();
    }

    std::vector<InsurancePercentage> loadMany(
        const InsurancePercentageFilters &filters) override
    {
        const auto baseStmt = "\
            SELECT `type`, `employerShare`, `employeeShare`, `year` \
              FROM `insurance_percentage` \
            WHERE {}; \
        ";
        std::string fullStmt;

        if (filters.type.has_value() && filters.year.has_value()) {
            fullStmt =
                std::format(baseStmt,
                            std::format("`type` = {} AND `year` = {}",
                                        static_cast<int>(filters.type.value()),
                                        filters.year.value()));
        }
        else if (filters.type.has_value() && !filters.year.has_value()) {
            fullStmt = std::format(
                baseStmt,
                std::format("`type` = {}",
                            static_cast<int>(filters.type.value())));
        }
        else if (!filters.type.has_value() && filters.year.has_value()) {
            fullStmt =
                std::format(baseStmt,
                            std::format("`year` = {}", filters.year.value()));
        }
        else {
            throw InsufficientFiltersException(
                "requires at least type or year");
        }

        char *errorMessage = nullptr;
        std::vector<InsurancePercentage> resultSet;

        // TODO benchmark SELECT COUNT(*) + allocating vs. one-query +
        // push_back()
        const auto result = sqlite3_exec(
            this->dbHandle,
            fullStmt.c_str(),
            [](void *userData, int argc, char **argv, char **azColName) -> int
            {
                auto resultSet =
                    static_cast<std::vector<InsurancePercentage> *>(userData);
                assert(resultSet != nullptr);
                assert(argv != nullptr);

                InsurancePercentage record = {
                    static_cast<InsuranceType>(std::atoi(argv[0])),
                    std::atof(argv[1]),
                    std::atof(argv[2]),
                    std::atoi(argv[3]),
                };
                resultSet->push_back(record);

                return 0;
            },
            &resultSet,
            &errorMessage);

        if (result != SQLITE_OK) {
            std::string msg(errorMessage);
            sqlite3_free(errorMessage);

            throw std::runtime_error(msg);
        }

        return resultSet;
    }

    void save(const InsurancePercentage &instance) override
    {
        const auto baseStmt = "\
        INSERT INTO `insurance_percentage` \
          (`type`, `employerShare`, `employeeShare`, `year`) \
        VALUES \
          ({}, {}, {}, {}) \
        ON CONFLICT (`type`, `year`) DO UPDATE SET \
          `employerShare` = {}, \
          `employeeShare` = {}; \
        ";
        auto fullStmt = std::format(baseStmt,
                                    static_cast<int>(instance.type),
                                    instance.employerShare,
                                    instance.employeeShare,
                                    instance.year,
                                    instance.employerShare,
                                    instance.employeeShare);

        char *errorMessage = nullptr;
        const auto result = sqlite3_exec(
            this->dbHandle,
            fullStmt.c_str(),
            [](void *, int, char **, char **) -> int { return 0; },
            NULL,
            &errorMessage);

        if (result != SQLITE_OK) {
            std::string msg(errorMessage);
            sqlite3_free(errorMessage);

            throw std::runtime_error(msg);
        }
    }

private:
    void createStructure()
    {
        auto stmt = "\
            CREATE TABLE IF NOT EXISTS `insurance_percentage` ( \
              \
              `id` INTEGER NOT NULL, \
              `type` INTEGER NOT NULL, \
              `employerShare` REAL NOT NULL, \
              `employeeShare` REAL NOT NULL, \
              `year` INTEGER NOT NULL, \
              \
              PRIMARY KEY (`id` AUTOINCREMENT), \
              UNIQUE (`type`, `year`) \
            ); \
        ";

        char *errorMessage = nullptr;
        const auto result = sqlite3_exec(
            this->dbHandle,
            stmt,
            [](void *, int, char **, char **) -> int { return 0; },
            NULL,
            &errorMessage);

        if (result != SQLITE_OK) {
            std::string msg(errorMessage);
            sqlite3_free(errorMessage);

            throw std::runtime_error(msg);
        }
    }
};

#endif // HIBERNATIONS_INSURANCE_PERCENTAGE_H_