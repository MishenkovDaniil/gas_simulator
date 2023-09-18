#ifndef GRAPHIC_STRUCTURES_H
#define GRAPHIC_STRUCTURES_H

#include <cstring>
#include <cassert>
#include <iostream>
#include <cmath>
#include <cassert>
#include <SFML/Graphics.hpp>

static const int START_CAPACITY = 10;
static const double LOAD_FACTOR = 0.7;
static const double MULT_COEFF = 1.5;

enum Buttons 
{
    NONE, 
    LIGHT, 
    CAMERA
};

class Color
{
public:
    uint8_t r_ = 0;
    uint8_t g_ = 0;
    uint8_t b_ = 0;
    uint8_t a_ = 0;

public:
    Color () {;};
    Color (uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r_(r), g_ (g), b_ (b), a_(a) {};

    Color operator *  (double mult) const;
    Color operator +  (const Color &color) const;
    Color operator += (const Color &color);
    operator sf::Color()const {return sf::Color (r_, g_, b_, a_);};
};

class Point 
{
public:

    double x_ = 0;
    double y_ = 0;
    double z_ = 0;

public:
    Point (double x, double y, double z) : x_ (x), y_ (y), z_ (z) {};
    Point (double x, double y) : x_ (x), y_ (y), z_ (0) {};
    Point (const Point &point) : x_ (point.x_), y_ (point.y_), z_ (point.z_) {};

    void update (double x, double y, double z);
};


//virtual function draw: one for drawing picture inside button, one for text button,etc.
//button manager: array of pointers to buttons, checks if some button is pressed, cause action for pressed button,, draws all buttons at once 
class Button 
{
protected:
    Point lh_corner_ = Point (0, 0, 0); /// z coord is not used!!!
    Point rl_corner_ = Point (0, 0, 0); /// z coord is not used!!!
    
    Color color_                 = Color ();
    Color pressed_button_color   = Color (0, 0xff, 0, 0xff);
    Color unpressed_button_color = Color ();
    
    int  button_use_ = NONE; //
    bool is_pressed_ = false;

    char *string_ = nullptr;
    int str_size = 0;

public:
    Button () {;};
    Button (Point lh_corner, Point rl_corner, Color color, const char *string, const int button_use) :
                                                        lh_corner_             (lh_corner), 
                                                        rl_corner_             (rl_corner),
                                                        button_use_            (button_use),//
                                                        color_                 (color),
                                                        unpressed_button_color (color),
                                                        is_pressed_            (false)
                                                        
                                                        {str_size = sizeof(string);
                                                         string_ = new char[str_size + 1];
                                                         assert (string_);
                                                         std::strcpy (string_, string); 
                                                        };
    ~Button () {if (string_) delete[] string_;};

    void draw (sf::RenderWindow &window, int screen_h, int screen_w)const;
    void update (bool is_pressed);
    bool get_status () const {return is_pressed_;};
    bool contains (double x, double y) const;
    int get_button_use () const {return button_use_;};
    // virtual void run (Point &object, sf::Keyboard::Key key);
};

// class Move_button : public Button 
// {
//     Point object_ = Point (0, 0, 0);
// public:
//     Move_button (Point &object, Point lh_corner, Point rl_corner, Color color, const char *string, const int button_use) :
//                                                         object_ (object),
//                                                         Button (lh_corner, rl_corner, color, string, button_use)
//                                                         {;};
// };

class Button_manager 
{
    Button **buttons_p = nullptr;
    int size = 0;
    int capacity = 0;

public: 
    Button_manager ();
    ~Button_manager ();

    void add (Button *button);
    void draw ();
    bool run ();
private:
    bool resize ();
};


class Vector 
{
public://
    double x_ = 0;
    double y_ = 0;
    double z_ = 0;

public:
    Vector (double x, double y, double z) : x_ (x), y_ (y), z_ (z) {};
    Vector (const Point &start, const Point &end) : x_ (end.x_ - start.x_),
                                                    y_ (end.y_ - start.y_), 
                                                    z_ (end.z_ - start.z_) {};
    Point vec_to_point () {return Point (x_, y_, z_);};

    Vector operator *  (const Vector &vec)const; //векторное произведение
    Vector operator +  (const Vector &vec)const;
    Vector operator += (const Vector &vec);
    Vector operator -= (const Vector &vec);
    Vector operator -  (const Vector &vec)const;
    Vector operator -  ()const;
    Vector operator && (const double mult)const;  // покоординатное умножение
    Vector operator !  () const;                  // нормализация вектора
    double operator || (const Vector &vec)const ; // скалярное умножение
    operator Point() {return Point (x_, y_, z_);};
};

class Sphere
{
    Point start_    = Point (0, 0, 0);
    double radius_  = 0;
    Color color_    = Color ();

public:
    Sphere (const Point& point, double r, Color sphere_color = Color (0, 0, 0, 0)) : start_ (point), radius_ (r), color_ (sphere_color){};
    Point  get_start  () {return start_ ;};
    double get_radius () {return radius_;};
    Color  get_color  () {return color_ ;};
};

class Observer 
{
public:
    Point start     = Point (0, 0, 0);
    Point lh_corner = Point (0, 0, 0);
    Point rl_corner = Point (0, 0, 0);

public:
    Observer (const Point& strt, const Point& lh, const Point& rl) : start (strt), lh_corner (lh), rl_corner (rl) {};
};

#endif /* GRAPHIC_STRUCTURES_H */