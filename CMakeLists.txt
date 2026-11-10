cmake_minimum_required(VERSION 3.22)

project(pcp_android_concept VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network Sql Core)
find_package(Qt6 QUIET COMPONENTS Multimedia)

add_subdirectory(src)
add_subdirectory(pcp)
add_subdirectory(libolm)
add_subdirectory(tests)

target_link_libraries(pcp_android_concept PRIVATE
    Qt6::Widgets
    Qt6::Network
    Qt6::Sql
    Qt6::Core
    pcp_native
)
if(TARGET Qt6::Multimedia)
    target_link_libraries(pcp_android_concept PRIVATE Qt6::Multimedia)
    target_compile_definitions(pcp_android_concept PRIVATE HAS_QT_MULTIMEDIA=1)
endif()

target_include_directories(pcp_android_concept PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/pcp/include
)

if(WIN32)
    set_target_properties(pcp_android_concept PROPERTIES
        WIN32_EXECUTABLE ON
    )
endif()

if(APPLE)
    set_target_properties(pcp_android_concept PROPERTIES
        MACOSX_BUNDLE ON
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.pcp.android.concept"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    )
endif()

install(TARGETS pcp_android_concept
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS pcp_native
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/pcp/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pcp
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
