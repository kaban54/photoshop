#include "canvas.h"

Canvas::Canvas(double x, double y, double w, double h, ToolManager* tm, FilterManager* fm):
    Widget (x, y, w, h), 
    drawing (false),
    tool_man (tm),
    filter_man (fm),
    data (w, h),
    tmp (w, h)
    {
        data.Fill(Color(255, 255, 255));
        tmp.Fill(Color(0, 0, 0, 0));
    }

bool Canvas::onMousePress (MouseContext context) {
    if (MouseOnWidget(context.position)) {
        Show();
        filter_man -> setRenderTarget(&data);
        drawing = true;
        tool_man -> paintOnPress (&data, &tmp, MouseContext(context.position - getPos(), context.button));
        RenderInRegset (*GetRendertarget(), GetRegset());
    }
    return false;
}

bool Canvas::onMouseRelease (MouseContext context) {
    if (drawing) {
        tool_man -> paintOnRelease (&data, &tmp, MouseContext(context.position - getPos(), context.button));
        RenderInRegset (*GetRendertarget(), GetRegset());
    }
    return false;
}

bool Canvas::onMouseMove (MouseContext context) {
    if (drawing) {
        if (MouseOnWidget (context.position))
            tool_man -> paintOnMove (&data, &tmp, MouseContext(context.position - getPos(), context.button));
        else {
            tool_man -> disableTool (&data, &tmp, MouseContext(context.position - getPos(), context.button));
            drawing = false;
        }
        RenderInRegset (*GetRendertarget(), GetRegset());
    }
    return false;
}

bool Canvas::onClock (uint64_t delta) {
    if (data.changed) {
        data.changed = false;
        RenderInRegset(*GetRendertarget(), GetRegset());
    }
    return false;
}

void Canvas::RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) {
    rt.DrawRenderTarget_rs(data, getPos(), to_draw);
    rt.DrawRenderTarget_rs(tmp , getPos(), to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(0, 255, 255, 128));
    #endif
}
