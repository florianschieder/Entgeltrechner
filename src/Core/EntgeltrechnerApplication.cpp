#include <iostream>

#include "EntgeltrechnerApplication.h"


EntgeltrechnerApplication::EntgeltrechnerApplication(
    const HINSTANCE& hInstance
)
    : AbstractApplication(
        hInstance,
        L"Entgeltrechner",
        FinalizeExceptionMode::MESSAGE_BOX
    )
{}


int EntgeltrechnerApplication::run()
{
    this->spawnMessageBox(L"Hellu! (:",
                          MB_OK | MB_ICONINFORMATION);
    return EXIT_SUCCESS;
}