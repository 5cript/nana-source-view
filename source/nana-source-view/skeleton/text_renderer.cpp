#include <nana-source-view/skeleton/text_renderer.hpp>

namespace nana_source_view::skeletons
{
//#####################################################################################################################
    text_renderer::text_renderer(data_store const* store)
        : store_{store}
        , area_{}
        , styler_{}
        , font_{}
        , scroll_top_{0}
    {
    }
//---------------------------------------------------------------------------------------------------------------------
    void text_renderer::text_area(nana::rectangle const& rect)
    {
        area_ = rect;
    }
//---------------------------------------------------------------------------------------------------------------------
    void text_renderer::render(text_renderer::graph_reference graph)
    {
        nana::paint::font f{12, "Tahoma", nana::paint::font::font_style{
            400, false, true, true
        }};

        graph.typeface(f);

        graph.string({0,0}, "lorem ipsum", nana::colors::white);
    }
//---------------------------------------------------------------------------------------------------------------------
    void text_renderer::update_scroll(index_type scroll_top_line)
    {
        scroll_top_ = scroll_top_line;
    }
//---------------------------------------------------------------------------------------------------------------------
    void text_renderer::font(nana::paint::font const& font, bool assume_monospace)
    {
        font_ = font;
    }
//---------------------------------------------------------------------------------------------------------------------
    nana::paint::font text_renderer::font() const
    {
        return font_;
    }
//#####################################################################################################################
}
