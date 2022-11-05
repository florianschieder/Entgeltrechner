#pragma once

#include <string>

const std::wstring ansiToWideString(const std::string &ansiStr) noexcept;
const std::string wideToAnsiString(const std::wstring &wideStr);