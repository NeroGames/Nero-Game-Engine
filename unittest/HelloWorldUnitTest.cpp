#include <gtest/gtest.h>

namespace
{
    constexpr auto GREETING{"Hello Nero"};
    constexpr auto EXPECTED_GREETING{"Hello Nero"};
}

TEST(HelloWorld, SayHello)
{
    EXPECT_STREQ(GREETING, EXPECTED_GREETING);
}
