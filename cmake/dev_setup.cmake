# Настройка библиотеки для разработки.

# Экспорт compile_commands.json и std include путей.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(
    CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
    ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES}
)
set(
    CMAKE_C_STANDARD_INCLUDE_DIRECTORIES
    ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES}
)

include_directories(
    ${CMAKE_CXX_STANDARD_INCLUDE_DIRECTIORIES}
    ${CMAKE_BINARY_DIR}/_deps/catch2-src
    ${CMAKE_BINARY_DIR}/_deps/ftxui-src
    ${CMAKE_BINARY_DIR}/_deps/glm-src
    ${PROJECT_SOURCE_DIR}/include
)

if (CMAKE_EXPORT_COMPILE_COMMANDS)
    # Добавление исполняемого файла для более корректной
    # настройки compile_commands.json.
    add_executable(
        clangd_header_indexer
        ${PROJECT_SOURCE_DIR}/src/clangd_header_indexer.cpp
    )
    target_link_libraries(
        clangd_header_indexer PRIVATE terminal::engine
    )
    set_target_properties(
        clangd_header_indexer PROPERTIES EXCLUDE_FROM_ALL TRUE
    )
endif ()
