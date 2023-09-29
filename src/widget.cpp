#include "widget.h"

Widget::Widget():
    pos (0, 0)
    {}

Widget::Widget (int x, int y):
    pos (x, y)
    {}

void Widget::Render (sf::RenderWindow& sfwindow) const {
    RenderThis (sfwindow);
    subwidgets.Render (sfwindow);
}

void Widget::AddSubWidget (Widget* wid) {
    wid -> Move (pos);
    subwidgets.AddWidget (wid);
}

void Widget::Move (const Vec& vec) {
    pos += vec;
}


WidgetManager::WidgetManager():
    size (0),
    capacity (BASE_WIDGETMAN_CAP)
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
}



void *Recalloc (void *memptr, size_t num, size_t size, size_t old_num) {
    memptr = realloc (memptr, num * size);
    if (memptr == nullptr) return nullptr;

    if (num > old_num) memset ((void *) ((char *) memptr + old_num * size), 0, (num - old_num) * size);

    return memptr;
}
