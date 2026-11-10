cmake_minimum_required(VERSION 3.22.0)
project(pcp-android-concept
    VERSION 0.3.0
    DESCRIPTION "PCP — 100% C++ desktop Matrix client"
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Network
    Sql
    Multimedia
    Svg
    LinguistTools
    Concurrent
    Core5Compat
)

# OLM encryption library
add_subdirectory(libolm)

# PCP core SDK library
add_subdirectory(pcp)

# Main application
add_subdirectory(src)

# Tests
option(BUILD_TESTS "Build test suite" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
