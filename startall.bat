@echo off

set SCRIPT_DIR=%~dp0

REM Start the C++ program in a new command prompt window
start cmd.exe /k "%SCRIPT_DIR%/build/Studienarbeit.exe"

REM Start the first Python script in a new command prompt window
start cmd.exe /k "python.exe src/DataRetrievalAndUi/ui.py"

REM Start the second Python script in a new command prompt window
start cmd.exe /k "python.exe d:/Programmieren/Studienarbeit/src/DataRetrievalAndUi/getAnchorData.py"
