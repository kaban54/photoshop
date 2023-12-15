#ifndef CURVES_H
#define CURVES_H

#include "pluginwidget.h"

using namespace plugin;


class CurvesFilter : public FilterI {
    GuiI* gui;
    public:

    explicit CurvesFilter(GuiI* gui_);

    virtual void apply(RenderTargetI *data) override;

    virtual Array<const char *> getParamNames() const override;
    virtual Array<double> getParams() const override;
    virtual void setParams(Array<double> new_params) override;
};

class CurvesFilterPlugin : public Plugin {
    App *app;
    CurvesFilter* filter;

    public:

    CurvesFilterPlugin(App* app_);

    ~CurvesFilterPlugin();

    virtual Interface *getInterface() const override {return filter;}

    virtual void selectPlugin() override {};
};

#endif