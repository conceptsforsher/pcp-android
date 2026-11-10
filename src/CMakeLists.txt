cmake_minimum_required(VERSION 3.22.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB_RECURSE APP_SOURCES
    "main.cpp"
    "app/*.cpp"
    "features/*/*.cpp"
    "ui/components/*.cpp"
    "ui/themes/*.cpp"
    "matrix/*.cpp"
    "irc/*.cpp"
    "lemmy/*.cpp"
    "protocol/*.cpp"
    "native/*.cpp"
    "util/*.cpp"
)

file(GLOB_RECURSE APP_HEADERS
    "app/*.hpp"
    "features/*/*.hpp"
    "ui/components/*.hpp"
    "ui/themes/*.hpp"
    "matrix/*.hpp"
    "irc/*.hpp"
    "lemmy/*.hpp"
    "protocol/*.hpp"
    "native/*.hpp"
    "util/*.hpp"
)

add_executable(pcp_chat ${APP_SOURCES} ${APP_HEADERS})

target_include_directories(pcp_chat PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(pcp_chat
    PRIVATE
        pcp
        Qt6::Widgets
        Qt6::Network
        Qt6::Sql
        Qt6::Multimedia
        Qt6::Svg
        Qt6::Concurrent
        Qt6::Core5Compat
)

set_target_properties(pcp_chat PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

install(TARGETS pcp_chat DESTINATION bin)
