cmake_minimum_required(VERSION 3.22)

add_library(
    SYS_INTERFACE
    INTERFACE
)

target_sources(
    SYS_INTERFACE
    INTERFACE
    src/syscalls.c
    src/sysmem.c
)

