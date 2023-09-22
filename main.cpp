#include <cassert>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>

#include "mol/mol.h"
#include "graphic_structures/graphic_structures.h"
#include "environment/environment.h"
#include "buttons/buttons.h"

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 800;
static const int DELTA_BUTTON_HEIGHT = 80;
static const int DELTA_BUTTON_WIDTH = 80;

static const int MAX_STR_LEN = 100;
static const int CALC_NUM = 1000;

static const int PISTON_THICKNESS = 40;
static const int PISTON_HEIGHT = 100;

static const char *PISTON_IMG = "buttons/imgs/piston_.jpg";
static const char *CIRCLE_IMG = "buttons/imgs/CIRCLE.jpg";
static const char *SQUARE_IMG = "buttons/imgs/SQUARE.jpg";
static const char *TEMP_IMG   = "buttons/imgs/TEMP.jpg";
static const char *PRESSED_PISTON_IMG = "buttons/imgs/pressed_piston.jpg";
static const char *PRESSED_CIRCLE_IMG = "buttons/imgs/pressed_circle.jpg";
static const char *PRESSED_SQUARE_IMG = "buttons/imgs/SQUARE.jpg";
static const char *PRESSED_TEMP_IMG   = "buttons/imgs/pressed_temp.jpg";
static const char *FPS_FILE   = "buttons/button_font.ttf";

// static const int FULLSCREEN_WIDTH  = 1920;
// static const int FULLSCREEN_HEIGHT = 1080;

void load_fps (sf::Text &fps_text, sf::Font &font, Point &pos, const char *fps_file, sf::Time elapsed_time, const size_t calc_num);

int main ()
{
    sf::RenderWindow window (sf::VideoMode (2000, 1100), "gas_exp", sf::Style::Fullscreen); //??The requested video mode is not available, switching to a valid mode
    
    // sf::Vector2u vec_window = window.getSize ();
    // std::cout << vec_window.x << "\n" << vec_window.y << "\n";
     
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
    sf::Texture pressed_piston_img_texture;
    sf::Texture circle_img_texture;
    sf::Texture pressed_circle_img_texture;
    sf::Texture square_img_texture;
    sf::Texture pressed_square_img_texture;
    sf::Texture temp_img_texture;
    sf::Texture pressed_temp_img_texture;

    piston_img_texture.loadFromFile (PISTON_IMG);
    circle_img_texture.loadFromFile (CIRCLE_IMG);
    square_img_texture.loadFromFile (SQUARE_IMG);
      temp_img_texture.loadFromFile  (TEMP_IMG);
    pressed_piston_img_texture.loadFromFile (PRESSED_PISTON_IMG);
    pressed_circle_img_texture.loadFromFile (PRESSED_CIRCLE_IMG);
    pressed_square_img_texture.loadFromFile (PRESSED_SQUARE_IMG);
      pressed_temp_img_texture.loadFromFile  (PRESSED_TEMP_IMG);

    Piston_button piston_button         (mol_manager, Point (button_start_x, WINDOW_HEIGHT), 
                                                      Point (80, WINDOW_HEIGHT + DELTA_BUTTON_HEIGHT),
                                                      pressed_piston_img_texture, piston_img_texture);
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
                                                      pressed_temp_img_texture, temp_img_texture);
    button_manager.add (&piston_button);
    button_manager.add (&square_mol_button);
    button_manager.add (&round_mol_button);
    button_manager.add (&temp_button);

    sf::Sprite sprite;

    clock_t start_t = clock (); 
    clock_t end_t; 
    sf::Text fps_text;
    sf::Font fps_font;
    sf::Sprite fps_sprite;
    Point fps_pos (1750, 0);

    sf::Clock clock;
    while (window.isOpen())
    {
        static int calc_num = 0;
        sf::Time frame_t;
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

        if (calc_num == CALC_NUM)
        {
            frame_t = clock.getElapsedTime ();
            load_fps (fps_text, fps_font, fps_pos, FPS_FILE, frame_t, CALC_NUM);
            clock.restart ();
            calc_num = 0;
        }
        else 
        {
            calc_num++;
        }

        window.draw (fps_text);

        window.display();

        mol_manager.move ();
    }
    
    return 0;
}

void load_fps (sf::Text &fps_text, sf::Font &font, Point &pos, const char *fps_file, sf::Time elapsed_time, const size_t calc_num)
{
    font.loadFromFile (fps_file);
    
    char string[MAX_STR_LEN] = "";

    sprintf (string, "FPS: %.0lf", (double) 1 / ((double)elapsed_time.asSeconds () / calc_num));
    
    fps_text.setString (string);
    fps_text.setFont (font);
    fps_text.setPosition (pos.x_, pos.y_);
}
