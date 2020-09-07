#include <gtest/gtest.h>
#include <optional>

int main(int argc, char* argv[])
{
    // --gtest_filter="type.name"
    ::testing::InitGoogleTest(&argc, argv);
#ifdef _DEBUG
    ::testing::FLAGS_gtest_break_on_failure = true;
#endif
    return RUN_ALL_TESTS();
}
