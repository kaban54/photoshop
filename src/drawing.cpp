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
    tool_man -> PaintOnPress (&data, &tmp, mousepos - pos);
}

void Canvas::MouseRelease (const Vec& mousepos, MouseButtons mousebtn) {
    if (drawing) {
        drawing = false;
        tool_man -> PaintOnRelease (&data, &tmp, mousepos - pos);
    }
}

void Canvas::MouseMove (const Vec& mousepos) {
    if (drawing)
        tool_man -> PaintOnMove (&data, &tmp, mousepos - pos);
}

bool Canvas::MouseOnWidget (const Vec& mousepos) const {
    return (mousepos.x >= pos.x && mousepos.x <= pos.x + size.x ) &&
           (mousepos.y >= pos.y && mousepos.y <= pos.y + size.y);
}

void Canvas::Render (RenderTarget& rt, RegionSet* to_draw) const {
    rt.DrawRenderTarget(data, pos, to_draw);
    rt.DrawRenderTarget(tmp, pos, to_draw);
}


Brush::Brush (unsigned int r):
    radius (r)
    {}

void Brush::SetRadius (unsigned int r) {
    radius = r;
}

void Brush::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    start_pos = pos;
    last_pos = pos;
    perm -> DrawCircle (pos, radius, col);
}

void Brush::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    last_pos = pos;
    perm -> DrawCircle (pos, radius, col);
}

void Brush::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    last_pos = pos;
    perm -> DrawCircle (pos, radius, col);
}


void RectTool::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    start_pos = pos;
    last_pos = pos;
    tmp -> DrawRect (Rect(pos, pos), col);
}

void RectTool::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    last_pos = pos;
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    perm -> DrawRect (Rect(start_pos, pos), col);
}

void RectTool::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    last_pos = pos;
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    tmp -> DrawRect (Rect(start_pos, pos), col);
}


void LineTool::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    start_pos = pos;
    last_pos = pos;
    tmp -> DrawLine (pos, pos, col);
}

void LineTool::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    last_pos = pos;
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    perm -> DrawLine (start_pos, pos, col);
}

void LineTool::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    last_pos = pos;
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    tmp -> DrawLine (start_pos, pos, col);
}


void EllipseTool::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    start_pos = pos;
    last_pos = pos;
    tmp -> DrawEllipse (Rect(pos, pos), col);
}

void EllipseTool::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    last_pos = pos;
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    perm -> DrawEllipse (Rect(start_pos, pos), col);
}

void EllipseTool::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    last_pos = pos;
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    tmp -> DrawEllipse (Rect(start_pos, pos), col);
}


ToolBtn::ToolBtn (double x, double y, size_t w, size_t h, const Texture* textures_, const Text& txt_, ToolManager* tm, Tool* tool_):
    TxtButton (x, y, w, h, textures_, txt_),
    tool_man (tm),
    tool (tool_)
    {}

void ToolBtn::MousePress (const Vec& mousepos, MouseButtons mousebtn) {
    tool_man -> SetTool (tool);
    state = BTN_PRESSED;
}

void ToolBtn::MouseRelease (const Vec& mousepos, MouseButtons mousebtn) {
    state = BTN_NORMAL;
}

ColorBtn::ColorBtn (double x, double y, size_t w, size_t h, const Texture* textures_, ToolManager* tm, const Color& col):
    ImgButton (x, y, w, h, textures_),
    tool_man (tm),
    color (col)
    {}

void ColorBtn::MousePress (const Vec& mousepos, MouseButtons mousebtn) {
    tool_man -> SetColor (color);
    state = BTN_PRESSED;
}

void ColorBtn::MouseRelease (const Vec& mousepos, MouseButtons mousebtn) {
    state = BTN_NORMAL;
}

void ColorBtn::Render (RenderTarget& rt, RegionSet* to_draw) const {
    rt.DrawTexture (textures[state], pos, size, to_draw);
    rt.DrawRect (Rect (pos + size / 4, pos + size * 3 / 4), color);
}