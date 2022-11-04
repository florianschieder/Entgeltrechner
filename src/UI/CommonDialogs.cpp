#include "CommonDialogs.h"

std::optional<std::wstring> CommonDialogs::saveFileDialog(
    HWND parent,
    const std::wstring &title,
    const std::list<std::pair<std::wstring, std::wstring>> &filters) noexcept
{
    auto processedFilter = std::wstring(L"");
    for (auto &filter : filters) {
        processedFilter += filter.first;
        processedFilter += L'\0';
        processedFilter += filter.second;
        processedFilter += L'\0';
    }

    OPENFILENAME data;
    wchar_t szFileName[MAX_PATH] = L"";

    ZeroMemory(&data, sizeof(data));

    data.lStructSize = sizeof(data);
    data.hwndOwner = parent;
    data.lpstrFilter = processedFilter.c_str();
    data.lpstrFile = szFileName;
    data.nMaxFile = MAX_PATH;
    data.lpstrDefExt = filters.front().second.c_str();
    data.lpstrTitle = title.c_str();
    data.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |
                 OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&data)) {
        return data.lpstrFile;
    }
    else {
        return std::nullopt;
    }
}