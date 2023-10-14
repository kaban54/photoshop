#ifndef WIDGET_H
#define WIDGET_H

#include "vec.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "rendertarget.h"
#include "region.h"
#include "color.h"
#include "mylist.h"

const size_t BASE_WIDGETMAN_CAP = 4;
const Color WINDOW_BG_COLOR = Color (128, 128, 128);


enum MouseButton {
    MOUSE_LEFT = 0,
    MOUSE_RIGHT = 1,

    NUM_OF_MBUTONS
};
/*
struct Mouse {
    enum MouseButtonState {
        MBTN_UP       = 0,
        MBTN_PRESSED  = 1,
        MBTN_DOWN     = 2,
        MBTN_RELEASED = 3,
    };

    Vec pos;
    MouseButtonState btns[NUM_OF_MBUTONS];

    Mouse () {}

    void Press (const Vec& pos_, MouseButton btn);

    void Release (const Vec& pos_, MouseButton btn);

    void Move (const Vec& pos_);
};*/

class Renderable {
    public:

    bool visible;

    Renderable(): visible(true) {}

    virtual void Render (RenderTarget& rt, RegionSet* to_draw) const = 0;
};

class Widget;

class WidgetManager {
    public:

    MyList<Widget*> widgets;
    
    explicit WidgetManager ();
    
    void AddWidget (Widget* widget);

    size_t GetSize() const;

    void Render (RenderTarget& rt) const;

    void Move (const Vec& vec);

    void SubtractRegset (const RegionSet& regions);

    void SetRenderTarget (RenderTarget *rt_);

    void MousePress (const Vec& mousepos, MouseButton mousebtn);

    void MouseRelease (const Vec& mousepos, MouseButton mousebtn);

    void MouseMove (const Vec& mousepos);

    void MoveToTail (Widget* wid);

    void UpdateRegset();
};

class Widget : public Renderable {
    public:
    WidgetManager subwidgets;
    Vec pos;
    Vec size;
    RegionSet regset;
    Widget* parent;
    RenderTarget* rt;

    explicit Widget();

    explicit Widget (int x, int y, int w, int h);
    
    void SetRenderTarget (RenderTarget *rt_);

    void AddSubWidget (Widget* wid);

    void RenderSubWidgets (RenderTarget& rt) const;

    virtual void Move (const Vec& vec);

    void SubtractRegset (const RegionSet& regions);

    void SetRegions (const RegionSet& regs);

    void UpdateRegset (const RegionSet& regs);

    void Show();

    virtual void MousePress (const Vec& mousepos, MouseButton mousebtn) = 0;

    virtual void MouseRelease (const Vec& mousepos, MouseButton mousebtn) = 0;

    virtual void MouseMove (const Vec& mousepos) = 0;

    virtual bool MouseOnWidget (const Vec& mousepos) const = 0;

};


class Window : public Widget {
    bool is_moving;
    Vec hold_pos;

    public:

    explicit Window (int x, int y, size_t w, size_t h);

    virtual void Render (RenderTarget& rt, RegionSet* to_draw) const override;

    virtual void MousePress (const Vec& mousepos, MouseButton mousebtn) override;

    virtual void MouseRelease (const Vec& mousepos, MouseButton mousebtn) override;

    virtual void MouseMove (const Vec& mousepos) override;

    virtual bool MouseOnWidget (const Vec& mousepos) const override;

};

#endif
