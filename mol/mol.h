#ifndef MOL_H
#define MOL_H

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <SFML/Graphics.hpp>

#include "../graphic_structures/graphic_structures.h"
#include "../environment/environment.h"

// #include "../list.h"
static const double MOL_RADIUS = 0;
static const double DT = 0.1;
static const int INIT_LIST_CAPACITY = 100;
// static const double LOAD_FACTOR = 0.7;

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

public:
    Mol (double speed, double mass, Color color, Vector &v, Vector &pos) : 
                                                                speed_ (speed),
                                                                mass_ (mass), 
                                                                color_ (color),
                                                                v_ (v),
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
    virtual void draw (sf::RenderTexture &texture) const; // ???
};

class Round_mol : public Mol
{
    const double RADIUS = MOL_RADIUS;
public:
    Round_mol (double speed, double mass, Color color, Vector &v, Vector &pos) : Mol (speed, mass, color, v, pos) {};
    Round_mol (double speed, double mass, Color color, int width, int height) : Mol (speed, mass, color, width, height) {};
    Round_mol () {;};
    ~Round_mol () {};

    void draw (sf::RenderTexture &texture) const override;
};

class Square_mol : public Mol
{
public:
    Square_mol (double speed, double mass, Color color, Vector &v, Vector &pos) : Mol (speed, mass, color, v, pos) {};
    Square_mol (double speed, double mass, Color color, int width, int height) : Mol (speed, mass, color, width, height) {};
    Square_mol () {;};
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

    int size = 0;
    int capacity = 0;
    
    bool is_changed = false;
    
    sf::RenderTexture *texture_;
    int width_ = 0;
    int height = 0;

public:
    Mol_manager (sf::RenderTexture &texture) : texture_ (&texture), walls (Walls(texture)) {mols = (Mol **)calloc (100, sizeof (Mol *));capacity = 100;};
    ~Mol_manager ();

    void add (Mol *mol);
    void add_wall (Wall *wall);
    void draw ();
    bool move ();
    bool create (int size, enum Mol_types type);
    void update_height (int new_height) {walls.update_piston_height (new_height);};

private:
    bool check_collisions ();
    Mol *collide (Mol *mol1, Mol *mol2);
    void wall_collide (Mol *mol, Wall *wall);
    Mol *remove (Mol *mol);
};


#endif /* MOL_H */