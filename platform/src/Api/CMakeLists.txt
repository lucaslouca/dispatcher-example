add_library(Api 
    "api.h"
    "api.cpp"
    "api_builder.h"
    "api_builder.cpp"
    )

find_package(cpprestsdk REQUIRED)
find_library(CRYPTO_LIB NAMES crypto PATHS /opt/homebrew/lib/ /usr/local/lib/)

target_link_libraries(Api LINK_PUBLIC cpprestsdk::cpprest)
target_link_libraries(Api LINK_PUBLIC ${CRYPTO_LIB}) # Mac OS - Required for cpprest