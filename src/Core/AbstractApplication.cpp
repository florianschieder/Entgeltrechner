#include <format>

#include <Windows.h>

#include "AbstractApplication.h"
#include "Utils.h"

AbstractApplication::AbstractApplication(
    const HINSTANCE &appInstance,
    const std::wstring &appName,
    const FinalizeExceptionMode finalizeExceptionMode) noexcept
{
#pragma comment(linker, "\"/manifestdependency:type='win32' \
        name='Microsoft.Windows.Common-Controls' \
        version='6.0.0.0' \
        processorArchitecture='*' \
        publicKeyToken='6595b64144ccf1df' \
        language='*'\
        \"")

    SetProcessDPIAware();

    this->appInstance = appInstance;
    this->appName = appName;
    this->finalizeExceptionMode = finalizeExceptionMode;
    this->parentWindow = GetDesktopWindow();
}

int AbstractApplication::start() noexcept
{
    try {
        return this->run();
    }
    catch (const std::exception &exc) {
        this->finalizeException(exc);
        return EXIT_FAILURE;
    }
    catch (...) {
        auto defaultExc = std::exception("unspecified error");
        this->finalizeException(defaultExc);
        return EXIT_FAILURE;
    }
}

void AbstractApplication::finalizeException(const std::exception &exc) noexcept
{
    switch (this->finalizeExceptionMode) {
        case FinalizeExceptionMode::MESSAGE_BOX:
            {
                std::wstring errMsg{};

                try {
                    errMsg = std::format(L"{} crashed: {}",
                                         this->appName,
                                         ansiToWideString(exc.what()));
                }
                catch (...) {
                    errMsg = L"unexpected error";
                }

                this->spawnMessageBox(errMsg, MB_OK | MB_ICONERROR);
                break;
            }
        case FinalizeExceptionMode::NOTHING:
        default:
            break;
    }
}

UINT AbstractApplication::spawnMessageBox(const std::wstring &text,
                                          UINT mode) noexcept
{
    return MessageBox(this->parentWindow,
                      text.c_str(),
                      this->appName.c_str(),
                      mode);
}

HINSTANCE AbstractApplication::getAppInstance() noexcept
{
    return this->appInstance;
}

HWND AbstractApplication::getParentWindow() noexcept
{
    return this->parentWindow;
}