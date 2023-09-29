#include "buttons.h"

Button::Button (double x_, double y_, double w_, double h_):
    x (x_),
    y (y_),
    w (w_),
    h (h_),
    state (BTN_NORMAL)
    {}

Button::Button():
    x (0),
    y (0),
    w (100),
    h (100),
    state (BTN_NORMAL)
    {}

void Button::SetPos (double x_, double y_) {
    x = x_;
    y = y_;
}

void Button::SetSize (double w_, double h_) {
    w = w_;
    h = h_;
}

void Button::SetState (double mousex, double mousey, bool mouse_pressed, double dt) {
    if (state == BTN_DISABLED) return;

    if (MouseOnButton (mousex, mousey)) {
        if (mouse_pressed) {
            if (state == BTN_NORMAL || state == BTN_FOCUSED) {
                state = BTN_PRESSED;
                Press();
            }
            else Pressed (dt);
        }
        else {
            if (state == BTN_NORMAL)
                state = BTN_FOCUSED;
            else if (state == BTN_PRESSED) {
                state = BTN_FOCUSED;
                Release();
            }
        }
    }
    else {
        if (state == BTN_FOCUSED)
            state = BTN_NORMAL;
        else if (state == BTN_PRESSED) {
            state = BTN_NORMAL;
            Release();
        }
    }
}

bool Button::MouseOnButton (double mousex, double mousey) const {
    return (mousex >= x && mousex <= x + w && mousey >= y && mousey <= y + h);
}


TextButton::TextButton (double x_, double y_, double w_, double h_, const sf::Font& font, const char* str):
    Button (x_, y_, w_, h_),
    text (str, font)
    {
        text.setFillColor (sf::Color::Black);
    }

void TextButton::SetText (const char* str) {
    text.setString (str);
}

void TextButton::SetFont (const sf::Font& font) {
    text.setFont (font);
}

void TextButton::Draw (sf::RenderWindow& window) {
    sf::RectangleShape rect (sf::Vector2f (w, h));
    rect.setFillColor (sf::Color (255 - state * 80, 255 - state * 80, 255 - state * 80)); // TEST
    rect.setPosition (x, y);
    text.setPosition (x, y);
    
    window.draw (rect);
    window.draw (text);
}


ImageButton::ImageButton (double x_, double y_, double w_, double h_):
    Button (x_, y_, w_, h_),
    textures ()
    {}

ImageButton::ImageButton():
    Button (),
    textures ()
    {}

void ImageButton::SetTextures (const sf::Texture* t0, const sf::Texture* t1, const sf::Texture* t2, const sf::Texture* t3) {
    textures[0] = t0;
    textures[1] = t1;
    textures[2] = t2;
    textures[3] = t3;
}

void ImageButton::Draw (sf::RenderWindow& window) {
    sf::Sprite sprite;
    sf::Texture texture;

    if (textures[state] == nullptr) {
        sf::Image white_img;
        white_img.create (1, 1, sf::Color::White);
        texture.loadFromImage (white_img);
    }
    else texture = *(textures[state]);

    sprite.setTexture (texture);
    sprite.setScale (w / texture.getSize().x, h / texture.getSize().y);
    sprite.setPosition (x, y);
    window.draw (sprite);
}


ButtonManager::~ButtonManager() {
    for (size_t i = 0; i < btns.size(); i++) {
        delete btns[i];
    }
}

void ButtonManager::AddButton (Button *btn) {
    btns.push_back (btn);
}

void ButtonManager::DrawButtons (sf::RenderWindow& window) {
    for (size_t i = 0; i < btns.size(); i++) {
        btns[i] -> Draw (window);
    }
}

void ButtonManager::SetStates (double mousex, double mousey, bool mouse_pressed, double dt) {
    for (size_t i = 0; i < btns.size(); i++) {
        btns[i] -> SetState (mousex, mousey, mouse_pressed, dt);
    }
}