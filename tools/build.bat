@echo off
for /F "delims=" %%i in ("%0") do set dirname=%%~dpi

MSBuild.exe -p:Platform=x64 -p:Configuration=Release %dirname%\..\Entgeltrechner.sln
