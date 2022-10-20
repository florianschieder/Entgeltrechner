#include <format>
#include <iostream>

#include "../Hibernations/InsurancePercentageHibernation.h"
#include "EntgeltrechnerApplication.h"

// TODO irgendwo anders hin damit, wenn die SQLite-Aufrufe vernünftig
// abstrahiert wurden
class CouldNotOpenSQLiteDatabaseException : public std::exception
{
public:
    CouldNotOpenSQLiteDatabaseException(const char *reason)
        : std::exception(
              std::format("could not open SQLite database - {}", reason)
                  .c_str())
    {
    }
};

EntgeltrechnerApplication::EntgeltrechnerApplication(const HINSTANCE &hInstance)
    : AbstractApplication(hInstance,
                          L"Entgeltrechner",
                          FinalizeExceptionMode::MESSAGE_BOX)
{
    // TODO: Datenspeicherort konfigurierbar machen
    const auto result = sqlite3_open("foo.db", &this->dbHandle);

    if (result != SQLITE_OK) {
        auto exc =
            CouldNotOpenSQLiteDatabaseException(sqlite3_errmsg(this->dbHandle));
        this->finalizeException(exc);
    }
}

int EntgeltrechnerApplication::run()
{
    const auto hibernation = InsurancePercentageHibernation(this->dbHandle);
    return EXIT_SUCCESS;
}