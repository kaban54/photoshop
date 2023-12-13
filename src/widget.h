#ifndef WIDGET_H
#define WIDGET_H

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "rendertarget.h"
#include "region.h"
#include "mylist.h"
#include "events.h"


namespace plugin {
    struct WidgetI {
        virtual void registerSubWidget(WidgetI* object) = 0;
        virtual void unregisterSubWidget(WidgetI* object) = 0;

        virtual Vec2 getSize() const = 0;
        virtual void setSize(Vec2) = 0;

        virtual Vec2 getPos() const = 0;
        virtual void setPos(Vec2) = 0;

        virtual WidgetI *getParent() const = 0;
        virtual void setParent(WidgetI *root) = 0;

        virtual void move(Vec2 shift) = 0;

        // Жив ли виджет
        // Если true, то это идейно равносильно вызову деструктору, то есть его не надо рендерить, ему не надо передавать 
        // ивенты и тд и тп
        virtual bool getAvailable() const = 0;
        virtual void setAvailable(bool) = 0;

        virtual ~WidgetI() = default;
    };

    struct PluginWidgetI: public EventProcessableI, public RenderableI {
        WidgetI* host;
    };
}

using namespace plugin;

class Widget;

class WidgetManager : public EventProcessableI {
    public:

    MyList<Widget*> widgets;
    
    explicit WidgetManager();

    ~WidgetManager();
    
    void AddWidget (Widget* wid);

    void RemoveWidget (Widget* wid);

    size_t GetSize() const;

    void Render (RenderTarget& rt) const;

    void Move_noupdate(const Vec2& vec);

    void SetRenderTarget (RenderTarget *rt_);

    bool MouseOnWidgets (const Vec2& mousepos) const;

    void MoveToTail (Widget* wid);

    void UpdateRegset (const RegionSet& parent_regs);

    virtual void GetMaxRegset (RegionSet* dst) const;

    virtual uint8_t getPriority() const override {return 0;}

    virtual bool onMousePress   (MouseContext context) override;
    virtual bool onMouseRelease (MouseContext context) override;
    virtual bool onMouseMove    (MouseContext context) override;
    virtual bool onKeyboardPress   (KeyboardContext context) override;
    virtual bool onKeyboardRelease (KeyboardContext context) override;
    virtual bool onClock (uint64_t delta) override;
};

class Widget : public WidgetI, public EventProcessableI {
    Rect bounds;
    WidgetManager subwidgets;
    RegionSet regset;
    Widget* parent;
    RenderTarget* rt;
    bool visible;

    public:

    explicit Widget();

    explicit Widget (double x, double y, double w, double h);
    
    virtual ~Widget();

    virtual void registerSubWidget(WidgetI* wid);
    virtual void unregisterSubWidget(WidgetI* wid);

    virtual Vec2 getSize() const override {return bounds.GetSize();}
    virtual void setSize(Vec2 new_size) override;

    virtual Vec2 getPos() const override {return bounds.GetPos();}
    virtual void setPos(Vec2 new_pos) override;

    virtual WidgetI *getParent() const override {return parent;}
    virtual void setParent(WidgetI *root) override;

    virtual void move(Vec2 shift);

    virtual bool getAvailable() const override {return visible;}
    virtual void setAvailable(bool vis_) override {visible = vis_;}

    void Move_noupdate(Vec2 shift);

    const Rect& GetBounds() const {return bounds;}
    void SetBounds (const Rect& bounds_) {bounds = bounds_;}

    RenderTarget* GetRendertarget() const {return rt;}
    void SetRenderTarget (RenderTarget *rt_);

    const RegionSet* GetRegset() const {return (&regset);}

    WidgetManager* GetSubwidgets() {return &subwidgets;}

    void UpdateRegset (const RegionSet& regs);
    void UpdateAllRegsets();
    void Show();
    virtual void GetMaxRegset (RegionSet* dst) const;
    virtual void RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) = 0;

    virtual bool MouseOnWidget (const Vec2& mousepos) const;

    bool MouseOnSubwidgets(const Vec2& mousepos) const;

    void RenderSubWidgets (RenderTarget& rt) const;

    virtual uint8_t getPriority() const override {return 0;}
};

class TxtWidget : public Widget{
    const char* txt;
    size_t char_size;
    Color fill_col;
    Color bg_col;
    public:

    TxtWidget (double x, double y, double w, double h, const char* txt_, size_t char_size_,
               Color fill_col_, Color bg_color_);

    virtual void RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) override;

    virtual bool onMousePress   (MouseContext context) override {return false;}
    virtual bool onMouseRelease (MouseContext context) override {return false;}
    virtual bool onMouseMove    (MouseContext context) override {return false;}
    virtual bool onKeyboardPress   (KeyboardContext context) override {return false;}
    virtual bool onKeyboardRelease (KeyboardContext context) override {return false;}
    virtual bool onClock (uint64_t delta) override {return false;}
};


class ExternWidget : public Widget {
    PluginWidgetI* plug_wid;

    public:

    explicit ExternWidget(PluginWidgetI* plug_wid);

    virtual void RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) override;

    virtual bool onMousePress   (MouseContext context) override;
    virtual bool onMouseRelease (MouseContext context) override;
    virtual bool onMouseMove    (MouseContext context) override;
    virtual bool onKeyboardPress   (KeyboardContext context) override;
    virtual bool onKeyboardRelease (KeyboardContext context) override;
    virtual bool onClock (uint64_t delta) override;

    virtual uint8_t getPriority() const override;
};

#endif
