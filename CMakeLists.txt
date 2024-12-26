cmake_minimum_required(VERSION 3.15)
project(libloader)
set(CMAKE_CXX_STANDARD 20)
include(GNUInstallDirs)

add_library(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/libloader.hpp")
target_include_directories(${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
if(APPLE OR UNIX)
    target_link_libraries(${PROJECT_NAME} INTERFACE dl)
endif()

option(LIBLOADER_BUILD_TEST "Build an application to test the library" OFF)
if(LIBLOADER_BUILD_TEST)
    add_library(${PROJECT_NAME}_test_lib SHARED ${CMAKE_CURRENT_SOURCE_DIR}/test/test_lib.cpp ${CMAKE_CURRENT_SOURCE_DIR}/test/test_lib.h)
    add_executable(${PROJECT_NAME}_test ${CMAKE_CURRENT_SOURCE_DIR}/test/test.cpp)
    target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} ${PROJECT_NAME}_test_lib) # to build the DLL/DYLIB/SO
endif()
