#ifndef MENU_H
#define MENU_H

#include "buttons.h"

class Menu : public Widget {
    public:

    explicit Menu () {visible = false;}

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override {}

    virtual void MousePress (const MouseState& mstate) override;

    virtual void MouseRelease (const MouseState& mstate) override;

    virtual void MouseMove (const MouseState& mstate) override;

    virtual bool MouseOnWidget (const Vec& mousepos) const override;
};


class MenuBtn : public TxtButton {
    Menu* menu;

    public:

    explicit MenuBtn (double x, double y, double w, double h, const Text& txt_, Menu* menu_);

    virtual void MousePress (const MouseState& mstate) override;

    virtual void MouseRelease (const MouseState& mstate) override;

    virtual void MouseMove (const MouseState& mstate) override;
};

class VerticalMenu : public Menu {
    Vec pos;
    double nextbtn_y;

    public:

    explicit VerticalMenu (double x, double y);

    void AddButton (Button* btn);
};

#endif
