@echo off
for /F "delims=" %%i in ("%0") do set dirname=%%~dpi

vstest.console.exe %dirname%\..\tests\bin\Release\x64\tests.dll