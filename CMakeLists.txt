cmake_minimum_required(VERSION 3.21)

project(Comparing_tree_structures VERSION 1.0.0 LANGUAGES C)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Add sources
add_subdirectory(src/bicolor)
add_subdirectory(src/avl)

# Add tests
enable_testing()
add_subdirectory(tests)
