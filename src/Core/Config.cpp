#include <format>

#include "Config.h"

Config::Config(const std::wstring &appName, const std::wstring &scope)
{
    this->key = std::make_unique<RegistryKey>(
        HKEY_CURRENT_USER,
        std::format(L"SOFTWARE\\{}\\{}", appName, scope).c_str(),
        RegistryKeyMode::Create);
}

void Config::drop_config_item(const std::wstring &key)
{
    this->key->dropValue(key);
}

bool Config::get_config_item(const std::wstring &key, bool defaultValue)
{
    return (this->key->loadValue(key, defaultValue) == 0 ? false : true);
}

int Config::get_config_item(const std::wstring &key, int defaultValue)
{
    return this->key->loadValue(key, defaultValue);
}

std::string Config::get_config_item(const std::wstring &key,
                                    const std::string &defaultValue)
{
    return this->key->loadValue(key, defaultValue);
}

std::wstring Config::get_config_item(const std::wstring &key,
                                     const std::wstring &defaultValue)
{
    return this->key->loadValue(key, defaultValue);
}

void Config::set_config_item(const std::wstring &key, bool value)
{
    this->key->saveValue(key, value);
}

void Config::set_config_item(const std::wstring &key, int value)
{
    this->key->saveValue(key, value);
}

void Config::set_config_item(const std::wstring &key, const std::string &value)
{
    this->key->saveValue(key, value);
}

void Config::set_config_item(const std::wstring &key, const std::wstring &value)
{
    this->key->saveValue(key, value);
}