cmake_minimum_required(VERSION 3.21)

project(FinMathsLibCpp VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Formatting
find_program(CLANG_FORMAT clang-format REQUIRED)
add_custom_target(fmt
    COMMAND find ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src
            ${CMAKE_SOURCE_DIR}/tests ${CMAKE_SOURCE_DIR}/examples
            -name "*.cpp" -o -name "*.hpp" | xargs ${CLANG_FORMAT} -i
    COMMENT "Formatting sources with clang-format"
)

# Linting
find_program(CLANG_TIDY clang-tidy REQUIRED)
add_custom_target(lint
    COMMAND find ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src
            -name "*.cpp" -o -name "*.hpp" | xargs ${CLANG_TIDY} -p ${CMAKE_BINARY_DIR}
    COMMENT "Linting sources with clang-tidy"
)

# Library
add_library(FinMaths
    src/maths/distributions.cpp
    src/maths/matrix.cpp
    src/maths/rng.cpp
    src/maths/statistics.cpp
)

target_include_directories(FinMaths
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (MSVC)
    target_compile_options(FinMaths PRIVATE /W4 /WX)
else()
    target_compile_options(FinMaths PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# GoogleTest
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Testing
enable_testing()

add_subdirectory(tests)
add_subdirectory(examples)
