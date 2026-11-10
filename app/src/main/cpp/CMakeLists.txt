cmake_minimum_required(VERSION 3.22.1)
project(pcp_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(pcp_app SHARED
    main.cpp
    native_activity.cpp
    ui_renderer.cpp
    ui_input_handler.cpp
    android_bridge.cpp
)

target_include_directories(pcp_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(pcp_app
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
        dialpad
        barcodescanner
        textdrawable
        android
        log
        GLESv3
        EGL
        OpenSLES
        nlohmann_json::nlohmann_json
)
