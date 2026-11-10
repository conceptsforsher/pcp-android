cmake_minimum_required(VERSION 3.22.0)
project(pcp_tests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(pcp_tests ${TEST_SOURCES})

target_include_directories(pcp_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/pcp/include
    ${CMAKE_SOURCE_DIR}/matrix-sdk/src/main/cpp
)

target_link_libraries(pcp_tests
    PRIVATE
        pcp
        matrix_sdk
        olm
        attachment_viewer
        multipicker
        autocomplete
        diff_match_patch
        jsonviewer
        span
        nlohmann_json::nlohmann_json
)

if(ANDROID)
    target_link_libraries(pcp_tests PRIVATE android log)
endif()

enable_testing()
add_test(NAME pcp_tests COMMAND pcp_tests)
