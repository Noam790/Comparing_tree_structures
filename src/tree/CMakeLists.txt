cmake_minimum_required(VERSION 3.0)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)

project(List C)
# add_executable(bicolor-tree bicolor-tree.c bicolor-tree.h)
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

# Ajout d'un fichier de configuration de type pkgconfig. Copie le 1er argument vers le 2ème. @ONLY = restreint le remplacement de variable dans bicolor-tree.pc.in
# à celles qui ont le format @<var>@ pour éviter les conflits avec la syntaxe CMake ${<var>}.
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

#  Ajout d'un fichier de configuration de type cmake
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

# Ajout d'un exécutable dépendant également de bicolor-tree.h
add_executable(test-bicolor-tree test-bicolor-tree.c bicolor-tree.h)
# Précision de l'ordre de construction: le programme de test doit se faire
# après la librairie
add_dependencies(test-bicolor-tree bicolor-tree)
# Aggrégation du programme de test avec la librairie bicolor-tree
target_link_libraries(test-bicolor-tree bicolor-tree)

# Activation des tests
enable_testing()
# Ajout d'un test
add_test(test-bicolor-tree ./test-bicolor-tree)
