#include <iostream>
#include <SFML/Graphics.hpp>

#include "mol.h"

bool Mol::update_speed (double new_speed)
{
    if (new_speed < 0)
        return false;
    speed_ = new_speed;
    
    return true;
}

Square_mol::Square_mol (double speed, double mass, Color color, Vector &v, Vector &pos, double side_size)
                      : Mol (speed, mass, color, v, pos),
                        SIDE_SIZE (side_size)
                        {type = SQUARE_MOL;}

Mol_manager::Mol_manager (sf::RenderTexture &texture, int width, int height, Point piston_lh)
:   texture_ (&texture),
    walls (Walls(texture)),
    width_ (width),
    height_ (height)
{ 
    width_shift_ = piston_lh.x_;
    height_shift_ -= piston_lh.y_;
}

void Mol_manager::add (Mol *mol)
{
    mols.add (mol);
}

void Mol_manager::draw ()
{
    int mol_num = mols.get_size ();
    for (int mol_idx = 0; mol_idx < mol_num; mol_idx++)
    {
        // (*(mols + mol_idx))->draw (*texture_);
        mols[mol_idx]->draw (*texture_);
    }
    walls.draw ();
}

bool Mol_manager::move ()
{
    int mol_num = mols.get_size ();
    for (int mol_idx = 0; mol_idx < mol_num; mol_idx++)
    {
        // (*(mols + mol_idx))->move ();
        mols[mol_idx]->move ();
    }

    // texture_->clear (sf::Color::Black);
    draw ();
    check_collisions ();

    return true;
}

bool Mol_manager::check_collisions ()
{
    bool is_true = false;

    int mol_num = mols.get_size ();
    for (int idx = 0; idx < mol_num; idx++)
    {
        Mol *mol1 = mols[idx];
        
        Point pos = (Point)(mol1->pos_);
        Wall *wall_to_collide = walls.contains (pos);

        if (wall_to_collide)
        {
            wall_collide (mol1, wall_to_collide);
            is_true = true;
        }
    }

    for (int idx = 0; idx < mol_num; idx++)
    {
        Mol *mol1 = mols[idx];
        
        for (int second  = idx + 1; second < mol_num - 1; second++)
        {
            Mol *mol2 = mols[second];

            if (collide (mol1, mol2, idx, second))
            {
               idx = 0;
               mol_num = mols.get_size ();
               break;
            }
        }
    }
    return is_true;
}   


void Mol_manager::remove (Mol *mol, int idx)
{
    mols.rm (idx);
    delete mol;
}

bool Mol_manager::create (int size, enum Mol_types type)
{
    if (size < 1 || type == EMPTY_MOL)
        return false;

    
    switch (type)
    {
        case SQUARE_MOL:
        {   
            for (int i = 0; i < size; ++i)
            {
                Square_mol *mol = new Square_mol();
                *mol = (Square_mol (INIT_SPEED, 1, SQUARE_COLOR, width_, height_, width_shift_, height_shift_));
                add (mol);
            }
            break;
        }
        case ROUND_MOL:
        {   
            for (int i = 0; i < size; ++i)
            {
                Round_mol *mol = new Round_mol();
                *mol = (Round_mol (INIT_SPEED, 1, ROUND_COLOR, width_, height_,  width_shift_, height_shift_));
                add (mol);
            }
            break;
        }
        default:
            return false;
    }

    return true;
}

void Mol_manager::update_height (int delta_height)
{
    height_ += delta_height; 
    height_ = height_ < 0 ? 0 : height_;
    height_shift_ += delta_height; 
    walls.update_piston_height (delta_height);
}

bool Mol_manager::create (int size, enum Mol_types type, double speed, double mass, const Color &color, Vector &v, Vector &pos, double molecul_size)
{
    if (size < 1 || type == EMPTY_MOL)
        return false;
    switch (type)
    {
        case SQUARE_MOL:
        {   
            for (int i = 0; i < size; ++i)
            {
                Square_mol *mol = new Square_mol();
                if (molecul_size)
                {
                    *mol = (Square_mol (speed, mass, color, v, pos, molecul_size));
                }
                else 
                    *mol = (Square_mol (speed, mass, color, v, pos));
                add (mol);
            }
            break;
        }
        case ROUND_MOL:
        {   
            for (int i = 0; i < size; ++i)
            {
                Round_mol *mol = new Round_mol();
                if (molecul_size)
                    *mol = (Round_mol (speed, mass, color, v, pos, molecul_size));
                else 
                    *mol = (Round_mol (speed, mass, color, v, pos));

                add (mol);
            }
            break;
        }
        default:
            return false;

    }
    return true;
}

void Mol_manager::wall_collide (Mol *mol, Wall *wall)
{
    mol->v_ = !(mol->v_ - (wall->normal_ && (2 * ((!wall->normal_) || (!mol->v_)))));
}

void Mol_manager::add_wall (Wall *wall)
{
    //size check

    walls.add (wall);
} 

bool Mol_manager::collide (Mol *mol1, Mol *mol2, int idx_1, int idx_2)
{
    assert (mol1 && mol2);

    static const collide_mols *v_collide_tab[2][2] = 
        {
            {collide_squares, collide_square_round},
            {collide_round_square, collide_rounds}
        };
    
    Mol_types mol1_type = mol1->get_type ();
    Mol_types mol2_type = mol2->get_type ();

    if ( mol1_type == EMPTY_MOL || mol2_type == EMPTY_MOL)
    {
        return false;
    }
    bool res = v_collide_tab[mol1_type - 1][mol2_type - 1] (*this, mol1, mol2);

    if (res == false)
        return false;

    remove (mol1, idx_1);
    if (idx_2 > idx_1) 
        remove (mol2, idx_2 - 1);
    else 
        remove (mol2, idx_2);

    return true;
}

bool collide_squares (Mol_manager &manager, Mol *mol1, Mol *mol2)
{
    const double SIDE_1 = ((Square_mol *)mol1)->SIDE_SIZE;
    const double SIDE_2 = ((Square_mol *)mol2)->SIDE_SIZE;
    Point center_1 (mol1->pos_.x_ + SIDE_1, mol1->pos_.y_ + SIDE_1);
    Point center_2 (mol2->pos_.x_ + SIDE_2, mol2->pos_.y_ + SIDE_2);

    if ((fabs (center_1.x_ - center_2.x_) < SIDE_1 + SIDE_2) && (fabs (center_1.y_ - center_2.y_) < SIDE_1 + SIDE_2))
    {
        Vector v = (!((mol1->v_ && mol1->mass_) + (mol2->v_ && mol2->mass_))) && 30; //???TODO
        Vector collide_pos = (mol1->pos_ + mol2->pos_) && 0.5;
        Vector new_mol_pos = collide_pos + v;

        int no_of_round_mols = mol1->mass_ + mol2->mass_;

        for (int i = 0; i < no_of_round_mols; ++i)
        {
            manager.create (1, ROUND_MOL, (mol1->speed_ + mol2->speed_) / 2, 1.0, ROUND_COLOR, v, new_mol_pos);
            v.rotate (360.0 / no_of_round_mols);
            new_mol_pos = collide_pos + v; 
        }
        return true;
    }

    return false;
}

bool collide_rounds (Mol_manager &manager, Mol *mol1, Mol *mol2)
{
    const double R = ((Round_mol *)mol1)->RADIUS;
    Point center_1 (mol1->pos_.x_ + R, mol1->pos_.y_ + R);
    Point center_2 (mol2->pos_.x_ + R, mol2->pos_.y_ + R);

    if (center_1.distance (center_2) < 2 * R)
    {
        Vector v = (mol1->v_ && mol1->mass_) + (mol2->v_ && mol2->mass_);
        Vector pos = (mol1->pos_ + mol2->pos_) && 0.5;

        manager.create (1, SQUARE_MOL, (mol1->speed_ + mol2->speed_) / (int)(mol1->mass_ + mol2->mass_), mol1->mass_ + mol2->mass_, SQUARE_COLOR, v, pos);
        // m = 1 + 1 
        return true;
    }
    
    return false;
}

bool collide_round_square (Mol_manager &manager, Mol *mol1, Mol *mol2)
{
    return collide_square_round (manager, mol2, mol1);
}

bool collide_square_round (Mol_manager &manager, Mol *mol1, Mol *mol2)
{
    const double SIDE = ((Square_mol *)mol1)->SIDE_SIZE;    
    const double R = ((Round_mol *)mol2)->RADIUS;           //think about making static
    Point center_1 (mol1->pos_.x_ + SIDE, mol1->pos_.y_ + SIDE);
    Point center_2 (mol2->pos_.x_ + R, mol2->pos_.y_ + R);
        
    Vector dir(center_1, center_2);
    Point side_point (center_1.x_ - SIDE, center_1.y_);

    Vector side_from_center (center_1, side_point);
    double cos_a = (!dir) || (!side_from_center);
    cos_a = cos_a < 0 ? -cos_a : cos_a;
    cos_a = cos_a < 0.5 ? 1.0 - cos_a : cos_a;

    if (center_1.distance (center_2) < R + SIDE / cos_a)
    {
        Vector v = (mol1->v_ && mol1->mass_) + (mol2->v_ && mol2->mass_);
        Vector pos = (mol1->pos_ + mol2->pos_) && 0.5;
        
        double new_mol_mass = mol1->mass_ + mol2->mass_;
        double speed = (mol1->speed_ * mol1->mass_ + mol2->speed_ * mol2->mass_) / new_mol_mass;    

        manager.create (1, SQUARE_MOL, speed, new_mol_mass, SQUARE_COLOR, v, pos, SIDE * sqrt((new_mol_mass / (new_mol_mass - mol2->mass_))));// (new_mol_mass / (new_mol_mass - mol2->mass_))); // m= m + 1

        return true;
    }

    return false;
}

void Round_mol::draw (sf::RenderTexture &texture) const 
{
    sf::CircleShape circle_mol (RADIUS);
    circle_mol.setFillColor ((sf::Color)color_);
    circle_mol.setOutlineThickness (1);
    circle_mol.setPosition (pos_.x_, pos_.y_);
    
    texture.draw (circle_mol);
}

void Square_mol::draw (sf::RenderTexture &texture) const 
{
    sf::RectangleShape square_mol (sf::Vector2f (SIDE_SIZE, SIDE_SIZE));

    square_mol.setFillColor ((sf::Color)color_);
    square_mol.setOutlineThickness (1);
    square_mol.setPosition (pos_.x_, pos_.y_);
    
    texture.draw (square_mol);
}

bool Mol_manager::update_temperature (double delta_temp)
{
    //TODO
    //if temp  == 0 --> mols speed == 0
    double new_temp = temperature_ + delta_temp;

    if (new_temp < MIN_TEMP || new_temp > MAX_TEMP)
        return false;
    
    double mult_coeff = sqrt (new_temp / temperature_);
    
    int mol_num = mols.get_size ();

    for (int idx = 0; idx < mol_num; ++idx)
    {
        Mol *mol = mols[idx];
        mol->update_speed (mol->get_speed () * mult_coeff);
    }

    // update_height (0.3 * (height_ * mult_coeff - height_)); ????

    temperature_ = new_temp;
    return true;
}

Mol_manager::~Mol_manager ()
{
    int mol_num = mols.get_size ();
    for (int i = 0; i < mol_num; ++i) 
    {
        delete (mols[i]);
        // mols[i] = nullptr; 
    }

    // free (mols);
    // mols = nullptr;
}

 int Mol_manager::get_mol_num (Mol_types type)
 {
    int num = 0;
    int mol_num = mols.get_size ();
    for (int idx = 0; idx < mol_num; ++idx)
    {
        num += (mols[idx]->type == type);
    }
    return num;
 }