#pragma once

#include "source_view_scheme.hpp"

#include <memory>

#include <nana/gui/widgets/widget.hpp>
#include <nana/gui/detail/general_events.hpp>
#include <nana/gui/detail/drawer.hpp>
#include <nana/basic_types.hpp>
#include <nana/unicode_bidi.hpp>

namespace nana_source_view::skeletons
{
    class source_editor_impl
    {
    public: // Typedefs
        using graph_reference = ::nana::paint::graphics&;

    public: // Interface
        source_editor_impl(nana::window, graph_reference, skeletons::source_editor_scheme const*);
        ~source_editor_impl();

        /**
         * @brief render Renders the textbox
         * @param focused
         */
        void render(bool focused);

        /**
         * @brief area Informs this impl of its size
         * @param rect
         */
        void area(nana::rectangle const& rect);

        /**
         * @brief text Sets the text of the editor
         * @param rect
         */
        void text(std::string_view const& text);

        /**
         * @brief try_refresh
         * @return Returns true if render was issued.
         */
        bool try_refresh();

    private: // Internal Implementations
        ::nana::color bgcolor_() const;

    private:
        struct implementation;
        std::unique_ptr <implementation> impl_;
        nana::window window_;
        graph_reference graph_;
        skeletons::source_editor_scheme const* scheme_;
    };
}
