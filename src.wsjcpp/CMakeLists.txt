# Automaticly generated by wsjcpp@v0.2.2
cmake_minimum_required(VERSION 3.0)

add_definitions(-DWSJCPP_APP_VERSION="v0.0.1")
add_definitions(-DWSJCPP_APP_NAME="visual-novel-web-maker.git")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

set(CMAKE_CXX_STANDARD 11)

set (WSJCPP_LIBRARIES "")
set (WSJCPP_INCLUDE_DIRS "")
set (WSJCPP_SOURCES "")

find_package(Threads REQUIRED)
list (APPEND WSJCPP_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

# wsjcpp-core:v0.2.3
list (APPEND WSJCPP_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/./src.wsjcpp/wsjcpp_core/")
list (APPEND WSJCPP_SOURCES "${CMAKE_SOURCE_DIR}/./src.wsjcpp/wsjcpp_core/wsjcpp_core.cpp")
list (APPEND WSJCPP_SOURCES "${CMAKE_SOURCE_DIR}/./src.wsjcpp/wsjcpp_core/wsjcpp_core.h")


