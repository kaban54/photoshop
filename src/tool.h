#ifndef TOOL_H
#define TOOL_H

#include "interface.h"
#include "rendertarget.h"
#include "events.h"
#include "buttons.h"

namespace plugin {
    struct ToolI: public Interface {
        virtual const Texture *getIcon() const = 0;

        virtual void paintOnPress   (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) = 0;
        virtual void paintOnRelease (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) = 0;
        virtual void paintOnMove    (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) = 0;
        virtual void disable        (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) = 0;

        virtual ~ToolI() = default;
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

    virtual const Texture *getIcon() const override;
};

class ToolManager {
    ToolI* tool;
    Color col;

    public:

    explicit ToolManager() {}

    void setColor(Color color);
    void setTool(ToolI *tool_);

    ToolI *getTool();
    Color getColor();

    void paintOnMove    (RenderTargetI *data, RenderTargetI *tmp, MouseContext context);
    void paintOnPress   (RenderTargetI *data, RenderTargetI *tmp, MouseContext context);
    void paintOnRelease (RenderTargetI *data, RenderTargetI *tmp, MouseContext context);
    void disableTool    (RenderTargetI *data, RenderTargetI *tmp, MouseContext context);
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

    virtual Array<const char *> getParamNames() const override;
    virtual Array<double> getParams() const override;
    virtual void setParams(Array<double> new_params) override;
};


struct ToolBtnArgs : public BtnArgs {
    ToolManager* tool_man;
    ToolI* tool;
    explicit ToolBtnArgs (ToolManager* tool_man_, ToolI* tool_):
        tool_man (tool_man_),
        tool (tool_) {}
};

class ToolTxtBtn : public TxtButton {
    ToolBtnArgs tool_btn_args;

    public:

    explicit ToolTxtBtn (double x, double y, double w, double h, const char *str, uint16_t char_size_,
                      ToolManager* tm, ToolI* tool_);
};

class ToolImgBtn : public ImgButton {
    ToolBtnArgs tool_btn_args;

    public:

    explicit ToolImgBtn (double x, double y, double w, double h, const Texture* texture,
                      ToolManager* tm, ToolI* tool_);
};


struct ColorBtnArgs : public BtnArgs {
    ToolManager* tool_man;
    Color col;
    explicit ColorBtnArgs (ToolManager* tool_man_, Color col_):
        tool_man (tool_man_),
        col (col_) {}
};

class ColorBtn : public Button {
    ColorBtnArgs col_btn_args;

    public:

    explicit ColorBtn (double x, double y, double w, double h, ToolManager* tm, const Color& col);

    virtual void RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) override;
};

#endif