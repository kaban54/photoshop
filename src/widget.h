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

const size_t BASE_WIDGETMAN_CAP = 4;
const Color WINDOW_BG_COLOR = Color (128, 128, 128);

enum MouseButtons {
    MOUSE_LEFT = 0,
    MOUSE_RIGHT = 1
};

class Renderable {
    public:

    bool visible;

    Renderable(): visible(true) {}

    virtual void Render (RenderTarget& rt, const RegionSet& to_draw) const = 0;
};

class Widget;

class WidgetManager {
    Widget** widgets;
    size_t size;
    size_t capacity;
    
    public:
    
    WidgetManager (size_t cap = BASE_WIDGETMAN_CAP);

    ~WidgetManager();
    
    void AddWidget (Widget* widget);

    size_t GetSize() const;

    Widget* operator[] (size_t index) const;

    void Render (RenderTarget& rt) const;

    void Move (const Vec& vec);

    void SetRenderTarget (RenderTarget *rt_);

    void MousePress (const Vec& mousepos, MouseButtons mousebtn);

    void MouseRelease (const Vec& mousepos, MouseButtons mousebtn);

    void MouseMove (const Vec& mousepos);
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

    explicit Widget (int x, int y, int w, int h, size_t subw_cap = BASE_WIDGETMAN_CAP);
    
    void SetRenderTarget (RenderTarget *rt_);

    void AddSubWidget (Widget* wid);

    void RenderSubWidgets (RenderTarget& rt) const;

    virtual void Move (const Vec& vec);

    void SetRegions (const RegionSet& regs);

    void UpdateRegSet (const Rect& old_pos, const Rect& new_pos, Widget* no_update = nullptr);

    void Show();

    virtual void MousePress (const Vec& mousepos, MouseButtons mousebtn) = 0;

    virtual void MouseRelease (const Vec& mousepos, MouseButtons mousebtn) = 0;

    virtual void MouseMove (const Vec& mousepos) = 0;

    virtual bool MouseOnWidget (const Vec& mousepos) = 0;

};


class Window : public Widget {
    bool is_moving;
    Vec hold_pos;

    public:

    explicit Window (size_t w, size_t h);

    explicit Window (int x, int y, size_t w, size_t h);

    virtual void Render (RenderTarget& rt, const RegionSet& to_draw) const override;

    virtual void MousePress (const Vec& mousepos, MouseButtons mousebtn) override;

    virtual void MouseRelease (const Vec& mousepos, MouseButtons mousebtn) override;

    virtual void MouseMove (const Vec& mousepos) override;

    virtual bool MouseOnWidget (const Vec& mousepos) override;

};


//----------------------------------------------------------------------------------------------


class Tool {
    Vec start_pos;
    Vec last_pos;

    virtual void PaintOnPress ();
};


class ToolManager {

};

class Canvas : public Widget {
    bool drawing;

};

//-----------------------------------------------------------------------------------------------


void *Recalloc (void *memptr, size_t num, size_t size, size_t old_num);

#endif
