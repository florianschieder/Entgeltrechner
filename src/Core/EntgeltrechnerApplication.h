#ifndef CORE_ENTGELTRECHNER_APPLICATION_H
#define CORE_ENTGELTRECHNER_APPLICATION_H

#include <memory>

#include <sqlite3/sqlite3.h>

#include "AbstractApplication.h"


class EntgeltrechnerApplication
    : public AbstractApplication
{
public:
    EntgeltrechnerApplication(const HINSTANCE& hInstance);
    int run() override;

private:
    sqlite3 *dbHandle;
};

#endif  // CORE_ENTGELTRECHNER_APPLICATION_H