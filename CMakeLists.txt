cmake_minimum_required(VERSION 3.16)
project(proc)

set(CMAKE_CXX_STANDARD 17)

# Path to SFML
set(SFML_DIR "/opt/homebrew/Cellar/sfml/2.6.1/lib/cmake/SFML")

# Set directory where you want to build code
set(WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/csene-3d")

# Specify the source file directly
set(SOURCE_FILES "${WORKING_DIRECTORY}/main.cpp")

# Find packages
find_package(OpenGL REQUIRED)
find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)
find_package(GLUT REQUIRED)

# Create executable
add_executable(proc ${SOURCE_FILES})

# Link libraries
target_link_libraries(proc sfml-graphics sfml-window sfml-system OpenGL::GL GLUT::GLUT)
