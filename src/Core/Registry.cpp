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

void RegistryKey::saveValue(const std::wstring &key, __int64 value)
{
    const auto result = RegSetValueEx(this->handle,
                                      key.c_str(),
                                      0,
                                      REG_DWORD,
                                      reinterpret_cast<BYTE *>(&value),
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
                                       reinterpret_cast<BYTE *>(
                                           const_cast<char *>(value.c_str())
                                       ),
                                       static_cast<DWORD>(value.size()));

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
                                      reinterpret_cast<BYTE *>(
                                          const_cast<wchar_t *>(value.c_str())
                                      ),
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
    auto size = sizeof(long);
    const auto result = RegQueryValueEx(this->handle,
                                        key.c_str(),
                                        0,
                                        NULL,
                                        reinterpret_cast<LPBYTE>(&value),
                                        reinterpret_cast<LPDWORD>(&size));
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
    char buffer[bufferSize]{};
    auto size = bufferSize * sizeof(char);

    const auto result = RegQueryValueExA(this->handle,
                                         wideToAnsiString(key).c_str(),
                                         0,
                                         NULL,
                                         reinterpret_cast<LPBYTE>(&buffer),
                                         reinterpret_cast<LPDWORD>(&size));
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
    wchar_t buffer[bufferSize]{};
    auto size = bufferSize * sizeof(wchar_t);

    const auto result = RegQueryValueEx(this->handle,
                                        key.c_str(),
                                        0,
                                        NULL,
                                        reinterpret_cast<LPBYTE>(&buffer),
                                        reinterpret_cast<LPDWORD>(&size));
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