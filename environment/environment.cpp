#include <iostream>
#include <cassert>

#include "environment.h"

bool Wall::contains (const Point &obj)
{
    if ((left_high_.x_ - thickness_ <  obj.x_) && 
        (obj.x_ < right_low_.x_ + thickness_)  &&
        (left_high_.y_ - thickness_ < obj.y_)  && 
        (obj.y_ < right_low_.y_ + thickness_))
        return true;
    
    return false;
}

void Piston::update (int delta_height)
{
    left_high_.y_ -= delta_height;
    right_low_.y_ -= delta_height;
}

void Wall::draw (sf::RenderTexture &texture) const
{
    //TODO
    // make all walls with thickness (fix pos)
    double width = right_low_.x_ - left_high_.x_;
    double height = right_low_.y_ - left_high_.y_;

    width = width > 0 ? width : thickness_;
    height = height > 0 ? height : thickness_;  

    sf::RectangleShape wall (sf::Vector2f (width, height));

    wall.setFillColor (sf::Color::White);
    wall.setOutlineThickness (1);
    wall.setPosition (left_high_.x_, left_high_.y_);
    
    texture.draw (wall);
}

Wall *Walls::contains (Point &obj)
{
    for (int idx = 0; idx < size_; ++idx)
    {
        if ((walls[idx])->contains (obj))
        {
            return walls[idx];
        }
    }

    return nullptr;
}