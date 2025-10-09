cmake_minimum_required(VERSION 3.0)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)

project(List C)
add_library(bicolor-tree SHARED bicolor-tree.c bicolor-tree.h)

install(
	TARGETS bicolor-tree
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
)

install(
	FILES bicolor-tree.h
	DESTINATION include
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/bicolor-tree.pc.in
	${CMAKE_CURRENT_BINARY_DIR}/bicolor-tree.pc
	@ONLY
)
install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/bicolor-tree.pc
	DESTINATION share/pkgconfig
	COMPONENT "PkgConfig"
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/bicolorTreeConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/bicolorTreeConfig.cmake
	INSTALL_DESTINATION cmake
)
install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/bicolorTreeConfig.cmake
	DESTINATION cmake
)

add_executable(test-bicolor-tree test-bicolor-tree.c bicolor-tree.h)
add_dependencies(test-bicolor-tree bicolor-tree)
target_link_libraries(test-bicolor-tree bicolor-tree)

enable_testing()
add_test(test-bicolor-tree ./test-bicolor-tree)
