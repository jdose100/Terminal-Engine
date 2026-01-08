cmake_minimum_required(VERSION 3.31)
project(term_engine LANGUAGES CXX VERSION 0.0.4)
include(FetchContent)

option(TERM_ENGINE_BUILD_DOCS "Set to OFF to disable build docs" ON)
option(TERM_ENGINE_BUILD_EXAMPLES "Set to ON to build examples" OFF)
option(TERM_ENGINE_BUILD_TESTS "Set to ON to build tests" OFF)
option(TERM_ENGINE_DEV "Set to ON to enable dev mode" OFF)

# --- Зависимости --- #

# - FTXUI
set(FTXUI_ENABLE_INSTALL OFF)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.1.9
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(ftxui)

# - gml
FetchContent_Declare(glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG a532f5b1cf27d6a3c099437e6959cf7e398a0a67 # v1.0.2
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(glm)

# --- Включение DEV режима --- #
if (TERM_ENGINE_DEV)
    include(cmake/dev_setup.cmake)
endif ()

# --- Term Engine Library --- #
# Установка стандарта.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Установка, что warnings воспринимаются как error.
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Создание библиотеки.
file(GLOB ROOT_SOURCE src/*.cpp)
add_library(${PROJECT_NAME} ${ROOT_SOURCE})
add_library(terminal::engine ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>
    # PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    # PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
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
    add_subdirectory(tests)
endif ()

if (TERM_ENGINE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
