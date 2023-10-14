#ifndef DRAWING_H
#define DRAWING_H

#include "buttons.h"



class Tool {
    Vec start_pos;
    Vec last_pos;

    public:

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

    virtual void Render (RenderTarget& rt, const RegionSet& to_draw) const override;
};


#endif