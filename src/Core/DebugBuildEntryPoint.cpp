#include "EntgeltrechnerApplication.h"

#if defined(_DEBUG)

int wmain()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    EntgeltrechnerApplication app(hInstance);
    return app.start();
}

#endif