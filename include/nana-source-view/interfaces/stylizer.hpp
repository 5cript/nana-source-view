#pragma once

#include "../abstractions/store.hpp"
#include "../abstractions/style_range.hpp"

namespace nana_source_view
{
    template <typename CRTP>
    class stylizer
    {
    public:
        using index_type = data_store::index_type;
        using range_type = std::pair <typename CRTP::iterator_type, typename CRTP::iterator_type>;

        stylizer(data_store const* store)
            : store{store}
        {
        }

        virtual ~stylizer() = default;

        /**
         * @brief on_line_change Called when there were changes made to a specific line.
         * Can be used to update the style list.
         * Implemented via on_multi_line_change, can be overriden
         */
        virtual void on_line_change(index_type line);

        /**
         * @brief on_multi_line_change Called when multiple lines change.
         * @param begin The first line that is included in the change
         * @param end The last line that is included in the change.
         */
        virtual void on_multi_line_change(index_type begin, index_type end) = 0;

        /**
         * @brief styles_on_lines Returns a range within some sort of container with style ranges.
         * @param begin Begin of relevant styles
         * @param end End of relevant styles.
         * @return
         */
        virtual range_type styles_on_lines(index_type begin, index_type end) = 0;

    protected:
        data_store const* store;
    };
}
