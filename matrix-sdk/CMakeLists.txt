cmake_minimum_required(VERSION 3.22.0)
project(matrix_sdk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE MATRIX_SDK_SOURCES "src/main/cpp/*.cpp")
file(GLOB_RECURSE MATRIX_SDK_HEADERS "src/main/cpp/*.hpp" "src/main/cpp/*.h")

add_library(matrix_sdk STATIC ${MATRIX_SDK_SOURCES} ${MATRIX_SDK_HEADERS})

target_include_directories(matrix_sdk
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp
)

target_link_libraries(matrix_sdk
    PUBLIC
        olm
        pcp
        nlohmann_json::nlohmann_json
)

if(ANDROID)
    target_link_libraries(matrix_sdk PUBLIC android log)
    target_compile_definitions(matrix_sdk PRIVATE MATRIX_SDK_ANDROID)
endif()

find_package(OpenSSL QUIET)
if(OpenSSL_FOUND)
    target_link_libraries(matrix_sdk PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif()
