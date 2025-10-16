cmake_minimum_required(VERSION 3.21)

project(bicolor-tree VERSION 1.0.0 LANGUAGES C)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Add sources
add_subdirectory(src)

# Add tests
enable_testing()
add_subdirectory(tests)
