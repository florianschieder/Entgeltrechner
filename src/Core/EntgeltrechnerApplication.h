#ifndef CORE_ENTGELTRECHNER_APPLICATION_H
#define CORE_ENTGELTRECHNER_APPLICATION_H

#include <memory>

#include "AbstractApplication.h"

#include "../../3rdparty/sqlite3/sqlite3.h"


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