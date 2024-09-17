cmake_minimum_required(VERSION 3.22 VERSION 0.1.0)

project(Opengl2dGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(STANDARD_CXX_VERSION_FLAG "-std=c++20")
set(OPTIMIZE_FLAG "-O3")
set(WARNING_FLAGS "-Werror -Wpedantic -Wall")
set(DEBUG_FLAGS "-g -ggdb")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STANDARD_CXX_VERSION_FLAG} ${OPTIMIZE_FLAG} ${WARNING_FLAGS} ${DEBUG_FLAGS}")

file(COPY music DESTINATION ${CMAKE_BINARY_DIR})
file(COPY shader DESTINATION ${CMAKE_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_BINARY_DIR})
file(COPY lint_codebase.sh DESTINATION ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

include_directories(SYSTEM include)

target_link_libraries(${PROJECT_NAME} -lglfw -lGLEW -lGLU -lGL -lSDL2 -lSDL2_mixer)

target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch/opengl_2d_game-pch.hpp)
