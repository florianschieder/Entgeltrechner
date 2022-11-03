#include <format>
#include <memory>

#include "Registry.h"
#include "Utils.h"

RegistryKey::RegistryKey(HKEY root,
                         const std::wstring &subKey,
                         const RegistryKeyMode mode)
{
    this->root = root;
    this->subKey = subKey;

    constexpr auto accessFlag = KEY_READ | KEY_WRITE;

    if (mode == RegistryKeyMode::Create) {
        auto result = RegCreateKeyEx(root,
                                     subKey.c_str(),
                                     0,
                                     NULL,
                                     0,
                                     accessFlag,
                                     NULL,
                                     &this->handle,
                                     NULL);
        switch (result) {
            case ERROR_SUCCESS:
                break;
            default:
                throw std::exception(
                    std::format("unexpected error: {}", result).c_str());
        }
    }
    else if (mode == RegistryKeyMode::Open) {
        auto result =
            RegOpenKeyEx(root, subKey.c_str(), 0, accessFlag, &this->handle);

        switch (result) {
            case ERROR_SUCCESS:
                break;
            case ERROR_FILE_NOT_FOUND:
                throw RegistryKeyNotFoundException();
            case ERROR_ACCESS_DENIED:
                throw RegistryKeyInaccessibleException();
            default:
                throw std::exception(
                    std::format("unexpected error: {}", result).c_str());
        }
    }
    else {
        throw std::exception("received unrecognized mode");
    }
}

RegistryKey::~RegistryKey()
{
    if (this->handle != nullptr) {
        RegCloseKey(this->handle);
    }
}

void RegistryKey::drop()
{
    auto result = RegDeleteKey(this->root, this->subKey.c_str());
    if (result != ERROR_SUCCESS) {
        throw std::exception(
            std::format("unexpected error: {}", result).c_str());
    }
    this->handle = nullptr;
}

void RegistryKey::dropValue(const std::wstring &key)
{
    auto result = RegDeleteValue(this->handle, key.c_str());
    if (result != ERROR_SUCCESS) {
        throw std::exception(
            std::format("unexpected error: {}", result).c_str());
    }
}

void RegistryKey::saveValue(const std::wstring &key, const long &value)
{
    const auto result = RegSetValueEx(this->handle,
                                      key.c_str(),
                                      0,
                                      REG_DWORD,
                                      (BYTE *) &value, // TODO C26493!
                                      sizeof(int));
    if (result != ERROR_SUCCESS) {
        throw std::exception(
            std::format("unexpected error: {}", result).c_str());
    }
}

void RegistryKey::saveValue(const std::wstring &key, const std::string &value)
{
    const auto result = RegSetValueExA(this->handle,
                                       wideToAnsiString(key).c_str(),
                                       0,
                                       REG_SZ,
                                       (BYTE *) value.c_str(), // TODO C26493
                                       (DWORD) value.size());  // TODO C26493

    if (result != ERROR_SUCCESS) {
        throw std::exception(
            std::format("unexpected error: {}", result).c_str());
    }
}

void RegistryKey::saveValue(const std::wstring &key, const std::wstring &value)
{
    const size_t size = sizeof(wchar_t) * value.size();
    const auto result = RegSetValueEx(this->handle,
                                      key.c_str(),
                                      0,
                                      REG_SZ,
                                      (BYTE *) value.c_str(), // TODO C26493!
                                      static_cast<DWORD>(size));
    if (result != ERROR_SUCCESS) {
        throw std::exception(
            std::format("unexpected error: {}", result).c_str());
    }
}

long RegistryKey::loadValue(const std::wstring &key,
                            const long &defaultValue) const
{
    int value;
    constexpr auto size = sizeof(long);
    const auto result = RegQueryValueEx(this->handle,
                                        key.c_str(),
                                        0,
                                        NULL,
                                        (LPBYTE) &value,  // TODO C26493
                                        (LPDWORD) &size); // TODO C26493
    switch (result) {
        case ERROR_SUCCESS:
            break;
        case ERROR_FILE_NOT_FOUND:
            return defaultValue;
        default:
            throw std::exception(
                std::format("unexpected error: {}", result).c_str());
    }
    return value;
}

std::string RegistryKey::loadValue(const std::wstring &key,
                                   const std::string &defaultValue) const
{
    constexpr auto bufferSize = 2048; // TODO besser machen
    const char buffer[bufferSize]{};
    constexpr auto size = bufferSize * sizeof(char);

    const auto result = RegQueryValueExA(this->handle,
                                         wideToAnsiString(key).c_str(),
                                         0,
                                         NULL,
                                         (LPBYTE) &buffer, // TODO C26493
                                         (LPDWORD) &size); // TODO C26493
    switch (result) {
        case ERROR_SUCCESS:
            break;
        case ERROR_FILE_NOT_FOUND:
            return defaultValue;
        default:
            throw std::exception(
                std::format("unexpected error: {}", result).c_str());
    }
    return std::string(buffer);
}

std::wstring RegistryKey::loadValue(const std::wstring &key,
                                    const std::wstring &defaultValue) const
{
    constexpr auto bufferSize = 2048; // TODO besser machen
    const wchar_t buffer[bufferSize]{};
    constexpr auto size = bufferSize * sizeof(char);

    const auto result = RegQueryValueEx(this->handle,
                                        key.c_str(),
                                        0,
                                        NULL,
                                        (LPBYTE) &buffer, // TODO C26493
                                        (LPDWORD) &size); // TODO C26493
    switch (result) {
        case ERROR_SUCCESS:
            break;
        case ERROR_FILE_NOT_FOUND:
            return defaultValue;
        default:
            throw std::exception(
                std::format("unexpected error: {}", result).c_str());
    }
    return std::wstring(buffer);
}