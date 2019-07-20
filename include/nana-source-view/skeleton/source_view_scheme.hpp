#pragma once

#include <nana/gui/detail/widget_geometrics.hpp>

namespace nana_source_view::skeletons
{
    struct source_view_scheme
    {
        nana::color_proxy selection {static_cast<nana::color_rgb>(0x3399FF)};
        nana::color_proxy selection_unfocused{ static_cast<nana::color_rgb>(0xF0F0F0) };
        nana::color_proxy selection_text{nana::colors::white};

        nana::parameters::mouse_wheel mouse_wheel;	///< The number of lines/characters to scroll when the vertical/horizontal mouse wheel is moved.
    };
}
