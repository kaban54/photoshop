#include "widget.h"
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>

const char* FONT_FILENAME = "fonts/font.ttf";

const int W = 2160;
const int H = 1440;

int main() {
    sf::Font font;
    font.loadFromFile (FONT_FILENAME);

    int frame_counter = 0;
    sf::Text fps_txt ("", font, 20);
    fps_txt.setPosition (0, 0);
    fps_txt.setFillColor (sf::Color::Cyan);
    char fps_str[8] = "";

    sf::RenderWindow window (sf::VideoMode (W, H), "PHOTOSHOP228", sf::Style::Fullscreen);
    window.setFramerateLimit (600);

    double mousex = 0;
    double mousey = 0;
    bool mouse_pressed = false;

    sf::Clock fps_clk;
    sf::Clock clk;
    double dt = 0;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent (event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }

            if (event.type == sf::Event::MouseMoved) {
                mousex = event.mouseMove.x;
                mousey = event.mouseMove.y;
            }

            if (event.type == sf::Event::MouseButtonPressed)
                mouse_pressed = true;

            if (event.type == sf::Event::MouseButtonReleased)
                mouse_pressed = false;
        }

        dt = clk.restart().asSeconds();

        frame_counter++;
        if (fps_clk.getElapsedTime().asSeconds() >= 1) {
            sprintf (fps_str, "%d", frame_counter);
            fps_txt.setString (fps_str);
            fps_clk.restart();
            frame_counter = 0;
        }
        
        window.clear(sf::Color (128, 128, 128));
        window.draw (fps_txt);
        window.display();
    }
    return 0;
}
