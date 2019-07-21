#include <nana-source-view/source_view.hpp>
#include <nana-source-view/skeleton/source_view_impl.hpp>

namespace nana_source_view::drawerbase::source_editor
{

//#####################################################################################################################
    event_agent::event_agent(::nana_source_view::source_editor& editor)
        : widget_{editor}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void event_agent::on_load()
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void event_agent::text_changed()
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void event_agent::selection_changed()
    {

    }
//#####################################################################################################################
    drawer::drawer()
        : widget_{nullptr}
        , editor_{nullptr}
        , evt_agent_{nullptr}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    skeletons::source_editor_impl* drawer::editor()
    {
        return editor_.get();
    }
//---------------------------------------------------------------------------------------------------------------------
    skeletons::source_editor_impl const* drawer::editor() const
    {
        return editor_.get();
    }
//---------------------------------------------------------------------------------------------------------------------
    void drawer::attached(widget_reference wdg, drawer::graph_reference graph)
    {
        auto wd = wdg.handle();
        widget_ = &wdg;

        auto scheme = nana::API::dev::get_scheme(wdg);

        editor_ = std::make_unique <skeletons::source_editor_impl> (
            wd, graph, dynamic_cast<skeletons::source_editor_scheme*>(scheme)
        );
    }
//---------------------------------------------------------------------------------------------------------------------
    void drawer::detached()
    {
        editor_.reset(nullptr);
    }
//---------------------------------------------------------------------------------------------------------------------
    void drawer::refresh(graph_reference)
    {
        editor_->render(nana::API::is_focus_ready(*widget_));
    }
//---------------------------------------------------------------------------------------------------------------------
    void drawer::focus(graph_reference, const nana::arg_focus&)
    {
        //if (!editor_->focus_changed(arg))
        //    refresh(graph);

        nana::API::dev::lazy_refresh();
    }
//---------------------------------------------------------------------------------------------------------------------
    void drawer::mouse_down(graph_reference, const nana::arg_mouse&)
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void drawer::mouse_move(graph_reference, const nana::arg_mouse&)
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void drawer::mouse_up(graph_reference, const nana::arg_mouse&)
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void drawer::mouse_enter(graph_reference, const nana::arg_mouse&)
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void drawer::mouse_leave(graph_reference, const nana::arg_mouse&)
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void drawer::dbl_click(graph_reference, const nana::arg_mouse&)
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void drawer::key_press(graph_reference, const nana::arg_keyboard&)
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void drawer::key_char(graph_reference, const nana::arg_keyboard&)
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void drawer::mouse_wheel(graph_reference, const nana::arg_wheel&)
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void drawer::resized(graph_reference, const nana::arg_resized&)
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void drawer::typeface_changed(graph_reference)
    {

    }
//#####################################################################################################################
}

namespace nana_source_view
{
//#####################################################################################################################
    arg_source_editor_basic::arg_source_editor_basic(source_editor& widget)
        : widget{widget}
    {

    }
//#####################################################################################################################
    arg_source_editor_change::arg_source_editor_change
    (
        source_editor& widget,
        std::vector <caret_type> carets_before,
        std::vector <caret_type> carets_after
    )
        : widget{widget}
        , carets_before{carets_before}
        , carets_after{carets_after}
    {

    }
//#####################################################################################################################
    struct source_editor::implementation
    {
        nana::window wd;

        implementation(nana::window wd);
        implementation();
    };
//---------------------------------------------------------------------------------------------------------------------
    source_editor::implementation::implementation(nana::window wd)
        : wd{std::move(wd)}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    source_editor::implementation::implementation()
    {

    }
//#####################################################################################################################
    source_editor::source_editor()
        : impl_{new implementation()}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    source_editor::~source_editor() = default;
    //---------------------------------------------------------------------------------------------------------------------
    source_editor::source_editor(nana::window wd, bool visible)
        : impl_{new implementation(std::move(wd))}
    {
        create(wd, nana::rectangle(), visible);
    }
//---------------------------------------------------------------------------------------------------------------------
    source_editor::source_editor(nana::window wd, std::string_view const& text, bool visible)
        : impl_{new implementation(std::move(wd))}
    {
        nana::throw_not_utf8(text);
        create(wd, nana::rectangle(), visible);
        caption(text);
    }
//---------------------------------------------------------------------------------------------------------------------
    source_editor::source_editor(nana::window wd, nana::rectangle const& rect, bool visible)
        : impl_{new implementation(std::move(wd))}
    {
        create(wd, rect, visible);
    }
//---------------------------------------------------------------------------------------------------------------------
    source_editor::source_editor(nana::window wd, nana::rectangle const& rect, std::string_view const& text, bool visible)
        : impl_{new implementation(std::move(wd))}
    {
        nana::throw_not_utf8(text);
        create(wd, rect, visible);
        caption(text);
    }
//---------------------------------------------------------------------------------------------------------------------
    void source_editor::text(std::string_view const& text)
    {
        caption(text);
    }
//---------------------------------------------------------------------------------------------------------------------
    void source_editor::caption(std::string_view const& text)
    {
        nana::internal_scope_guard lock;
        auto editor = get_drawer_trigger().editor();
        if (editor)
        {
            editor->text(text);

            if (editor->try_refresh())
                nana::API::update_window(this->handle());
        }
    }
//#####################################################################################################################
}
