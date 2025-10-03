cmake_minimum_required(VERSION 3.22)

add_library(
    ${MODULE_NAME}_RCU_INTERFACE
    INTERFACE
)

target_include_directories(
    ${MODULE_NAME}_RCU_INTERFACE
    INTERFACE
    inc
)

add_library(
    ${MODULE_NAME}_RCU
    src/hal_rcu.cpp
)

target_link_libraries(
    ${MODULE_NAME}_RCU
    ${MODULE_NAME}_RCU_INTERFACE
    ${MODULE_NAME}_CORE
    ${MODULE_NAME}_GPIO
)

