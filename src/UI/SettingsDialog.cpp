#include <UserEnv.h>
#include <format>

#include "../../assets/resource.h"
#include "../Core/Config.h"
#include "../Core/Utils.h"
#include "CommonDialogs.h"
#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(AbstractApplication *app) noexcept
    : AbstractDialog(app, IDD_SETTINGS)
{
}

void SettingsDialog::onInit(HWND hDlg) noexcept
{
    // TODO: C:\Temp\... -> %UserProfile%\Documents\Entgeltrechner.db
    // TODO: duplicate code with EntgeltrechnerApplication
    Config cfg(L"Entgeltrechner", L"Persistence");
    auto dbPath =
        cfg.get_config_item(std::wstring(L"dbPath"),
                            std::wstring(L"C:\\Temp\\Entgeltrechner.db"));
    SetDlgItemText(hDlg, IDC_SETTINGS_DB_PATH, dbPath.c_str());
}

void SettingsDialog::onClose(HWND hDlg) noexcept
{
    EndDialog(hDlg, 0);
}

void SettingsDialog::onCommand(HWND hDlg,
                               WPARAM control,
                               LPARAM lParam) noexcept
{
    switch (control) {
        case IDC_SETTINGS_CHOOSE_DB_PATH:
            {
                // TODO: i18n
                auto dbPath = CommonDialogs::saveFileDialog(
                    hDlg,
                    L"Speicherort für Datenbank auswählen",
                    {{L"Datenbank (*.db)", L"*.db"}});
                // TODO wegabstrahieren
                if (dbPath.has_value()) {
                    SetDlgItemText(hDlg,
                                   IDC_SETTINGS_DB_PATH,
                                   dbPath.value().c_str());
                }
                break;
            }
        case IDOK:
            try {
                this->persistSettings(hDlg);
                EndDialog(hDlg, 0);
            }
            catch (const std::exception &e) {
                // TODO: resolve noexcept problems
                MessageBox(
                    hDlg,
                    std::format(L"Die Einstellungen konnten nicht gespeichert "
                                L"werden: {}",
                                ansiToWideString(e.what()))
                        .c_str(),
                    L"Fehler",
                    MB_OK | MB_ICONERROR);
            }
            break;
        case IDCANCEL:
            EndDialog(hDlg, 0);
            break;
        default:
            break;
    }
}

void SettingsDialog::persistSettings(HWND hDlg)
{
    // TODO: implement tests if appropriate (ATTENTION: GitHub Workflow)
    // TODO: abstract it
    auto dbPathHandle = GetDlgItem(hDlg, IDC_SETTINGS_DB_PATH);
    const auto textLength = GetWindowTextLength(dbPathHandle) + 1;
    auto buffer = new wchar_t[textLength];
    GetWindowText(dbPathHandle, buffer, textLength);

    auto lastError = GetLastError();
    if (lastError != ERROR_SUCCESS) {
        throw std::exception(
            std::format("unexpected error: {}", lastError).c_str());
    }

    // TODO: make Config() a member.
    Config cfg(L"Entgeltrechner", L"Persistence");
    cfg.set_config_item(L"dbPath", std::wstring(buffer));
    delete[] buffer;

    // TODO: Check for valid path or prevent changing the edit box!
    // TODO: Copy the file if the path has changed!
}