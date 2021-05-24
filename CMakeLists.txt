cmake_minimum_required(VERSION 2.6)
project(monitor)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

# Add GoogleTest library subdirectories
add_subdirectory(thirdparty/googletest)

add_executable(test src/helper/format_helper.cpp test/utest_rp_format.cpp
        src/parsing/linux_parser.cpp test/utest_rp_linux_parser.cpp
        src/helper/memory_helper.cpp test/utest_rp_memory_helper.cpp
        src/model/process.cpp test/utest_rp_string_helper.cpp src/helper/cpu_helper.cpp test/utest_rp_cpu_helper.cpp test/utest_rp_process.cpp)

file(COPY test/resources DESTINATION Testing)

target_link_libraries(test
        stdc++fs
        gtest_main)


include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES})

# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)
