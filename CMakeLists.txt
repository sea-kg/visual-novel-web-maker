cmake_minimum_required(VERSION 3.18)

project(visual-novel-web-maker)

include(${CMAKE_CURRENT_SOURCE_DIR}/src.wsjcpp/CMakeLists.txt)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${visual-novel-web-maker_SOURCE_DIR})

# Sources

# include header dirs
list (APPEND WSJCPP_INCLUDE_DIRS "src")

# http_server
list (APPEND WSJCPP_SOURCES "src/visual_novel_web_maker_http_server.cpp")

# main
list (APPEND WSJCPP_SOURCES "src/main.cpp")

# libhv
set(WITH_OPENSSL OFF CACHE BOOL "")
set(WITH_CURL OFF CACHE BOOL "")
set(BUILD_EXAMPLES OFF CACHE BOOL "")
set(BUILD_SHARED OFF CACHE BOOL "")

add_subdirectory(src/3rdparty/libhv)
list (APPEND WSJCPP_LIBRARIES hv_static)
list (APPEND WSJCPP_INCLUDE_DIRS "${PROJECT_BINARY_DIR}/src/3rdparty/libhv/include/hv")

include_directories(${WSJCPP_INCLUDE_DIRS})

add_executable (visual-novel-web-maker ${WSJCPP_SOURCES})

target_link_libraries(visual-novel-web-maker -lpthread ${WSJCPP_LIBRARIES} )

install(
    TARGETS
        visual-novel-web-maker
)
