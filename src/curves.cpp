#include "curves.h"

CurvesFilter::CurvesFilter(GuiI* gui_):
    gui (gui_)
    {}

void CurvesFilter::apply (RenderTargetI *data) {
    PluginWindow* win = new PluginWindow(gui, Vec2(100, 100), Vec2(400, 400));
    gui -> getRoot() -> registerSubWidget(win -> host);
}

Array<const char *> CurvesFilter::getParamNames() const {
    return Array<const char *> (0, nullptr);
}

Array<double> CurvesFilter::getParams() const {
    return Array<double> (0, nullptr);
}

void CurvesFilter::setParams(Array<double> new_params) {
    assert(new_params.size == 0);
}


CurvesFilterPlugin::CurvesFilterPlugin(App* app_):
    app (app_)
    {
        id = (1ull << 54) + 'c' + 'u' * 'r' + 'v' * 'e' + 's';
        name = "Curves";
        type = InterfaceType::Filter;
        filter = new CurvesFilter(app -> root);
    }

CurvesFilterPlugin::~CurvesFilterPlugin() {
    delete filter;
}


extern "C" plugin::Plugin* getInstance(plugin::App *app) {
    return new CurvesFilterPlugin(app);
}