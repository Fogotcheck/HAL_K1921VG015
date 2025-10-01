cmake_minimum_required(VERSION 3.22)

set(MCU_LINKER_SCRIPT
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts/k1921vg015_flash.ld"
    CACHE STRING "" FORCE
)

add_library(
    ${MODULE_NAME}_CORE_INTERFACE
    INTERFACE
)

target_include_directories(
    ${MODULE_NAME}_CORE_INTERFACE
    INTERFACE
    inc
)

add_library(
    ${MODULE_NAME}_CORE
    start/startup_k1921vg015.S
    src/sys_init.c
)

target_link_libraries(
    ${MODULE_NAME}_CORE
    ${MODULE_NAME}_CORE_INTERFACE
)

