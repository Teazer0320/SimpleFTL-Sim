cmake_minimum_required(VERSION 3.10)

project(SimpleFTL_Simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(FTL_Sim ${SOURCES})

if(MSVC)
    target_compile_options(FTL_Sim PRIVATE /W4)
else()
    target_compile_options(FTL_Sim PRIVATE -Wall -Wextra -Wpedantic)
endif()