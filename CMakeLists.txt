cmake_minimum_required(VERSION 3.10)
project(opengl_test)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Add executable
add_executable(opengl_test main.cpp)

# Link libraries
target_link_libraries(opengl_test
    OpenGL::GL
    glfw
)