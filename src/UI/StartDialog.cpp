#include "StartDialog.h"
#include "../../assets/resource.h"

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
            MessageBox(hDlg,
                       L"Noch nicht viel mehr zu sehen hier!",
                       L"Entgeltrechner",
                       MB_OK | MB_ICONINFORMATION);
            break;
        case IDC_MAIN_WINDOW_EXIT:
            EndDialog(hDlg, 0);
            break;
        default:
            break;
    }
}