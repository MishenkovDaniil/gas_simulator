#include <iostream>
#include <SFML/Graphics.hpp>

#include "mol.h"

void Mol_manager::add (Mol *mol)
{
    if ((double)size_ / (double) capacity_ > LOAD_FACTOR)
    {
        // this->resize ();
        ;
    }

    mols[size_++] = mol;
}

void Mol_manager::draw ()
{
    for (int mol_idx = 0; mol_idx < size_; mol_idx++)
    {
        (*(mols + mol_idx))->draw (*texture_);
    }
    walls.draw ();
}

bool Mol_manager::move ()
{
    for (int mol_idx = 0; mol_idx < size_; mol_idx++)
    {
        (*(mols + mol_idx))->move ();
    }

    texture_->clear (sf::Color::Black);
    draw ();
    check_collisions ();

    return true;
}

bool Mol_manager::check_collisions ()
{
    bool is_true = false;

    for (int idx = 0; idx < size_; idx++)
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

    for (int idx = 0; idx < size_; idx++)
    {
        Mol *mol1 = mols[idx];
        
        for (int second  = idx + 1; second < size_ - 1; second++)
        {
            Mol *mol2 = mols[second];

            if (collide (mol1, mol2))
            {
               idx = 0;
               break;
            }
        }
    }
    return is_true;
}

void Mol_manager::remove (Mol *mol)
{
    for (int idx = 0; idx < size_; idx++)
    {
        if (mols[idx] == mol)
        {
            // mol->~Mol ();
            for (int rm_idx = idx + 1; rm_idx < size_; rm_idx++)
            {
                mols[rm_idx - 1] = mols[rm_idx];
            }
                

            mols[--size_] = nullptr;

            delete mol;
            break;
        }
    }
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
                Mol *mol = new Square_mol();
                *mol = (Square_mol (INIT_SPEED, 1, Color (0, 255, 0, 255), 800, 800));
                add (mol);
            }
            break;
        }
        case ROUND_MOL:
        {   
            for (int i = 0; i < size; ++i)
            {
                Mol *mol = new Round_mol();
                *mol = (Round_mol (INIT_SPEED, 1, Color (255, 0, 0, 255), 800, 800));
                add (mol);
            }
            break;
        }
        default:
            return false;
    }

    return true;
}

bool Mol_manager::create (int size, enum Mol_types type, double speed, double mass, Color &color, Vector &v, Vector &pos)
{
    if (size < 1 || type == EMPTY_MOL)
        return false;
    switch (type)
    {
        case SQUARE_MOL:
        {   
            for (int i = 0; i < size; ++i)
            {
                Mol *mol = new Square_mol();
                *mol = (Square_mol (speed, mass, color, v, pos));
                add (mol);
            }
            break;
        }
        case ROUND_MOL:
        {   
            for (int i = 0; i < size; ++i)
            {
                Mol *mol = new Round_mol();
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

bool Mol_manager::collide (Mol *mol1, Mol *mol2)
{
    if (mol1->type == SQUARE_MOL && mol2->type == SQUARE_MOL)
    {      
        if (collide_squares (mol1, mol2) == false)
            return false;
    }
    else if (mol1->type == ROUND_MOL && mol2->type == ROUND_MOL)
    {
        if (collide_rounds (mol1, mol2) == false)
            return false;
    }
    else 
    {
        //CHECK IF ALWAYS WORKS PROPERLY
        bool res = false;
        if (mol1->type == ROUND_MOL)
            res = collide_square_round(mol1, mol2);
        else
            res = collide_square_round (mol2, mol1);

        if (res == false)
            return false;
    }
    
    remove (mol1);
    remove (mol2);

    return true;
}

bool Mol_manager::collide_squares (Mol *mol1, Mol *mol2)
{
    Color color (255, 0, 0, 255);

    const double SIDE = ((Square_mol *)mol1)->SIDE_SIZE;
    Point center_1 (mol1->pos_.x_ + SIDE, mol1->pos_.y_ + SIDE);
    Point center_2 (mol2->pos_.x_ + SIDE, mol2->pos_.y_ + SIDE);

    if ((fabs (center_1.x_ - center_2.x_) < 2 * SIDE) && (fabs (center_1.y_ - center_2.y_) < 2 * SIDE))
    {
        Vector v = (!((mol1->v_ && mol1->mass_) + (mol2->v_ && mol2->mass_))) && 30; //???TODO
        Vector collide_pos = (mol1->pos_ + mol2->pos_) && 0.5;
        Vector new_mol_pos = collide_pos + v;

        int no_of_round_mols = mol1->mass_ + mol2->mass_;

        for (int i = 0; i < no_of_round_mols; ++i)
        {
            create (1, ROUND_MOL, (mol1->speed_ + mol2->speed_) / 2, 1.0, color, v, new_mol_pos);
            v.rotate (360.0 / no_of_round_mols);
            new_mol_pos = collide_pos + v; 
        }
        return true;
    }

    return false;
}

bool Mol_manager::collide_rounds (Mol *mol1, Mol *mol2)
{
    Color color (0, 255, 0, 255);

    const double R = ((Round_mol *)mol1)->RADIUS;
    Point center_1 (mol1->pos_.x_ + R, mol1->pos_.y_ + R);
    Point center_2 (mol2->pos_.x_ + R, mol2->pos_.y_ + R);

    if (center_1.distance (center_2) < 2 * R)
    {
        Vector v = (mol1->v_ && mol1->mass_) + (mol2->v_ && mol2->mass_);
        Vector pos = (mol1->pos_ + mol2->pos_) && 0.5;

        create (1, SQUARE_MOL, (mol1->speed_ + mol2->speed_) / (int)(mol1->mass_ + mol2->mass_), mol1->mass_ + mol2->mass_, color, v, pos);
        // m = 1 + 1 
        return true;
    }
    
    return false;
}

bool Mol_manager::collide_square_round (Mol *mol1, Mol *mol2)
{
    Color color (0, 255, 0, 255);
    
    const double R = ((Round_mol *)mol1)->RADIUS;           //think about making static
    const double SIDE = ((Square_mol *)mol2)->SIDE_SIZE;    //the same
    Point center_1 (mol1->pos_.x_ + R, mol1->pos_.y_ + R);
    Point center_2 (mol2->pos_.x_ + SIDE, mol2->pos_.y_ + SIDE);
        
    Vector dir(center_1, center_2);
    Point side_point (center_2.x_ - SIDE, center_2.y_);

    Vector side_from_center (center_2, side_point);
    double cos_a = (!dir) || (!side_from_center);
    cos_a = cos_a < 0 ? -cos_a : cos_a;
    cos_a = cos_a < 0.5 ? 1.0 - cos_a : cos_a;

    if (center_1.distance (center_2) < R + SIDE / cos_a)
    {
        Vector v = (mol1->v_ && mol1->mass_) + (mol2->v_ && mol2->mass_);
        Vector pos = (mol1->pos_ + mol2->pos_) && 0.5;
        
        double new_mol_mass = mol1->mass_ + mol2->mass_;
        double speed = (mol1->speed_ * mol1->mass_ + mol2->speed_ * mol2->mass_) / new_mol_mass;

        create (1, SQUARE_MOL, speed, new_mol_mass, color, v, pos); // m= m + 1

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
    sf::RectangleShape square_mol (sf::Vector2f (15, 15));

    square_mol.setFillColor ((sf::Color)color_);
    square_mol.setOutlineThickness (1);
    square_mol.setPosition (pos_.x_, pos_.y_);
    
    texture.draw (square_mol);
}

void Mol::draw (sf::RenderTexture &texture) const 
{
    sf::CircleShape circle_mol (10);
    circle_mol.setFillColor ((sf::Color)color_);
    circle_mol.setOutlineThickness (1);
    circle_mol.setPosition (pos_.x_, pos_.y_);
    
    texture.draw (circle_mol);
}

Mol_manager::~Mol_manager ()
{
    for (int i = 0; i < size_; ++i) 
    {
        delete (mols[i]);
        mols[i] = nullptr; 
    }

    free (mols);
    mols = nullptr;
}