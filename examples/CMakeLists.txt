cmake_minimum_required(VERSION 3.31)
project(term_engine_tests LANGUAGES CXX)
link_libraries(terminal::engine)

# Добавление примеров.
add_executable(run_app ${PROJECT_SOURCE_DIR}/run_app.cpp)
add_executable(simple_entity ${PROJECT_SOURCE_DIR}/simple_entity.cpp)
add_executable(drawable_entity ${PROJECT_SOURCE_DIR}/drawable_entity.cpp)
