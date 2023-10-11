#include "widget.h"
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "rendertarget.h"

const char* FONT_FILENAME = "fonts/font.ttf";
sf::Font GLOBAL_FONT;

const int W = 2160;
const int H = 1440;

int main() {
    GLOBAL_FONT.loadFromFile (FONT_FILENAME);

    sf::RenderWindow sfwindow (sf::VideoMode (W, H), "PHOTOSHOP228", sf::Style::Fullscreen);
    sfwindow.setFramerateLimit (300);

    Vec mousepos (0, 0);
    bool mouse_pressed = false;

    Window mainwin (200, 300, 800, 800);
    mainwin.AddSubWidget (new Window (100, 100, 200, 300));

    RenderTarget rt (W, H);

    while (sfwindow.isOpen()) {
        sf::Event event;

        while (sfwindow.pollEvent (event)) {
            if (event.type == sf::Event::Closed)
                sfwindow.close();
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    sfwindow.close();
            }

            if (event.type == sf::Event::MouseMoved) {
                mousepos.x = event.mouseMove.x;
                mousepos.y = event.mouseMove.y;
                mainwin.MouseMove (mousepos);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                mainwin.MousePress (mousepos, MOUSE_LEFT);
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                mainwin.MouseRelease (mousepos, MOUSE_RIGHT);
            }
        }
        
        
        sfwindow.clear(sf::Color (192, 192, 192));
        rt.ClearScreen (sf::Color (192, 192, 192));
        mainwin.Render(rt);
        rt.Display(sfwindow);
        //sfwindow.draw (fps_txt);
        sfwindow.display();
    }
    return 0;
}
