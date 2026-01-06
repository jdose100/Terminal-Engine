set(TEST_DIR ${PROJECT_SOURCE_DIR}/tests)

add_executable(tests_with_catch_main 
    ${TEST_DIR}/add_entity_test.cpp
    ${TEST_DIR}/delete_entity_test.cpp
    ${TEST_DIR}/get_entity_test.cpp
)
target_link_libraries(tests_with_catch_main PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests_with_catch_main PRIVATE terminal::engine)
