cmake_minimum_required(VERSION 3.21)
project(Studienarbeit)

set(CMAKE_CXX_STANDARD 14)


add_executable(Studienarbeit
#[[
        src/new/serial.cpp
        src/new/serial.h
        src/new/main.cpp
        src/new/calc.cpp
        src/new/calc.h
        src/new/display.cpp
        src/new/display.h
        ]]
        src/new/test_python.cpp
        )


# Find Python
find_package(Python COMPONENTS Development)
#Include Python headers
include_directories(${Python_INCLUDE_DIRS}) 
# Link with Python library
target_link_libraries(Studienarbeit ${Python_LIBRARIES})