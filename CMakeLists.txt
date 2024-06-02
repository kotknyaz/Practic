cmake_minimum_required(VERSION 3.22)

set(project "Practic")
project(${project})

set(CMAKE_CXX_STANDARD 17)

set(${project}_SOURCES main.cpp)

add_subdirectory(ptdGame)

set(${project}_SOURCE_LIST ${${project}_SOURCES})

add_executable(${project} ${${project}_SOURCE_LIST})


target_link_libraries(${project} ptdGame)


target_link_libraries(${project} ${CMAKE_SOURCE_DIR}/SFML/library/sfml-audio.lib)
target_link_libraries(${project} ${CMAKE_SOURCE_DIR}/SFML/library/sfml-graphics.lib)
target_link_libraries(${project} ${CMAKE_SOURCE_DIR}/SFML/library/sfml-network.lib)
target_link_libraries(${project} ${CMAKE_SOURCE_DIR}/SFML/library/sfml-system.lib)
target_link_libraries(${project} ${CMAKE_SOURCE_DIR}/SFML/library/sfml-window.lib)

include_directories(${CMAKE_SOURCE_DIR}/SFML/include)
target_include_directories(${project} PUBLIC ptdGame)