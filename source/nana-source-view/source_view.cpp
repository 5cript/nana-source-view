#include <nana-source-view/source_view.hpp>

namespace nana_source_view
{
//#####################################################################################################################
    struct source_editor_impl
    {
        nana::window wd;

        source_editor_impl(nana::window wd);
        source_editor_impl();
    };
//---------------------------------------------------------------------------------------------------------------------
    source_editor_impl::source_editor_impl(nana::window wd)
        : wd{std::move(wd)}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    source_editor_impl::source_editor_impl()
    {

    }
//#####################################################################################################################
    source_editor::source_editor()
        : impl_{new source_editor_impl()}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    source_editor::source_editor(nana::window wd, bool visible)
        : impl_{new source_editor_impl(std::move(wd))}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    source_editor::source_editor(nana::window wd, std::string_view const& text, bool visible)
        : impl_{new source_editor_impl(std::move(wd))}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    source_editor::source_editor(nana::window wd, nana::rectangle const& rect, bool visible)
        : impl_{new source_editor_impl(std::move(wd))}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    source_editor::source_editor(nana::window wd, nana::rectangle const& rect, std::string_view const& text, bool visible)
        : impl_{new source_editor_impl(std::move(wd))}
    {

    }
//#####################################################################################################################
}
