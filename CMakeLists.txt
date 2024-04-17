cmake_minimum_required(VERSION 3.21)
project(Studienarbeit)

set(CMAKE_CXX_STANDARD 14)


add_executable(Studienarbeit
        #src/Backend/main.cpp
        #src/Backend/socket.cpp
        #src/Backend/socket.h
        src/Backend/calc.cpp
        src/Backend/calc.h
        #src/Backend/display.cpp
        #src/Backend/display.h
        #src/Backend/stringparser.cpp     
        #src/Backend/stringparser.h

        
        #src/Backend/stringparser_test.cpp
        src/Backend/calc_test.cpp
        #src/archive/testSocket/testpixmap/pixmap.cpp
        #src/archive/testSocket/testpixmap/painthelper.h
        #src/archive/testSocket/testpixmap/painthelper.cpp
        
        #src/new/testSocket/withbluetooth/testbluetoothwithsocket.cpp

        

        )





# Find Python
find_package(Python COMPONENTS Development)
#Include Python headers
include_directories(${Python_INCLUDE_DIRS}) 
# Link with Python library
target_link_libraries(Studienarbeit ${Python_LIBRARIES})

target_link_libraries(Studienarbeit ws2_32)