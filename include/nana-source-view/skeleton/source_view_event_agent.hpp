#pragma once

namespace nana_source_view::skeletons
{
    class source_view_event_agent_interface
    {
    public:
        virtual ~source_view_event_agent_interface() = default;

        virtual void on_load() = 0;
        virtual void text_changed() = 0;	///< An event for the text first change after text has been opened or stored.
        virtual void selection_changed() = 0;	///< An event for the change of text.
    };
}
