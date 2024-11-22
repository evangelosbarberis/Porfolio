cmake_minimum_required(VERSION 3.14)

project(banana_slug)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_BUILD_TYPE "Debug")
include_directories(.)

add_executable(${PROJECT_NAME}
    src/EmuCpu.cpp
    src/EmuMmu.cpp
    src/EmuGpu.cpp
    src/EmuIO.cpp
    src/EmuConsole.cpp
    src/main.cpp
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
message(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
