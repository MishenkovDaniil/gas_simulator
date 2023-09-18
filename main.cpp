#include <cassert>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "mol/mol.h"
#include "graphic_structures/graphic_structures.h"
#include "environment/environment.h"

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 800;

int main ()
{
    sf::RenderWindow window (sf::VideoMode (WINDOW_WIDTH, WINDOW_HEIGHT), "gas_exp");

    sf::RenderTexture texture;
    texture.create (WINDOW_WIDTH, WINDOW_HEIGHT);
    
    Mol_manager mol_manager (texture);
    mol_manager.create (10, SQUARE_MOL);
    mol_manager.create (10, ROUND_MOL);
    
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
        }
        const sf::Texture draw_texture = texture.getTexture ();
        sprite.setTexture (draw_texture);
        
        window.clear();
        window.draw (sprite);

        // if (a++ == 1000)
        mol_manager.update_height (a++ / 1000);
        window.display();

        mol_manager.move ();
    }
    
    return 0;
}