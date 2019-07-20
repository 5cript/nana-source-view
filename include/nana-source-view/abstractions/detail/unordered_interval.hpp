#pragma once

#include <interval-tree/interval_types.hpp>

namespace nana_source_view::detail
{
    /**
     *  We can use this interval in the interval tree to preserve caret selection directedness
     *  And still have deoverlapping work.
     */
    template <typename numerical_type, typename interval_kind = lib_interval_tree::closed>
    struct unordered_interval
    {
    public:
        using value_type = numerical_type;

        /**
         *  Constructs an interval. low MUST be smaller than high.
         */
        unordered_interval(value_type low, value_type high)
            : low_{low}
            , high_{high}
            , actual_low_{std::min(low_, high_)}
            , actual_high_{std::max(low_, high_)}
        {
        }

        /**
         *  Returns if both intervals equal.
         */
        friend bool operator==(unordered_interval const& lhs, unordered_interval const& other)
        {
            return lhs.low_ == other.low_ && lhs.high_ == other.high_;
        }

        /**
         *  Returns if both intervals are different.
         */
        friend bool operator!=(unordered_interval const& lhs, unordered_interval const& other)
        {
            return lhs.low_ != other.low_ || lhs.high_ != other.high_;
        }

        /**
         *  Returns the lower bound of the interval
         */
        value_type low() const
        {
            return actual_low_;
        }

        /**
         *  Returns the upper bound of the interval
         */
        value_type high() const
        {
            return actual_high_;
        }

        /**
         *  Returns the low and high border that were actually passed.
         */
        std::pair <value_type, value_type> unordered() const
        {
            return {low_, high_};
        }

        /**
         *  Returns whether the intervals overlap.
         *  For when both intervals are closed.
         */
        bool overlaps(value_type l, value_type h) const
        {
            return low() <= h && l <= high();
        }

        /**
         *  Returns whether the intervals overlap, excluding border.
         *  For when at least one interval is open (l&r).
         */
        bool overlaps_exclusive(value_type l, value_type h) const
        {
            return low() < h && l < high();
        }

        /**
         *  Returns whether the intervals overlap
         */
        bool overlaps(unordered_interval const& other) const
        {
            return overlaps(other.low(), other.high());
        }

        /**
         *  Returns whether the intervals overlap, excluding border.
         */
        bool overlaps_exclusive(unordered_interval const& other) const
        {
            return overlaps_exclusive(other.low(), other.high());
        }

        /**
         *  Returns whether the given value is in this.
         */
        bool within(value_type value) const
        {
            return interval_kind::within(low(), high(), value);
        }

        /**
         *  Returns whether the given interval is in this.
         */
        bool within(unordered_interval const& other) const
        {
            return low() <= other.low() && high() >= other.high();
        }

        /**
         *  Calculates the distance between the two intervals.
         *  Overlapping intervals have 0 distance.
         */
        value_type operator-(unordered_interval const& other) const
        {
            if (overlaps(other))
                return 0;
            if (high() < other.low())
                return other.low() - high();
            else
                return low() - other.high();
        }

        /**
         *  Returns the size of the interval.
         */
        value_type size() const
        {
            return high() - low();
        }

        /**
         *  Creates a new interval from this and other, that contains both intervals and whatever
         *  is between.
         */
        unordered_interval join(unordered_interval const& other) const
        {
            return {std::min(actual_low_, other.actual_low_), std::max(actual_high_, other.actual_high_)};
        }

    private:
        value_type low_;
        value_type high_;

        value_type actual_low_;
        value_type actual_high_;
    };
}
