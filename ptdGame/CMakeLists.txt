cmake_minimum_required(VERSION 3.22)

set(project "ptdGame")
project(${project})

set(CMAKE_CXX_STANDARD 17)

set(${project}_SOURCES
        ptdGame.cpp)

set(${project}_HEADERS
        ptdGame.h)

set(${project}_SOURCE_LIST
        ${${project}_SOURCES}
        ${${project}_HEADERS})

add_library(${project}
        STATIC
        ${${project}_SOURCE_LIST})
