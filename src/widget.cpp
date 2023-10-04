#include "widget.h"

Widget::Widget():
    pos (0, 0),
    visible (true),
    subwidgets ()
    {}

Widget::Widget (int x, int y, size_t subw_cap):
    pos (x, y),
    visible (true),
    subwidgets (subw_cap)
    {}

void Widget::Render (sf::RenderWindow& sfwindow) const {
    if (visible) {
        RenderThis (sfwindow);
        subwidgets.Render (sfwindow);
    }
}

void Widget::AddSubWidget (Widget* wid) {
    wid -> Move (pos);
    subwidgets.AddWidget (wid);
}

void Widget::Move (const Vec& vec) {
    pos += vec;
}

void Widget::MousePress (const Vec& mousepos, MouseButtons mousebtn) {
    MousePressAction (mousebtn);
    subwidgets.MousePress (mousepos, mousebtn);
}

void Widget::MouseRelease (const Vec& mousepos, MouseButtons mousebtn) {
    MouseReleaseAction (mousebtn);
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

void WidgetManager::Render (sf::RenderWindow& sfwindow) const {
    for (size_t i = 0; i < size; i++) {
        widgets[i] -> Render (sfwindow);
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
    height (h)
    {}


Window::Window (int x, int y, size_t w, size_t h):
    Widget (x, y),
    width (w),
    height (h)
    {}


void Window::RenderThis (sf::RenderWindow& sfwindow) const {
    sf::RectangleShape rect (sf::Vector2f(width, height));
    rect.setPosition (pos.x, pos.y);
    rect.setOutlineThickness (2);
    rect.setFillColor (WINDOW_BG_COLOR);
    rect.setOutlineColor (sf::Color::Black);
    sfwindow.draw (rect);

    rect.setFillColor (sf::Color::Black);
    rect.setSize (sf::Vector2f(width, 20));
    sfwindow.draw (rect);
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
