cmake_minimum_required(VERSION 3.22.1)
project(pcp-chat
    VERSION 0.3.0
    DESCRIPTION "PCP — 100% C++ Android Matrix client"
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── FetchContent — all deps downloaded at build time ──
include(FetchContent)

FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(olm
    GIT_REPOSITORY https://gitlab.matrix.org/matrix-org/olm.git
    GIT_TAG 3.2.16
)
FetchContent_MakeAvailable(olm)

# SQLite3 amalgamation — downloaded at build time
FetchContent_Declare(sqlite3
    URL https://www.sqlite.org/2025/sqlite-amalgamation-3490100.zip
)
FetchContent_GetProperties(sqlite3)
if(NOT sqlite3_POPULATED)
    FetchContent_Populate(sqlite3)
    add_library(sqlite3 STATIC ${sqlite3_SOURCE_DIR}/sqlite3.c)
    target_include_directories(sqlite3 PUBLIC ${sqlite3_SOURCE_DIR})
endif()

# ── Libraries ──
add_subdirectory(matrix-sdk)
add_subdirectory(pcp)

add_subdirectory(library/attachment-viewer)
add_subdirectory(library/multipicker)
add_subdirectory(library/autocomplete)
add_subdirectory(library/diff-match-patch)
add_subdirectory(library/jsonviewer)
add_subdirectory(library/span)
add_subdirectory(library/dialpad)
add_subdirectory(library/barcodescanner)
add_subdirectory(library/textdrawable)

# ── Application shared library (loaded by Java via System.loadLibrary) ──
file(GLOB APP_SOURCES
    app/src/main/cpp/main.cpp
    app/src/main/cpp/native_activity.cpp
    app/src/main/cpp/ui_renderer.cpp
    app/src/main/cpp/ui_input_handler.cpp
    app/src/main/cpp/android_bridge.cpp
)

add_library(pcp_app SHARED ${APP_SOURCES})

target_include_directories(pcp_app PRIVATE app/src/main/cpp)

if(ANDROID)
    # NDK native_app_glue (Android NativeActivity helper)
    add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
    target_include_directories(native_app_glue PUBLIC
        ${ANDROID_NDK}/sources/android/native_app_glue)
    
    target_include_directories(pcp_app PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue)
    target_link_libraries(pcp_app PRIVATE 
        android log GLESv3 EGL OpenSLES
        native_app_glue
-Wl,--whole-archive pcp -Wl,--no-whole-archive
        
        
        matrix_sdk olm sqlite3 attachment_viewer multipicker autocomplete
        diff_match_patch jsonviewer span dialpad barcodescanner textdrawable
        nlohmann_json::nlohmann_json)
    target_compile_definitions(pcp_app PRIVATE PCP_ANDROID)
endif()

# Output the .so to jniLibs so Gradle picks it up automatically
if(ANDROID)
    set_target_properties(pcp_app PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY
        "${CMAKE_SOURCE_DIR}/app/libs/${ANDROID_ABI}"
    )
endif()
