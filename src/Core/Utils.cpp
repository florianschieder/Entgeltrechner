#include <codecvt>

#include "Utils.h"

const std::wstring ansiToWideString(const std::string &ansiStr) noexcept
{
    auto wideStr = std::wstring{};
    wideStr.assign(ansiStr.begin(), ansiStr.end());
    return wideStr;
}

const std::string wideToAnsiString(const std::wstring &wideStr)
{
    return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(
        wideStr);
}