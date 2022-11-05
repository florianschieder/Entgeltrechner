#pragma once

#include <memory>

#include <sqlite3/sqlite3.h>

#include "AbstractApplication.h"

class EntgeltrechnerApplication : public AbstractApplication
{
public:
    EntgeltrechnerApplication(const HINSTANCE &hInstance);
    int run() override;

private:
    sqlite3 *dbHandle;
};