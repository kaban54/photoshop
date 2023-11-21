#ifndef WIDGET_H
#define WIDGET_H

#include "vec2.h"
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


namespace plugin {
    struct WidgetI: public EventProcessableI {
        virtual void registerSubWidget(WidgetI* object) = 0;
        virtual void unregisterSubWidget(WidgetI* object) = 0;

        virtual Vec2 getSize() = 0;
        virtual void setSize(Vec2) = 0;

        virtual Vec2 getPos() = 0;
        virtual void setPos(Vec2) = 0;

        /// Нужно для обновления регинов.
        /// верно тогда и только тогда, когда виджет принадлежит плагину.
        /// В таком случае вызов getDefaultRegion невалиден (поэтому тут его и нет), и нужно 
        virtual bool isExtern() = 0;

        virtual void setParent(WidgetI *root) = 0;
        virtual WidgetI *getParent() = 0;

        virtual void move(Vec2 shift) = 0;

        // Жив ли виджет
        // Если true, то это идейно равносильно вызову деструктору, то есть его не надо рендерить, ему не надо передавать 
        // ивенты и тд и тп
        virtual bool getAvailable() = 0;
        virtual void setAvailable(bool) = 0;

        virtual void render(RenderTargetI* ) = 0;
        virtual void recalcRegion() = 0;

        virtual ~WidgetI() = 0;
    };
}

using namespace plugin;

class Widget;

struct WidgetShell {
    WidgetI* external;
    Widget*  internal;

    explicit WidgetShell(WidgetI* wid):
        external (wid),
        internal ((wid != nullptr && !wid -> isExtern()) ? dynamic_cast<Widget*>(wid) : nullptr) {}
};

class WidgetManager : public EventProcessableI {
    public:

    MyList<WidgetShell> widgets;
    
    explicit WidgetManager();

    ~WidgetManager();
    
    void AddWidget (WidgetI* wid);

    void RemoveWidget (WidgetI* wid);

    size_t GetSize() const;

    void Render (RenderTarget& rt) const;

    void Move (const Vec2& vec);

    void SetRenderTarget (RenderTarget *rt_);

    bool MouseOnWidgets (const Vec2& mousepos) const;

    void MoveToTail (WidgetI* wid);

    void UpdateRegset (const RegionSet& parent_regs);

    virtual void GetMaxRegset (RegionSet* dst) const;

    virtual uint8_t getPriority() const {return 0;}

    virtual bool onMouseMove(MouseContext context) override;

    virtual bool onMouseRelease(MouseContext context) override;

    virtual bool onMousePress(MouseContext context) override;

    virtual bool onKeyboardPress(KeyboardContext context) override;

    virtual bool onKeyboardRelease(KeyboardContext context) override;

    virtual bool onClock(uint64_t delta) override;
};

class Widget : public WidgetI {
    Rect bounds;
    WidgetManager subwidgets;
    RegionSet regset;
    WidgetShell parent;
    RenderTarget* rt;
    bool visible;

    public:

    explicit Widget();

    explicit Widget (double x, double y, double w, double h);
    
    ~Widget();

    virtual void registerSubWidget(WidgetI* wid);
    virtual void unregisterSubWidget(WidgetI* wid);

    virtual Vec2 getSize() {return bounds.GetSize();}
    virtual void setSize(Vec2 new_size);

    virtual Vec2 getPos() {return bounds.GetPos();}
    virtual void setPos(Vec2 new_pos);

    virtual bool isExtern() {return false;}

    virtual void setParent(WidgetI *root);
    virtual WidgetI *getParent();

    virtual void move(Vec2 shift);

    virtual bool getAvailable() {return visible;}
    virtual void setAvailable(bool vis_) {visible = vis_;}

    virtual void recalcRegion() {};


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
    virtual void RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) const = 0;

    virtual bool MouseOnWidget (const Vec2& mousepos) const;

    bool MouseOnSubwidgets(const Vec2& mousepos) const;

    void RenderSubWidgets (RenderTarget& rt) const;
};


// class TxtWidget : public Widget{
//     public:
//     Text txt;

//     TxtWidget (double x, double y, double w, double h, const std::string& str_, const Font& fnt_, size_t char_size_);

//     virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;
// };

#endif
