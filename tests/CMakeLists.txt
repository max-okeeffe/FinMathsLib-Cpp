add_executable(FinMaths_Tests
    test_distributions.cpp
    test_statistics.cpp
)

target_link_libraries(FinMaths_Tests
    PRIVATE
        FinMaths
        GTest::gtest
        GTest::gtest_main
)

target_compile_options(FinMaths_Tests PRIVATE -Wall -Wextra -Wpedantic -Werror)

include(GoogleTest)
gtest_discover_tests(FinMaths_Tests)
