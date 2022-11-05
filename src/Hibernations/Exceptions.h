#pragma once

#include <exception>
#include <format>

class MultipleObjectsReturnedException : public std::exception
{
public:
    MultipleObjectsReturnedException(size_t size)
        : std::exception(
              std::format("multiple objects returned: {}", size).c_str())
    {
    }
};

class InsufficientFiltersException : public std::exception
{
public:
    InsufficientFiltersException(const char *msg)
        : std::exception(
              std::format("hibernation received insufficient filters: {}", msg)
                  .c_str())
    {
    }
};