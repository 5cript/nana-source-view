#pragma once

#include "caret.hpp"

#include <interval-tree/interval_tree.hpp>

#include <set>
#include <vector>
#include <string_view>

namespace nana_source_view
{
    class data_store;

    enum class line_end_type
    {
        LF,
        CR,
        CRLF
    };

    /**
     *  A basis for navigator classes. Navigation can be language dependent.
     *  The basic_navigator aims to be useful by its own in a utf8 context.
     *  If one wants to extend the capability beyond utf8, deriving this is a necessity.
     *  Navigating the caret is tightly coupled to how the user actually navigates via input devices.
     *
     *  This gives the navigator the responsibility to make, destroy or modify carets within constraints.
     *  Instead of being offset based, the navigator is also using (X,Y) coordination (line representation).
     */
    class basic_navigator
    {
        friend data_store;

    public:
        using caret_type = caret<>;

        /**
         *  These are used for Ctrl Navigation.
         */
        enum class basic_character_classes
        {
            identifier,
            op/*erator*/, // is a keyword :/
            whitespace,
            other
        };

        basic_navigator(data_store* store);
        basic_navigator(data_store const* store);
        virtual ~basic_navigator();

        basic_navigator(basic_navigator const&);
        basic_navigator(basic_navigator&&);

        basic_navigator& operator=(basic_navigator const&);
        basic_navigator& operator=(basic_navigator&&);

        /**
         *  Arrow left action. Moves all carets in the editor.
         */
        void arrow_left(bool shift, bool ctrl);

        /**
         *  Arrow right action. Moves all carets in the editor.
         */
        void arrow_right(bool shift, bool ctrl);

        /**
         *  Arrow up action. Moves all carets in the editor.
         */
        void arrow_up(bool shift, bool ctrl);

        /**
         *  Arrow down action. Moves all carets in the editor.
         */
        void arrow_down(bool shift, bool ctrl);

    protected:
        /**
         *  Goes an entire complete character left. Respects utf-8 encoding.
         *  @param from the origin to go left from.
         */
        caret_type::index_type utf8_go_left(caret_type::index_type from) const;

        /**
         *  Goes an entire complete character right. Respects utf-8 encoding.
         *  @param from the origin to go right from.
         */
        caret_type::index_type utf8_go_right(caret_type::index_type from) const;

        /**
         *  Goes an entire class of characters left, like an entire identifier or a set of operators.
         */
        caret_type::index_type utf8_go_left_class(caret_type::index_type from) const;

        /**
         *  Goes an entire class of characters right, like an entire identifier or a set of operators.
         */
        caret_type::index_type utf8_go_right_class(caret_type::index_type from) const;

        virtual void arrow_left_impl(bool shift, bool ctrl);
        virtual void arrow_right_impl(bool shift, bool ctrl);
        virtual void arrow_up_impl(bool shift, bool ctrl);
        virtual void arrow_down_impl(bool shift, bool ctrl);

    private:
        basic_character_classes assess_class(caret_type::index_type offset) const;

    private:
        data_store* store;
    };

    class data_store
    {
    public:
        friend class basic_navigator;

        // todo: std::set is not optimal, boost::flat_set is a dependency, make own container (maybe)
        using caret_type = caret<>;
        using byte_type = char;
        using index_type = caret_type::index_type;
        using codepage_character = int32_t;
        using caret_container_type = std::set <caret_type>;
        using byte_container_type = std::vector <byte_type>;
        using iterator = byte_container_type::iterator;
        using const_iterator = byte_container_type::const_iterator;

        using caret_iterator = caret_container_type::iterator;

    public:
        /**
         *  Creates the data store, puts data inside and sets the caret to the end of the data.
         */
        explicit data_store(byte_container_type initial_data);

        /**
         *  Creates the data store and places the initial caret and data
         */
        explicit data_store(byte_container_type initial_data, caret_type initial_caret);

        /**
         *  Creates the data store and places the initial caret.
         */
        explicit data_store(caret_type initial_caret);

        /**
         *  Creates the data store and places the initial caret at the end and data.
         */
        explicit data_store(std::basic_string_view <byte_type> const& view);

        /**
         *  nothing special to decon.
         */
        ~data_store() = default;

        /**
         *  Sets the line ending. LF by default.
         */
        void set_line_end(line_end_type let);

        /**
         *  Insert a byte at all carets.
         *  Does overwrite if a caret is a range.
         */
        void insert_byte(byte_type byte);

        const_iterator begin() const;
        const_iterator end() const;

        const_iterator cbegin() const;
        const_iterator cend() const;

        /**
         *  Returns the amount of bytes in the store.
         */
        std::size_t size() const;

        /**
         *  Returns true when the store is empty.
         */
        bool empty() const;

        /**
         *  Returns the contents as utf8.
         */
        std::string utf8_string() const;

        /**
         *  Sets the text and resets all carets.
         */
        void utf8_string(std::string_view const& text);

        /**
         *  Direct access to data via index.
         */
        byte_type operator[](caret_type::index_type pos) const;

        /**
         *  Retrieves a character from the given position.
         *  Since character can be multi-byte, return a wide type.
         *
         *  Does NOT assume to be at a valid position and corrects it automatically by jumping one back and then back forth.
         *  Does perform bounds checking.
         */
        codepage_character utf8_character_safe(caret_type::index_type pos) const;

        /**
         *  Retrieves a character from the given position.
         *  Since character can be multi-byte, return a wide type.
         *
         *  WARNING! Assumes that pos is at a valid position, and not WITHIN a character
         *  WARNING! Does not perform bounds checking!
         */
        codepage_character utf8_character_fast(caret_type::index_type pos) const;

        /**
         *  Inserts a caret into the caret collection. Uses text offset.
         */
        void add_caret(caret_type::index_type pos, caret_type::index_type range = 0);

        /**
         *  Removes a caret from the store. Be careful not to remove all of them without adding one.
         */
        void remove_caret(caret_iterator c);

        caret_iterator caret_begin() const;
        caret_iterator caret_end() const;

        /**
         *  Retrieves the amount of carets
         */
        std::size_t caret_count() const;

        /**
         *  Get all carets.
         */
        caret_container_type retrieve_carets() const;

        /**
         *  Clears data and carets. Leaves a single caret at 0.
         */
        void clear();

        /**
         * Retrieves the line from the given index.
         * Likely costly.
         */
        index_type line_from_index(index_type index) const;

        /**
         * Retrieves the index from a given line.
         * Likely costly.
         */
        index_type index_from_line(index_type line) const;

        /**
         * @brief line_count Returns the amount of lines in the store
         * @return A number of lines.
         */
        std::size_t line_count() const;
    private:
        struct low_level_ops
        {
            /**
             *  Removes a range from the byte_container. Does not update carets etc.
             */
            static void remove(byte_container_type& data, caret_type const& car);

            /**
             *  Inserts a byte at a caret position into the container. Does not update carets etc.
             */
            static void insert(byte_container_type& data, caret_type const& car, byte_type byte);
        };
        /**
         *  Fills the "line_ends" set with a list of all line endings.
         *  Necessary on loading an entire block of text.
         */
        void reform_line_end_tree();

    private:

        /**
         *  Insert a byte at each caret and update all.
         */
        void insert_byte_multi_caret(byte_type byte);

        /**
         *  Removes range from store and updates the given caret.
         */
        caret_type remove_range_single_caret(caret_type car);

    private:
        struct line_container
        {
            /// Index position in data_store where this line begins
            index_type index;

            /// Which line is this? | Count of all lineendings before this.
            index_type line;

            bool operator<(line_container const& other) const
            {
                return index < other.index;
            }

            struct line_number_compare
            {
                bool operator()(line_container const& lhs, line_container const& rhs) const
                {
                    return lhs.line < rhs.line;
                }
            };
        };

        byte_container_type data;
        caret_container_type carets;
        line_end_type let;

        // TODO: Better container possibly needed. Candidates: boost::flat_set or maybe even a b-tree?
        std::set <line_container> line_ends_index_sorted;

        // TODO: Better container might by vector, depends on how much modifications are made to it.
        std::set <line_container, line_container::line_number_compare> line_ends_line_sorted;
    };
}
