
message("CMAKE_SOURCE_DIR= ${CMAKE_SOURCE_DIR}")
message("CMAKE_BINARY_DIR= ${CMAKE_BINARY_DIR}")



cmake_minimum_required(VERSION 2.6)


project(cmake_test)

# thai bitmap test
add_executable(thailand_bitmap_test thailand_bitmap_test.c )


add_executable(hanoi_test hanoi_test.c )