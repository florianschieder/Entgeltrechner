#include "EntgeltrechnerApplication.h"

#if !defined(_DEBUG)

int __stdcall wWinMain(_In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_ LPWSTR lpCmdLine,
                       _In_ int nCmdShow)
{
    EntgeltrechnerApplication app(hInstance);
    return app.start();
}

#endif