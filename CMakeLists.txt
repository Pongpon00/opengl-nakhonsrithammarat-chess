cmake_minimum_required(VERSION 3.10)
project(3DChessFPS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")

# Dependencies built from source
add_subdirectory(external/glfw)
add_subdirectory(external/assimp)

# Header/include paths
include_directories(
    external/glad/include
    external/glfw/include
    external/glm
    external/stb
    external/assimp/include
    src
)

# Create GLAD as a static library from provided source
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# Find OpenGL in a cross-platform way
find_package(OpenGL REQUIRED)

# Chess logic library (header-only) provided as a git submodule in external/libchess
add_library(libchess_headers INTERFACE)
target_include_directories(libchess_headers INTERFACE external/libchess/include)

file(GLOB SRC_FILES
    src/*.cpp
    src/core/*.cpp
    src/game/*.cpp
    src/objects/*.cpp
    src/physics/*.cpp
    src/utils/*.cpp
)

add_executable(3DChessFPS ${SRC_FILES})
target_link_libraries(3DChessFPS
	glad
	glfw
	assimp
	libchess_headers
	OpenGL::GL
)
