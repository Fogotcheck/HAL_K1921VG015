cmake_minimum_required(VERSION 3.22)

add_library(
    ${MODULE_NAME}_PERIPH_BASE_INTERFACE
    INTERFACE
)

target_include_directories(
    ${MODULE_NAME}_PERIPH_BASE_INTERFACE
    INTERFACE
    base
)

add_subdirectory(rcu)
add_subdirectory(gpio)
add_subdirectory(communications)
add_subdirectory(plic)

