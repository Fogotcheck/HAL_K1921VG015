cmake_minimum_required(VERSION 3.22)

add_library(
    ${MODULE_NAME}_UART_INTERFACE
    INTERFACE
)

target_include_directories(
    ${MODULE_NAME}_UART_INTERFACE
    INTERFACE
    inc
)

add_library(
    ${MODULE_NAME}_UART
    src/hal_uart.cpp
)

target_link_libraries(
    ${MODULE_NAME}_UART
    ${MODULE_NAME}_UART_INTERFACE
    ${MODULE_NAME}_CORE
    ${MODULE_NAME}_GPIO
    ${MODULE_NAME}_PERIPH_BASE_INTERFACE
)

