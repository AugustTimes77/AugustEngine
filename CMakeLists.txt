cmake_minimum_required(VERSION 3.10)
project(AugustEngine)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED) # Find SDL2
find_package(soil2 REQUIRED)


# Add executable
add_executable(AugustEngine
    src/main.cpp
    src/Utils.cpp
    src/math/Vec3.cpp
    src/math/Mat4.cpp
)

target_include_directories(AugustEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS} # Include SDL2 headers
)

# Link libraries
target_link_libraries(AugustEngine
    OpenGL::GL
    GLEW::GLEW
    SDL2::SDL2 # Link SDL2
    soil2::soil2 # Link SOIL2
)

# Copy assets directory to build directory
add_custom_command(TARGET AugustEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:AugustEngine>/assets
)
