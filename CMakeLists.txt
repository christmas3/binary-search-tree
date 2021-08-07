cmake_minimum_required(VERSION 3.14)
project(binary-search-tree)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")

add_executable(${PROJECT_NAME} main.cpp BinarySearchTree.h BstTest.cpp Avl.h Dop.h Dop.cpp DopTest.cpp)

target_link_libraries(${PROJECT_NAME} gtest_main gtest pthread)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
