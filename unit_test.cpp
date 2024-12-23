#include "unit_test.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(filter) = "GlobalTest.camera_move_test";

    int result = RUN_ALL_TESTS();
    return result;
}