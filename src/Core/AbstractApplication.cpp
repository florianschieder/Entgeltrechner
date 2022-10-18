#include <format>

#include <Windows.h>

#include "AbstractApplication.h"
#include "Utils.h"


AbstractApplication::AbstractApplication(
    const HINSTANCE& appInstance,
    const std::wstring& appName,
    const FinalizeExceptionMode finalizeExceptionMode
) noexcept(true)
{
    #pragma comment( \
        linker, \
        "\"/manifestdependency:type='win32' \
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

int AbstractApplication::start() noexcept(true)
{
    try
    {
        return this->run();
    }
    catch (const std::exception& exc)
    {
        this->finalizeException(exc);
        return EXIT_FAILURE;
    }
    catch (...)
    {
        auto defaultExc = std::exception("unspecified error");
        this->finalizeException(defaultExc);
        return EXIT_FAILURE;
    }
}

void AbstractApplication::finalizeException(const std::exception& exc)
    noexcept(true)
{
    switch (this->finalizeExceptionMode)
    {
        case FinalizeExceptionMode::MESSAGE_BOX:
            this->spawnMessageBox(
                std::format(
                    L"{} crashed: {}",
                    this->appName,
                    ansiToWideString(exc.what())
                ).c_str(),
                MB_OK | MB_ICONERROR);
            break;

        case FinalizeExceptionMode::NOTHING:
        default:
            break;
    }
}

UINT AbstractApplication::spawnMessageBox(const std::wstring& text, UINT mode)
{
    return MessageBox(this->parentWindow,
                      text.c_str(),
                      this->appName.c_str(),
                      mode);
}