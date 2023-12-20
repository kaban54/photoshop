#include "canvas.h"

Canvas::Canvas(double x, double y, double w, double h, ToolManager* tm, CanvasWindow* win_):
    Widget (x, y, w, h), 
    drawing (false),
    tool_man (tm),
    win (win_),
    data (w, h),
    tmp (w, h)
    {
        data.Fill(Color(255, 255, 255));
        tmp.Fill(Color(0, 0, 0, 0));
    }

bool Canvas::onMousePress (MouseContext context) {
    if (MouseOnWidget(context.position)) {
        if (win != nullptr) win -> SetActive();
        Show();
        drawing = true;
        tool_man -> paintOnPress (&data, &tmp, MouseContext(context.position - getPos(), context.button));
        RenderInRegset (*GetRendertarget(), GetRegset());
    }
    return false;
}

bool Canvas::onMouseRelease (MouseContext context) {
    if (drawing) {
        tool_man -> paintOnRelease (&data, &tmp, MouseContext(context.position - getPos(), context.button));
        RenderInRegset (*GetRendertarget(), GetRegset());
    }
    return false;
}

bool Canvas::onMouseMove (MouseContext context) {
    if (drawing) {
        if (MouseOnWidget (context.position))
            tool_man -> paintOnMove (&data, &tmp, MouseContext(context.position - getPos(), context.button));
        else {
            tool_man -> disableTool (&data, &tmp, MouseContext(context.position - getPos(), context.button));
            drawing = false;
        }
        RenderInRegset (*GetRendertarget(), GetRegset());
    }
    return false;
}

bool Canvas::onClock (uint64_t delta) {
    if (data.changed) {
        data.changed = false;
        RenderInRegset(*GetRendertarget(), GetRegset());
    }
    return false;
}

void Canvas::RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) {
    rt.DrawRenderTarget_rs(data, getPos(), to_draw);
    rt.DrawRenderTarget_rs(tmp , getPos(), to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(0, 255, 255, 128));
    #endif
}


CanvasWindow::CanvasWindow(double x, double y, double w, double h,
                           ImageManager* image_man_, ToolManager* tool_man):
    Window (x, y, w, h),
    image_man (image_man_),
    canvas (new Canvas(5, 35, w - 10, h - 40, tool_man, this))
    {
        registerSubWidget(canvas);
        image_man -> AddWindow(this);
    }

void CanvasWindow::SetActive() {
    image_man -> SetActive(this);
}

void CanvasWindow::Close() {
    image_man -> RemoveWindow(this);
    delete this;
}


ImageManager::ImageManager():
    windows(),
    window_menu (nullptr) {}

void ImageManager::SetWindowMenu(VerticalMenu* wm) {
    window_menu = wm;
    if (wm == nullptr) return;
    if (windows.GetSize() > 0) {
        ListNode<CanvasWindow*>* node = nullptr;
        windows.Iterate(node);
        while (node != nullptr) {
            node -> val -> SetShowBtn(new WindowShowBtn(0, 0, 300, 80, node -> val));
            window_menu -> AddButton(node -> val -> GetShowBtn());
            windows.Iterate(node);
        }
    }
}

void ImageManager::AddWindow(CanvasWindow* win) {
    if (windows.GetNode(win) == nullptr) {
        windows.InsertHead(win);
        if (window_menu != nullptr) {
            win -> SetShowBtn(new WindowShowBtn(0, 0, 300, 80, win));
            window_menu -> AddButton(win -> GetShowBtn());
        }
    }
}

void ImageManager::RemoveWindow(CanvasWindow* win) {
    ListNode<CanvasWindow*>* win_node = windows.GetNode(win);
    if (win_node != nullptr) windows.Remove(win_node);

    window_menu -> Reset();
    if (windows.GetSize() > 0) {
        ListNode<CanvasWindow*>* node = nullptr;
        windows.Iterate(node);
        while (node != nullptr) {
            node -> val -> SetShowBtn(new WindowShowBtn(0, 0, 300, 80, node -> val));
            window_menu -> AddButton(node -> val -> GetShowBtn());
            windows.Iterate(node);
        }
    }
}

CanvasWindow* ImageManager::GetActive() const {
    if (windows.GetSize() == 0) return nullptr;
    else return windows.GetTail() -> val;
}

void ImageManager::SetActive(CanvasWindow* win) {
    ListNode<CanvasWindow*>* node = windows.GetNode(win);
    if (node != nullptr) {
        node -> next -> prev = node -> prev;
        node -> prev -> next = node -> next;
        node -> prev = windows.GetTail();
        node -> next = windows.EndOfList();
        node -> next -> prev = node;
        node -> prev -> next = node;
    }
}

void window_show_btn_action (BtnArgs* btn_args) {
    WindowShowBtnArgs* args = (WindowShowBtnArgs*) btn_args;
    if (args -> win) args -> win -> Show();
    if (args -> btn) args -> btn -> Show();
}

WindowShowBtn::WindowShowBtn (double x, double y, double w, double h, Window* win_):
    TxtButton (x, y, w, h, window_show_btn_action, &wshow_btn_args, win_ -> GetName()),
    wshow_btn_args (win_, this)
    {}