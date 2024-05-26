set(PYTHON_APP_NAME "Playground")

find_package(Cython REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

file(COPY ${PYTHON_APP_NAME}.pyx DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(
    OUTPUT ${PYTHON_APP_NAME}.c ${PYTHON_APP_NAME}.h 
    COMMAND ${CYTHON_EXECUTABLE} ${PYTHON_APP_NAME}.pyx
    DEPENDS ${PYTHON_APP_NAME}.pyx)

set(SOURCES ${PYTHON_APP_NAME}.c)
Python3_add_library(${PYTHON_APP_NAME} STATIC ${SOURCES})

add_library(PythonFunctions ${PYTHON_APP_NAME}.c)

target_include_directories(PythonFunctions
    PUBLIC
        $<INSTALL_INTERFACE:${PYTHON_APP_NAME}.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(PythonFunctions PUBLIC ${PYTHON_APP_NAME} ${Python3_INCLUDE_DIRS})
target_link_libraries(PythonFunctions PRIVATE ${PYTHON_APP_NAME} ${Python3_LIBRARIES})