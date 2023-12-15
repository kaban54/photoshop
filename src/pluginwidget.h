#ifndef PLUGINWIDGET_H
#define PLUGINWIDGET_H

#include "standart2.h"

using namespace plugin;

struct PluginWidget : public PluginWidgetI {
    explicit PluginWidget (GuiI* gui, Vec2 pos, Vec2 size);

    virtual uint8_t getPriority() const override {return 0;}
};

class PluginWindow : public PluginWidget {

    static const Color BG_COLOR;
    static const Color BAR_COLOR;
    static const double BAR_HEIGHT;

    Vec2 hold_pos;
    bool is_moving;

    public:

    explicit PluginWindow (GuiI* gui, Vec2 pos, Vec2 size);
    
    virtual bool onMousePress     (MouseContext    context) override;
    virtual bool onMouseRelease   (MouseContext    context) override;
    virtual bool onMouseMove      (MouseContext    context) override;
    virtual bool onKeyboardPress  (KeyboardContext context) override {return false;}
    virtual bool onKeyboardRelease(KeyboardContext context) override {return false;}
    virtual bool onClock          (uint64_t delta)          override {return false;}

    virtual void render(RenderTargetI* rt) override;
};



#endif