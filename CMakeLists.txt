cmake_minimum_required(VERSION 3.21)

project(bicolor-tree VERSION 1.0.0 LANGUAGES C)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Add sources
add_subdirectory(src)

# Add tests
enable_testing()
add_subdirectory(tests)

set(CPACK_PACKAGE_NAME "bicolor-tree")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_GENERATOR "ZIP;TGZ")

include(CPack)
