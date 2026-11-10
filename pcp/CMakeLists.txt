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
        sqlite3
        nlohmann_json::nlohmann_json
)

if(ANDROID)
    target_link_libraries(pcp PUBLIC android log)
    target_compile_definitions(pcp PRIVATE PCP_ANDROID)
endif()

find_package(OpenSSL QUIET)
if(OpenSSL_FOUND)
    target_link_libraries(pcp PRIVATE OpenSSL::SSL OpenSSL::Crypto)
    target_compile_definitions(pcp PRIVATE PCP_HAS_OPENSSL)
endif()
