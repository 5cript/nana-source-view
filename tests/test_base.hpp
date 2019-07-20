#pragma once

#include <nana-source-view/abstractions/iterator.hpp>

#include <ctime>
#include <random>
#include <cmath>

class TestBase
{
public:
    using caret_type = nana_source_view::data_store::caret_type;
    using index_type = caret_type::index_type;

protected:
    std::default_random_engine gen;
    std::uniform_int_distribution <int> distSmall{-500, 500};
    std::uniform_int_distribution <int> distLarge{-50000, 50000};
};
