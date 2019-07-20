#include <nana-source-view/abstractions/store.hpp>
#include <nana-source-view/abstractions/iterator.hpp>
#include <nana-source-view/assert/assert.hpp>

#include <nana-source-view/abstractions/detail/unordered_interval.hpp>

#include <interval-tree/interval_tree.hpp>

#include <nana/charset.hpp>

#include <stdexcept>
#include <cctype>

namespace nana_source_view
{
    namespace
    {
        template <typename T>
        T sgn(T val)
        {
            return (T(0) < val) - (val < T(0));
        }
    }

//#####################################################################################################################
    void data_store::low_level_ops::remove(byte_container_type& data, caret_type const& car)
    {
        sv_assert(static_cast <caret_type::index_type> (data.size()) > (car.offset + car.range), "cannot erase out of bounds");
        sv_assert(car.offset + car.range > 0, "cannot erase out of bounds (negative direction)");
        sv_assert(car.offset >= 0, "offset cannot be negative");
        sv_assert(car.range > 0, "cannot erase 0-range");

        auto front = data.begin() + car.offset;
        auto back = data.begin() + car.offset + car.range;
        if (car.range < 0)
            data.erase(back, front);
        else
            data.erase(front, back);
    }
//---------------------------------------------------------------------------------------------------------------------
    void data_store::low_level_ops::insert(byte_container_type& data, caret_type const& car, byte_type byte)
    {
        sv_assert(static_cast <caret_type::index_type> (data.size()) >= car.offset, "caret outside of bounds");

        data.insert(data.begin() + car.offset, byte);
    }
//#####################################################################################################################
    basic_navigator::basic_navigator(data_store* store)
        : store{store}
    {
    }
//---------------------------------------------------------------------------------------------------------------------
    basic_navigator::basic_navigator(data_store const* store)
        : store{const_cast <data_store* > (store)}
    {
    }
//---------------------------------------------------------------------------------------------------------------------
    basic_navigator::~basic_navigator() = default;
//---------------------------------------------------------------------------------------------------------------------
    basic_navigator::basic_navigator(basic_navigator const&) = default;
//---------------------------------------------------------------------------------------------------------------------
    basic_navigator::basic_navigator(basic_navigator&&) = default;
//---------------------------------------------------------------------------------------------------------------------
    basic_navigator& basic_navigator::operator=(basic_navigator const&) = default;
//---------------------------------------------------------------------------------------------------------------------
    basic_navigator& basic_navigator::operator=(basic_navigator&&) = default;
//---------------------------------------------------------------------------------------------------------------------
    void basic_navigator::arrow_left(bool shift, bool ctrl)
    {
        arrow_left_impl(shift, ctrl);
    }
//---------------------------------------------------------------------------------------------------------------------
    void basic_navigator::arrow_right(bool shift, bool ctrl)
    {
        arrow_right_impl(shift, ctrl);
    }
//---------------------------------------------------------------------------------------------------------------------
    void basic_navigator::arrow_up(bool shift, bool ctrl)
    {
        arrow_up_impl(shift, ctrl);
    }
//---------------------------------------------------------------------------------------------------------------------
    void basic_navigator::arrow_down(bool shift, bool ctrl)
    {
        arrow_down_impl(shift, ctrl);
    }
//---------------------------------------------------------------------------------------------------------------------
    void basic_navigator::arrow_left_impl(bool shift, bool ctrl)
    {
        if (!shift && !ctrl)
        {
            data_store::caret_container_type updated;
            for (auto const& c : store->carets)
            {
                // set semantics auto merge, because duplicates are impossible.
                updated.emplace(utf8_go_left(c.offset), 0);
            }
            store->carets = std::move(updated);
        }
        else if (shift && !ctrl)
        {
            // we need 2 interval trees because we need to preserve the directedness of the caret-range
            // since the tree must work with ordered interval bounds.
            // Alternatively: A custom interval
            lib_interval_tree::interval_tree <lib_interval_tree::interval <caret_type::index_type>> positive_direction_intervals;
            lib_interval_tree::interval_tree <lib_interval_tree::interval <caret_type::index_type>> negative_direction_intervals;
            for (auto const& c : store->carets)
            {
                // new range = current range - (1 to 4)
                auto new_offset = utf8_go_left(c.offset);

                if (c.range + 1 < 0)
                    negative_direction_intervals.insert_overlap(
                        lib_interval_tree::interval <caret_type::index_type>(new_offset, new_offset - c.range - 1)
                    );
                else
                    positive_direction_intervals.insert_overlap(
                        lib_interval_tree::interval <caret_type::index_type>(new_offset, new_offset + c.range + 1)
                    );
            }

            store->carets.clear();

            // There CANNOT be any duplicates, where cursors overlap.
            // If there are, this means the deoverlap of the interval trees was faulty, which is tested.
            for (auto const& i : positive_direction_intervals)
                store->carets.emplace(i.low(), i.size());
            for (auto const& i : negative_direction_intervals)
                store->carets.emplace(i.high(), -i.size());
        }
        else if (!shift && ctrl)
        {
            data_store::caret_container_type updated;
            for (auto const& c : store->carets)
            {
                if (c.offset <= 1)
                {
                    updated.emplace(0, 0);
                    continue;
                }

                updated.emplace(utf8_go_left_class(c.offset), 0);
            }
            store->carets = std::move(updated);
        }
        else // (shift && ctrl)
        {
            lib_interval_tree::interval_tree <detail::unordered_interval <caret_type::index_type>> itree;
            for (auto const& c : store->carets)
            {
                auto start_point = c.offset + c.range;

                itree.insert_overlap({c.offset, utf8_go_left_class(start_point)});
            }

            store->carets.clear();
            for (auto const& i : itree)
            {
                auto pair = i.unordered();
                store->carets.insert(caret_type{pair.first, pair.second - pair.first});
            }
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    basic_navigator::caret_type::index_type basic_navigator::utf8_go_left_class(caret_type::index_type from) const
    {
        from = utf8_go_left(from);

        // 1) skip whitespace
        for(auto ch = store->operator[](from);
            from != 0 &&
            std::isspace(ch) &&
            ch != '\r' &&
            ch != '\n';
        )
        {
            from = utf8_go_left(from);
            ch = store->operator[](from);
        }

        if (from == 0)
        {
            return 0;
        }

        // 2) go as long as same class
        auto clas = assess_class(from);

        for
        (
            auto clasSearch = clas;
            from != 0 && clasSearch == clas;
        )
        {
            from = utf8_go_left(from);
            clasSearch = assess_class(from);
        }

        return from + 1;
    }
//---------------------------------------------------------------------------------------------------------------------
    basic_navigator::caret_type::index_type basic_navigator::utf8_go_right_class(caret_type::index_type from) const
    {
        if (from == static_cast <caret_type::index_type> (store->size()))
        {
            return store->size();
        }

        // go as long as same class
        auto clas = assess_class(from);
        decltype(clas) nextClass;

        do {
            from = utf8_go_right(from);
            nextClass = assess_class(from);
        } while (nextClass == clas);

        // skip whitespace, except \n \r
        for(auto ch = store->operator[](from); from != static_cast <caret_type::index_type> (store->size()) &&
            std::isspace(ch) &&
            ch != '\r' &&
            ch != '\n';
        )
        {
            from = utf8_go_right(from);
            ch = store->operator[](from);
        }

        return from;
    }
//---------------------------------------------------------------------------------------------------------------------
    void basic_navigator::arrow_right_impl(bool shift, bool ctrl)
    {
        if (!shift && !ctrl)
        {
            data_store::caret_container_type updated;
            if (!shift && !ctrl)
            {
                for (auto const& c : view::reverse(store->carets))
                {
                    updated.emplace(utf8_go_right(c.offset), 0);
                }
            }
            store->carets = std::move(updated);
        }
        else if (shift && !ctrl)
        {
            // we need 2 interval trees because we need to preserve the directedness of the caret-range
            // since the tree must work with ordered interval bounds.
            // Alternatively: A custom interval
            lib_interval_tree::interval_tree <lib_interval_tree::interval <caret_type::index_type>> positive_direction_intervals;
            lib_interval_tree::interval_tree <lib_interval_tree::interval <caret_type::index_type>> negative_direction_intervals;
            for (auto const& c : view::reverse(store->carets))
            {
                auto new_offset = utf8_go_right(c.offset);

                if (c.range - 1 < 0)
                    negative_direction_intervals.insert_overlap(
                        lib_interval_tree::interval <caret_type::index_type>(new_offset, new_offset - c.range + 1)
                    );
                else
                    positive_direction_intervals.insert_overlap(
                        lib_interval_tree::interval <caret_type::index_type>(new_offset, new_offset + c.range - 1)
                    );
            }

            store->carets.clear();

            //std::cout << positive_direction_intervals.size() << "\n";
            //std::cout << negative_direction_intervals.size() << "\n";

            // There CANNOT be any duplicates, where cursors overlap.
            // If there are, this means the deoverlap of the interval trees was faulty, which is tested.
            for (auto const& i : positive_direction_intervals)
                store->carets.emplace(i.low(), i.size());
            for (auto const& i : negative_direction_intervals)
                store->carets.emplace(i.low(), -i.size());
        }
        else if (!shift && ctrl)
        {
            data_store::caret_container_type updated;
            for (auto const& c : view::reverse(store->carets))
            {
                if (c.offset >= static_cast <caret_type::index_type> (store->size()))
                {
                    updated.emplace(store->size(), 0);
                    continue;
                }
                updated.emplace(utf8_go_right_class(c.offset), 0);
            }
            store->carets = std::move(updated);
        }
        else // (shift && ctrl)
        {
            lib_interval_tree::interval_tree <detail::unordered_interval <caret_type::index_type>> itree;
            for (auto const& c : view::reverse(store->carets))
            {
                auto start_point = c.offset + c.range;

                itree.insert_overlap({c.offset, utf8_go_right_class(start_point)});
            }

            store->carets.clear();
            for (auto const& i : itree)
            {
                auto pair = i.unordered();
                store->carets.insert(caret_type{pair.first, pair.second - pair.first});
            }

        }
    }
//---------------------------------------------------------------------------------------------------------------------
    basic_navigator::basic_character_classes basic_navigator::assess_class(caret_type::index_type offset) const
    {
        //auto ch = store->utf8_character_fast(offset);

        // is sufficient, since all utf-8 chars that are checked are within single byte range.
        auto ch = store->operator[](offset);
        if (std::isspace(ch))
            return basic_character_classes::whitespace;

        if (std::isalnum(ch) || ch == '_')
            return basic_character_classes::identifier;

        if (std::isgraph(ch))
            return basic_character_classes::op;

        return basic_character_classes::other;
    }
//---------------------------------------------------------------------------------------------------------------------
    basic_navigator::caret_type::index_type basic_navigator::utf8_go_left(caret_type::index_type from) const
    {
        sv_assert(from >= 0, "origin of movement cannot be beyond store size (negative)");

        if (from == 0)
            return 0;

        caret_type::index_type i = 1;
        for (; i != static_cast <caret_type::index_type> (5) && ((*store)[from-i] & 0b1100'0000) == 0b1000'0000; ++i)
        {
        }

        return from - std::min(i, static_cast <caret_type::index_type> (4));
    }
//---------------------------------------------------------------------------------------------------------------------
    basic_navigator::caret_type::index_type basic_navigator::utf8_go_right(caret_type::index_type from) const
    {
        sv_assert(from <= static_cast <caret_type::index_type> (store->size()), "origin of movement cannot be beyond store size");

        if (from >= static_cast <caret_type::index_type> (store->size()))
            return static_cast <caret_type::index_type> (store->size());

        if ((store->operator[](from) & 0b1000'0000) == 0)
            return from+1;

        if ((store->operator[](from) & 0b1100'0000) == 0b1000'0000)
        {
            int i = 0;
            for (; i != 3 && ((store->operator[](from+i) & 0b1100'0000) == 0b1000'0000); ++i)
            {
            }
            return from+i;
        }

        if ((store->operator[](from) & 0b1110'0000) == 0b1100'0000)
            return from+2;

        if ((store->operator[](from) & 0b1111'0000) == 0b1110'0000)
            return from+3;

        return from+4;
    }
//---------------------------------------------------------------------------------------------------------------------
    void basic_navigator::arrow_up_impl(bool shift, bool ctrl)
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void basic_navigator::arrow_down_impl(bool shift, bool ctrl)
    {

    }
//#####################################################################################################################
    data_store::data_store(byte_container_type initial_data, caret_type initial_caret)
        : data{std::move(initial_data)}
        , carets{std::move(initial_caret)}
        , let{line_end_type::LF}
        , line_ends{}
    {
        reform_line_end_tree();
    }
//---------------------------------------------------------------------------------------------------------------------
    data_store::data_store(caret_type initial_caret)
        : data_store(byte_container_type{}, std::move(initial_caret))
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    data_store::data_store(std::basic_string_view <byte_type> const& view)
        : data(view.size())
        , carets{{static_cast <caret_type::index_type> (view.size()), 0}}
    {
        view.copy(&data[0], view.size());
        reform_line_end_tree();
    }
//---------------------------------------------------------------------------------------------------------------------
    data_store::data_store(byte_container_type initial_data)
        : data(std::move(initial_data))
        , carets{{static_cast <caret_type::index_type> (data.size()), 0}}
    {
        reform_line_end_tree();
    }
//---------------------------------------------------------------------------------------------------------------------
    data_store::byte_type data_store::operator[](caret_type::index_type pos) const
    {
        if (pos >= static_cast <caret_type::index_type> (data.size()))
            throw std::out_of_range("index out of bounds");
        return data[static_cast <std::size_t> (pos)];
    }
//---------------------------------------------------------------------------------------------------------------------
    void data_store::set_line_end(line_end_type let)
    {
        this->let = let;
    }
//---------------------------------------------------------------------------------------------------------------------
    data_store::codepage_character data_store::utf8_character_fast(caret_type::index_type pos) const
    {
        // use the basic_navigator to get an entire char:
        const basic_navigator nav(this);
        auto wideness = nav.utf8_go_right(pos) - pos;

        sv_assert(
            pos + wideness <= static_cast <caret_type::index_type> (data.size()),
                  "Character end cannot be exceeding store size"
        );

        if (wideness == 1)
            return data[pos];

        if (wideness == 2)
            return  (static_cast <codepage_character> (data[pos + 1]  & 0b0011'1111)) |
                    ((data[pos] & 0b0001'1111) << 6)
            ;

        if (wideness == 3)
            return  (static_cast <codepage_character> (data[pos + 2]  & 0b0011'1111)) |
                    ((data[pos + 1] & 0b0011'1111) << 6) |
                    ((data[pos] & 0b0000'1111) << 12)
            ;

        if (wideness == 4)
            return  (static_cast <codepage_character> (data[pos + 3]  & 0b0011'1111)) |
                    ((data[pos + 2] & 0b0011'1111) << 6) |
                    ((data[pos + 1] & 0b0011'1111) << 12) |
                    ((data[pos] & 0b0000'0111) << 18)
            ;

        throw std::runtime_error("utf8 character encoding is invalid");
    }
//---------------------------------------------------------------------------------------------------------------------
    data_store::codepage_character data_store::utf8_character_safe(caret_type::index_type pos) const
    {
        if (pos >= static_cast <caret_type::index_type> (data.size()))
            throw std::out_of_range("index out of bounds");
        if (pos < 0)
            throw std::out_of_range("index out of bounds");

        basic_navigator nav(this);
        auto corr_pos = nav.utf8_go_right(nav.utf8_go_left(pos));
        if (corr_pos != pos)
            corr_pos = nav.utf8_go_left(pos);

        return utf8_character_fast(corr_pos);
    }
//---------------------------------------------------------------------------------------------------------------------
    std::size_t data_store::size() const
    {
        return data.size();
    }
//---------------------------------------------------------------------------------------------------------------------
    bool data_store::empty() const
    {
        return data.empty();
    }
//---------------------------------------------------------------------------------------------------------------------
    data_store::const_iterator data_store::begin() const
    {
        return data.cbegin();
    }
//---------------------------------------------------------------------------------------------------------------------
    data_store::const_iterator data_store::end() const
    {
        return data.cend();
    }
//---------------------------------------------------------------------------------------------------------------------
    data_store::const_iterator data_store::cbegin() const
    {
        return data.cbegin();
    }
//---------------------------------------------------------------------------------------------------------------------
    data_store::const_iterator data_store::cend() const
    {
        return data.cend();
    }
//---------------------------------------------------------------------------------------------------------------------
    data_store::caret_iterator data_store::caret_begin() const
    {
        return carets.cbegin();
    }
//---------------------------------------------------------------------------------------------------------------------
    data_store::caret_iterator data_store::caret_end() const
    {
        return carets.cend();
    }
//---------------------------------------------------------------------------------------------------------------------
    void data_store::remove_caret(caret_iterator c)
    {
        carets.erase(c);
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string data_store::utf8_string() const
    {
        return nana::charset(std::string{std::begin(data), std::end(data)}).to_bytes(nana::unicode::utf8);
    }
//---------------------------------------------------------------------------------------------------------------------
    void data_store::insert_byte(byte_type byte)
    {
        sv_assert(!carets.empty(), "a data store should always have a caret");

        // use simplified algorithm for single caret operation
        if (carets.size() > 1)
            return insert_byte_multi_caret(byte);

        // single caret editing
        auto caret_iter = carets.begin();
        auto car = *caret_iter;
        if (caret_iter->is_range())
        {
            carets.erase(caret_iter);
            carets.insert(remove_range_single_caret(car));
        }

        low_level_ops::insert(data, car, byte);
    }
//---------------------------------------------------------------------------------------------------------------------
    data_store::caret_type data_store::remove_range_single_caret(caret_type car)
    {
        sv_assert(carets.size() == 1, "this function should only be called from a context where there is only one caret.");
        sv_assert(car.range > 0, "this function should only be called if the give caret has a range.");

        low_level_ops::remove(data, car);

        // update caret:
        if (car.range < 0)
            car.offset = std::max(static_cast <caret_type::index_type> (0), car.offset + car.range);
        else
            car.offset = std::max(static_cast <caret_type::index_type> (0), car.offset - car.range);
        car.range = 0;

        return car;
    }
//---------------------------------------------------------------------------------------------------------------------
    void data_store::add_caret(caret_type::index_type pos, caret_type::index_type range)
    {
        if (pos > static_cast <caret_type::index_type> (data.size()))
            throw std::out_of_range("index out of bounds");
        if (pos < 0)
            throw std::out_of_range("index out of bounds");

        carets.insert(caret_type{pos, range});
    }
//---------------------------------------------------------------------------------------------------------------------
    void data_store::insert_byte_multi_caret(byte_type byte)
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    std::size_t data_store::caret_count() const
    {
        return carets.size();
    }
//---------------------------------------------------------------------------------------------------------------------
    data_store::caret_container_type data_store::retrieve_carets() const
    {
        return carets;
    }
//---------------------------------------------------------------------------------------------------------------------
    void data_store::clear()
    {
        carets.clear();
        data.clear();

        carets.insert({0, 0});
    }
//---------------------------------------------------------------------------------------------------------------------
    void data_store::reform_line_end_tree()
    {
        // add fake character at end to avoid checks against eol.
        // this can cost some performance if data has to be resized, but its worth it on average.
        if (let == line_end_type::CRLF)
            data.push_back(' ');

        auto is_lf = [](auto const& dataStore, std::size_t offset)
        {
            return dataStore[offset] == '\n';
        };
        auto is_cr = [](auto const& dataStore, std::size_t offset)
        {
            return dataStore[offset] == '\r';
        };
        auto is_crlf = [](auto const& dataStore, std::size_t offset)
        {
            return dataStore[offset] == '\r' && dataStore[offset + 1] == '\n';
        };

        auto impl = [this](auto const fn)
        {
            auto end = data.size();
            if (let == line_end_type::CRLF)
                --end;

            for (decltype(data)::size_type i = 0; i != end; ++i)
                if (fn(data, i))
                    line_ends.insert(static_cast <index_type> (i));
        };

        auto impl2 = [&, this]()
        {
            switch (let)
            {
            case (line_end_type::CR):
                return impl(is_cr);

            case (line_end_type::LF):
                return impl(is_lf);

            case (line_end_type::CRLF):
                return impl(is_crlf);
            }
        };
        impl2();

        if (let == line_end_type::CRLF)
            data.pop_back();
    }
//#####################################################################################################################
}
