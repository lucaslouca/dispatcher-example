add_library(Logging 
    "factory.cpp"
    "file_logger.cpp"
    "logging.h"
    "log_processor.cpp"
    "spd_logger.cpp"
    "stdout_logger.cpp"
    "safe_queue.h"
    )


find_package(spdlog REQUIRED)
target_link_libraries(Logging LINK_PUBLIC spdlog::spdlog) 