#ifndef BUTTONS_H
#define BUTTONS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <assert.h>
#include <cstdlib>
#include <string>
#include <iostream>

const size_t TXTBTN_BUFSIZE = 128;

enum ButtonState {
    BTN_NORMAL = 0,
    BTN_PRESSED = 1,
    BTN_FOCUSED = 2,
    BTN_DISABLED = 3,
};


class Button {
    public:

    double x;
    double y;
    double w;
    double h;
    ButtonState state;

    explicit Button (double x_, double y_, double w_, double h_);

    explicit Button();

    void SetPos (double x_, double y_);

    void SetSize (double w_, double h_);

    void SetState (double mousex, double mousey, bool mouse_pressed, double dt);

    bool MouseOnButton (double mousex, double mousey) const;

    virtual void Press() = 0;

    virtual void Pressed(double dt) = 0;

    virtual void Release() = 0;

    virtual void Draw (sf::RenderWindow& window) = 0;
};


class TextButton : public Button {
    sf::Text text;

    public:

    explicit TextButton (double x_, double y_, double w_, double h_, const sf::Font& font, const char* str = "");

    void SetText (const char* str);

    void SetFont (const sf::Font& font);

    virtual void Draw (sf::RenderWindow& window) override;
};


class ImageButton : public Button {
    const sf::Texture* textures[4];

    public:

    explicit ImageButton (double x_, double y_, double w_, double h_);

    explicit ImageButton();

    void SetTextures (const sf::Texture* t0, const sf::Texture* t1, const sf::Texture* t2, const sf::Texture* t3);

    virtual void Draw (sf::RenderWindow& window) override;
};

class ButtonManager {
    std::vector<Button*> btns;

    public:

    ~ButtonManager();

    void AddButton (Button *btn);

    void DrawButtons (sf::RenderWindow& window);

    void SetStates (double mousex, double mousey, bool mouse_pressed, double dt);
};

#endif