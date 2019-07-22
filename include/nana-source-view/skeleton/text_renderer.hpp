#pragma once

#include <nana-source-view/interfaces/stylizer.hpp>
#include <nana-source-view/abstractions/store.hpp>

#include <nana/basic_types.hpp>
#include <nana/paint/graphics.hpp>

namespace nana_source_view::skeletons
{
    class text_renderer
    {
    public: // Typedefs
        using graph_reference = ::nana::paint::graphics&;
        using index_type = data_store::index_type;

    public:
        text_renderer(data_store const* store);

        template <typename T>
        void make_stylizer()
        {
            stylizer_.reset(new T{store_});
        }

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

        /**
         * @brief update_scroll Set the first line that is scrolled to.
         * @param scroll_top_line the top line in the scrolled area.
         */
        void update_scroll(index_type scroll_top_line);

        /**
         * @brief font Sets the base font.
         * @param font
         */
        void font(nana::paint::font const& font, bool assume_monospace = false);

        /**
         * @brief font Retrieves the font.
         * @return
         */
        nana::paint::font font() const;

    private:
        data_store const* store_;
        nana::rectangle area_;
        std::unique_ptr <stylizer> stylizer_;
        nana::paint::font font_;
        index_type scroll_top_;

    private: // memoized data
        bool monospace;
        font_height;
        font_width;
    };
}
