#include "canvas.h"

Canvas::Canvas(double x, double y, double w, double h, ToolManager* tm, FilterManager* fm, CanvasWindow* win_):
    Widget (x, y, w, h), 
    drawing (false),
    tool_man (tm),
    filter_man (fm),
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
        filter_man -> setRenderTarget(&data);
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
                           ImageManager* image_man_, ToolManager* tool_man, FilterManager* filter_man):
    Window (x, y, w, h),
    image_man (image_man_),
    canvas (new Canvas(5, 35, w - 10, h - 40, tool_man, filter_man, this))
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
    windows() {}

void ImageManager::AddWindow(CanvasWindow* win) {
    if (windows.GetNode(win) == nullptr) {
        windows.InsertHead(win);
    }
}

void ImageManager::RemoveWindow(CanvasWindow* win) {
    ListNode<CanvasWindow*>* node = windows.GetNode(win);
    if (node != nullptr) windows.Remove(node);
}

CanvasWindow* ImageManager::GetActive() {
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