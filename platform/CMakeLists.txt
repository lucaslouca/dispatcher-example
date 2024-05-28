cmake_minimum_required(VERSION 3.13)

project(MyApp VERSION 1.0)

#########################################################
# CMake Modules
#########################################################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)

add_subdirectory(src)