cmake_minimum_required(VERSION 3.21)
project(Studienarbeit)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/new boost_1_83_0)

add_executable(Studienarbeit
        src/new/serial.cpp
        src/new/serial.h
        src/new/main.cpp
        src/new/calc.cpp
        src/new/calc.h
        src/new/display.cpp
        src/new/display.h)


# Find and include Boost
find_package(boost_1_83_0 REQUIRED)
include_directories(${boost_1_83_0/boost/asio})
# Link with the Winsock library
target_link_libraries(Studienarbeit PRIVATE ${Boost_LIBRARIES} ws2_32)