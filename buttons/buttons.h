#ifndef BUTTONS_H
#define BUTTONS_H

class Piston;
class Mol_manager;

#include <SFML/Graphics.hpp>

#include "../graphic_structures/color/color.h"
#include "../graphic_structures/point/point.h"
#include "../graphic_structures/vector/vector.h"
#include "../environment/environment.h"
#include "../mol/mol.h"

static const int START_CAPACITY = 10;
//virtual function draw: one for drawing picture inside button, one for text button,etc.

class Button 
{
protected:
    Point lh_corner_ = Point (0, 0); /// z coord is not used
    Point rl_corner_ = Point (0, 0); /// z coord is not used
    
    bool is_pressed_ = false;

public:
    Button () {;};
    Button (Point lh_corner, Point rl_corner) :
            lh_corner_ (lh_corner), 
            rl_corner_ (rl_corner) {};
    virtual ~Button () {};

    bool get_status () const {return is_pressed_;};
    bool contains (double x, double y) const;

    virtual void draw (sf::RenderWindow &window, int screen_w, int screen_h)const = 0;
    virtual void update (bool is_pressed)                                         = 0;
    virtual bool run (Point &object, sf::Keyboard::Key key)                       = 0;
};

class Texture_button : public Button
{
protected:
    sf::Texture *cur_texture_ = nullptr;
    sf::Texture &pressed_texture_;
    sf::Texture &released_texture_;
    sf::Sprite *sprite = nullptr;

public:
    Texture_button (Point lh_corner, Point rl_corner, sf::Texture &pressed, sf::Texture &released) : 
                    Button (lh_corner, rl_corner),
                    pressed_texture_ (pressed),
                    released_texture_ (released) 
                    {cur_texture_ = &released_texture_;
                    sprite = new sf::Sprite;};
    ~Texture_button () override {delete sprite;};

    void draw (sf::RenderWindow &window, int screen_w, int screen_h)const override;
    void update (bool is_pressed) override final;
    bool run (Point &object, sf::Keyboard::Key key) override = 0;
};

class String_button : public Button
{
protected:
    Color *cur_color_ = nullptr;
    Color &pressed_color_;
    Color &released_color_;
    
    char *string_ = nullptr;
    int str_size = 0;
    
public:
    String_button (Point lh_corner, Point rl_corner, Color &pressed_color, Color &released_color, const char *string = nullptr) :
                   Button (lh_corner, rl_corner),
                   pressed_color_ (pressed_color),
                   released_color_ (released_color)
                   { cur_color_ = &released_color_;
                    if (!string) return; 
                    str_size = std::strlen(string);
                    string_ = new char[str_size + 1];
                    assert (string_);
                    std::strcpy (string_, string); 
                   };
    ~String_button () {if (string_) delete[] string_;};

    void draw (sf::RenderWindow &window, int screen_w, int screen_h)const override;
    void update (bool is_pressed) override final;
    bool run (Point &object, sf::Keyboard::Key key) override = 0;
};

class Piston_button : public Texture_button 
{
    Mol_manager *mol_manager_ = nullptr;
    const double delta_height = 1;

public:
    Piston_button (Mol_manager &mol_manager, Point lh_corner, Point rl_corner, sf::Texture &pressed, sf::Texture &released) :
                   mol_manager_ (&mol_manager),
                   Texture_button (lh_corner, rl_corner, pressed, released){};
    ~Piston_button () override {};

    bool run (Point &object, sf::Keyboard::Key key) override;
};

class Button_manager 
{
    Button **buttons_p = nullptr;
    int size_ = 0;
    int capacity_ = 0;

    sf::RenderWindow *window_ = nullptr;
    int screen_h_ = 0;
    int screen_w_ = 0;

public: 
    Button_manager (sf::RenderWindow &window, int screen_w, int screen_h);
    ~Button_manager ();

    void add (Button *button);
    void draw ();
    bool run (Point object = Point (0, 0, 0), sf::Keyboard::Key key = sf::Keyboard::Key::Unknown);
    Button *contains (double x, double y);
// private:
    // bool resize ();
};

class Square_mol_button : public Texture_button 
{
    Mol_manager *mol_manager_ = nullptr;
    const double delta_mols = 10;

public:
    Square_mol_button (Mol_manager &mol_manager, Point lh_corner, Point rl_corner, sf::Texture &pressed, sf::Texture &released) :
                       mol_manager_ (&mol_manager),
                       Texture_button (lh_corner, rl_corner, pressed, released){};
    ~Square_mol_button () override {};

    bool run (Point &object, sf::Keyboard::Key key) override;
};

class Round_mol_button : public Texture_button 
{
    Mol_manager *mol_manager_ = nullptr;
    const double delta_mols = 10;

public:
    Round_mol_button (Mol_manager &mol_manager, Point lh_corner, Point rl_corner, sf::Texture &pressed, sf::Texture &released) :
                      mol_manager_ (&mol_manager),
                      Texture_button (lh_corner, rl_corner, pressed, released){};
    ~Round_mol_button () override {};

    bool run (Point &object, sf::Keyboard::Key key) override;
};

class Temp_button : public Texture_button
{
    Mol_manager *mol_manager_ = nullptr;
    const double delta_temp = 10;

public:
    Temp_button (Mol_manager &mol_manager, Point lh_corner, Point rl_corner, sf::Texture &pressed, sf::Texture &released) :
                 mol_manager_ (&mol_manager),
                 Texture_button (lh_corner, rl_corner, pressed, released){};
    ~Temp_button () {};

    bool run (Point &object, sf::Keyboard::Key key) override;
};

#endif /* BUTTONS_H */