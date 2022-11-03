#ifndef CORE_CONFIG_H_
#define CORE_CONFIG_H_

#include <memory>
#include <string>

#include "Registry.h"

class Config
{
public:
    Config(const std::wstring &appName, const std::wstring &scope);

    void drop_config_item(const std::wstring &key);

    bool get_config_item(const std::wstring &key, bool defaultValue);
    int get_config_item(const std::wstring &key, int defaultValue);
    std::string get_config_item(const std::wstring &key,
                                const std::string &defaultValue);
    std::wstring get_config_item(const std::wstring &key,
                                 const std::wstring &defaultValue);

    void set_config_item(const std::wstring &key, bool value);
    void set_config_item(const std::wstring &key, int value);
    void set_config_item(const std::wstring &key, const std::string &value);
    void set_config_item(const std::wstring &key, const std::wstring &value);

private:
    std::unique_ptr<RegistryKey> key;
};

#endif // CORE_CONFIG_H_