#ifndef DRAWING_H
#define DRAWING_H

#include "buttons.h"



class Tool {
    public:
    Vec start_pos;
    Vec last_pos;

    explicit Tool() {};

    virtual bool PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) = 0;

    virtual bool PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) = 0;

    virtual bool PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) = 0;

};


class ToolManager {
    Tool* cur_tool;
    Color col;

    public:

    explicit ToolManager();

    void SetTool (Tool* tool);

    void SetColor (const Color& col_);

    bool PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn);

    bool PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn);

    bool PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos);
};


class Canvas : public Widget {
    bool drawing;
    ToolManager* tool_man;
    RenderTarget data;
    RenderTarget tmp;

    public:

    explicit Canvas (int x, int y, int w, int h, ToolManager* tm);

    virtual void MousePress (const Vec& mousepos, MouseButton mousebtn) override;

    virtual void MouseRelease (const Vec& mousepos, MouseButton mousebtn) override;

    virtual void MouseMove (const Vec& mousepos) override;

    virtual bool MouseOnWidget (const Vec& mousepos) const override;

    virtual void Render (RenderTarget& rt, RegionSet* to_draw) const override;
};


class Brush : public Tool {
    unsigned int radius;

    public:

    explicit Brush (unsigned int r);

    void SetRadius (unsigned int r);

    virtual bool PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) override;

    virtual bool PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) override;

    virtual bool PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) override;
};

class RectTool : public Tool {
    public:

    explicit RectTool() {}

    virtual bool PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) override;

    virtual bool PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) override;

    virtual bool PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) override;
};

class LineTool : public Tool {
    public:

    explicit LineTool() {}

    virtual bool PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) override;

    virtual bool PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) override;

    virtual bool PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) override;
};

class EllipseTool : public Tool {
    public:

    explicit EllipseTool() {}

    virtual bool PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) override;

    virtual bool PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) override;

    virtual bool PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) override;
};

class PolyLine : public Tool {
    public:

    explicit PolyLine();

    virtual bool PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) override;

    virtual bool PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, MouseButton btn, const Color& col) override;

    virtual bool PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) override;
};


class ToolBtn : public TxtButton {
    ToolManager* tool_man;
    Tool* tool;

    public:

    ToolBtn (double x, double y, size_t w, size_t h, const Texture* textures_, const Text& txt_, ToolManager* tm, Tool* tool_);

    virtual void MousePress (const Vec& mousepos, MouseButton mousebtn) override;

    virtual void MouseRelease (const Vec& mousepos, MouseButton mousebtn) override;

};

class ColorBtn : public ImgButton {
    ToolManager* tool_man;
    Color color;

    public:

    ColorBtn (double x, double y, size_t w, size_t h, const Texture* textures_, ToolManager* tm, const Color& col);

    virtual void MousePress (const Vec& mousepos, MouseButton mousebtn) override;

    virtual void MouseRelease (const Vec& mousepos, MouseButton mousebtn) override;

    virtual void Render (RenderTarget& rt, RegionSet* to_draw) const override;
};

#endif