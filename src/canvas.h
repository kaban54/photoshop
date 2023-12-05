#ifndef CANVAS_H
#define CANVAS_H

#include "widget.h"
#include "tool.h"
#include "filter.h"

class Canvas : public Widget {
    bool drawing;
    ToolManagerI* tool_man;
    FilterManagerI* filter_man;
    RenderTarget data;
    RenderTarget tmp;

    public:

    explicit Canvas (double x, double y, double w, double h, ToolManagerI* tm, FilterManagerI* fm);

    virtual bool onMousePress   (MouseContext context) override;
    virtual bool onMouseRelease (MouseContext context) override;
    virtual bool onMouseMove    (MouseContext context) override;
    virtual bool onKeyboardPress   (KeyboardContext context) override {return false;}
    virtual bool onKeyboardRelease (KeyboardContext context) override {return false;}
    virtual bool onClock (uint64_t delta) override;

    virtual void render(RenderTargetI* rt) override;

    virtual void RenderInRegset(RenderTarget& rt, const RegionSet* to_draw) override;
};

#endif