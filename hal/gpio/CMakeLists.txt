cmake_minimum_required(VERSION 3.22)

add_library(
    ${MODULE_NAME}_GPIO_INTERFACE
    INTERFACE
)

target_include_directories(
    ${MODULE_NAME}_GPIO_INTERFACE
    INTERFACE
    inc
)

add_library(
    ${MODULE_NAME}_GPIO
    src/hal_gpio.cpp
)

target_link_libraries(
    ${MODULE_NAME}_GPIO
    ${MODULE_NAME}_GPIO_INTERFACE
    ${MODULE_NAME}_CORE
)

