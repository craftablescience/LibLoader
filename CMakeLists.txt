cmake_minimum_required(VERSION 3.15)
project(LibLoader)
set(CMAKE_CXX_STANDARD 17)
include(GNUInstallDirs)

add_library(${PROJECT_NAME} INTERFACE)
set(LIBLOADER_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/libloader.hpp)
target_sources(${PROJECT_NAME} INTERFACE "$<BUILD_INTERFACE:${LIBLOADER_HEADER}>")
target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)
