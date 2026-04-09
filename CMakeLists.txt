cmake_minimum_required(VERSION 3.21)

project(FinMathsLibCpp VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Library
add_library(FinMaths
    src/dummy.cpp
)

target_include_directories(FinMaths
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

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
