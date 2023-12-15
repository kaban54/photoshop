#ifndef CURVES_H
#define CURVES_H

#include "standart2.h"

using namespace plugin;

class CurvesFilterWindow : public PluginWidgetI {
    
    static const unsigned int WIDTH  = 400;
    static const unsigned int HEIGHT = 400;

    public:

    explicit CurvesFilterWindow(GuiI* gui);

    virtual bool onMouseMove      (MouseContext    context) override {return false;}
    virtual bool onMouseRelease   (MouseContext    context) override {return false;}
    virtual bool onMousePress     (MouseContext    context) override;
    virtual bool onKeyboardPress  (KeyboardContext context) override {return false;}
    virtual bool onKeyboardRelease(KeyboardContext context) override {return false;}
    virtual bool onClock          (uint64_t delta)          override {return false;}

    virtual uint8_t getPriority() const override {return 0;}

    virtual void render(RenderTargetI* rt) override;
};



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