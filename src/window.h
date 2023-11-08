#ifndef WINDOW_H
#define WINDOW_H

#include "widget.h"
#include "buttons.h"

class WindowCloseBtn;

class Window : public Widget {
    Vec hold_pos;
    bool is_moving;

    public:
    bool need_to_close;

    explicit Window (double x, double y, double w, double h);

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;

    virtual void MousePress (const MouseState& mstate) override;

    virtual void MouseRelease (const MouseState& mstate) override;

    virtual void MouseMove (const MouseState& mstate) override;

    virtual void KeyboardPress (const KeyboardState& kstate) override;

    virtual void KeyboardRelease (const KeyboardState& kstate) override;
};


class Background : public Widget {
    public:

    explicit Background (double w_, double h_);

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;

    virtual void MousePress (const MouseState& mstate) override;

    virtual void MouseRelease (const MouseState& mstate) override;

    virtual void MouseMove (const MouseState& mstate) override;

    virtual void KeyboardPress (const KeyboardState& kstate) override;

    virtual void KeyboardRelease (const KeyboardState& kstate) override;
};

class ModalWindow : public Window {
    EventManager* event_man;

    public:

    ModalWindow (double x, double y, double w, double h, EventManager* event_man_);

    ~ModalWindow();
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