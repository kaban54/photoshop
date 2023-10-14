#include "drawing.h"


ToolManager::ToolManager ():
    cur_tool (nullptr),
    col (Color(0, 0, 0))
    {}


void ToolManager::SetTool (Tool* tool) {
    cur_tool = tool;
}

void ToolManager::SetColor (const Color& col_) {
    col = col_;
}

void ToolManager::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos) {
    cur_tool -> PaintOnPress (perm, tmp, pos, col);
}

void ToolManager::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos) {
    cur_tool -> PaintOnRelease (perm, tmp, pos, col);
}

void ToolManager::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos) {
    cur_tool -> PaintOnMove (perm, tmp, pos, col);
}



Canvas::Canvas (int x, int y, int w, int h, ToolManager* tm):
    Widget (x, y, w, h), 
    drawing (false),
    tool_man (tm),
    data (w, h),
    tmp (w, h)
    {
        data.ClearScreen(Color(255, 255, 255));
        tmp.ClearScreen(Color(0, 0, 0, 0));
    }

void Canvas::MousePress (const Vec& mousepos, MouseButtons mousebtn) {
    drawing = true;
    tool_man -> PaintOnPress (&data, &tmp, mousepos);
}

void Canvas::MouseRelease (const Vec& mousepos, MouseButtons mousebtn) {
    drawing = false;
    tool_man -> PaintOnRelease (&data, &tmp, mousepos);
}

void Canvas::MouseMove (const Vec& mousepos) {
    if (drawing)
        tool_man -> PaintOnMove (&data, &tmp, mousepos);
}

bool Canvas::MouseOnWidget (const Vec& mousepos) const {
    return (mousepos.x >= pos.x && mousepos.x <= pos.x + size.x ) &&
           (mousepos.y >= pos.y && mousepos.y <= pos.y + size.y);
}

void Canvas::Render (RenderTarget& rt, const RegionSet& to_draw) const {
    rt.DrawRenderTarget(data, pos, to_draw);
    rt.DrawRenderTarget(tmp, pos, to_draw);
}