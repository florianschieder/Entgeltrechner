#ifndef HIBERNATIONS_ABSTRACT_HIBERNATION_H_
#define HIBERNATIONS_ABSTRACT_HIBERNATION_H_

#include <format>
#include <vector>

#include "Exceptions.h"
#include "../../3rdparty/sqlite3/sqlite3.h"


template <typename Model, typename Filters>
class AbstractHibernation
{
public:
    AbstractHibernation(sqlite3 *dbHandle) noexcept(true)
    {
        this->dbHandle = dbHandle;
    }

    Model loadOne(const Filters& filters)
    {
        auto result = this->loadMany(filters);
        if (result.size() != 1)
        {
            throw MultipleObjectsReturnedException(result.size());
        }
        return result[0];
    }
    virtual std::vector<Model> loadMany(const Filters& filters) = 0;
    virtual void save(const Model& instance) = 0;

protected:
    sqlite3 *dbHandle;
};

#endif  // HIBERNATIONS_ABSTRACT_HIBERNATION_H_