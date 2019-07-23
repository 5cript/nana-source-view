/**
 * A base implemenation for text styling for C
 **/
#pragma once

#include <nana-source-view/interfaces/styler.hpp>

namespace nana_source_view::styles
{
    class c_style : public styler
    {
        using iterator_type = std::unordered_map <data_store::index_type, std::vector <style_range>>;

        void initialize() override;
        void on_multi_line_change(index_type begin, index_type end) override;
        range_type styles_on_lines(index_type begin, index_type end) override;
    };
}
