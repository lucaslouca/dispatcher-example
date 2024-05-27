add_library(Dispatcher 
    "dispatcher.h"
    "dispatcher_builder.h"
    "dispatcher_builder.cpp"
    "lf_queue.h"
    "tasks_queue.h"
    "thread_pool.h"
    "thread_pool.cpp"
    )

find_package(Cython REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)


target_include_directories(Dispatcher
    PUBLIC
        $<INSTALL_INTERFACE:${PYTHON_APP_NAME}.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(Dispatcher PUBLIC ${PYTHON_APP_NAME} ${Python3_INCLUDE_DIRS})
target_link_libraries(Dispatcher PRIVATE ${PYTHON_APP_NAME} ${Python3_LIBRARIES})