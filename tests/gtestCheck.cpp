#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "WifiWrapperMock.hpp"

using namespace std;

TEST(SimpleTest, CompareEqualValues)
{
//    WifiSniffer sniffer(std::unique_ptr<IWifiWrapper>(new WifiWrapper()));
//    sniffer.start();
//    sniffer.stop();
    ASSERT_EQ(1, 1);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


