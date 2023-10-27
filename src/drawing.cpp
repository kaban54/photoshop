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

bool ToolManager::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn) {
    return cur_tool -> PaintOnPress (perm, tmp, pos, btn, col);
}

bool ToolManager::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn) {
    return cur_tool -> PaintOnRelease (perm, tmp, pos, btn, col);
}

bool ToolManager::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos) {
    return cur_tool -> PaintOnMove (perm, tmp, pos, col);
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

void Canvas::MousePress (const Vec& mousepos, MouseButton mousebtn) {
    if (MouseOnWidget(mousepos)) {
        Show();
        drawing = tool_man -> PaintOnPress (&data, &tmp, mousepos - pos, mousebtn);
        Render (*rt, &regset);
    }
}

void Canvas::MouseRelease (const Vec& mousepos, MouseButton mousebtn) {
    if (drawing) {
        drawing = tool_man -> PaintOnRelease (&data, &tmp, mousepos - pos, mousebtn);
        Render (*rt, &regset);
    }
}

void Canvas::MouseMove (const Vec& mousepos) {
    if (drawing) {
        drawing = tool_man -> PaintOnMove (&data, &tmp, mousepos - pos);
        Render (*rt, &regset);
    }
}

void Canvas::Render (RenderTarget& rt, RegionSet* to_draw) const {
    rt.DrawRenderTarget(data, pos, to_draw);
    rt.DrawRenderTarget(tmp, pos, to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(rand() % 128 + 128, rand() % 128 + 128, 0));
    #endif
}


Brush::Brush (unsigned int r):
    radius (r)
    {}

void Brush::SetRadius (unsigned int r) {
    radius = r;
}

bool Brush::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) {
    start_pos = pos;
    last_pos = pos;
    perm -> DrawCircle (pos, radius, col);
    return true;
}

bool Brush::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) {
    last_pos = pos;
    perm -> DrawCircle (pos, radius, col);
    return false;
}

bool Brush::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    last_pos = pos;
    perm -> DrawCircle (pos, radius, col);
    return true;
}


bool RectTool::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) {
    start_pos = pos;
    tmp -> DrawRect (Rect(pos, pos), col);
    return true;
}

bool RectTool::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    perm -> DrawRect (Rect(start_pos, pos), col);
    return false;
}

bool RectTool::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    tmp -> DrawRect (Rect(start_pos, pos), col);
    return true;
}


bool LineTool::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) {
    start_pos = pos;
    tmp -> DrawLine (pos, pos, col);
    return true;
}

bool LineTool::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    perm -> DrawLine (start_pos, pos, col);
    return false;
}

bool LineTool::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    tmp -> DrawLine (start_pos, pos, col);
    return true;
}


bool EllipseTool::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) {
    start_pos = pos;
    tmp -> DrawEllipse (Rect(pos, pos), col);
    return true;
}

bool EllipseTool::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    perm -> DrawEllipse (Rect(start_pos, pos), col);
    return false;
}

bool EllipseTool::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    tmp -> DrawEllipse (Rect(start_pos, pos), col);
    return true;
}


PolyLine::PolyLine () {
    start_pos = Vec(-1, -1);
}

bool PolyLine::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) {
    if (start_pos.x == -1 && start_pos.y == -1) {
        start_pos = pos;
        tmp -> DrawLine (pos, pos, col);
        return true;
    }
    else {
        perm -> DrawLine (start_pos, pos, col);
        if (btn == MOUSE_RIGHT) {
            start_pos = Vec (-1, -1);
            return false;
        }
        else {
            start_pos = pos;
            return true;
        }
    }
}

bool PolyLine::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) {
    return true;
}

bool PolyLine::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    tmp -> DrawLine (start_pos, pos, col);
    return true;
}


ToolBtn::ToolBtn (double x, double y, size_t w, size_t h, const Texture* textures_, const Text& txt_, ToolManager* tm, Tool* tool_):
    TxtButton (x, y, w, h, textures_, txt_),
    tool_man (tm),
    tool (tool_)
    {}

void ToolBtn::MousePress (const Vec& mousepos, MouseButton mousebtn) {
    if (MouseOnWidget(mousepos) && state != BTN_PRESSED) {
        tool_man -> SetTool (tool);
        state = BTN_PRESSED;
        Render (*rt, &regset);
    }
}

void ToolBtn::MouseRelease (const Vec& mousepos, MouseButton mousebtn) {
    if (state == BTN_PRESSED) {
        state = BTN_NORMAL;
        Render (*rt, &regset);
    }
}

ColorBtn::ColorBtn (double x, double y, size_t w, size_t h, const Texture* textures_, ToolManager* tm, const Color& col):
    ImgButton (x, y, w, h, textures_),
    tool_man (tm),
    color (col)
    {}

void ColorBtn::MousePress (const Vec& mousepos, MouseButton mousebtn) {
    if (MouseOnWidget(mousepos)) {
        tool_man -> SetColor (color);
        state = BTN_PRESSED;
        Render (*rt, &regset);
    }
}

void ColorBtn::MouseRelease (const Vec& mousepos, MouseButton mousebtn) {
    if (state == BTN_PRESSED) {
        state = BTN_NORMAL;
        Render (*rt, &regset);
    }
}

void ColorBtn::Render (RenderTarget& rt, RegionSet* to_draw) const {
    rt.DrawTexture (textures[state], pos, size, to_draw);
    rt.DrawRect (Rect (pos + size / 4, pos + size * 3 / 4), color, to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(0, rand() % 128 + 128, 0));
    #endif
}