cmake_minimum_required(VERSION 3.21)

project(bicolor_tree VERSION 1.0.0 LANGUAGES C)

option(BUILD_TESTING "Build unit tests" ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Add sources
add_subdirectory(src)

# Add tests
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
