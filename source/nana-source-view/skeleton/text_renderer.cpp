#include <nana-source-view/skeleton/text_renderer.hpp>

namespace nana_source_view::skeletons
{
//#####################################################################################################################
    text_renderer::text_renderer(data_store const* store)
        : store_{store}
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

    }
//#####################################################################################################################
}
