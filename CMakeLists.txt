cmake_minimum_required(VERSION 3.10)
project(opengl_test)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED) # Find SDL2

# Add executable
add_executable(opengl_test 
    ${CMAKE_SOURCE_DIR}/main.cpp 
    ${CMAKE_SOURCE_DIR}/Utils.cpp
)

target_include_directories(opengl_test PRIVATE 
    ${CMAKE_SOURCE_DIR}  # This is where your Utils.h is located
    ${SDL2_INCLUDE_DIRS} # Include SDL2 headers
)

# Link libraries
target_link_libraries(opengl_test
    OpenGL::GL
    GLEW::GLEW
    SDL2::SDL2 # Link SDL2
)
