@echo off

set PROJ_NAME=Blinky
set PROJ_PATH=%~dp0
rem echo %PROJ_PATH%

d:\Keil\ARM\BIN40\fromelf.exe --bin "%PROJ_PATH%\%PROJ_NAME%.axf" -o "%PROJ_PATH%\%PROJ_NAME%.bin"

cp "%PROJ_PATH%\%PROJ_NAME%.bin" "p:\%PROJ_NAME%.bin"
