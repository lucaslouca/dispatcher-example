add_library(Api 
    "api.h"
    "api.cpp"
    "api_builder.h"
    "api_builder.cpp"
    )


find_package(Boost REQUIRED COMPONENTS system thread log program_options chrono)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(cpprestsdk REQUIRED)
find_library(CRYPTO_LIB NAMES crypto PATHS /opt/homebrew/lib/ /usr/local/lib/)
set(OPENSSL_VERSION "1.0.1")

target_include_directories(Api PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(Api PUBLIC ${OPENSSL_INCLUDE_DIR})

target_link_libraries(Api LINK_PUBLIC ${CRYPTO_LIB}) # Mac OS - Required for cpprest
target_link_libraries(Api LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(Api LINK_PUBLIC ${OPENSSL_LIBRARIES})
target_link_libraries(Api LINK_PUBLIC cpprestsdk::cpprest)