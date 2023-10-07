#include "rendertarget.h"

RenderTarget::RenderTarget (unsigned int w, unsigned int h):
    screen ()
    {
        screen.create (w, h);
    }

void RenderTarget::Display (sf::RenderWindow& window) const {
    sf::Sprite sprite;
    sprite.setTexture (screen.getTexture());
    sprite.setPosition (0, 0);
    window.draw (sprite);
}