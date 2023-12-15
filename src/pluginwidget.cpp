#include "pluginwidget.h"
#include <iostream>

PluginWidget::PluginWidget(GuiI* gui, Vec2 pos, Vec2 size) {
    gui -> createWidgetI(this);
    host -> setSize(size);
    host -> setPos(pos);
}


const Color PluginWindow::BG_COLOR  = Color (128, 128, 128);
const Color PluginWindow::BAR_COLOR = Color (0, 0, 0);
const double PluginWindow::BAR_HEIGHT = 30;

PluginWindow::PluginWindow (GuiI* gui, Vec2 pos, Vec2 size):
    PluginWidget(gui, pos, size),
    is_moving (false) {}

bool PluginWindow::onMousePress(MouseContext context) {
    if (context.button == MouseButton::Left &&
        context.position.x >= host -> getPos().x                       &&
        context.position.x <= host -> getPos().x + host -> getSize().x &&
        context.position.y >= host -> getPos().y                       &&
        context.position.y <= host -> getPos().y + BAR_HEIGHT)
    {
        is_moving = true;
        hold_pos = context.position;
        return true;
    }
    else return false;
}

bool PluginWindow::onMouseRelease(MouseContext context) {
    if (context.button == MouseButton::Left && is_moving) {
        is_moving = false;
        hold_pos = Vec2 (0, 0);
    }
    return false;
}

bool PluginWindow::onMouseMove(MouseContext context) {
    if (is_moving) {
        Vec2 mousepos = context.position;
        if (mousepos.x != hold_pos.x || mousepos.y != hold_pos.y) {
            host -> move (mousepos - hold_pos);
            hold_pos = mousepos;
            return true;
        }
    }
    return false;
}



void PluginWindow::render(RenderTargetI* rt) {
    rt -> drawRect(host -> getPos(), host -> getSize(), BAR_COLOR);
    rt -> drawRect(host -> getPos() + Vec2(2, BAR_HEIGHT), host -> getSize() - Vec2(4, BAR_HEIGHT + 2), BG_COLOR);
}