cmake_minimum_required(VERSION 3.22.1)
project(lemmy_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build as a static library for embedding into pcp-android
add_library(lemmy_client STATIC
    src/lemmy_api.cpp
)

target_include_directories(lemmy_client
    PUBLIC
    include
)

# Depends on pcp_native's http_client for transport.
# When used standalone, link against pcp_native or provide your own.
# When used as subdirectory from pcp-android,
# set PCP_NATIVE_TARGET to the correct target name.
if(DEFINED PCP_NATIVE_TARGET)
    target_link_libraries(lemmy_client PUBLIC ${PCP_NATIVE_TARGET})
else()
    # Expect the parent to provide this target
    if(NOT TARGET pcp_native)
        message(WARNING "pcp_native target not found. "
                "Set PCP_NATIVE_TARGET to point to the http_client library.")
    endif()
endif()
