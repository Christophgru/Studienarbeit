@echo off

REM Build the project using CMake
echo Building the project...
cd %SCRIPT_DIR%
if not exist build (
    mkdir build
)
cd build
cmake ..
cmake --build .

set SCRIPT_DIR=%~dp0

REM Start the C++ program in a new command prompt window
start /min cmd.exe /k "%SCRIPT_DIR%/build/Studienarbeit.exe"

REM Start the first Python script in a new command prompt window
start /min cmd.exe /k "python.exe %SCRIPT_DIR%/src/DataRetrievalAndUi/ui.py"

REM Start the second Python script in a new command prompt window
start /min cmd.exe /k "python.exe %SCRIPT_DIR%/src/DataRetrievalAndUi/getAnchorData.py"

REM Keep the main window open and wait for user input to terminate
echo Press Ctrl+C to terminate all processes
:loop
pause >nul
goto loop

REM Insert Function to terminate all processes here, we couldnt figure one out

exit /b 0
