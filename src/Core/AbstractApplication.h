#ifndef CORE_ABSTRACT_APPLICATION_H_
#define CORE_ABSTRACT_APPLICATION_H_

#include <exception>
#include <string>

#include <Windows.h>

enum class FinalizeExceptionMode
{
    MESSAGE_BOX = 0,
    NOTHING = -1,
};

class AbstractApplication
{
public:
    AbstractApplication(
        const HINSTANCE &appInstance,
        const std::wstring &appName,
        const FinalizeExceptionMode finalizeExceptionMode) noexcept;

    HINSTANCE getAppInstance() noexcept;
    HWND getParentWindow() noexcept;
    UINT spawnMessageBox(const std::wstring &text, UINT mode) noexcept;
    int start() noexcept;

protected:
    void finalizeException(const std::exception &exc) noexcept;

    virtual int run() = 0;

private:
    HINSTANCE appInstance;
    std::wstring appName;
    FinalizeExceptionMode finalizeExceptionMode;
    HWND parentWindow;
};

#endif // CORE_ABSTRACT_APPLICATION_H_