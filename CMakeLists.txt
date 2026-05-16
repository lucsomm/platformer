cmake_minimum_required(VERSION 4.0)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(platformer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

add_executable(platformer src/main.cpp
        src/game.cpp
        src/player.cpp
        src/collision.cpp
        src/tile_map.cpp)

target_include_directories(platformer PRIVATE src)
target_link_libraries(platformer PRIVATE raylib glm)

target_compile_definitions(platformer PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)