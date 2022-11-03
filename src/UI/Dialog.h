#include "../Core/AbstractApplication.h"

class Dialog
{
public:
    Dialog(AbstractApplication *app, int dialogId) noexcept;
    void show() const noexcept;

protected:
    AbstractApplication *app;
    int dialogId;
};