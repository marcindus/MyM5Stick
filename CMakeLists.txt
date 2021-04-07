cmake_minimum_required(VERSION 3.11)
project(project)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        master
)

FetchContent_GetProperties(googletest)
if (NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif ()

add_definitions(-Wno-unused-variable)
add_definitions(-Wno-unused-parameter)
add_definitions(-Wall -Werror -pedantic -Wextra)


add_executable(gtests unit_tests/gtestCheck.cpp)
target_link_libraries(gtests gtest_main gmock_main)
