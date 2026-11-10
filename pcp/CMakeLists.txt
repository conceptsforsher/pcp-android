cmake_minimum_required(VERSION 3.22.0)
project(pcp_native VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PCP_SOURCES "src/*.cpp")
file(GLOB_RECURSE PCP_HEADERS "include/*.hpp" "include/*.h")

add_library(pcp STATIC ${PCP_SOURCES} ${PCP_HEADERS})

target_include_directories(pcp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(pcp
    PUBLIC
        olm
        Qt6::Core
        Qt6::Network
        Qt6::Sql
        Qt6::Concurrent
)

if(UNIX)
    target_link_libraries(pcp PRIVATE pthread)
endif()

# Platform defines
if(ANDROID)
    target_compile_definitions(pcp PRIVATE PCP_ANDROID)
elseif(APPLE)
    target_compile_definitions(pcp PRIVATE PCP_APPLE)
elseif(WIN32)
    target_compile_definitions(pcp PRIVATE PCP_WINDOWS)
elseif(UNIX)
    target_compile_definitions(pcp PRIVATE PCP_LINUX)
endif()

# Remove JNI bridge on non-Android platforms
if(NOT ANDROID)
    set_property(TARGET pcp PROPERTY JNI_EXCLUDES TRUE)
endif()
