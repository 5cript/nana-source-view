#pragma once

#include "../abstractions/store.hpp"
#include "../abstractions/style_range.hpp"

#include <unordered_map>
#include <vector>

namespace nana_source_view
{
    /**
     * The stylizer provides ranges for text coloration.
     */
    class styler
    {
    public:
        using index_type = data_store::index_type;
        using iterator_type = std::unordered_map <index_type, std::vector <style_range>>::const_iterator;
        using range_type = std::pair <iterator_type, iterator_type>;

        styler(data_store const* store)
            : store{store}
        {
        }

        virtual ~styler() = default;

        /**
         * @brief initialize Initialize style information for current data store.
         */
        virtual void initialize() = 0;

        /**
         * @brief on_line_change Called when there were changes made to a specific line.
         * Can be used to update the style list.
         * Implemented via on_multi_line_change, can be overriden
         */
        virtual void on_line_change(index_type line)
        {
            on_multi_line_change(line, line + 1);
        }

        /**
         * @brief on_multi_line_change Called when multiple lines change.
         * @param begin The first line that is included in the change
         * @param end past the end index. The first line NOT included in the change
         */
        virtual void on_multi_line_change(index_type begin, index_type end) = 0;

        /**
         * @brief styles_on_line The style on the specific line, base implementation uses styles_on_lines.
         * @param begin Which line.
         * @return A range of styles.
         */
        virtual range_type styles_on_line(index_type line)
        {
            return styles_on_lines(line, line + 1);
        }

        /**
         * @brief styles_on_lines Returns a range within some sort of container with style ranges.
         * @param begin Begin of relevant styles
         * @param end Past the end index.
         * @return
         */
        virtual range_type styles_on_lines(index_type begin, index_type end) = 0;

    protected:
        data_store const* store;
    };
}
