#ifndef WINDOW_H
#define WINDOW_H

#include "widget.h"


class Window : public Widget {
    bool is_moving;
    Vec hold_pos;

    public:

    explicit Window (double x, double y, double w, double h);

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;

    virtual void MousePress (const MouseState& mstate) override;

    virtual void MouseRelease (const MouseState& mstate) override;

    virtual void MouseMove (const MouseState& mstate) override;
};


class Background : public Widget {
    public:

    explicit Background (double w_, double h_);

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;

    virtual void MousePress (const MouseState& mstate) override;

    virtual void MouseRelease (const MouseState& mstate) override;

    virtual void MouseMove (const MouseState& mstate) override;
};

class ModalWindow : public Window {
    EventManager* event_man;

    public:

    ModalWindow (double x, double y, double w, double h, EventManager* event_man_);

    ~ModalWindow();
};

#endif