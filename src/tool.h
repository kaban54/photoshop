#ifndef TOOL_H
#define TOOL_H

#include "interface.h"
#include "rendertarget.h"
#include "events.h"
#include "buttons.h"

namespace plugin {
    struct ToolI: public Interface {
        virtual const Texture *getIcon() = 0;

        virtual void paintOnPress   (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) = 0;
        virtual void paintOnRelease (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) = 0;
        virtual void paintOnMove    (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) = 0;
        virtual void disable        (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) = 0;
    };

    struct ToolManagerI {
        virtual void setColor(Color color) = 0;
        virtual void setTool(ToolI *tool) = 0;

        virtual ToolI *getTool() = 0;
        virtual Color  getColor() = 0;

        virtual void paintOnMove    (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) = 0;
        virtual void paintOnPress   (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) = 0;
        virtual void paintOnRelease (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) = 0;
        virtual void disableTool    (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) = 0;
    };
}

using namespace plugin;

struct Tool : public ToolI {
    bool drawing;
    Vec2 start_pos;
    Vec2 last_pos;
    Texture* icon;

    explicit Tool(Texture* icon_ = nullptr);

    void SetIcon(Texture* icon_);

    virtual const Texture *getIcon() override;
};

class ToolManager : public ToolManagerI{
    ToolI* tool;
    Color col;

    public:

    explicit ToolManager() {}

    virtual void setColor(Color color) override;
    virtual void setTool(ToolI *tool_) override;

    virtual ToolI *getTool() override;
    virtual Color  getColor() override;

    virtual void paintOnMove    (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) override;
    virtual void paintOnPress   (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) override;
    virtual void paintOnRelease (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) override;
    virtual void disableTool    (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) override;
};

class Brush : public Tool {
    static const size_t NUM_OF_PARAMS = 1;
    static const char* const PARAM_NAMES[];

    double params[1];

    public:

    explicit Brush (double r = 1);

    virtual void paintOnPress   (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) override;
    virtual void paintOnRelease (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) override;
    virtual void paintOnMove    (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) override;
    virtual void disable        (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) override;

    virtual Array<const char *> getParamNames() override;
    virtual Array<double> getParams() override;
    virtual void setParams(Array<double> new_params) override;
};


struct ToolBtnArgs : public BtnArgs {
    ToolManagerI* tool_man;
    ToolI* tool;
    explicit ToolBtnArgs (ToolManagerI* tool_man_, ToolI* tool_):
        tool_man (tool_man_),
        tool (tool_) {}
};

class ToolBtn : public TxtButton {
    ToolBtnArgs tool_btn_args;

    public:

    explicit ToolBtn (double x, double y, double w, double h, const char *str, uint16_t char_size_,
                      ToolManagerI* tm, ToolI* tool_);
};


struct ColorBtnArgs : public BtnArgs {
    ToolManagerI* tool_man;
    Color col;
    explicit ColorBtnArgs (ToolManagerI* tool_man_, Color col_):
        tool_man (tool_man_),
        col (col_) {}
};

class ColorBtn : public Button {
    ColorBtnArgs col_btn_args;

    public:

    explicit ColorBtn (double x, double y, double w, double h, ToolManagerI* tm, const Color& col);

    virtual void RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) override;
};

#endif