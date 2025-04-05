#include <gtest/gtest.h>

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    // Enable verbose output
    ::testing::GTEST_FLAG(print_time) = true;

    return RUN_ALL_TESTS();
}