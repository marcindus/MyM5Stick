#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

TEST(SimpleTest, CompareEqualValues)
{
    ASSERT_EQ(1, 1);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


