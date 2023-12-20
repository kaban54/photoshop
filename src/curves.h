#ifndef CURVES_H
#define CURVES_H

#include "pluginwidget.h"
#include "mylist.h"

using namespace plugin;

class CurveWid : public PluginWidget {
    static const double POINT_RADIUS;
    static const size_t MAX_POINTS = 32;
    static const Color BG_COLOR;

    Vec2 size;
    Color color;
    MyVector<Vec2> points;

    int moving_point;

    void move_point(Vec2 mpos);

    public:

    CurveWid(GuiI* gui, Vec2 pos_, Vec2 size_);

    void SetColor(Color col) {color = col;}

    void GetValues(uint8_t values[256]);

    virtual bool onMousePress     (MouseContext    context) override;
    virtual bool onMouseRelease   (MouseContext    context) override;
    virtual bool onMouseMove      (MouseContext    context) override;
    virtual bool onKeyboardPress  (KeyboardContext context) override {
        if (context.key != Key::Q) return false;
        uint8_t values[255];
        GetValues(values);
        for (size_t i = 0; i < 256; i++) {
            std::cerr << "val[" << i << "] = " << int(values[i]) << "\n";
        }
        return false;
    }
    virtual bool onKeyboardRelease(KeyboardContext context) override {return false;}
    virtual bool onClock          (uint64_t delta)          override {return false;}

    virtual void render(RenderTargetI* rt) override;
};

Vec2 Get256Coords(Vec2 point, Vec2 size);
void get_values (uint8_t values[256], Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3);
void get_values (uint8_t values[256], Vec2 p0, Vec2 p1, Vec2 p2         );
void get_values (uint8_t values[256], Vec2 p0, Vec2 p1                  );


struct CurvesApplyBtnArgs : public BtnArgs {
    RenderTargetI* data;
    CurveWid* r_curve;
    CurveWid* g_curve;
    CurveWid* b_curve;
    PluginWindow* win;
};

class CurvesApplyBtn : public PluginTxtButton {
    CurvesApplyBtnArgs args;
    public:

    CurvesApplyBtn(GuiI* gui, Vec2 pos, Vec2 size, CurvesApplyBtnArgs args_);
};

struct CloseBtnArgs : public BtnArgs {
    PluginWindow* win;
};

class CloseBtn : public PluginTxtButton {
    CloseBtnArgs args;
    public:

    CloseBtn(GuiI* gui, Vec2 pos, Vec2 size, PluginWindow* win);
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