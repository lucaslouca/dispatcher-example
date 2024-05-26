# https://cmake.org/cmake/help/book/mastering-cmake/cmake/Help/guide/tutorial/index.html#build-and-test

#########################################################
# C++ Settings
#########################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)

#########################################################
# Configure a header file to pass some of 
# the CMake settings to the source code
#########################################################
configure_file(${PROJECT_NAME}Config.in ${PROJECT_NAME}Config.h)

#########################################################
# Main Executable
#########################################################
add_executable(${PROJECT_NAME} "main.cpp")

#########################################################
# Common
#########################################################
add_subdirectory(Common)
target_link_libraries(${PROJECT_NAME} PUBLIC Common)

#########################################################
# PythonFunctions
#########################################################
add_subdirectory(PythonFunctions)
target_link_libraries(${PROJECT_NAME} PUBLIC PythonFunctions)

#########################################################
# Dispatcher
#########################################################
add_subdirectory(Dispatcher)
target_link_libraries(${PROJECT_NAME} PUBLIC Dispatcher)

#########################################################
# Add the binary tree to the search path for include 
# files so that we will find ${PROJECT_NAME}Config.h
#########################################################
target_include_directories(${PROJECT_NAME} PUBLIC  "${PROJECT_BINARY_DIR}/src")
