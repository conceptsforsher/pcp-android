cmake_minimum_required(VERSION 3.22)

add_library(pcp_native STATIC pcp_stubs.cpp)

target_include_directories(pcp_native PUBLIC include)

target_compile_features(pcp_native PUBLIC cxx_std_20)

target_link_libraries(pcp_native PUBLIC olm Threads::Threads)
