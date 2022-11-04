#include "../Core/AbstractApplication.h"

class AbstractDialog
{
public:
    AbstractDialog(AbstractApplication *app, int dialogId) noexcept;
    void show() const noexcept;

    virtual void onCommand(HWND hDlg,
                           WPARAM control,
                           LPARAM lParam) noexcept = 0;
    virtual void onClose(HWND hDlg) noexcept = 0;

protected:
    AbstractApplication *app;
    int dialogId;
};