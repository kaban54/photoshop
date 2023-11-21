#ifndef WIDGET_H
#define WIDGET_H

#include "vec.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "rendertarget.h"
#include "region.h"
#include "mylist.h"
#include "events.h"


const Color WINDOW_BG_COLOR = Color (128, 128, 128);
const Color BG_COLOR = Color (192, 192, 192);


class Renderable {
    public:

    bool visible;

    Renderable(): visible (true) {}

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const = 0;
};

class Widget;

class WidgetManager : public EventProcessable{
    public:

    MyList<Widget*> widgets;
    
    explicit WidgetManager();

    ~WidgetManager();
    
    void AddWidget (Widget* wid);

    void RemoveWidget (Widget* wid);

    size_t GetSize() const;

    void Render (RenderTarget& rt) const;

    void Move (const Vec& vec);

    void SetRenderTarget (RenderTarget *rt_);

    virtual void MousePress (const MouseState& mstate) override;

    virtual void MouseRelease (const MouseState& mstate) override;

    virtual void MouseMove (const MouseState& mstate) override;

    virtual void KeyboardPress (const KeyboardState& kstate) override;

    virtual void KeyboardRelease (const KeyboardState& kstate) override;
    
    virtual void TimerEvent (double time) override;

    bool MouseOnWidgets (const Vec& mousepos) const;

    void MoveToTail (Widget* wid);

    void UpdateRegset (const RegionSet& parent_regs);

    virtual void GetMaxRegset (RegionSet* dst) const;
};

class Widget : public Renderable, public EventProcessable {
    Rect bounds;
    WidgetManager subwidgets;
    RegionSet regset;
    Widget* parent;
    RenderTarget* rt;
    public:

    explicit Widget();

    explicit Widget (double x, double y, double w, double h);
    
    ~Widget();

    const Rect& GetBounds() const {return bounds;}

    Vec GetPos() const {return bounds.GetPos();}

    Vec GetSize() const {return bounds.GetSize();}

    Widget* GetParent() const {return parent;}

    const RegionSet* GetRegset() const {return (&regset);}

    RenderTarget* GetRendertarget() const {return rt;}
    
    WidgetManager* GetSubwidgets() {return &subwidgets;}

    void SetBounds (const Rect& bounds_) {bounds = bounds_;}

    void SetParent (Widget* parent_) {parent = parent_;}

    void SetRenderTarget (RenderTarget *rt_);

    void AddSubWidget (Widget* wid);

    void RenderSubWidgets (RenderTarget& rt) const;

    void Move (const Vec& vec);

    void UpdateRegset (const RegionSet& regs);

    void UpdateAllRegsets();

    void Show();

    virtual void GetMaxRegset (RegionSet* dst) const;

    virtual bool MouseOnWidget (const Vec& mousepos) const;

    bool MouseOnSubwidgets(const Vec& mousepos) const;
};


class TxtWidget : public Widget{
    public:
    Text txt;

    TxtWidget (double x, double y, double w, double h, const std::string& str_, const Font& fnt_, size_t char_size_);

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;
};

#endif
