#ifndef MOL_H
#define MOL_H

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <SFML/Graphics.hpp>

// class Walls;
// class Wall;

#include "../environment/environment.h"
#include "../graphic_structures/graphic_structures.h"

// #include "../list.h"
static const double MOL_RADIUS = 10;
static const double DT = 0.1;
static const int INIT_LIST_CAPACITY = 100;
// static const double LOAD_FACTOR = 0.7;
static const double INIT_SPEED = 1;
static const double MOL_SIDE_SIZE = 15;

enum Mol_types 
{
    EMPTY_MOL, 
    SQUARE_MOL, 
    ROUND_MOL
};

class Mol 
{
public://protected
    double speed_ = 0;
    double mass_ =  0;
    Vector v_ = Vector (0, 0, 0);
    Vector pos_ = Vector (0, 0, 0);
    Color color_ = Color (0, 0, 0, 0);
    enum Mol_types type = EMPTY_MOL;

public:
    Mol (double speed, double mass, Color color, Vector &v, Vector &pos) : 
                                                                speed_ (speed),
                                                                mass_ (mass), 
                                                                color_ (color),
                                                                v_ (!v),
                                                                pos_ (pos){;};

    Mol (double speed, double mass, Color color, int width, int height):
                                                                speed_ (speed),
                                                                mass_ (mass), 
                                                                color_ (color)
                                                                {
                                                                    double pos_x = std::rand () % width;
                                                                    double pos_y = std::rand () % height;
                                                                    pos_ = Vector (pos_x, pos_y, 0);

                                                                    double v_x = std::rand ();
                                                                    double v_y = std::rand ();
                                                                    v_ = !(Vector (v_x, v_y, 0));
                                                                };
    Mol () {;};
    ~Mol () {;};

    Vector move () {pos_ += v_ && (speed_ *DT); return pos_;};
    virtual void draw (sf::RenderTexture &texture) const;
};

class Round_mol : public Mol
{
    public:
    const double RADIUS = MOL_RADIUS;

public:
    Round_mol (double speed, double mass, Color color, Vector &v, Vector &pos) : Mol (speed, mass, color, v, pos) {type = ROUND_MOL;};
    Round_mol (double speed, double mass, Color color, int width, int height) : Mol (speed, mass, color, width, height) {type = ROUND_MOL;};
    Round_mol () {type = ROUND_MOL;};
    ~Round_mol () {};

    void draw (sf::RenderTexture &texture) const override;
};

class Square_mol : public Mol
{
public:
    const double SIDE_SIZE = MOL_SIDE_SIZE;
public:
    Square_mol (double speed, double mass, Color color, Vector &v, Vector &pos) : Mol (speed, mass, color, v, pos) {type = SQUARE_MOL;};
    Square_mol (double speed, double mass, Color color, int width, int height) : Mol (speed, mass, color, width, height) {type = SQUARE_MOL;};
    Square_mol () {type = SQUARE_MOL;};
    ~Square_mol () {};

    void draw (sf::RenderTexture &texture) const override;
};

class Mol_manager
{
    //TODO 
    //connect mols with list.h 
    //delete allocated mem and make alloc with new 
    Mol **mols = nullptr;
    Walls walls;

    int size_ = 0;
    int capacity_ = 0;
    
    sf::RenderTexture *texture_;
    int width_ = 0;
    int height = 0;

public:
    Mol_manager (sf::RenderTexture &texture) : texture_ (&texture), walls (Walls(texture)) {mols = (Mol **)calloc (100, sizeof (Mol *));capacity_ = 100;};
    ~Mol_manager ();

    void add (Mol *mol);
    void add_wall (Wall *wall);
    void draw ();
    bool move ();
    bool create (int size, enum Mol_types type);
    bool create (int size, enum Mol_types type, double speed, double mass, Color &color, Vector &v, Vector &pos);
    void update_height (int new_height) {walls.update_piston_height (new_height);};
    void remove (Mol *mol);

private:
    bool check_collisions ();
    bool collide (Mol *mol1, Mol *mol2);
    bool collide_squares (Mol *mol1, Mol *mol2);
    bool collide_rounds (Mol *mol1, Mol *mol2);
    bool collide_square_round (Mol *mol1, Mol *mol2);
    void wall_collide (Mol *mol, Wall *wall);
};


#endif /* MOL_H */