#include "pluginwidget.h"
#include <iostream>

PluginWidget::PluginWidget(GuiI* gui, Vec2 pos, Vec2 size) {
    gui -> createWidgetI(this);
    host -> setSize(size);
    host -> setPos(pos);
}

bool PluginWidget::MouseOnWidget(Vec2 mpos) {
    Vec2 pos  = host -> getPos();
    Vec2 size = host -> getSize();
    return (mpos.x > pos.x && mpos.x < pos.x + size.x &&
            mpos.y > pos.y && mpos.y < pos.y + size.y);
}


PluginTxtButton::PluginTxtButton(GuiI* gui, Vec2 pos, Vec2 size, BtnFunc* action_, BtnArgs* action_args_,
                                 const char *str, uint16_t char_size_):
    PluginWidget(gui, pos, size),
    action (action_),
    action_args (action_args_),
    state (BTN_NORMAL),
    txt (str),
    char_size (char_size_)
    {}

void PluginTxtButton::SetText (const char *str, uint16_t char_size_) {
    txt = str;
    char_size = char_size_;
}

bool PluginTxtButton::onMousePress(MouseContext context) {
    if (state == BTN_DISABLED) return false;
    if (MouseOnWidget(context.position)) {
        state = BTN_PRESSED;
        action(action_args);
        return true;
    }
    return false;
}

bool PluginTxtButton::onMouseRelease(MouseContext context) {
    if (state == BTN_PRESSED) {
        if (MouseOnWidget(context.position)) state = BTN_FOCUSED;
        else                                 state = BTN_NORMAL;
    }
    return false;
}

bool PluginTxtButton::onMouseMove(MouseContext context) {
    if (state == BTN_DISABLED) return false;

    if (MouseOnWidget (context.position)) {
        if (state == BTN_NORMAL) state = BTN_FOCUSED;
    }
    else if (state == BTN_FOCUSED) state = BTN_NORMAL;
    return false;
}

void PluginTxtButton::render(RenderTargetI* rt) {
    DrawButton (rt, host -> getPos(), host -> getSize(), state);
    rt -> drawText(host -> getPos() + txt_pos, txt, char_size, Color(0, 0, 0));
}

void DrawButton (RenderTargetI* rt, Vec2 pos, Vec2 size, ButtonState state) {
    if (state == BTN_NORMAL) {
        rt -> drawRect(pos                                , size,                 Color(128, 128, 128));
        rt -> drawRect(pos                                , Vec2(size.x - 4, 8) , Color(192, 192, 192));
        rt -> drawRect(pos + Vec2(0, 8)                   , Vec2(8, size.y - 12), Color(192, 192, 192));
        rt -> drawRect(Vec2(pos.x + 4, pos.y + size.y - 8), Vec2(size.x - 4, 8) , Color(64, 64, 64));
        rt -> drawRect(Vec2(pos.x + size.x - 8, pos.y + 4), Vec2(8, size.y - 12), Color(64, 64, 64));
        rt -> drawRect(Vec2(pos.x + 4, pos.y + size.y - 8), Vec2(4, 4)          , Color (128, 128, 128));
        rt -> drawRect(Vec2(pos.x + size.x - 8, pos.y + 4), Vec2(4, 4)          , Color (128, 128, 128));
    }
    else if (state == BTN_FOCUSED) {
        rt -> drawRect(pos                                , size,                 Color(160, 160, 160));
        rt -> drawRect(pos                                , Vec2(size.x - 4, 8) , Color(224, 224, 224));
        rt -> drawRect(pos + Vec2(0, 8)                   , Vec2(8, size.y - 12), Color(224, 224, 224));
        rt -> drawRect(Vec2(pos.x + 4, pos.y + size.y - 8), Vec2(size.x - 4, 8) , Color(96, 96, 96));
        rt -> drawRect(Vec2(pos.x + size.x - 8, pos.y + 4), Vec2(8, size.y - 12), Color(96, 96, 96));
        rt -> drawRect(Vec2(pos.x + 4, pos.y + size.y - 8), Vec2(4, 4)          , Color (160, 160, 160));
        rt -> drawRect(Vec2(pos.x + size.x - 8, pos.y + 4), Vec2(4, 4)          , Color (160, 160, 160));
    }
    else if (state == BTN_PRESSED) {
        rt -> drawRect (pos, size, Color(64, 64, 64));
        rt -> drawRect (pos + Vec2(4, 4), size - Vec2(8, 8), Color (128, 128, 128));
    }
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