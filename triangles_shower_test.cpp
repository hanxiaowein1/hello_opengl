#include "triangles_shower.h"
#include "unit_test.h"

TEST(GlobalTest, move_to_origin)
{
    // usually one display info is for one model
    std::vector<DisplayInfo<std::vector<double>,std::vector<int>>> mul_display_info = {
        {
            {
                {0,0,0},
                {1,0,0},
                {0,1,0},
                {0,0,1},
            },
            {},
            {}
        }
    };
    move_to_origin(mul_display_info);
    // check
    
    std::vector<double> desired_vertex0 = {-0.5f, -0.5f, -0.5f};
    auto translated_vertex0 = mul_display_info[0].vertices[0];
    ASSERT_EQ(true, std::abs(desired_vertex0[0] - translated_vertex0[0]) < 0.000001f);
    ASSERT_EQ(true, std::abs(desired_vertex0[1] - translated_vertex0[1]) < 0.000001f);
    ASSERT_EQ(true, std::abs(desired_vertex0[2] - translated_vertex0[2]) < 0.000001f);
    // std::vector<double> desired_vertex1 = {-0.5f, -0.5f, -0.5f};
    // std::vector<double> desired_vertex2 = {-0.5f, -0.5f, -0.5f};
    // std::vector<double> desired_vertex3 = {-0.5f, -0.5f, -0.5f};

}