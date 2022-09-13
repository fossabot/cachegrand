file(GLOB SRC_FILES_T1HA "iodlr/*.c")

add_library(
        iodlr
        ${SRC_FILES_T1HA})

target_compile_options(
        iodlr
        PRIVATE
        -O3 -D_FORTIFY_SOURCE=2 -z noexecstack -fstack-protector -Wformat -Wformat-security -Wall -fPIC -DPIC)

target_compile_definitions(
        iodlr
        PUBLIC
        ENABLE_LARGE_CODE_PAGES=1)

target_include_directories(
        iodlr
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/iodlr)

list(APPEND DEPS_LIST_LIBRARIES "iodlr")
list(APPEND DEPS_LIST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/iodlr")
