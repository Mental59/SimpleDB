cmake_minimum_required(VERSION 3.20)

project(SimpleDB C)

if(NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR "This project requires Clang. Current compiler: ${CMAKE_C_COMPILER_ID}")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

file(GLOB_RECURSE SIMPLEDB_C_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

file(GLOB_RECURSE SIMPLEDB_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release)

add_executable(simpledb
    ${SIMPLEDB_C_SOURCES}
    ${SIMPLEDB_HEADERS}
)

target_include_directories(simpledb PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_compile_definitions(simpledb PRIVATE _CRT_SECURE_NO_WARNINGS)

target_compile_features(simpledb PRIVATE c_std_11)

target_compile_options(simpledb PRIVATE
    $<$<CONFIG:Debug>:-O0 -g -Wall -Wextra -Wpedantic>
    $<$<CONFIG:Release>:-O3 -DNDEBUG -Wall -Wextra -Wpedantic>
)
