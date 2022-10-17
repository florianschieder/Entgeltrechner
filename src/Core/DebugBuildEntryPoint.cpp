#if defined(_DEBUG)

#include "EntgeltrechnerApplication.h"


int wmain()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    EntgeltrechnerApplication app(hInstance);
    return app.start();
}

#endif