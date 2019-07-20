#include "gtest/gtest.h"

#include <nana-source-view/source_view.hpp>

// following headers expect to be included after gtest headers and source_view
#include "data_store_tests.hpp"
#include "navigation_tests.hpp"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
