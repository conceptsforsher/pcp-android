cmake_minimum_required(VERSION 3.22)
project(pcore_jni)
add_library(pcore_jni SHARED pcore_jni.cpp)
find_library(log-lib log)
target_link_libraries(pcore_jni ${log-lib})
# Phase 2+: link the prebuilt pcp-core artifact here.
