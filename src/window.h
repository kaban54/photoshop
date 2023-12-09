#ifndef WINDOW_H
#define WINDOW_H

#include "widget.h"
#include "buttons.h"


class Window : public Widget {

    static const Color BG_COLOR;
    static const Color BAR_COLOR;
    static const double BAR_HEIGHT;

    Vec2 hold_pos;
    bool is_moving;
    const char* name;

    public:
    bool need_to_close;

    explicit Window (double x, double y, double w, double h, bool close_btn = true);

    void SetName(const char* name_) {name = name_;}
    const char* GetName() {return name;}

    virtual void RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) override;

    virtual bool onMousePress   (MouseContext context) override;
    virtual bool onMouseRelease (MouseContext context) override;
    virtual bool onMouseMove    (MouseContext context) override;
    virtual bool onKeyboardPress   (KeyboardContext context) override;
    virtual bool onKeyboardRelease (KeyboardContext context) override;
    virtual bool onClock (uint64_t delta) override;

    virtual void Close() {delete this;}
};


class Background : public Widget {
    
    static const Color BG_COLOR;

    public:

    explicit Background (double w_, double h_);

    virtual void RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) override;

    virtual bool onMousePress   (MouseContext context) override;
    virtual bool onMouseRelease (MouseContext context) override;
    virtual bool onMouseMove    (MouseContext context) override;
    virtual bool onKeyboardPress   (KeyboardContext context) override;
    virtual bool onKeyboardRelease (KeyboardContext context) override;
    virtual bool onClock (uint64_t delta) override;
};

class ModalWindow : public Window {
    EventManagerI* event_man;

    public:

    ModalWindow (double x, double y, double w, double h, EventManagerI* event_man_);

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

    virtual void RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) override;
};

#endif