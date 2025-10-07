cmake_minimum_required(VERSION 3.22)

add_library(
    ${MODULE_NAME}_PLIC_INTERFACE
    INTERFACE
)

target_include_directories(
    ${MODULE_NAME}_PLIC_INTERFACE
    INTERFACE
    inc
)

add_library(
    ${MODULE_NAME}_PLIC
    src/hal_plic.cpp
)

target_link_libraries(
    ${MODULE_NAME}_PLIC
    ${MODULE_NAME}_PLIC_INTERFACE
    ${MODULE_NAME}_CORE
    ${MODULE_NAME}_PERIPH_BASE_INTERFACE
)

