#ifndef CANVAS_H
#define CANVAS_H

#include "widget.h"
#include "tool.h"
#include "filter.h"

class Canvas : public Widget {
    bool drawing;
    ToolManager* tool_man;
    FilterManager* filter_man;
    RenderTarget data;
    RenderTarget tmp;

    public:

    explicit Canvas (double x, double y, double w, double h, ToolManager* tm, FilterManager* fm);

    virtual bool onMousePress   (MouseContext context) override;
    virtual bool onMouseRelease (MouseContext context) override;
    virtual bool onMouseMove    (MouseContext context) override;
    virtual bool onKeyboardPress   (KeyboardContext context) override {return false;}
    virtual bool onKeyboardRelease (KeyboardContext context) override {return false;}
    virtual bool onClock (uint64_t delta) override;

    virtual void RenderInRegset(RenderTarget& rt, const RegionSet* to_draw) override;
};

#endif