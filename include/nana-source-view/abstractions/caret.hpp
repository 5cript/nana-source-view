#pragma once

#include <type_traits>
#include <cstdint>

namespace nana_source_view
{
    template <typename IndexType = int64_t>
    struct caret
    {
        /**
         *  The range has to be signed anyway. This way we save a lot of conversions.
         *  signed 64 bit integers are far big enough for anything this editor can handle anyway
         *  and signed ints are faster to do operations with.
         */
        using index_type = typename std::make_signed <IndexType>::type;

        /**
         *  The offset in the data store. This is encoding agnostic / operates on byte level.
         */
        index_type offset;

        /**
         *  Turns the carret into a range if > 0
         */
        index_type range;

        /**
         *  Is this caret a range and not just a point?
         */
        constexpr bool is_range() const
        {
            return range > static_cast <index_type> (0);
        }

        /**
         *  Constructor for a caret
         */
        constexpr caret(index_type offset, index_type range)
            : offset{offset}
            , range{range}
        {
        }

        constexpr caret(index_type offset = {})
            : offset{offset}
            , range{0}
        {
        }

        constexpr bool operator==(caret const& cat) const
        {
            return cat.offset == offset && cat.range == range;
        };

        friend constexpr bool operator<(caret const& lhs, caret const& rhs)
        {
            return lhs.offset < rhs.offset;
        }

        caret& operator=(caret const&) = default;
        caret& operator=(caret&&) = default;

        caret(caret const&) = default;
        caret(caret&&) = default;
    };
}
