#pragma once

#include <format>
#include <vector>

#include <sqlite3/sqlite3.h>

#include "Exceptions.h"

template <typename Model, typename Filters>
class AbstractDBHibernation
{
public:
    AbstractDBHibernation(sqlite3 *dbHandle) noexcept
    {
        this->dbHandle = dbHandle;
    }

    Model loadOne(const Filters &filters)
    {
        auto result = this->loadMany(filters);
        if (result.size() != 1) {
            throw MultipleObjectsReturnedException(result.size());
        }
        return result[0];
    }
    virtual std::vector<Model> loadMany(const Filters &filters) = 0;
    virtual void save(const Model &instance) = 0;

protected:
    sqlite3 *dbHandle;
};