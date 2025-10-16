file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test-*.c")

if(TEST_SOURCES)
    foreach(TEST_FILE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

        add_executable(${TEST_NAME} ${TEST_FILE})

        target_link_libraries(${TEST_NAME} PRIVATE bicolor-tree)

        add_dependencies(${TEST_NAME} bicolor-tree)

        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

        message(STATUS "✅ Test added : ${TEST_NAME}")
    endforeach()
else()
    message(WARNING "⚠️ Can't find any file test-*.c in ${CMAKE_CURRENT_SOURCE_DIR}")
endif()
