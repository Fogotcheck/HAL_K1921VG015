cmake_minimum_required(VERSION 3.22)

set(MODULE_NAME ${PROJECT_NAME}_K1921VG015_HAL)

add_library(${MODULE_NAME}_INTERFACE INTERFACE)

add_subdirectory(core)
add_subdirectory(gpio)
add_subdirectory(rcu)

target_link_libraries(
    ${MODULE_NAME}_INTERFACE 
    INTERFACE
    ${MODULE_NAME}_CORE
    ${MODULE_NAME}_RCU
    ${MODULE_NAME}_GPIO
)