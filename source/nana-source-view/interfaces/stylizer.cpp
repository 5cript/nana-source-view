#include <nana-source-view/interfaces/stylizer.hpp>

namespace nana_source_view
{
    void stylizer::on_line_change(index_type line)
    {
        on_multi_line_change(line, line);
    }
}
