#include "mylist.h"
#include "region.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>

void DrawRegset (sf::RenderWindow& window, const RegionSet& regset,
                 const sf::Color& col, int text_pos = 0);

const char* FONT_FILENAME = "fonts/font.ttf";
sf::Font GLOBAL_FONT;

int main() {
    GLOBAL_FONT.loadFromFile (FONT_FILENAME);

    RegionSet regset;
    regset.AddRegion (Rect (Vec (0, 200), Vec (600, 600)));

    RegionSet regset2;
    regset2.AddRegion (Rect (Vec (300, 300), Vec (500, 500)));

    RegionSet regset3;
    regset3.AddRegion (Rect (Vec (100, 100), Vec (350, 550)));


    sf::RenderWindow window (sf::VideoMode (800, 800), "set 1");

    while (window.isOpen()) {
      sf::Event event;

        while (window.pollEvent (event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Q) 
                    regset -= regset2;
                if (event.key.code == sf::Keyboard::W) 
                    regset -= regset3;
                if (event.key.code == sf::Keyboard::E) 
                    regset.MergeRegions();
            }
        }          
        
        window.clear();
        DrawRegset (window, regset, sf::Color::White);
        DrawRegset (window, regset2, sf::Color::Blue);
        DrawRegset (window, regset3, sf::Color::Red);
        window.display();
    }

    return 0;
}

void DrawRegset (sf::RenderWindow& window, const RegionSet& regset,
                 const sf::Color& col, int text_pos) {
    ListNode<Rect>* end_of_list = regset.regions.EndOfList();
    ListNode<Rect>* node = regset.regions.GetHead();

    sf::RectangleShape rect;
    rect.setFillColor (sf::Color::Transparent);
    rect.setOutlineThickness (3);
    rect.setOutlineColor (col);

    sf::Text txt ("", GLOBAL_FONT, 20);
    txt.setFillColor (col);
    char num_str[8] = "";

    int counter = 0;

    while (node != end_of_list) {
        counter++;
        Rect region = node -> val;
        rect.setSize (sf::Vector2f(region.GetWidth() - 6, region.GetHeight() - 6));
        rect.setPosition (region.vert1.x + 3, region.vert1.y + 3);

        snprintf (num_str, 7, "%d", counter);
        txt.setPosition (region.vert1.x + 8, region.vert1.y + 8);
        txt.setString (num_str);

        window.draw (rect);
        window.draw (txt);
        node = node -> next;
    }
}