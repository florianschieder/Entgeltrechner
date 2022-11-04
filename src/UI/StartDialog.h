#include "AbstractDialog.h"

class StartDialog : public AbstractDialog
{
public:
    StartDialog(AbstractApplication *app) noexcept;

    void onInit(HWND hDlg) noexcept override
    {
    }
    void onCommand(HWND hDlg, WPARAM control, LPARAM lParam) noexcept override;
    void onClose(HWND hDlg) noexcept override;
};