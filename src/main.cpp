#include "widget.h"
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>

const char* FONT_FILENAME = "fonts/font.ttf";
sf::Font GLOBAL_FONT;

const int W = 2160;
const int H = 1440;

int main() {
    GLOBAL_FONT.loadFromFile (FONT_FILENAME);

    int frame_counter = 0;
    sf::Text fps_txt ("", GLOBAL_FONT, 20);
    fps_txt.setPosition (0, 0);
    fps_txt.setFillColor (sf::Color::Cyan);
    char fps_str[8] = "";

    sf::RenderWindow sfwindow (sf::VideoMode (W, H), "PHOTOSHOP228", sf::Style::Fullscreen);
    sfwindow.setFramerateLimit (600);

    Vec mousepos (0, 0);
    bool mouse_pressed = false;

    sf::Clock fps_clk;
    sf::Clock clk;
    double dt = 0;

    Window mainwin (400, 400, 800, 800);
    mainwin.AddSubWidget (new Window (100, 100, 200, 300));


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

        dt = clk.restart().asSeconds();

        frame_counter++;
        if (fps_clk.getElapsedTime().asSeconds() >= 1) {
            sprintf (fps_str, "%d", frame_counter);
            fps_txt.setString (fps_str);
            fps_clk.restart();
            frame_counter = 0;
        }
        
        sfwindow.clear(sf::Color (192, 192, 192));
        mainwin.Render(sfwindow);
        sfwindow.draw (fps_txt);
        sfwindow.display();
    }
    return 0;
}
