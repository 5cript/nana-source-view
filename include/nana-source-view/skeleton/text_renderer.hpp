#pragma once

#include <nana-source-view/abstractions/store.hpp>

#include <nana/basic_types.hpp>
#include <nana/paint/graphics.hpp>

namespace nana_source_view::skeletons
{
    class text_renderer
    {
    public: // Typedefs
        using graph_reference = ::nana::paint::graphics&;

    public:
        text_renderer(data_store const* store);

        /**
         * @brief text_area Sets the text area
         * @param rect
         */
        void text_area(nana::rectangle const& rect);

        /**
         * @brief render Renders the visible text into the box.
         * @param graph
         */
        void render(graph_reference graph);

    private:
        nana::rectangle area_;
        data_store const* store_;
    };
}
