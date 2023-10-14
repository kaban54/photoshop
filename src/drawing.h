#ifndef DRAWING_H
#define DRAWING_H

#include "buttons.h"



class Tool {
    public:
    Vec start_pos;
    Vec last_pos;

    explicit Tool() {};

    virtual void PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) = 0;

    virtual void PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) = 0;

    virtual void PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) = 0;

};


class ToolManager {
    Tool* cur_tool;
    Color col;

    public:

    explicit ToolManager();

    void SetTool (Tool* tool);

    void SetColor (const Color& col_);

    void PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos);

    void PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos);
    
    void PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos);
};


class Canvas : public Widget {
    bool drawing;
    ToolManager* tool_man;
    RenderTarget data;
    RenderTarget tmp;

    public:

    explicit Canvas (int x, int y, int w, int h, ToolManager* tm);

    virtual void MousePress (const Vec& mousepos, MouseButtons mousebtn) override;

    virtual void MouseRelease (const Vec& mousepos, MouseButtons mousebtn) override;

    virtual void MouseMove (const Vec& mousepos) override;

    virtual bool MouseOnWidget (const Vec& mousepos) const override;

    virtual void Render (RenderTarget& rt, RegionSet* to_draw) const override;
};


class Brush : public Tool {
    unsigned int radius;

    public:

    explicit Brush (unsigned int r);

    void SetRadius (unsigned int r);

    virtual void PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) override;

    virtual void PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) override;

    virtual void PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) override;
};

class RectTool : public Tool {
    public:

    explicit RectTool() {}

    virtual void PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) override;

    virtual void PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) override;

    virtual void PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) override;
};


class ToolBtn : public TxtButton {
    ToolManager* tool_man;
    Tool* tool;

    public:

    ToolBtn (double x, double y, size_t w, size_t h, const Texture* textures_, const Text& txt_, ToolManager* tm, Tool* tool_);

    virtual void MousePress (const Vec& mousepos, MouseButtons mousebtn) override;

    virtual void MouseRelease (const Vec& mousepos, MouseButtons mousebtn) override;

};

#endif