#pragma once

#include "source_view_fwd.hpp"

#include "abstractions/caret.hpp"
#include "abstractions/store.hpp"

#include "skeleton/source_view_event_agent.hpp"
#include "skeleton/source_view_scheme.hpp"

#include <nana/gui/widgets/widget.hpp>
#include <nana/unicode_bidi.hpp>
#include <nana/gui/detail/general_events.hpp>
#include <nana/gui/detail/drawer.hpp>
#include <nana/basic_types.hpp>

#include <memory>
#include <vector>
#include <string>
#include <string_view>

namespace nana_source_view
{
    class source_editor;

    struct arg_source_editor_basic
        : public nana::event_arg
    {
        source_editor& widget;

        arg_source_editor_basic(source_editor& widget);
    };

    struct arg_source_editor_change
        : public nana::event_arg
    {
        using caret_type = caret <int64_t>;

        source_editor& widget;
        std::vector <caret_type> const& carets_before; // The carets before the time of the event
        std::vector <caret_type> const& carets_after; // The carets after

        arg_source_editor_change
        (
            source_editor& widget,
            std::vector <caret_type> const& carets_before,
            std::vector <caret_type> const& carets_after
        );
    };

    namespace drawerbase::source_editor
	{
        struct source_editor_events
            : public nana::general_events
        {
            nana::basic_event <arg_source_editor_basic> on_load;
            nana::basic_event <arg_source_editor_change> text_changed;
            nana::basic_event <arg_source_editor_change> selection_changed;
        };

        class event_agent
            : public skeletons::source_view_event_agent_interface
        {
        public:
            using caret_type = caret <int64_t>;

            event_agent
            (
                ::nana_source_view::source_editor&,
                std::vector <caret_type> const& carets_before,
                std::vector <caret_type> const& carets_after
            );
        private:
            void on_load() override;
            void text_changed() override;
            void selection_changed() override;
        private:
            ::nana_source_view::source_editor& widget_;
            std::vector <caret_type> const& carets_before_;
            std::vector <caret_type> const& carets_after_;
        };

        class drawer
            : public nana::drawer_trigger
        {
        public:
            drawer();
            ::nana_source_view::source_editor* editor();
            ::nana_source_view::source_editor const* editor() const;

        private:
            void attached(widget_reference, graph_reference)	override;
            void detached()	override;
            void refresh(graph_reference)	override;
            void focus(graph_reference, const nana::arg_focus&)	override;
            void mouse_down(graph_reference, const nana::arg_mouse&)	override;
            void mouse_move(graph_reference, const nana::arg_mouse&)	override;
            void mouse_up(graph_reference, const nana::arg_mouse&)	override;
            void mouse_enter(graph_reference, const nana::arg_mouse&)	override;
            void mouse_leave(graph_reference, const nana::arg_mouse&)	override;
            void dbl_click(graph_reference, const nana::arg_mouse&)	override;
            void key_press(graph_reference, const nana::arg_keyboard&)override;
            void key_char(graph_reference, const nana::arg_keyboard&)	override;
            void mouse_wheel(graph_reference, const nana::arg_wheel&)	override;
            void resized(graph_reference, const nana::arg_resized&)	override;
            void typeface_changed(graph_reference)				override;

        private:
            void _m_text_area(unsigned width, unsigned heigt);

        private:
            nana::widget* widget_;
            ::nana_source_view::source_editor* editor_;
            std::unique_ptr <event_agent> evt_agent_;
        };
	}

	struct source_editor_impl;

    class source_editor
        : public nana::widget_object
        <
            nana::category::widget_tag,
            drawerbase::source_editor::drawer,
            drawerbase::source_editor::source_editor_events,
            nana_source_view::skeletons::source_view_scheme
        >
    {
    public:
        /**
         *  The default constructor without creating the widget.
         */
        source_editor();

        /**
         *  Creates the source editor visible/invisible without text.
         */
        source_editor(nana::window wd, bool visible);

        /**
         *  Creates the source editor visible/invisible with text.
         *  @param wd The parent (containing) window.
         *  @param text Some initial text to put in the box.
         *  @param visible Show on/after construction?
         */
        source_editor(nana::window wd, std::string_view const& text, bool visible = true);

        /**
         *  Create the source editor with a given rectangle in the parent window.
         *  @param wd The parent (containing) window.
         *  @param rectangle A give rectangle indicating the textbox position.
         *  @param visible Shall show after creation?
         */
        source_editor(nana::window wd, nana::rectangle const& rect = nana::rectangle(), bool visible = true);

        /**
         *  Create the source editor with a given rectangle in the parent window.
         *  @param wd The parent (containing) window.
         *  @param rect A give rectangle indicating the textbox position.
         *  @param text A text to initially set.
         *  @param visible Shall show after creation?
         */
        source_editor(nana::window wd, nana::rectangle const& rect, std::string_view const& text, bool visible = true);

    private:
        std::unique_ptr <source_editor_impl> impl_;
    };
}
