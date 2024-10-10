cmake_minimum_required(VERSION 3.16)
project(proc)

set(CMAKE_CXX_STANDARD 17)

# Path to SFML
set(SFML_DIR "/opt/homebrew/Cellar/sfml/2.6.1/lib/cmake/SFML")

# set directory where you want to build code
set(WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/hexagon_movement")

# Specify the source file directly
set(SOURCE_FILES "${WORKING_DIRECTORY}/main.cpp")

find_package(OpenGL REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

add_executable(proc ${SOURCE_FILES})

target_link_libraries(proc sfml-system sfml-window sfml-graphics OpenGL::GL)
