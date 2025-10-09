cmake_minimum_required(VERSION 3.0)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)

project(List C)
# add_executable(avl-tree avl-tree.c avl-tree.h)
add_library(avl-tree SHARED avl-tree.c avl-tree.h)

install(
	TARGETS avl-tree
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
)

install(
	FILES avl-tree.h
	DESTINATION include
)

# Ajout d'un fichier de configuration de type pkgconfig. Copie le 1er argument vers le 2ème. @ONLY = restreint le remplacement de variable dans avl-tree.pc.in
# à celles qui ont le format @<var>@ pour éviter les conflits avec la syntaxe CMake ${<var>}.
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/avl-tree.pc.in
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
	${CMAKE_CURRENT_SOURCE_DIR}/avlTreeConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/avlTreeConfig.cmake
	INSTALL_DESTINATION cmake
)
install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/avlTreeConfig.cmake
	DESTINATION cmake
)

# Ajout d'un exécutable dépendant également de avl-tree.h
add_executable(test-avl-tree test-avl-tree.c avl-tree.h)
# Précision de l'ordre de construction: le programme de test doit se faire
# après la librairie
add_dependencies(test-avl-tree avl-tree)
# Aggrégation du programme de test avec la librairie avl-tree
target_link_libraries(test-avl-tree avl-tree)

# Activation des tests
enable_testing()
# Ajout d'un test
add_test(test-avl-tree ./test-avl-tree)
