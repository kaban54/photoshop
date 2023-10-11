#include "widget.h"

Widget::Widget():
    pos (0, 0),
    visible (true),
    parent (nullptr),
    subwidgets ()
    {}

Widget::Widget (int x, int y, size_t subw_cap):
    pos (x, y),
    visible (true),
    parent (nullptr),
    subwidgets (subw_cap)
    {}


void Widget::AddSubWidget (Widget* wid) {
    wid -> Move (pos);
    wid -> parent = this;
    subwidgets.AddWidget (wid);
}

void Widget::RenderSubWidgets (RenderTarget& screen) const {
    subwidgets.Render(screen);
}

void Widget::Move (const Vec& vec) {
    pos += vec;
    subwidgets.Move(vec);
}

void Widget::MousePress (const Vec& mousepos, MouseButtons mousebtn) {
    MousePressAction (mousepos, mousebtn);
    subwidgets.MousePress (mousepos, mousebtn);
}

void Widget::MouseRelease (const Vec& mousepos, MouseButtons mousebtn) {
    MouseReleaseAction (mousepos, mousebtn);
    subwidgets.MouseRelease (mousepos, mousebtn);
}

void Widget::MouseMove (const Vec& mousepos) {
    MouseMoveAction (mousepos);
    subwidgets.MouseMove (mousepos);
}




WidgetManager::WidgetManager (size_t cap):
    size (0),
    capacity (cap)
    {
        widgets = (Widget**) calloc (capacity, sizeof (widgets[0]));
        assert (widgets != nullptr);
    }

WidgetManager::~WidgetManager() {
    for (size_t i = 0; i < size; i++) {
        delete widgets[i];
    }
    free (widgets);
}

void WidgetManager::AddWidget (Widget *wid) {
    if (size >= capacity) {
        widgets = (Widget**) Recalloc (widgets, capacity * 2, sizeof (widgets[0]), capacity);
        assert (widgets != nullptr);
        capacity *= 2;
    }
    widgets[size++] = wid;
}

void WidgetManager::Render (RenderTarget& screen) const {
    for (size_t i = 0; i < size; i++) {
        widgets[i] -> Render (screen);
    }
}

void WidgetManager::Move (const Vec& vec) {
    for (size_t i = 0; i < size; i++) {
        widgets[i] -> Move (vec);
    }
}

void WidgetManager::MousePress (const Vec& mousepos, MouseButtons mousebtn) {
    for (size_t i = 0; i < size; i++) {
        if (widgets[i] -> MouseOnWidget (mousepos))
            widgets[i] -> MousePress (mousepos, mousebtn);
    }
}

void WidgetManager::MouseRelease (const Vec& mousepos, MouseButtons mousebtn) {
    for (size_t i = 0; i < size; i++) {
        widgets[i] -> MouseRelease (mousepos, mousebtn);
    }
}

void WidgetManager::MouseMove (const Vec& mousepos) {
    for (size_t i = 0; i < size; i++) {
        widgets[i] -> MouseMove (mousepos);
    }
}



Window::Window (size_t w, size_t h):
    width (w),
    height (h),
    is_moving (false)
    {}


Window::Window (int x, int y, size_t w, size_t h):
    Widget (x, y),
    width (w),
    height (h),
    is_moving (false)
    {}


void Window::Render (RenderTarget& screen) const {
    Rect rect (Vec(pos.x, pos.y), Vec(pos.x + width, pos.y + height));
    screen.DrawRect (rect, Color(0, 0, 0), regset);
    rect.vert1 += Vec (2, 20);
    rect.vert2 -= Vec (2, 2);
    screen.DrawRect (rect, WINDOW_BG_COLOR, regset);

    RenderSubWidgets(screen);
}

void Window::MousePressAction (const Vec& mousepos, MouseButtons mousebtn) {
    if (mousepos.x >= pos.x && mousepos.x <= pos.x + width &&
        mousepos.y >= pos.y && mousepos.y <= pos.y + 30 && mousebtn == MOUSE_LEFT) {
            is_moving = true;
            hold_pos = mousepos;
        }
}

void Window::MouseReleaseAction (const Vec& mousepos, MouseButtons mousebtn) {
    if (mousebtn == MOUSE_RIGHT) {
        is_moving = false;
        hold_pos = Vec (0, 0);
    }
}

void Window::MouseMoveAction (const Vec& mousepos) {
    if (is_moving) {
        if (mousepos.x != hold_pos.x || mousepos.y != hold_pos.y) {
            Move (mousepos - hold_pos);
            hold_pos = mousepos;
        }
    }
}

bool Window::MouseOnWidget (const Vec& mousepos) {
    return (mousepos.x >= pos.x && mousepos.x <= pos.x + width ) &&
           (mousepos.y >= pos.y && mousepos.y <= pos.y + height);
}



Button::Button (double x, double y, size_t w, size_t h):
    Widget (x, y, 0),
    width (w),
    height (h)
    {}




void *Recalloc (void *memptr, size_t num, size_t size, size_t old_num) {
    memptr = realloc (memptr, num * size);
    if (memptr == nullptr) return nullptr;

    if (num > old_num) memset ((void *) ((char *) memptr + old_num * size), 0, (num - old_num) * size);

    return memptr;
}
