#include "unit_test.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    // ::testing::GTEST_FLAG(filter) = "GlobalTest.phong_lighting_test";
    ::testing::GTEST_FLAG(filter) = "GlobalTest.move_to_origin";

    int result = RUN_ALL_TESTS();
    return result;
}