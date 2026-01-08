cmake_minimum_required(VERSION 3.16)
project(PacmanSFML LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32 AND NOT DEFINED SFML_DIR)
    # Try to find local SFML installation (optional)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.6.2-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.2/lib/cmake/SFML")
        set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.6.2-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.2/lib/cmake/SFML")
        message(STATUS "Using local Windows SFML installation")
    endif()
endif()

message(STATUS "ROOT = ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "SFML_DIR = ${SFML_DIR}")

add_subdirectory(Pacman_Project/logic)
add_subdirectory(Pacman_Project/representation)