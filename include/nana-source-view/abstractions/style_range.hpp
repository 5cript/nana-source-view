#pragma once

#include <interval-tree/interval_tree.hpp>
#include <nana/basic_types.hpp>

#include <optional>

namespace nana_source_view
{
    enum font_flags : unsigned char
    {
        nothing = 0b0000'0000,
        bold = 0b0000'0001,
        italic = 0b0000'0010,
        strikeout = 0b0000'0100,
        underline = 0b0000'1000
    };

    struct style
    {
        nana::color fgcolor;
        nana::color bgcolor;

        unsigned char font_mods;
    };

    struct style_range
    {
        lib_interval_tree::interval <int64_t> range;
        style styling;
    };
}
