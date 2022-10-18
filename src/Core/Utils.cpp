#include "Utils.h"


const std::wstring ansiToWideString(const std::string& ansiStr) noexcept(true)
{
    auto wideStr = std::wstring{};
    wideStr.assign(
        ansiStr.begin(), ansiStr.end()
    );
    return wideStr;
}