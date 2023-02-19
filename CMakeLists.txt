cmake_minimum_required(VERSION 3.15)
project(LibLoader)
set(CMAKE_CXX_STANDARD 20)
include(GNUInstallDirs)

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/libloader.hpp>")
target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)
if(APPLE OR UNIX)
    target_link_libraries(${PROJECT_NAME} INTERFACE dl)
endif()

option(LIBLOADER_BUILD_TEST "Build an application to test the library" OFF)
if(LIBLOADER_BUILD_TEST)
    add_library(test_lib SHARED ${CMAKE_CURRENT_SOURCE_DIR}/test/test_lib.cpp ${CMAKE_CURRENT_SOURCE_DIR}/test/test_lib.h)
    add_executable(test ${CMAKE_CURRENT_SOURCE_DIR}/test/test.cpp)
    target_link_libraries(test ${PROJECT_NAME} test_lib) # to build the DLL/DYLIB/SO
endif()
