cmake_minimum_required(VERSION 3.21)

project(Studienarbeit)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(external/json)

add_executable(Studienarbeit
        src/Backend/main.cpp
        src/Backend/socket.cpp
        src/Backend/socket.h
        src/Backend/calc.cpp
        src/Backend/calc.h
        src/Backend/display.cpp
        src/Backend/display.h
        src/Backend/stringparser.cpp     
        src/Backend/stringparser.h
        src/Backend/painthelper.cpp
        src/Backend/painthelper.h
        src/Backend/kalman.cpp
        src/Backend/kalman.h


        #src/archive/test/nDimDist/nDimDist.cpp
        #src/Backend/main.cpp
        #src/Backend/stringparser_test.cpp
        #src/Backend/calc_test.cpp
       # src/archive/test/testpixmap/pixmap.cpp
       # src/archive/test/cppUI/main.cpp
        
        

        )
        
        target_link_libraries(Studienarbeit PRIVATE ws2_32 nlohmann_json::nlohmann_json)

        #target_link_libraries(Studienarbeit ws2_32)


        


        


