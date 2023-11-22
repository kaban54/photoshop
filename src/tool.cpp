#include "tool.h"

Tool::Tool (Texture* icon_):
    drawing (false),
    icon (icon_),
    start_pos (),
    last_pos () {}


inline void Tool::SetIcon (Texture* icon_) {
    icon = icon_;
}

inline const Texture* Tool::getIcon() {
    return icon;
}


inline void ToolManager::setColor(Color color) {
    col = color;
}

inline void ToolManager::setTool(ToolI *tool_) {
    tool = tool_;
}

inline ToolI* ToolManager::getTool() {
    return tool;
}

inline Color ToolManager::getColor() {
    return col;
}

inline void ToolManager::paintOnMove (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) {
    tool -> paintOnMove(data, tmp, context, col);
}

inline void ToolManager::paintOnPress (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) {
    tool -> paintOnPress(data, tmp, context, col);
}

inline void ToolManager::paintOnRelease (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) {
    tool -> paintOnRelease(data, tmp, context, col);
}

inline void ToolManager::disableTool (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) {
    tool -> disable(data, tmp, context, col);
}


Brush::Brush (double r) {
    params[0] = r;
}

const char* const Brush::PARAM_NAMES[] = {"radius"};

void Brush::paintOnPress (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) {
    drawing = true;
    double radius = params[0];
    data -> drawEllipse (context.position - Vec2(radius, radius), Vec2(radius * 2, radius * 2), color);
    last_pos = context.position;
}

void Brush::paintOnRelease (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) {
    drawing = false;
}

void Brush::paintOnMove (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) {
    if (!drawing) return;
    double radius = params[0];
    data -> drawEllipse (context.position - Vec2(radius, radius), Vec2(radius * 2, radius * 2), color);
    //data -> drawLine (last_pos, pos, radius * 2, col);
    last_pos = context.position;
}

void Brush::disable (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) {
    drawing = false;
}

Array<const char *> Brush::getParamNames() {
    MyVector<const char *> ret (NUM_OF_PARAMS);
    for (size_t i = 0; i < NUM_OF_PARAMS; i++) ret[i] = PARAM_NAMES[i];
    return Array<const char *> (ret);
}

Array<double> Brush::getParams() {
    return Array<double> (NUM_OF_PARAMS, params);
}

void Brush::setParams(Array<double> new_params) {
    assert(NUM_OF_PARAMS == new_params.size);
    for (size_t i = 0; i < NUM_OF_PARAMS; i++) params[i] = new_params.data[i];
}



void tool_btn_action (BtnArgs* tool_btn_args) {
    ToolManagerI* tool_man = ((ToolBtnArgs*)tool_btn_args) -> tool_man;
    ToolI*        tool     = ((ToolBtnArgs*)tool_btn_args) -> tool;
    tool_man -> setTool (tool);
}

ToolBtn::ToolBtn (double x, double y, double w, double h, const char *str, uint16_t char_size_,
                  ToolManagerI* tm, ToolI* tool_):
    TxtButton (x, y, w, h, tool_btn_action, &tool_btn_args, str, char_size_),
    tool_btn_args (tm, tool_)
    {}


void col_btn_action (BtnArgs* col_btn_args) {
    ToolManagerI* tool_man = ((ColorBtnArgs*)col_btn_args) -> tool_man;
    Color         color    = ((ColorBtnArgs*)col_btn_args) -> col;
    tool_man -> setColor (color);
}

ColorBtn::ColorBtn (double x, double y, double w, double h, ToolManagerI* tm, const Color& col):
    Button (x, y, w, h, col_btn_action, &col_btn_args),
    col_btn_args {tm, col}
    {}

void ColorBtn::RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) {
    DrawButton (rt, GetBounds(), state, to_draw);
    rt.DrawRect_rs (Rect (getPos() + getSize() / 4, getPos() + getSize() * 3 / 4), col_btn_args.col, to_draw);
}