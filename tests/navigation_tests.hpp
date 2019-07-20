#pragma once

#include "test_base.hpp"

class NavigationTests
    : public TestBase
    , public ::testing::Test
{
protected:
    std::string testData =
#       include "test_data/data1.txt"
    ;
    nana_source_view::data_store store{testData};
    nana_source_view::basic_navigator navi{&store};
};

TEST_F(NavigationTests, LeftArrowSingle1)
{
    EXPECT_EQ(store.caret_count(), 1);
    EXPECT_EQ(store.caret_begin()->offset, store.size());
    EXPECT_EQ(store.caret_begin()->range, 0);

    navi.arrow_left(false, false);

    EXPECT_EQ(store.caret_count(), 1);
    EXPECT_EQ(store.caret_begin()->offset, store.size() - 1);
    EXPECT_EQ(store.caret_begin()->range, 0);
}

TEST_F(NavigationTests, LeftArrowSingle2)
{
    for (int i = 0; i != 100; ++i)
        navi.arrow_left(false, false);

    EXPECT_EQ(store.caret_count(), 1);
    EXPECT_EQ(store.caret_begin()->offset, store.size() - 100);
    EXPECT_EQ(store.caret_begin()->range, 0);
}

TEST_F(NavigationTests, LeftArrowSingleBorder)
{
    for (int i = 0; i != 10'000; ++i)
        navi.arrow_left(false, false);

    EXPECT_EQ(store.caret_count(), 1);
    EXPECT_EQ(store.caret_begin()->offset, 0);
    EXPECT_EQ(store.caret_begin()->range, 0);
}

TEST_F(NavigationTests, LeftArrowShift1)
{
    navi.arrow_left(true, false);

    EXPECT_EQ(store.caret_count(), 1);
    EXPECT_EQ(store.caret_begin()->offset, store.size() - 1);
    EXPECT_EQ(store.caret_begin()->range, 1);
}

TEST_F(NavigationTests, LeftArrowShift100)
{
    for (int i = 0; i != 100; ++i)
        navi.arrow_left(true, false);

    EXPECT_EQ(store.caret_count(), 1);
    EXPECT_EQ(store.caret_begin()->offset, store.size() - 100);
    EXPECT_EQ(store.caret_begin()->range, 100);
}

TEST_F(NavigationTests, RightArrow1)
{
    store.remove_caret(store.caret_begin());
    EXPECT_EQ(store.caret_count(), 0);
    store.add_caret(0);
    EXPECT_EQ(store.caret_count(), 1);

    navi.arrow_right(false, false);

    EXPECT_EQ(store.caret_count(), 1);
    EXPECT_EQ(store.caret_begin()->offset, 1);
    EXPECT_EQ(store.caret_begin()->range, 0);
}

TEST_F(NavigationTests, RightArrow100)
{
    store.remove_caret(store.caret_begin());
    EXPECT_EQ(store.caret_count(), 0);
    store.add_caret(0);
    EXPECT_EQ(store.caret_count(), 1);

    for (int i = 0; i != 100; ++i)
        navi.arrow_right(false, false);

    EXPECT_EQ(store.caret_count(), 1);
    EXPECT_EQ(store.caret_begin()->offset, 100);
    EXPECT_EQ(store.caret_begin()->range, 0);
}

TEST_F(NavigationTests, RightArrowShift)
{
    store.remove_caret(store.caret_begin());
    EXPECT_EQ(store.caret_count(), 0);
    store.add_caret(0);
    EXPECT_EQ(store.caret_count(), 1);

    navi.arrow_right(true, false);

    EXPECT_EQ(store.caret_count(), 1);
    EXPECT_EQ(store.caret_begin()->offset, 1);
    EXPECT_EQ(store.caret_begin()->range, -1);
}

TEST_F(NavigationTests, RightArrowShift100)
{
    store.remove_caret(store.caret_begin());
    EXPECT_EQ(store.caret_count(), 0);
    store.add_caret(0);
    EXPECT_EQ(store.caret_count(), 1);

    for (int i = 0; i != 100; ++i)
        navi.arrow_right(true, false);

    EXPECT_EQ(store.caret_count(), 1);
    EXPECT_EQ(store.caret_begin()->offset, 100);
    EXPECT_EQ(store.caret_begin()->range, -100);
}

TEST_F(NavigationTests, RightArrowCtrl)
{
    store.remove_caret(store.caret_begin());
    EXPECT_EQ(store.caret_count(), 0);
    store.add_caret(1);
    EXPECT_EQ(store.caret_count(), 1);
    EXPECT_EQ(store.caret_begin()->offset, 1);

    navi.arrow_right(false, true);

    EXPECT_EQ(store.caret_count(), 1);
    EXPECT_EQ(store.caret_begin()->offset, 2);
    EXPECT_EQ(store.caret_begin()->range, 0);

    navi.arrow_right(false, true);

    EXPECT_EQ(store.caret_begin()->offset, 10);
    EXPECT_EQ(store.caret_begin()->range, 0);

    navi.arrow_right(false, true);

    EXPECT_EQ(store.caret_begin()->offset, 11);
    EXPECT_EQ(store.caret_begin()->range, 0);

    navi.arrow_right(false, true);

    EXPECT_EQ(store.caret_begin()->offset, 19);
    EXPECT_EQ(store.caret_begin()->range, 0);

    navi.arrow_right(false, true);

    EXPECT_EQ(store.caret_begin()->offset, 20);
    EXPECT_EQ(store.caret_begin()->range, 0);

    navi.arrow_right(false, true);

    EXPECT_EQ(store.caret_begin()->offset, 21);
    EXPECT_EQ(store.caret_begin()->range, 0);
}

TEST_F(NavigationTests, LeftArrowCtrl)
{
    store.remove_caret(store.caret_begin());
    EXPECT_EQ(store.caret_count(), 0);
    store.add_caret(169);
    EXPECT_EQ(store.caret_count(), 1);
    EXPECT_EQ(store.caret_begin()->offset, 169);

    navi.arrow_left(false, true);
    EXPECT_EQ(store.caret_begin()->offset, 168);

    navi.arrow_left(false, true);
    EXPECT_EQ(store.caret_begin()->offset, 166);

    navi.arrow_left(false, true);
    EXPECT_EQ(store.caret_begin()->offset, 161);

    navi.arrow_left(false, true);
    EXPECT_EQ(store.caret_begin()->offset, 156);
}
