#include <nana-source-view/skeleton/source_view_impl.hpp>

#include <nana-source-view/abstractions/store.hpp>

namespace nana_source_view::skeletons
{
//#####################################################################################################################
    struct source_editor_impl::implementation
    {
        implementation();

        data_store store;
        nana::rectangle area;
    };
//---------------------------------------------------------------------------------------------------------------------
    source_editor_impl::implementation::implementation()
        : store{data_store::byte_container_type{}}
    {

    }
//#####################################################################################################################
    source_editor_impl::source_editor_impl(nana::window wd, graph_reference graph, skeletons::source_editor_scheme const* scheme)
        : impl_{new implementation}
        , renderer_{&impl_->store}
        , window_{wd}
        , graph_{graph}
        , scheme_{scheme}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    source_editor_impl::~source_editor_impl() = default;
//---------------------------------------------------------------------------------------------------------------------
    void source_editor_impl::render(bool focused)
    {
        auto const bgcolor = bgcolor_();

        auto fgcolor = scheme_->foreground.get_color();

        // TODO: Improve
        // Blending for disabled window.
        if (!nana::API::window_enabled(window_))
            fgcolor = fgcolor.blend(bgcolor, 0.5);

        if (nana::API::widget_borderless(window_))
        {
            graph_.rectangle(true, bgcolor);
        }
        else
        {
            graph_.rectangle(true, bgcolor);
        }

        renderer_.render(graph_);
    }
//---------------------------------------------------------------------------------------------------------------------
    ::nana::color source_editor_impl::bgcolor_() const
    {
        // TODO: Improve
        // - Disabled Color should be configurable
        // - Maybe dont use nana scheming system for background colors
        //return (!nana::API::window_enabled(window_) ? static_cast<nana::color_rgb>(0xE0E0E0) : nana::API::bgcolor(window_));
        return static_cast <nana::color_rgb>(0x303030);
    }
//---------------------------------------------------------------------------------------------------------------------
    void source_editor_impl::area(nana::rectangle const& rect)
    {
        impl_->area = rect;
    }
//---------------------------------------------------------------------------------------------------------------------
    void source_editor_impl::text(std::string_view const& text)
    {
        impl_->store.utf8_string(text);
    }
//---------------------------------------------------------------------------------------------------------------------
    bool source_editor_impl::try_refresh()
    {
        return true;
    }
//#####################################################################################################################
}
