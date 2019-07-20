#pragma once

#include "test_base.hpp"

class DataStoreTests
    : public TestBase
    , public ::testing::Test
{
protected:
    std::string testData =
#       include "test_data/data1.txt"
    ;
    nana_source_view::data_store store{testData};
};

TEST_F(DataStoreTests, DefaultConstruction)
{
    EXPECT_EQ(store.caret_count(), 1);
    EXPECT_EQ(store.caret_begin()->offset, store.size());
    EXPECT_EQ(store.caret_begin()->range, 0);

    EXPECT_EQ(store.utf8_string(), testData);
}

TEST_F(DataStoreTests, OverlayCaret)
{
    store.add_caret(store.size());

    EXPECT_EQ(store.caret_count(), 1);
    EXPECT_EQ(store.caret_begin()->offset, store.size());
    EXPECT_EQ(store.caret_begin()->range, 0);
}

TEST_F(DataStoreTests, AddCaret)
{
    store.add_caret(0);

    EXPECT_EQ(store.caret_count(), 2);

    std::set <caret_type> expectedCarets = {
        {0, 0},
        {static_cast <index_type> (store.size()), 0}
    };

    EXPECT_EQ(store.retrieve_carets(), expectedCarets);
}

TEST_F(DataStoreTests, Clear)
{
    store.clear();

    EXPECT_EQ(store.size(), 0);
    EXPECT_EQ(store.caret_count(), 1);
    EXPECT_EQ(store.caret_begin()->offset, store.size());
    EXPECT_EQ(store.caret_begin()->range, 0);
}
