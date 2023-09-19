#include <cassert>
#include <iostream>
#include <SFML/Graphics.hpp>

// #include "mol/mol.h"
// #include "graphic_structures/graphic_structures.h"
// #include "environment/environment.h"
#include "buttons/buttons.h"

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 800;

int main ()
{
    sf::RenderWindow window (sf::VideoMode (WINDOW_WIDTH, WINDOW_HEIGHT), "gas_exp");

    sf::RenderTexture texture;
    texture.create (WINDOW_WIDTH, WINDOW_HEIGHT);
    
    Mol_manager mol_manager (texture);
    mol_manager.create (10, ROUND_MOL);
    mol_manager.create (10, SQUARE_MOL);
    
    Point ll (0, WINDOW_HEIGHT); 
    Point lh (0, 0);
    Point rl (WINDOW_WIDTH, WINDOW_HEIGHT);
    Point rh (WINDOW_WIDTH, 0);

    Wall low (ll, rl, texture);
    Wall left (lh, ll, texture);
    Wall right (rh, rl, texture);
    Piston piston (lh, rh, texture);

    mol_manager.add_wall (&low);
    mol_manager.add_wall (&right);
    mol_manager.add_wall (&left);
    mol_manager.add_wall (&piston);
    
    texture.display ();
    Button_manager button_manager (window, WINDOW_WIDTH, WINDOW_HEIGHT);

    Piston_button piston_button (piston, Point (0, 0), Point (100, 50), Color (255, 0, 255, 255), "piston", NONE, Color (0, 255, 0, 255));
    Square_mol_button square_mol_button (mol_manager, Point (0, 50), Point (100, 100), Color (255, 0, 255, 255), "add square", NONE);
    Round_mol_button round_mol_button (mol_manager, Point (0, 100), Point (100, 150), Color (255, 0, 255, 255), "add round", NONE);
    button_manager.add (&piston_button);
    button_manager.add (&square_mol_button);
    button_manager.add (&round_mol_button);

    sf::Sprite sprite;

    while (window.isOpen())
    {
        static double a = 0;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed && 
                     event.mouseButton.button == sf::Mouse::Left &&
                     (event.mouseButton.x || event.mouseButton.y))
            {
                Button *pressed_button = button_manager.contains(event.mouseButton.x, event.mouseButton.y);

                if (pressed_button)
                {   
                    pressed_button->update (!(pressed_button->get_status ()));
                    button_manager.run ();
                }
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                button_manager.run (Point (0, 0, 0), event.key.code);
            }
        }

        const sf::Texture draw_texture = texture.getTexture ();
        sprite.setTexture (draw_texture);
        
        window.clear();
        
        window.draw (sprite);
        button_manager.draw ();

        window.display();

        mol_manager.move ();
    }
    
    return 0;
}