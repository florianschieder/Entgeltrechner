#include <format>
#include <iostream>

#include "../../assets/resource.h"
#include "../Hibernations/InsurancePercentageHibernation.h"
#include "../UI/StartDialog.h"
#include "Config.h"
#include "EntgeltrechnerApplication.h"
#include "Utils.h"

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
    // TODO: doppelt zu SettingsDialog, auflösen
    Config cfg(L"Entgeltrechner", L"Persistence");
    auto dbPath =
        cfg.get_config_item(std::wstring(L"dbPath"),
                            std::wstring(L"C:\\Temp\\Entgeltrechner.db"));
    const auto result =
        sqlite3_open(wideToAnsiString(dbPath).c_str(), &this->dbHandle);

    if (result != SQLITE_OK) {
        auto exc =
            CouldNotOpenSQLiteDatabaseException(sqlite3_errmsg(this->dbHandle));
        this->finalizeException(exc);
    }
}

int EntgeltrechnerApplication::run()
{
    const StartDialog dlg(this);
    dlg.show();

    const auto hibernation = InsurancePercentageHibernation(this->dbHandle);
    return EXIT_SUCCESS;
}