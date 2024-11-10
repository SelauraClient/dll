cmake_minimum_required(VERSION 3.14)
project(Selaura LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

if(MSVC)
    add_compile_options(/await)
endif()

set(DLL_NAME Selaura)

file(GLOB_RECURSE SOURCE_FILES
    "*.c"
    "*.cpp"
    "*.h"
    "*.hpp"
)

add_library(${DLL_NAME} SHARED ${SOURCE_FILES})

set_target_properties(${DLL_NAME} PROPERTIES
    OUTPUT_NAME "Selaura"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
