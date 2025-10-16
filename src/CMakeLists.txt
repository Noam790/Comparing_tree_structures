# add_executable(tree tree.c tree.h)
add_library(bicolor-tree bicolor-tree.c ${CMAKE_SOURCE_DIR}/include/bicolor-tree.h)

set_target_properties(bicolor-tree PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

install(
	TARGETS bicolor-tree
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
)

install(
	FILES ${CMAKE_SOURCE_DIR}/include/bicolor-tree.h
	DESTINATION include
)

# Ajout d'un fichier de configuration de type pkgconfig. Copie le 1er argument vers le 2ème. @ONLY = restreint le remplacement de variable dans tree.pc.in
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
	${CMAKE_CURRENT_SOURCE_DIR}/BicolorTreeConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/BicolorTreeConfig.cmake
	INSTALL_DESTINATION cmake
)
install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/BicolorTreeConfig.cmake
	DESTINATION cmake
)
