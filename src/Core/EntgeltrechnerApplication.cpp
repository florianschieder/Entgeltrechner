#include <format>
#include <iostream>

#include "EntgeltrechnerApplication.h"
#include "../Hibernations/InsurancePercentageHibernation.h"


// TODO irgendwo anders hin damit
class CouldNotOpenSQLiteDatabaseException : public std::exception
{
public:
    CouldNotOpenSQLiteDatabaseException(const char* reason) :
        std::exception(std::format("could not open SQLite database - {}",
            reason)
            .c_str())
    {}
};


EntgeltrechnerApplication::EntgeltrechnerApplication(
    const HINSTANCE& hInstance
)
    : AbstractApplication(
        hInstance,
        L"Entgeltrechner",
        FinalizeExceptionMode::MESSAGE_BOX
    )
{
    const auto result = sqlite3_open("foo.db", &this->dbHandle);

    if (result != SQLITE_OK) {
        this->finalizeException(
            CouldNotOpenSQLiteDatabaseException(
                sqlite3_errmsg(this->dbHandle)
            )
        );
    }
}


int EntgeltrechnerApplication::run()
{
    const auto hibernation = InsurancePercentageHibernation(this->dbHandle);
    return EXIT_SUCCESS;
}