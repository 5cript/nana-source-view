#pragma once

namespace nana_source_view::view
{
    template <typename T>
    struct reverse_wrapper
    {
        T& t;
        reverse_wrapper(T& t)
            : t{t}
        {
        }

        typename T::reverse_iterator begin() {return t.rbegin();}
        typename T::reverse_iterator end() {return t.rend();}
    };

    template <typename T>
    reverse_wrapper<T> reverse(T& t)
    {
        return reverse_wrapper<T>{t};
    }

    template <typename T>
    struct caret_access_wrapper
    {
        T& t;
        caret_access_wrapper(T& t)
            : t{t}
        {
        }

        typename T::reverse_iterator begin() {return t.caret_begin();}
        typename T::reverse_iterator end() {return t.caret_end();}
    };

    template <typename T>
    caret_access_wrapper<T> carets(T& t)
    {
        return caret_access_wrapper<T>{t};
    }
}
