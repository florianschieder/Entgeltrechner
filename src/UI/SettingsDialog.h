#pragma once

#include "AbstractDialog.h"

class SettingsDialog : public AbstractDialog
{
public:
    SettingsDialog(AbstractApplication *app) noexcept;

    void onInit(HWND hDlg) noexcept override;
    void onCommand(HWND hDlg, WPARAM control, LPARAM lParam) noexcept override;
    void onClose(HWND hDlg) noexcept override;

private:
    void persistSettings(HWND hDlg);
};