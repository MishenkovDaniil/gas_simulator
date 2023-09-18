#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>
#include <cassert>

#include "../graphic_structures/graphic_structures.h"

class Wall 
{
public:
    Point left_high_;
    Point right_low_;
    int thickness_ = 0;
    Vector normal_ = Vector (0, 0, 0);

public:
    Wall (Point &lh, Point &rl, sf::RenderTexture &texture, int thickness = 5) :
                                left_high_ (lh), 
                                right_low_ (rl),
                                thickness_ (thickness)
                                { 
                                    Vector wall = Vector ((const Point &)left_high_, (const Point &)right_low_);
                                    normal_ = !(Vector (-wall.y_, wall.x_, 0));
                                };
    ~Wall () {;};

    bool contains (const Point &obj);
    void draw (sf::RenderTexture &texture) const;//TODO
    virtual void update (int new_height) {};
};

class Piston : public Wall 
{
public:
    Piston (Point &lh, Point &rl, sf::RenderTexture &texture, int thickness = 20) : Wall (lh, rl, texture, thickness) {};
    ~Piston () {};

    void update (int new_height) override;
};

class Walls
{
public:
    Wall **walls  = nullptr;
    int size_     = 0;
    int capacity_ = 0;
    sf::RenderTexture *texture_;

public:
    Walls (sf::RenderTexture &texture) :texture_ (&texture) {walls = new Wall *[5];capacity_ = 5;};
    ~Walls () {delete[] walls;};

    void add (Wall *wall) {walls[size_++] = wall;};
    Wall *contains (Point &obj);
    void draw () {for (int i = 0; i < size_; ++i) walls[i]->draw (*texture_);};
    void update_piston_height (int new_height) {for (int i = 0; i < size_; ++i) walls[i]->update (new_height);}
};

#endif /* ENVIRONMENT_H */