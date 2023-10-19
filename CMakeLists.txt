cmake_minimum_required(VERSION 3.21)
project(Studienarbeit)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/new)

add_executable(Studienarbeit
        src/new/serial.cpp
        src/new/serial.h)
