#include "StartDialog.h"
#include "../../assets/resource.h"
#include "SettingsDialog.h"

StartDialog::StartDialog(AbstractApplication *app) noexcept
    : AbstractDialog(app, IDD_MAIN_WINDOW)
{
}

void StartDialog::onClose(HWND hDlg) noexcept
{
    EndDialog(hDlg, 0);
}

void StartDialog::onCommand(HWND hDlg, WPARAM control, LPARAM lParam) noexcept
{
    switch (control) {
        case IDC_MAIN_WINDOW_SETTINGS:
            {
                const SettingsDialog dlg(this->app);
                dlg.show();
                break;
            }
        case IDC_MAIN_WINDOW_EXIT:
            EndDialog(hDlg, 0);
            break;
        default:
            break;
    }
}