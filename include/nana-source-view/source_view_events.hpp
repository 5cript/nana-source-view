#pragma once

#include "source_view_fwd.hpp"

#include <nana/gui/detail/general_events.hpp>

namespace nana_source_view::events
{
    struct arg_char_change
        : public event_arg
    {
        source_editor& widget;

    };
}
