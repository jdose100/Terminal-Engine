cmake_minimum_required(VERSION 3.31)
project(term_engine_tests LANGUAGES CXX)
include(FetchContent)

# Получаем Catch2.
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.3.1
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(Catch2)

# Делаем так, чтобы Catch2 тоже существовал в compile_commands.json
if (TERM_ENGINE_DEV)
    include(../cmake/dev_setup.cmake)
endif ()

# --- Добавление тестов --- #

add_executable(tests_with_catch_main 
    ${PROJECT_SOURCE_DIR}/add_entity_test.cpp
    ${PROJECT_SOURCE_DIR}/delete_entity_test.cpp
    ${PROJECT_SOURCE_DIR}/get_entity_test.cpp
)
target_link_libraries(tests_with_catch_main PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests_with_catch_main PRIVATE terminal::engine)

# --- Регистрация тестов для CTest --- #
include(CTest)
include(Catch)

catch_discover_tests(tests_with_catch_main)
