cmake_minimum_required(VERSION 3.21)

project(Studienarbeit)

set(CMAKE_CXX_STANDARD 14)

add_executable(Studienarbeit
        #src/Backend/main.cpp
        src/Backend/socket.cpp
        src/Backend/socket.h
        src/Backend/calc.cpp
        src/Backend/calc.h
        src/Backend/display.cpp
        src/Backend/display.h
        src/Backend/stringparser.cpp     
        src/Backend/stringparser.h

        #src/Backend/main.cpp
        #src/Backend/stringparser_test.cpp
        #src/Backend/calc_test.cpp
        #src/archive/test/testpixmap/pixmap.cpp
        src/archive/test/cppUI/main.cpp
        
        

        )
        set(SFML_ROOT D:/Programs/SFML-2.6.1-mingw)
        find_package(SFML 2.6.1 COMPONENTS system window graphics REQUIRED)



        if (SFML_FOUND)
                target_link_libraries(Studienarbeit ws2_32 sfml-system sfml-window sfml-graphics )
        endif()


        


        


