cmake_minimum_required(VERSION 3.22)

set(MODULE_NAME ${PROJECT_NAME}_uart)

add_executable(
    ${MODULE_NAME}
    main.cpp
)

target_link_options(
    ${MODULE_NAME}
    PRIVATE
    -T${MCU_LINKER_SCRIPT}
)

target_link_libraries(
    ${MODULE_NAME}
    ${PROJECT_NAME}_K1921VG015_HAL_INTERFACE
    SYS_INTERFACE
)

target_link_options(
    ${MODULE_NAME}
    PRIVATE
    -Wl,-Map=${MODULE_NAME}.map,--cref,--print-memory-usage
)

target_post_build(${MODULE_NAME})