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
#include "events.h"

const size_t BASE_WIDGETMAN_CAP = 4;
const Color WINDOW_BG_COLOR = Color (128, 128, 128);
const Color BG_COLOR = Color (192, 192, 192);


class Renderable {
    public:

    Renderable() {}

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const = 0;
};

class Widget;

class WidgetManager {
    public:

    MyList<Widget*> widgets;
    
    explicit WidgetManager();

    ~WidgetManager();
    
    void AddWidget (Widget* widget);

    size_t GetSize() const;

    void Render (RenderTarget& rt) const;

    void Move (const Vec& vec);

    void SubtractRegset (const RegionSet& regions);

    void SetRenderTarget (RenderTarget *rt_);

    void MousePress (const Vec& mousepos, MouseButton mousebtn);

    void MouseRelease (const Vec& mousepos, MouseButton mousebtn);

    bool MouseOnWidgets (const Vec& mousepos) const;

    void MouseMove (const Vec& mousepos);

    void MoveToTail (Widget* wid);

    void UpdateRegset (const RegionSet& parent_regs);
};

class Widget : public Renderable, public EventProcessable {
    Rect bounds;
    WidgetManager subwidgets;
    RegionSet regset;
    Widget* parent;
    RenderTarget* rt;
    public:

    explicit Widget();

    explicit Widget (int x, int y, int w, int h);
    
    const Rect& GetBounds() const {return bounds;}

    Vec GetPos() const {return bounds.GetPos();}

    Vec GetSize() const {return bounds.GetSize();}

    const RegionSet* GetRegset() const {return (&regset);}

    RenderTarget* GetRendertarget() const {return rt;}
    
    WidgetManager* GetSubwidgets() {return &subwidgets;}

    void SetBounds (const Rect& bounds_) {bounds = bounds_;}

    void SetRenderTarget (RenderTarget *rt_);

    void AddSubWidget (Widget* wid);

    void RenderSubWidgets (RenderTarget& rt) const;

    void Move (const Vec& vec);

    void SubtractRegset (const RegionSet& regions);

    void UpdateRegset (const RegionSet& regs);

    void Show();

    virtual void GetMaxRegset (RegionSet* dst) const;

    virtual bool MouseOnWidget (const Vec& mousepos) const;

    bool MouseOnSubwidgets(const Vec& mousepos) const;
};


class Window : public Widget {
    bool is_moving;
    Vec hold_pos;

    public:

    explicit Window (int x, int y, size_t w, size_t h);

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;

    virtual void MousePress (const Vec& mousepos, MouseButton mousebtn) override;

    virtual void MouseRelease (const Vec& mousepos, MouseButton mousebtn) override;

    virtual void MouseMove (const Vec& mousepos) override;
};


class Background : public Widget {
    public:

    explicit Background (double w_, double h_);

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;

    virtual void MousePress (const Vec& mousepos, MouseButton mousebtn) override;

    virtual void MouseRelease (const Vec& mousepos, MouseButton mousebtn) override;

    virtual void MouseMove (const Vec& mousepos) override;
};

#endif
