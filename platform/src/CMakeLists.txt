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
configure_file(config.in config.h)

#########################################################
# Main Executable
#########################################################
add_executable(${PROJECT_NAME} "main.cpp")

include_directories(/usr/local/include)
include_directories(/opt/homebrew/include)

#########################################################
# Common
#########################################################
add_subdirectory(Common)
target_link_libraries(${PROJECT_NAME} PUBLIC Common)

#########################################################
# Logging
#########################################################
add_subdirectory(Logging)
target_link_libraries(${PROJECT_NAME} PUBLIC Logging)

#########################################################
# PythonFunctions
#########################################################
add_subdirectory(PythonFunctions)
target_link_libraries(${PROJECT_NAME} PUBLIC PythonFunctions)

#########################################################
# Strategies
#########################################################
add_subdirectory(Strategies)
target_link_libraries(Strategies PRIVATE PythonFunctions)
target_link_libraries(${PROJECT_NAME} PUBLIC Strategies)

#########################################################
# Dispatcher
#########################################################
add_subdirectory(Dispatcher)
target_link_libraries(Dispatcher PRIVATE PythonFunctions)
target_link_libraries(${PROJECT_NAME} PUBLIC Dispatcher)

#########################################################
# API
#########################################################
add_subdirectory(Api)
target_link_libraries(Api PRIVATE PythonFunctions)
target_link_libraries(Api PRIVATE Dispatcher)
target_link_libraries(${PROJECT_NAME} PUBLIC Api)

#########################################################
# Add the binary tree to the search path for include 
# files so that we will find ${PROJECT_NAME}Config.h
#########################################################
target_include_directories(${PROJECT_NAME} PUBLIC  "${PROJECT_BINARY_DIR}/src")
