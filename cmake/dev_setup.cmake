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

if (CMAKE_EXPORT_COMPILE_COMMANDS AND NOT ${TERM_ENGINE_DEV_INDEXER_ALREADY})
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
    
    set(TERM_ENGINE_DEV_INDEXER_ALREADY TRUE)
endif ()
