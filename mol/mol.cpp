#include <iostream>
#include <SFML/Graphics.hpp>

#include "mol.h"

void Mol_manager::add (Mol *mol)
{
    if ((double)size / (double) capacity > LOAD_FACTOR)
    {
        // this->resize ();
        ;
    }

    mols[size++] = mol;
    // mol->draw (*texture_);
    is_changed = true;
}

void Mol_manager::draw ()
{
    for (int mol_idx = 0; mol_idx < size; mol_idx++)
    {
        (*(mols + mol_idx))->draw (*texture_);
    }
    walls.draw ();
}

bool Mol_manager::move ()
{
    for (int mol_idx = 0; mol_idx < size; mol_idx++)
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

    for (int idx = 0; idx < size; idx++)
    {
        Mol *mol1 = mols[idx];
        
        Point pos = (Point)(mol1->pos_);
        Wall *wall_to_collide = walls.contains (pos);

        if (wall_to_collide)
        {
            wall_collide (mol1, wall_to_collide);
            is_true = true;
        }
        for (int second  = idx + 1; second < size; second++)
        {
            Mol *mol2 = mols[idx + 1];

            if (mol2 && (std::abs(mol1->pos_.x_ - mol2->pos_.x_) < 20) && (std::abs(mol1->pos_.y_ - mol2->pos_.y_) < 20))
            {
                collide (mol1, mol2);
                idx++;
            }
        }
    }
    return is_true;
}

Mol *Mol_manager::remove (Mol *mol) //void
{
    for (int idx = 0; idx <size; idx++)
    {
        if (mols[idx] == mol)
        {
            // mol->~Mol ();
            for (int rm_idx = idx + 1; rm_idx < size; rm_idx++)
            {
                mols[rm_idx - 1] = mols[rm_idx];
            }
            mols[--size] = nullptr;
            delete mol;
            is_changed = true;
            
            break;
        }
    }


    return nullptr;
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
                *mol = (Square_mol (INIT_SPEED, 10, Color (255, 0, 0, 255), 800, 800));
                add (mol);
            }
            break;
        }
        case ROUND_MOL:
        {   
            for (int i = 0; i < size; ++i)
            {
                Mol *mol = new Round_mol();
                *mol = (Round_mol (INIT_SPEED, 10, Color (255, 0, 0, 255), 800, 800));
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

void Mol_manager::collide (Mol *mol1, Mol *mol2)
{
    // Mol *mol = (Mol *)calloc (1, sizeof (Mol));
    //WRONG: WE MUST NOT CREATE RAND MOLS BUT IN THE SAME POS
    Color color (0, 255, 0, 255);

    if (mol1->type == SQUARE_MOL && mol2->type == SQUARE_MOL)
    {
        Vector v = (mol1->v_ && mol1->mass_) + (mol2->v_ && mol2->mass_);
        Vector pos = (mol1->pos_ + mol2->pos_) && 0.5;
        
        for (int i = 0; i < mol1->mass_ + mol2->mass_; ++i)
        {
            create (1, ROUND_MOL, 10.0, 1.0, color, v, pos);
            v.rotate (360.0 / mol1->mass_ + mol2->mass_);
            pos += Vector (20, 20, 0);                      // TODO make somehow that new round mols will scatter before their possible collide
        }
    }
    else if (mol1->type == ROUND_MOL && mol2->type == ROUND_MOL)
    {
        // Vector v = mol1->v_ + mol2->v_;
        Vector v = (mol1->v_ && mol1->mass_) + (mol2->v_ && mol2->mass_);
        Vector pos = (mol1->pos_ + mol2->pos_) && 0.5;
        create (1, SQUARE_MOL, INIT_SPEED, mol1->mass_ + mol2->mass_, color, v, mol1->pos_); // m = 1 + 1 //TODO :: SPEED
    }
    else 
    {
        Vector v = (mol1->v_ && mol1->mass_) + (mol2->v_ && mol2->mass_);
        Vector pos = (mol1->pos_ + mol2->pos_) && 0.5;
        create (1, SQUARE_MOL, INIT_SPEED, mol1->mass_ + mol2->mass_, color, v, mol1->pos_); // m= m + 1
    }
    
    remove (mol1);
    remove (mol2);
    // draw ();
}


void Round_mol::draw (sf::RenderTexture &texture) const 
{
    sf::CircleShape circle_mol (10);
    circle_mol.setFillColor ((sf::Color)color_);
    circle_mol.setOutlineThickness (1);
    circle_mol.setPosition (pos_.x_, pos_.y_);
    
    texture.draw (circle_mol);
}

void Square_mol::draw (sf::RenderTexture &texture) const 
{
    Mol::draw (texture);
}

void Mol::draw (sf::RenderTexture &texture) const 
{
    sf::RectangleShape square_mol (sf::Vector2f (15, 15));

    square_mol.setFillColor ((sf::Color)color_);
    square_mol.setOutlineThickness (1);
    square_mol.setPosition (pos_.x_, pos_.y_);
    
    texture.draw (square_mol);
}

Mol_manager::~Mol_manager ()
{
    for (int i = 0; i < size; ++i) 
    {
        delete (mols[i]);
        mols[i] = nullptr; 
    }

    free (mols);
    mols = nullptr;
}