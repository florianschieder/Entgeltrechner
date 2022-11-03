#ifndef CORE_UTILS_H_
#define CORE_UTILS_H_

#include <string>

const std::wstring ansiToWideString(const std::string &ansiStr) noexcept(true);
const std::string wideToAnsiString(const std::wstring &wideStr);

#endif // CORE_UTILS_H_