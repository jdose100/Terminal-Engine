cmake_minimum_required(VERSION 3.31)
project(term_engine LANGUAGES CXX VERSION 0.0.3)
include(FetchContent)

option(TERM_ENGINE_BUILD_DOCS "Set to OFF to disable build docs" ON)
option(TERM_ENGINE_BUILD_EXAMPLES "Set to ON to build examples" OFF)
option(TERM_ENGINE_BUILD_TESTS "Set to ON to build tests" ON)

# --- Зависимости --- #

# - FTXUI
set(FTXUI_ENABLE_INSTALL OFF)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

# - gml
FetchContent_Declare(glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG a532f5b1cf27d6a3c099437e6959cf7e398a0a67 # v1.0.2
)
FetchContent_MakeAvailable(glm)

# --- Term Engine Library --- #

# Настройка экспорта compile_commands, и импорта
# стандартных заголовочных файлов.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(
    CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
    ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES}
)
set(
    CMAKE_C_STANDARD_INCLUDE_DIRECTORIES
    ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES}
)

# Установка стандарта.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Установка, что warnings воспринимаются как error.
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Создание библиотеки.
file(GLOB ROOT_SOURCE src/*.cpp)
add_library(term_engine ${ROOT_SOURCE})
add_library(terminal::engine ALIAS term_engine)

target_include_directories(term_engine
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(term_engine
  # FTXUI
  PUBLIC ftxui::screen
  PUBLIC ftxui::dom
  PUBLIC ftxui::component
  # glm
  PUBLIC glm::glm
)

# Включение всех warning в компиляторе.
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else ()
    target_compile_options(
        ${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror
    )
endif ()

# --- Документация --- #
find_package(Doxygen)

if (Doxygen_FOUND AND TERM_ENGINE_BUILD_DOCS)
    set(DOXYGEN_FILE_PATTERNS *.cpp *.hpp)
    set(DOXYGEN_OUTPUT_LANGUAGE Russian)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_QUIET YES)

    doxygen_add_docs(
        term_engine_docs
        ${PROJECT_SOURCE_DIR}/include/term_engine/
        ${PROJECT_SOURCE_DIR}/README.md
        ${PROJECT_SOURCE_DIR}/src/
        ALL
    )
endif ()

# --- Добавление тестов и примеров --- #

if (TERM_ENGINE_BUILD_TESTS)
    # Получение Catch 2
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.3.1
    )
    FetchContent_MakeAvailable(Catch2)

    # Получение тестов.
    include(tests/CMakeLists.txt)
endif ()

if(TERM_ENGINE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
