cmake_minimum_required(VERSION 3.21)
project(library C CXX)

set(PROJECT_NAME djtools_lib)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CONAN_DISABLE_CHECK_COMPILER 1)

include(FetchContent)
include(cmake/GoogleTest.cmake)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(SOURCE_FILES
        src/encode.c
        src/decode.c
        src/endian.c

        src/gzip.c

        src/fsar.c
)

set(TEST_FILES
        src/tests/decode_tests.cc
        src/tests/fsar_tests.cc
)


enable_testing()

add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

add_executable(${PROJECT_NAME}_test ${TEST_FILES})
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_test gtest_main)


add_custom_command(TARGET ${PROJECT_NAME}_test PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/test/ $<TARGET_FILE_DIR:${PROJECT_NAME}_test>)


