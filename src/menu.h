#ifndef MENU_H
#define MENU_H

#include "buttons.h"

class Menu : public Widget {
    public:

    explicit Menu () {
        setAvailable(false);
    }

    virtual bool onMousePress   (MouseContext context) override;
    virtual bool onMouseRelease (MouseContext context) override;
    virtual bool onMouseMove    (MouseContext context) override;
    virtual bool onKeyboardPress   (KeyboardContext context) override {return false;}
    virtual bool onKeyboardRelease (KeyboardContext context) override {return false;}
    virtual bool onClock (uint64_t delta) override {return false;}

    virtual void RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) override {};

    virtual void render(RenderTargetI*) override {};

    virtual bool MouseOnWidget (const Vec2& mousepos) const override;
};


class MenuBtn : public TxtButton {
    Menu* menu;

    public:

    explicit MenuBtn (double x, double y, double w, double h, const char *str, uint16_t char_size_, Menu* menu_);

    virtual bool onMousePress   (MouseContext context) override;
    virtual bool onMouseRelease (MouseContext context) override;
    virtual bool onMouseMove    (MouseContext context) override;
};

class VerticalMenu : public Menu {
    Vec2 pos;
    double nextbtn_y;

    public:

    explicit VerticalMenu (double x, double y);

    void AddButton (Button* btn);
};

class TwoColMenu : public Menu {
    Vec2 pos;
    Vec2 btn_size;
    Vec2 nextbtn_pos;

    public:

    explicit TwoColMenu (double x, double y, double w, double h);

    void AddButton (Button* btn);
};

#endif
