#include "graphic_structures.h"


Color Color::operator * (double mult) const 
{
    double r = r_ * mult;
           r = r > 0xff ? 0xff : r;
    double g = g_ * mult;
           g = g > 0xff ? 0xff : g;
    double b = b_ * mult;
           b = b > 0xff ? 0xff : b;
    double a = a_ * mult;
           a = a > 0xff ? 0xff : a;        
    
    return Color (r, g, b, a);
}

Color Color::operator + (const Color &color) const
{
    double r = r_ + color.r_;
           r = r > 0xff ? 0xff : r;
    double g = g_ + color.g_;
           g = g > 0xff ? 0xff : g;
    double b = b_ + color.b_;
           b = b > 0xff ? 0xff : b;
    double a = a_ + color.a_;
           a = a > 0xff ? 0xff : a; 
            
    return Color (r, g, b, a);
}

Color Color::operator += (const Color &color)      
{
    r_ += color.r_;
    r_ = r_ < color.r_ ? 0xff : r_;
    g_ += color.g_;
    g_ = g_ < color.g_ ? 0xff : g_;
    b_ += color.b_;
    b_ = b_ < color.b_ ? 0xff : b_;
    a_ += color.a_;
    a_ = a_ < color.a_ ? 0xff : a_;
                                                        
    return Color (r_, g_, b_, a_);
}

void Point::update (double x, double y, double z)
{
    x_ = x;
    y_ = y;
    z_ = z;
}

void Vector::rotate (double deg)
{  
    double radians = deg * 3.14 / 180.0;
    
    double deg_cos = cos (radians);
    double deg_sin = sin (radians);
    
    double x = x_;
    double y = y_;

    x_ = deg_cos * x - deg_sin * y; ///(cos_a + sin_a * i) * (x + y * i) = (cos_a * x - sin_a * y) + (y * cos_a + sin_a * x)
    y_ = deg_cos * y + deg_sin * x;
}

Vector Vector::operator * (const Vector &vec) const
{
    return Vector (y_ * vec.z_ - z_ * vec.y_, x_ * vec.z_ - z_ * vec.x_, x_ * vec.y_ - y_ * vec.x_);
}

Vector Vector::operator += (const Vector &vec)
{
    x_ += vec.x_;
    y_ += vec.y_;
    z_ += vec.z_;
    return *this;
}

Vector Vector::operator -= (const Vector &vec)
{
    x_ -= vec.x_;
    y_ -= vec.y_;
    z_ -= vec.z_;
    return *this;
}

Vector Vector::operator + (const Vector &vec) const
{
    return Vector (x_ + vec.x_, y_ + vec.y_, z_ + vec.z_);
}

Vector Vector::operator - (const Vector &vec) const
{
    return Vector (x_ - vec.x_, y_ - vec.y_, z_ - vec.z_);
}

Vector Vector::operator ! () const
{
    double module = sqrt (x_ * x_ + y_ * y_  + z_ * z_);

    return Vector (x_ / module, y_ / module, z_ / module);
}

Vector Vector::operator - () const
{
    return Vector (-x_, -y_, -z_);
}

Vector Vector::operator && (const double mult) const
{
    return Vector (x_ * mult, y_ * mult, z_ * mult);
}

double Vector::operator || (const Vector &vec) const 
{
    return x_ * vec.x_ + y_  * vec.y_ + z_ * vec.z_;
}

// may be made base class for text and image to add it as parameter in button and use int it whether image or text
//image of our size-->texture-->sprite-->sprite.set_texture () and sprite.set_texture_rect ()
void Button::draw (sf::RenderWindow &window, int screen_h, int screen_w) const
{
    int width = rl_corner_.x_ - lh_corner_.x_;
    int height = rl_corner_.y_ - lh_corner_.y_;

    sf::Texture button_texture;
    sf::RectangleShape button (sf::Vector2f (width, height));
                       button.setFillColor ((sf::Color)color_);
                    // button.setOutlineColor (sf::Color::White);
                       button.setOutlineThickness (1);
                       button.setTexture (&button_texture);
                       button.setPosition (lh_corner_.x_, lh_corner_.y_);
    
    sf::Text text;
    sf::Font font;
    font.loadFromFile ("button_font.ttf");
    text.setString (string_);
    text.setFont (font);
    text.setFillColor (sf::Color::White);
    text.setCharacterSize (10);

    double text_width = text.findCharacterPos(str_size - 1).x - text.findCharacterPos (0).x;
    text.setPosition (lh_corner_.x_ + (width - text_width) / 2, lh_corner_.y_ + height / 2 - 5);
    
    window.draw (button);
    window.draw (text);
}

bool Button::contains (double x, double y) const
{
    return (lh_corner_.x_ <= x && x <= rl_corner_.x_ && rl_corner_.x_ <= y && y <= rl_corner_.y_);
}

void Button::update (bool is_pressed)
{
    if (is_pressed)
    {
        color_ = pressed_button_color;
    }                                 
    else 
    {
        color_ = unpressed_button_color;
    }   
    is_pressed_ = is_pressed;
}


Button_manager::Button_manager ()
{
    buttons_p = new Button *[START_CAPACITY];
    assert (buttons_p);

    capacity = START_CAPACITY;
}

void Button_manager::draw ()
{
    assert (buttons_p);

    for (int idx = 0; idx < size; ++idx)
    {
        // *(buttons_p + idx)->draw ();
    }    
}

void Button_manager::add (Button *button)
{
    assert (buttons_p);

    if (((double)(size + 1) /(double)capacity) > LOAD_FACTOR)
    {
        // this->resize ();
        ;
    }

    *(buttons_p + size++) = button;
}

bool Button_manager::run ()
{
    assert (buttons_p);

    for (int idx = 0; idx < size; ++idx)
    {
        // *(buttons_p + idx)->run ();
    }
}

Button_manager::~Button_manager ()
{
    //should we delete every button or would it do button destroyer??
    delete[] buttons_p;
}