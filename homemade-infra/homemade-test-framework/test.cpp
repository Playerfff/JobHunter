#include "unittest.h"
//g++ test.cpp unittest.cpp -o test -I./
int Foo(int a, int b)
{
    return a + b;
}

NTEST(FooTest_PassDemo)
{
    EXPECT_EQ(3, Foo(1, 2));
    EXPECT_EQ(2, Foo(1, 1));
}

NTEST(FooTest_FailDemo)
{
    EXPECT_EQ(4, Foo(1, 2));
    EXPECT_EQ(2, Foo(1, 2));
}

int main(int argc, const char* argv[])
{
    return RUN_ALL_TESTS();
}
