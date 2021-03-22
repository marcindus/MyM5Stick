#include <string>
#include <iostream>
#include "gtest/gtest.h"

struct SomeUsefulClass
{
    SomeUsefulClass() : message_(""){};
    SomeUsefulClass(std::string message) : message_(message){};
    std::string message_;
};

class MyM5StickCTestSuite : public ::testing::Test {
public:
  MyM5StickCTestSuite() {}
};


TEST_F(MyM5StickCTestSuite, sanityCheck)
{
    EXPECT_EQ(1, 1);
}

