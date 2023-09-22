#include <iostream>
#include <cassert>
#include <SFML/Graphics.hpp>

#include "buttons.h"


// may be made base class for text and image to add it as parameter in button and use int it whether image or text
//image of our size-->texture-->sprite-->sprite.set_texture () and sprite.set_texture_rect ()


bool Button::contains (double x, double y) const
{
    return (lh_corner_.x_ <= x && x <= rl_corner_.x_ && lh_corner_.y_ <= y && y <= rl_corner_.y_);
}


Button_manager::Button_manager (sf::RenderWindow &window, int screen_w, int screen_h) : 
window_ (&window),
screen_w_ (screen_w),
screen_h_ (screen_h)
{
    buttons_p = new Button *[START_CAPACITY];
    assert (buttons_p);

    capacity_ = START_CAPACITY;
}

void Button_manager::draw ()
{
    assert (buttons_p);

    for (int idx = 0; idx < size_; ++idx)
    {
        (*(buttons_p + idx))->draw (*window_, screen_w_, screen_h_);
    }    
}

void Button_manager::add (Button *button)
{
    assert (buttons_p);

    if (((double)(size_ + 1) /(double)capacity_) > LOAD_FACTOR)
    {
        // this->resize ();
        ;
    }

    *(buttons_p + size_++) = button;
}

Button *Button_manager::contains (double x, double y)
{
    for (int idx = 0; idx < size_; ++idx)
    {
        if ((*(buttons_p + idx))->contains (x, y))
        {
            return *(buttons_p + idx);
        }
    }

    return nullptr;
}

bool Button_manager::run (Point object, sf::Keyboard::Key key)
{
    assert (buttons_p);

    for (int idx = 0; idx < size_; ++idx)
    {
        (*(buttons_p + idx))->run (object, key);
    }
}

Button_manager::~Button_manager ()
{
    //should we delete every button or would it do button destroyer??
    delete[] buttons_p;
}


void Texture_button::draw (sf::RenderWindow &window, int screen_w, int screen_h)const
{
    sprite->setTexture (*cur_texture_);
    sprite->setPosition (lh_corner_.x_, lh_corner_.y_);

    window.draw (*sprite);
}

void Texture_button::update (bool is_pressed)
{
    if (is_pressed == true)
        cur_texture_ = &pressed_texture_;
    else 
        cur_texture_ = &released_texture_;

    is_pressed_ = is_pressed;
}


void String_button::draw (sf::RenderWindow &window, int screen_w, int screen_h)const
{
    int width = rl_corner_.x_ - lh_corner_.x_;
    int height = rl_corner_.y_ - lh_corner_.y_;

    sf::Texture button_texture;
    sf::RectangleShape button (sf::Vector2f (width, height));
    is_pressed_ ? button.setFillColor((sf::Color)pressed_color_) : button.setFillColor ((sf::Color)*cur_color_);//may be setfillcolor (cur_color_) ???
                    // button.setOutlineColor (sf::Color::White);
                       button.setOutlineThickness (1);
                       button.setTexture (&button_texture);
                       button.setPosition (lh_corner_.x_, lh_corner_.y_);
    
    sf::Text text;
    sf::Font font;
    font.loadFromFile ("buttons/button_font.ttf");
    text.setString (string_);
    text.setFont (font);
    text.setFillColor (sf::Color::White);
    text.setCharacterSize (10);

    double text_width = text.findCharacterPos(str_size - 1).x - text.findCharacterPos (0).x;
    text.setPosition (lh_corner_.x_ + (width - text_width) / 2, lh_corner_.y_ + height / 2 - 5);
    
    window.draw (button);
    window.draw (text);
}

void String_button::update (bool is_pressed)
{
    if (is_pressed == true)
        cur_color_ = &pressed_color_;
    else 
        cur_color_ = &released_color_;

    is_pressed_ = is_pressed;
}


bool Piston_button::run (Point &object, sf::Keyboard::Key key)
{
    if (is_pressed_ == false)
        return false;

    switch (key)
    {
        case sf::Keyboard::Key::Up:
        {
            mol_manager_->update_height (delta_height);
            break;
        }
        case sf::Keyboard::Key::Down:
        {
            mol_manager_->update_height (-delta_height);
            break;
        }
        default:
        {
            return false;
        }
    }

    return true;
}


bool Square_mol_button::run (Point &object, sf::Keyboard::Key key)
{
    if (is_pressed_ == false)
        return false;

    mol_manager_->create (delta_mols, SQUARE_MOL);
    is_pressed_ = false;

    return true;
}

bool Round_mol_button::run (Point &object, sf::Keyboard::Key key)
{
    if (is_pressed_ == false)
        return false;

    mol_manager_->create (delta_mols, ROUND_MOL);
    is_pressed_ = false;

    return true;
}

bool Temp_button::run (Point &object, sf::Keyboard::Key key)
{
    if (is_pressed_ == false)
        return false;

    switch (key)
    {
        case sf::Keyboard::Key::Up:
        {
            mol_manager_->update_temperature (delta_temp);
            break;
        }
        case sf::Keyboard::Key::Down:
        {
            mol_manager_->update_temperature (-delta_temp);
            break;
        }
        default:
        {
            return false;
        }
    }

    return true;
}