@echo off

REM Build the project using CMake
echo Building the project...
cd %SCRIPT_DIR%
if not exist build (
    mkdir build
)
cd build
cmake .. 
REM if compalation doesn't work, try adapting command like following: cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH=C:\mingw64\bin\gcc.exe -DCMAKE_CXX_COMPILER:FILEPATH=C:\mingw64\bin\g++.exe -G "MinGW Makefiles" ..
cmake --build . 
REM same here: cmake --build . --config Debug --target all -j 10

set SCRIPT_DIR=%~dp0

REM Start the C++ program in a new command prompt window
start /min cmd.exe /k "%SCRIPT_DIR%\\build\\Studienarbeit.exe"

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
