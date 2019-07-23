#pragma once

#include <nana-source-view/interfaces/styler.hpp>
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

        /**
         * Inplace creates a styler.
         * Returns a pointer to it, that is NON-OWNING. The renderer owns it.
         */
        template <typename T, typename... Args>
        T* replace_styler(Args&&... args)
        {
            auto* sty = new T{store_, std::forward <Args&&> (args)...};
            styler_.reset(sty);
            return sty;
        }

        /**
         * Retrieves the text styler casted to the given type.
         */
        template <typename T>
        T* get_styler()
        {
            return dynamic_cast <T*> (styler_.get());
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
        std::unique_ptr <styler> styler_;
        nana::paint::font font_;
        index_type scroll_top_;
    };
}
