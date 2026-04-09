add_executable(FinMaths_Tests
    test_placeholder.cpp
)

target_link_libraries(FinMaths_Tests
    PRIVATE
        GTest::gtest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(FinMaths_Tests)
