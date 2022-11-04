#include "AbstractDialog.h"

AbstractDialog::AbstractDialog(AbstractApplication *app, int dialogId) noexcept
{
    this->app = app;
    this->dialogId = dialogId;
}

void AbstractDialog::show() const noexcept
{
    DialogBoxParam(
        this->app->getAppInstance(),
        MAKEINTRESOURCE(this->dialogId),
        this->app->getParentWindow(),
        [](HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) -> INT_PTR
        {
            switch (uMsg) {
                case WM_INITDIALOG:
                    SetWindowLongPtr(hDlg, GWLP_USERDATA, lParam);
                    break;
                case WM_COMMAND:
                    {
                        auto *dialog = reinterpret_cast<AbstractDialog *>(
                            GetWindowLongPtr(hDlg, GWLP_USERDATA));
                        dialog->onCommand(hDlg, wParam, lParam);
                        break;
                    }
                case WM_CLOSE:
                    {
                        auto *dialog = reinterpret_cast<AbstractDialog *>(
                            GetWindowLongPtr(hDlg, GWLP_USERDATA));
                        dialog->onClose(hDlg);
                        break;
                    }
                default:
                    return false;
            }
            return 0;
        },
        reinterpret_cast<LPARAM>(this));
}