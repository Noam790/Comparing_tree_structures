# add_executable(tree tree.c tree.h)
add_library(avl-tree SHARED avl-tree.c ../../include/avl-tree.h)

target_include_directories(avl-tree PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(avl-tree PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

install(
	TARGETS avl-tree
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
)

install(
	FILES ../../include/avl-tree.h
	DESTINATION include
)

# Ajout d'un fichier de configuration de type pkgconfig. Copie le 1er argument vers le 2ème. @ONLY = restreint le remplacement de variable dans tree.pc.in
# à celles qui ont le format @<var>@ pour éviter les conflits avec la syntaxe CMake ${<var>}.
configure_file(
		avl-tree.pc.in
	${CMAKE_CURRENT_BINARY_DIR}/avl-tree.pc
	@ONLY
)
install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/avl-tree.pc
	DESTINATION share/pkgconfig
	COMPONENT "PkgConfig"
)

#  Ajout d'un fichier de configuration de type cmake
include(CMakePackageConfigHelpers)
configure_package_config_file(
		AvlTreeConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/AvlTreeConfig.cmake
	INSTALL_DESTINATION cmake
)
install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/AvlTreeConfig.cmake
	DESTINATION cmake
)
