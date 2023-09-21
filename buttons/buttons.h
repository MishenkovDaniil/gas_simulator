#ifndef BUTTONS_H
#define BUTTONS_H

 class Piston;
 class Mol_manager;

#include <SFML/Graphics.hpp>
#include "../environment/environment.h"
#include "../graphic_structures/graphic_structures.h"
#include "../mol/mol.h"

//virtual function draw: one for drawing picture inside button, one for text button,etc.
//button manager: array of pointers to buttons, checks if some button is pressed, cause action for pressed button,, draws all buttons at once 

// class Button 
// {
// protected:
//     Point lh_corner_ = Point (0, 0); /// z coord is not used
//     Point rl_corner_ = Point (0, 0); /// z coord is not used
    
//     // Color color_                = Color ();
//     // Color pressed_button_color  = Color ();
//     // Color released_button_color = Color ();
    
//     // int  button_use_ = NONE; //
//     bool is_pressed_ = false;

//     // char *string_ = nullptr;
//     // int str_size = 0;

//     // sf::Texture *texture_ = nullptr;

// public:
//     Button () {;};
//     Button (Point lh_corner, Point rl_corner, Color color, const char *string, const int button_use,
//             Color pressed_button_clr = Color (0, 0, 0,0), sf::Texture *texture = nullptr) :
//             lh_corner_             (lh_corner), 
//             rl_corner_             (rl_corner),
//             button_use_            (button_use),//
//             color_                 (color),
//             released_button_color  (color),
//             pressed_button_color   (pressed_button_clr.a_ ? pressed_button_clr : color),
//             is_pressed_            (false),
//             texture_               (texture)
//             {str_size = std::strlen(string);
//                 string_ = new char[str_size + 1];
//                 assert (string_);
//                 std::strcpy (string_, string); 
//             };
//     ~Button () {if (string_) delete[] string_;};

//     virtual void draw (sf::RenderWindow &window, int screen_w, int screen_h)const;
//     void update (bool is_pressed);
//     bool get_status () const {return is_pressed_;};
//     bool contains (double x, double y) const;
//     int get_button_use () const {return button_use_;};
//     virtual bool run (Point &object, sf::Keyboard::Key key) = 0;
// };


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

public:
    Texture_button (Point lh_corner, Point rl_corner, sf::Texture &pressed, sf::Texture &released) : 
                    Button (lh_corner, rl_corner),
                    pressed_texture_ (pressed),
                    released_texture_ (released) 
                    {cur_texture_ = &released_texture_;};
    ~Texture_button () override {};

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

// class Piston_button : public Button 
// {
//     Mol_manager *mol_manager_ = nullptr;
//     const double delta_height = 1;

// public:
//     Piston_button (Mol_manager &mol_manager, Point lh_corner, Point rl_corner, Color color, const char *string, const int button_use, 
//                    Color pressed_button_clr = Color (0, 0, 0,0), sf::Texture *texture = nullptr) :
//                     mol_manager_ (&mol_manager),
//                     Button (lh_corner, rl_corner, color, string, button_use, pressed_button_clr, texture){};
//     ~Piston_button () {};

//     bool run (Point &object, sf::Keyboard::Key key) override;
// };

// class Square_mol_button : public Button 
// {
//     Mol_manager *mol_manager_ = nullptr;
//     const double delta_mols = 10;

// public:
//     Square_mol_button (Mol_manager &mol_manager, Point lh_corner, Point rl_corner, Color color, const char *string, const int button_use, Color pressed_button_clr = Color (0, 0, 0,0)) :
//                                                         mol_manager_ (&mol_manager),
//                                                         Button (lh_corner, rl_corner, color, string, button_use, pressed_button_clr){};
//     ~Square_mol_button () {};

//     bool run (Point &object, sf::Keyboard::Key key) override;
// };

// class Round_mol_button : public Button 
// {
//     Mol_manager *mol_manager_ = nullptr;
//     const double delta_mols = 10;

// public:
//     Round_mol_button (Mol_manager &mol_manager, Point lh_corner, Point rl_corner, Color color, const char *string, const int button_use, Color pressed_button_clr = Color (0, 0, 0,0)) :
//                                                         mol_manager_ (&mol_manager),
//                                                         Button (lh_corner, rl_corner, color, string, button_use, pressed_button_clr){};
//     ~Round_mol_button () {};

//     bool run (Point &object, sf::Keyboard::Key key) override;
// };

// class Temp_button : public Button
// {
//     Mol_manager *mol_manager_ = nullptr;
//     const double delta_temp = 10;

// public:
//     Temp_button (Mol_manager &mol_manager, Point lh_corner, Point rl_corner, Color color, const char *string, const int button_use, Color pressed_button_clr = Color (0, 0, 0,0)) :
//                                                         mol_manager_ (&mol_manager),
//                                                         Button (lh_corner, rl_corner, color, string, button_use, pressed_button_clr){};
//     ~Temp_button () {};

//     bool run (Point &object, sf::Keyboard::Key key) override;
// };

#endif /* BUTTONS_H */