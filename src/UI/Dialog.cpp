#include "Dialog.h"
#include "../../assets/resource.h"

Dialog::Dialog(AbstractApplication *app, int dialogId) noexcept
{
    this->app = app;
    this->dialogId = dialogId;
}

void Dialog::show() const noexcept
{
    DialogBox(
        this->app->getAppInstance(),
        MAKEINTRESOURCE(this->dialogId),
        this->app->getParentWindow(),
        [](HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) -> INT_PTR
        {
            switch (uMsg) {
                case WM_COMMAND:
                    {
                        const auto ctrl = wParam;
                        switch (ctrl) {
                            case IDC_MAIN_WINDOW_SETTINGS:
                                MessageBox(
                                    hDlg,
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
                        break;
                    }
                case WM_CLOSE:
                    EndDialog(hDlg, 0);
                    break;
                default:
                    return false;
            }
            return 0;
        });
}