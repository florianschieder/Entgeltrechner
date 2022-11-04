#include <exception>
#include <string>

#include <Windows.h>

enum class RegistryKeyMode
{
    Open = 0,
    Create = 1,
};

class RegistryKeyNotFoundException : public std::exception
{
public:
    RegistryKeyNotFoundException() noexcept
    {
    }
};

class RegistryKeyInaccessibleException : public std::exception
{
public:
    RegistryKeyInaccessibleException() noexcept
    {
    }
};

class RegistryKeyWritingProtectedException : public std::exception
{
public:
    RegistryKeyWritingProtectedException() noexcept
    {
    }
};

class RegistryKey
{
public:
    RegistryKey(HKEY root,
                const std::wstring &subKey,
                const RegistryKeyMode mode);
    ~RegistryKey();

    void drop();
    void dropValue(const std::wstring &key);

    void saveValue(const std::wstring &key, __int64 value);
    void saveValue(const std::wstring &key, const std::string &value);
    void saveValue(const std::wstring &key, const std::wstring &value);

    long loadValue(const std::wstring &key, const long &defaultValue) const;
    std::string loadValue(const std::wstring &key,
                          const std::string &defaultValue) const;
    std::wstring loadValue(const std::wstring &key,
                           const std::wstring &defaultValue) const;

private:
    HKEY handle;
    HKEY root;
    std::wstring subKey;
};