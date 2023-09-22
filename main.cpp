#include <cassert>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "mol/mol.h"
#include "graphic_structures/graphic_structures.h"
#include "environment/environment.h"
#include "buttons/buttons.h"

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 800;
static const int DELTA_BUTTON_HEIGHT = 80;
static const int DELTA_BUTTON_WIDTH = 80;

static const Color PURPLE = Color (255, 0, 255, 255);
static const Color GREEN  = Color (0, 255, 0, 255);
static const Color RED    = Color (255, 0, 0, 255);
static const Color BLUE   = Color (0, 0, 255, 255);
static const Color TREE   = Color (105, 85, 50, 255);

static const int PISTON_THICKNESS = 40;
static const int PISTON_HEIGHT = 100;

static const char *PISTON_IMG = "buttons/imgs/piston_.jpg";
static const char *CIRCLE_IMG = "buttons/imgs/CIRCLE.jpg";
static const char *SQUARE_IMG = "buttons/imgs/SQUARE.jpg";
static const char *TEMP_IMG   = "buttons/imgs/TEMP.jpg";

int main ()
{
    sf::RenderWindow window (sf::VideoMode (WINDOW_WIDTH, WINDOW_HEIGHT), "gas_exp", sf::Style::Fullscreen); //??The requested video mode is not available, switching to a valid mode

    sf::RenderTexture texture;
    texture.create (WINDOW_WIDTH, WINDOW_HEIGHT);
    
    Point ll (0, WINDOW_HEIGHT); 
    Point lh (0, 0);
    Point rl (WINDOW_WIDTH, WINDOW_HEIGHT);
    Point rh (WINDOW_WIDTH, 0);

    Point piston_lh (0, PISTON_HEIGHT);
    Point piston_rl (WINDOW_WIDTH, PISTON_HEIGHT);
    
    Wall low (ll, rl, texture);
    Wall left (lh, ll, texture);
    Wall right (rh, rl, texture);
    // Piston piston (lh, rh, texture);
    Piston piston (piston_lh, piston_rl, texture, PISTON_THICKNESS);
    
    Mol_manager mol_manager (texture, WINDOW_WIDTH, WINDOW_HEIGHT, piston_lh);

    mol_manager.add_wall (&low);
    mol_manager.add_wall (&right);
    mol_manager.add_wall (&left);
    mol_manager.add_wall (&piston);

    mol_manager.create (10, ROUND_MOL);
    mol_manager.create (10, SQUARE_MOL);
    
    texture.display ();
    Button_manager button_manager (window, WINDOW_WIDTH, WINDOW_HEIGHT);

    int button_start_x = 0;


    sf::Texture piston_img_texture;
    sf::Texture circle_img_texture;
    sf::Texture square_img_texture;
    sf::Texture temp_img_texture;
    piston_img_texture.loadFromFile (PISTON_IMG);
    circle_img_texture.loadFromFile (CIRCLE_IMG);
    square_img_texture.loadFromFile (SQUARE_IMG);
      temp_img_texture.loadFromFile  (TEMP_IMG);

    Piston_button piston_button         (mol_manager, Point (button_start_x, WINDOW_HEIGHT), 
                                                      Point (80, WINDOW_HEIGHT + DELTA_BUTTON_HEIGHT),
                                                      piston_img_texture, piston_img_texture);
                                                      ///button_start_x = 1 * DELTA_BUTTON_WIDTH
    Square_mol_button square_mol_button (mol_manager, Point (80, WINDOW_HEIGHT), 
                                                      Point (160, WINDOW_HEIGHT + DELTA_BUTTON_HEIGHT),
                                                      square_img_texture, square_img_texture);
                                                      ///button_start_x = 2 * DELTA_BUTTON_WIDTH
    Round_mol_button round_mol_button   (mol_manager, Point (160, WINDOW_HEIGHT),
                                                      Point (240, WINDOW_HEIGHT + DELTA_BUTTON_HEIGHT),
                                                      circle_img_texture, circle_img_texture);
                                                      ///button_start_x = 3 * DELTA_BUTTON_WIDTH
    Temp_button      temp_button        (mol_manager, Point (240, WINDOW_HEIGHT),
                                                      Point (320, WINDOW_HEIGHT + DELTA_BUTTON_HEIGHT),
                                                      temp_img_texture, temp_img_texture);
    button_manager.add (&piston_button);
    button_manager.add (&square_mol_button);
    button_manager.add (&round_mol_button);
    button_manager.add (&temp_button);

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
                button_manager.run (Point (), event.key.code);
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