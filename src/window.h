#ifndef WINDOW_H
#define WINDOW_H

#include "widget.h"
#include "buttons.h"

const Color WINDOW_BG_COLOR = Color (128, 128, 128);
const Color BG_COLOR = Color (192, 192, 192);

class Window : public Widget {
    Vec2 hold_pos;
    bool is_moving;

    public:
    bool need_to_close;

    explicit Window (double x, double y, double w, double h, bool close_btn = true);

    virtual void RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) const override;

    virtual void render(RenderTargetI*) override;

    virtual bool onMousePress   (MouseContext context) override;
    virtual bool onMouseRelease (MouseContext context) override;
    virtual bool onMouseMove    (MouseContext context) override;
    virtual bool onKeyboardPress   (KeyboardContext context) override;
    virtual bool onKeyboardRelease (KeyboardContext context) override;
    virtual bool onClock (uint64_t delta) override;

    virtual void Close() {delete this;}
};


class Background : public Widget {
    public:

    explicit Background (double w_, double h_);

    virtual void RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) const override;

    virtual bool onMousePress   (MouseContext context) override;
    virtual bool onMouseRelease (MouseContext context) override;
    virtual bool onMouseMove    (MouseContext context) override;
    virtual bool onKeyboardPress   (KeyboardContext context) override;
    virtual bool onKeyboardRelease (KeyboardContext context) override;
    virtual bool onClock (uint64_t delta) override;
};

class ModalWindow : public Window {
    EventManager* event_man;

    public:

    ModalWindow (double x, double y, double w, double h, EventManager* event_man_);

    ~ModalWindow();

    virtual uint8_t getPriority() const override {return 1;}

    virtual void Close() override {delete this;}
};


struct WindowCloseBtnArgs : public BtnArgs {
    Window* win;
    explicit WindowCloseBtnArgs (Window* win_):
        win (win_) {}
};

void window_close_btn_action (BtnArgs* btn_args);

class WindowCloseBtn : public Button {
    WindowCloseBtnArgs wclose_btn_args;

    public:

    explicit WindowCloseBtn (double x, double y, double w, double h, Window* win_);

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;
};

#endif