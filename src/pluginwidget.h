#ifndef PLUGINWIDGET_H
#define PLUGINWIDGET_H

#include "standart2.h"
#include <string>

using namespace plugin;

enum ButtonState {
    BTN_NORMAL   = 0,
    BTN_PRESSED  = 1,
    BTN_FOCUSED  = 2,
    BTN_DISABLED = 3,
};

struct PluginWidget : public PluginWidgetI {
    explicit PluginWidget (GuiI* gui, Vec2 pos, Vec2 size);

    virtual uint8_t getPriority() const override {return 0;}

    bool MouseOnWidget(Vec2 mpos);
};


struct BtnArgs {};

typedef void BtnFunc (BtnArgs* args);

class PluginTxtButton : public PluginWidget {
    BtnFunc* action;
    BtnArgs* action_args;
    ButtonState state;
    const char *txt;
    uint16_t char_size;
    Vec2 txt_pos;

    static const uint16_t DEFAULT_CHAR_SIZE = 30;

    public:

    explicit PluginTxtButton(GuiI* gui, Vec2 pos, Vec2 size, BtnFunc* action_, BtnArgs* action_args_,
                             const char *str = "", uint16_t char_size_ = DEFAULT_CHAR_SIZE);

    void SetText(const char *str, uint16_t char_size_ = DEFAULT_CHAR_SIZE);
    void SetTxtPos(Vec2 pos) {txt_pos = pos;}

    virtual bool onMousePress     (MouseContext    context) override;
    virtual bool onMouseRelease   (MouseContext    context) override;
    virtual bool onMouseMove      (MouseContext    context) override;
    virtual bool onKeyboardPress  (KeyboardContext context) override {return false;}
    virtual bool onKeyboardRelease(KeyboardContext context) override {return false;}
    virtual bool onClock          (uint64_t delta)          override {return false;}

    virtual void render(RenderTargetI* rt) override;
};

void DrawButton (RenderTargetI* rt, Vec2 pos, Vec2 size, ButtonState state);

class PluginWindow : public PluginWidget {

    static const Color BG_COLOR;
    static const Color BAR_COLOR;
    static const double BAR_HEIGHT;

    Vec2 hold_pos;
    bool is_moving;
    std::string name;

    public:

    explicit PluginWindow (GuiI* gui, Vec2 pos, Vec2 size);

    void SetName(const char* name_) {name.assign(name_);}
    const char* GetName() {return name.c_str();}

    virtual bool onMousePress     (MouseContext    context) override;
    virtual bool onMouseRelease   (MouseContext    context) override;
    virtual bool onMouseMove      (MouseContext    context) override;
    virtual bool onKeyboardPress  (KeyboardContext context) override {return false;}
    virtual bool onKeyboardRelease(KeyboardContext context) override {return false;}
    virtual bool onClock          (uint64_t delta)          override {return false;}

    virtual void render(RenderTargetI* rt) override;
};



#endif