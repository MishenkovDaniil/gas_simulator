#ifndef MOL_H
#define MOL_H

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <SFML/Graphics.hpp>

#include "../environment/environment.h"
#include "../graphic_structures/graphic_structures.h"

class Mol;
class Mol_manager;
// typedef bool collide_mols (Mol_manager &manager, Mol *mol1, Mol *mol2);

// #include "../list.h"
static const double MOL_RADIUS = 10;
static const double DT = 0.1;
static const int INIT_LIST_CAPACITY = 100;
// static const double LOAD_FACTOR = 0.7;
static const double INIT_SPEED = 10;
static const double MOL_SIDE_SIZE = 15;
static const int INIT_TEMP = 273 + 25;
static const int MAX_TEMP = 273 + 300;
static const int MIN_TEMP = 0;

//TODO 
//button interface through my textures
//temperature changing (causes speed to change)
//fix wall collide  (and wall draw)
//make buttons aside from screen
//make graphics

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

    Mol (double speed, double mass, Color color, int width, int height, int start_width = 0, int start_height = 0):
                                                                speed_ (speed),
                                                                mass_ (mass), 
                                                                color_ (color)
                                                                {
                                                                    double pos_x = start_width + std::rand () % width;
                                                                    double pos_y = -start_height + std::rand () % height;
                                                                    pos_ = Vector (pos_x, pos_y, 0);

                                                                    double v_x = std::rand ();
                                                                    double v_y = std::rand ();
                                                                    v_ = !(Vector (v_x, v_y, 0));
                                                                };
    Mol () {;};
    ~Mol () {;};

    Vector move () {pos_ += v_ && (speed_ *DT); return pos_;};
    virtual void draw (sf::RenderTexture &texture) const;
    virtual Mol_types get_type () const {return EMPTY_MOL;};
    double get_speed () const {return speed_;};
    bool update_speed (double new_speed);
};

class Round_mol : public Mol
{
    public:
    const double RADIUS = MOL_RADIUS;

public:
    Round_mol (double speed, double mass, Color color, Vector &v, Vector &pos) 
            : Mol (speed, mass, color, v, pos) 
              {type = ROUND_MOL;};
    Round_mol (double speed, double mass, Color color, int width, int height, int width_shift = 0, int height_shift = 0) 
            : Mol (speed, mass, color, width, height, width_shift, height_shift) 
              {type = ROUND_MOL;};
    Round_mol () {type = ROUND_MOL;};
    ~Round_mol () {};

    void draw (sf::RenderTexture &texture) const override;
    Mol_types get_type () const override {return ROUND_MOL;};
};

class Square_mol : public Mol
{
public:
    const double SIDE_SIZE = MOL_SIDE_SIZE;
public:
    Square_mol (double speed, double mass, Color color, Vector &v, Vector &pos)
              : Mol (speed, mass, color, v, pos)
               {type = SQUARE_MOL;};
    Square_mol (double speed, double mass, Color color, int width, int height, int width_shift = 0, int height_shift = 0) 
              : Mol (speed, mass, color, width, height, width_shift, height_shift) 
               {type = SQUARE_MOL;};
    Square_mol () {type = SQUARE_MOL;};
    ~Square_mol () {};

    void draw (sf::RenderTexture &texture) const override;
    Mol_types get_type () const override {return SQUARE_MOL;};
};

class Mol_manager
{
    //TODO 
    //connect mols with list.h 
    sf::RenderTexture *texture_;
    Mol **mols = nullptr;
    Walls walls;

    int size_ = 0;
    int capacity_ = 0;
    
    int width_ = 0;
    int height_ = 0;
    int width_shift_ = 0;
    int height_shift_ = 0;

    double temperature_ = INIT_TEMP;
    
public:
    Mol_manager (sf::RenderTexture &texture, int width, int height, Point piston = Point ());
    ~Mol_manager ();

    void add (Mol *mol);
    void add_wall (Wall *wall);
    void draw ();
    bool move ();
    bool create (int size, enum Mol_types type);
    bool create (int size, enum Mol_types type, double speed, double mass, Color &color, Vector &v, Vector &pos);
    void update_height (int delta_height);
    void remove (Mol *mol);
    bool update_temperature (double delta_temp);
    double get_temperature () const {return temperature_;};

private:
    bool check_collisions ();
    bool collide (Mol *mol1, Mol *mol2);
    bool collide_rounds (Mol *mol1, Mol *mol2);
    bool collide_squares (Mol *mol1, Mol *mol2);
    bool collide_square_round (Mol *mol1, Mol *mol2);
    bool collide_round_square (Mol *mol1, Mol *mol2);

    void wall_collide (Mol *mol, Wall *wall);
};

// bool collide_rounds (Mol_manager &manager, Mol *mol1, Mol *mol2);
// bool collide_squares (Mol_manager &manager, Mol *mol1, Mol *mol2);
// bool collide_square_round (Mol_manager &manager, Mol *mol1, Mol *mol2);
// bool collide_round_square (Mol_manager &manager, Mol *mol1, Mol *mol2);

#endif /* MOL_H */