cmake_minimum_required(VERSION 3.22)

project(hal VERSION 0.1.0 LANGUAGES CXX C ASM)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(CPACK_PACKAGE_CHECKSUM MD5)
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")

include(FetchContent)
include(CPack)
include(cmake/opts/custom_build_opts.cmake)
include(cmake/utils/pre-commit.cmake)
include(cmake/utils/change_log.cmake)

add_compile_options(
    -Wall
    -Wextra
    -Werror
    -Wfatal-errors
    -Wdouble-promotion
    # -Wstrict-prototypes # is valid for C/ObjC but not for C++ [-Werror]
    -Wstrict-overflow
    # -Werror-implicit-function-declaration # is valid for C/ObjC but not for C++ [-Werror]
    -Wfloat-equal
    -Wundef
    -Wshadow
    -Wwrite-strings
    -Wsign-compare
    -Wmissing-format-attribute
    -Wunreachable-code
    -Wcast-align
    -Wcast-function-type
    -Wcast-qual
    -Wnull-dereference
    -Wuninitialized
    -Wunused
    -Wunused-function
    -Wreturn-type
    -Wredundant-decls
)

add_subdirectory(hal)
add_subdirectory(exm)

target_install_binary(${PROJECT_NAME})
