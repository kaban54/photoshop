#include "canvas.h"

Canvas::Canvas(double x, double y, double w, double h, ToolManagerI* tm):
    Widget (x, y, w, h), 
    drawing (false),
    tool_man (tm),
    // filter_man (fm),
    data (w, h),
    tmp (w, h)
    {
        data.Fill(Color(255, 255, 255));
        tmp.Fill(Color(0, 0, 0, 0));
    }

bool Canvas::onMousePress (MouseContext context) {
    if (MouseOnWidget(context.position)) {
        Show();
        /*if (filter_man -> IsActive()) filter_man -> Apply (data);
        else*/ drawing = true;
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

void Canvas::render(RenderTargetI* rt) {
    Texture* data_texture = data.getTexture();
    Texture*  tmp_texture =  tmp.getTexture();
    rt -> drawTexture(getPos(), getSize(), data_texture);
    rt -> drawTexture(getPos(), getSize(),  tmp_texture);
    delete data_texture;
    delete  tmp_texture;
}

void Canvas::RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) {
    Texture* data_texture = data.getTexture();
    Texture*  tmp_texture =  tmp.getTexture();
    rt.DrawTexture_rs(getPos(), getSize(), data_texture, to_draw);
    rt.DrawTexture_rs(getPos(), getSize(),  tmp_texture, to_draw);
    delete data_texture;
    delete  tmp_texture;

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(0, 255, 255, 128));
    #endif
}
