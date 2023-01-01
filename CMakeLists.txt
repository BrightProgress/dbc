cmake_minimum_required(VERSION 3.16)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

project(DBC
    VERSION 0.1.0
    LANGUAGES CXX)

include(CTest)

# Testing DBC only enabled in debug configuration
if($<CONFIG:Debug>)
    enable_testing()
endif()

FIND_PACKAGE( Boost COMPONENTS log REQUIRED )

set (CMAKE_CXX_STANDARD 17)

add_library(dbc SHARED dbc.cpp dbc.hpp)
target_link_libraries(dbc ${Boost_LOG_LIBRARY} ${Boost_THREAD_LIBRARY})
target_include_directories(dbc PUBLIC ${Boost_INCLUDE_DIR} ${PROJECT_SOURCE_DIR})
add_subdirectory(unit_test)