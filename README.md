# Entgeltrechner

Der "Entgeltrechner" ist eine Desktopanwendung, die dafür gedacht ist, eine Entgeltabrechnung für Angestellte eines Unternehmens zu erstellen.

Das Programm ist **nicht** als professionelle Software zu sehen und einzusetzen!

## Projektinitiierung

Das Projekt wurde von Schülern der [Staatl. Berufsschule Wiesau](https://www.bsz-wiesau.de) im Zuge der IHK-Ausbildung zum Fachinformatiker (insbesondere Fachrichtung Anwendungsentwicklung) gestartet.

Für genau dieses Projekt wurde sich entschieden, da es Bezug zu ausbildungsrelevanten Fachgebieten (der Entgeltabrechnung in Sozialkunde) hat und für Übungszwecke die folgenden IT-Fachgebiete und Technologien werden durch dieses Projekt abgedeckt:
- GUI-Design
- Datenbanken (SQLite)
- Versionierung (git)
- Anwendungsprogrammierung (C++)
- Kollaboration
- Qualitätsmanagement (Code Reviews, Static Analysis, Unit Tests, ...)

## Entwicklerhinweise

### Benötigte Software zum Entwickeln

- LLVM: https://llvm.org/builds/, aktuell Version 12.x.x
- Python: https://www.python.org/downloads/, aktuell Version 3.10.x
- Visual Studio: https://visualstudio.microsoft.com/de/vs/community/, derzeit noch Visual Studio 2019
- Wir verwenden für die Versionierung [GitHub](https://github.com/) und clientseitig [Git for Windows](https://gitforwindows.org/) bzw. die Git-Integration in Visual Studio.

Wir setzen C/C++ für das Programm an sich und Python für Tools ein.

### Drittanbieter-Code

In der Praxis wird es passieren, dass man z. B. auf GitHub ein Repository findet, welches ein Problem löst, das sonst mit sehr viel Mühe und Zeitaufwand implementiert werden müsste. Oder dass Standardsoftware/-bibliotheken wie `sqlite` mit integriert werden muss / müssen.

Es gibt drei Möglichkeiten, wie die Abhängigkeiten zu verwalten sind:

- Direkt ins Repo integrieren, wie bspw. bei der `sqlite`-Amalgamation (siehe [hier](https://www.sqlite.org/amalgamation.html)).
- Integration per [git submodule](https://git-scm.com/book/de/v2/Git-Tools-Submodule).
- Installation per [NuGet](https://www.nuget.org/) (= Verwaltung per Visual Studio).

In allen Fällen ist das Vorhandensein einer Abhängigkeit und die Art wie sie installiert wurde, in der `UPGRADE.md` zu dokumentieren.

Bei den aufgeführten Methoden handelt es sich lediglich um die häufigst verwendeten und vermutlich sinnvollsten.

### Ausführen der Tests, Projektorganisation

- Das Projekt wird über GitHub Projects verwaltet: [Board für V1-Implementierung](https://github.com/users/florianschieder/projects/1/views/1?layout=board)
- Um die Tests auszuführen, sind folgende Schritte notwendig:
  - `tools/build.bat` ausführen
  - `tools/run-tests.bat` ausführen
  - Damit alle Visual Studio-Tools sich im Systempfad befinden, ist es ratsam, eine "Developer Command Prompt" zu starten. Diese findet sich in der Regel im Startmenü unter "Visual Studio 2019".