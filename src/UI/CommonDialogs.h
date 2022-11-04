#pragma once

#include <list>
#include <optional>
#include <string>

#include <Windows.h>

class CommonDialogs
{
public:
    static std::optional<std::wstring> saveFileDialog(
        HWND parent,
        const std::wstring &title,
        const std::list<std::pair<std::wstring, std::wstring>>
            &filters) noexcept;
};