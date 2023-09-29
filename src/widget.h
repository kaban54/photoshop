#ifndef WIDGET_H
#define WIDGET_H

#include "vec.h"

class Widget {
    public:
    Widget** subwidgets;
    Vec pos;

    explicit Widget();

    explicit Widget(double x, double y);

    ~Widget();

    virtual void Render(sf::RenderWindow& sfwindow) const = 0;
};

class Window : public Widget {
    size_t width;
    size_t height;
    public:

    explicit Window();

    explicit Window(size_t w, size_t h);

    virtual void Render(sf::RenderWindow& sfwindow) const override;
};

#endif
