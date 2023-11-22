#include "tool.h"

Tool::Tool (Texture* icon_):
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


Brush::Brush (unsigned int r):
    radius (r)
    {}

void Brush::paintOnPress (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) {
    data -> drawEllipse (context.position - Vec2(radius, radius), Vec2(radius * 2, radius * 2), color);
    last_pos = context.position;
}

void Brush::paintOnRelease (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) {}

void Brush::paintOnMove (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) {
    data -> drawEllipse (context.position - Vec2(radius, radius), Vec2(radius * 2, radius * 2), color);
    //data -> drawLine (last_pos, pos, radius * 2, col);
    last_pos = context.position;
}

void Brush::disable (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) {}