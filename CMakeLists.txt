cmake_minimum_required(VERSION 3.10)
project(3DChessFPS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")

add_subdirectory(external/glfw)
add_subdirectory(external/assimp)

include_directories(
    external/glad/include
    external/glfw/include
    external/glm
    external/stb
    external/assimp/include
    src
)

file(GLOB SRC_FILES
    src/*.cpp
    src/core/*.cpp
    src/game/*.cpp
    src/objects/*.cpp
    src/physics/*.cpp
    src/utils/*.cpp
)

add_executable(3DChessFPS ${SRC_FILES})
target_link_libraries(3DChessFPS glfw assimp "-framework OpenGL")
