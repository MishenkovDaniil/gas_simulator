#include <iostream>
#include <SFML/Graphics.hpp>

#include "mol.h"

int compare_mols(const void* a, const void* b)
{
    const Mol *mol_a = (const Mol *)a;
    const Mol *mol_b = (const Mol *)b;

    double a_x = mol_a->pos_.x_;
    double b_x = mol_b->pos_.x_;
    double a_y = mol_a->pos_.y_;
    double b_y = mol_b->pos_.y_;

    if (a_x > b_x)
        return 1;
    else if (a_x < b_x)
        return -1;
    
    return (a_y > b_y) - (a_y < b_y);
}

void Mol_manager::add (Mol *mol)
{
    if ((double)size / (double) capacity > LOAD_FACTOR)
    {
        // this->resize ();
        ;
    }

    mols[size++] = mol;
    mol->draw (*texture_);
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
    if (is_changed)
    {
        std::qsort ((void **)mols, size, sizeof (Mol *), compare_mols);
        is_changed = false;
    }
    bool is_true = false;

    for (int idx = 0; idx < size; idx++)
    {
        Mol *mol1 = mols[idx];
        Mol *mol2 = mols[idx + 1];
        
        Point pos = (Point)(mol1->pos_);
        Wall *wall_to_collide = walls.contains (pos);

        if (wall_to_collide)
        {
            wall_collide (mol1, wall_to_collide);
            is_true = true;
        }
        // if (mol2 && (std::abs(mol1->pos_.x_ - mol2->pos_.x_) < 10) && (std::abs(mol1->pos_.y_ - mol2->pos_.y_) < 10))
        // {
        //     fprintf (stderr, "pos1.x = %lf, pos1. y = %lf\n", mol1->pos_.x_, mol1->pos_.y_);
        //     fprintf (stderr, "pos2.x = %lf, pos2. y = %lf\n", mol2->pos_.x_, mol2->pos_.y_);
        //     Mol *new_mol = collide (mol1, mol2);
        //     remove (mol1); ///
        //     remove (mol2); /// here is where is_changed becomes true
        //     add (new_mol); /// 
        //     draw ();

        // }
    }
    return is_true;
}

Mol *Mol_manager::remove (Mol *mol) //void
{
    for (int idx = 0; idx <size; idx++)
    {
        if (mols[idx] == mol)
        {
            mol->~Mol ();
            for (int rm_idx = idx + 1; rm_idx < size; rm_idx++)
            {
                mols[rm_idx - 1] = mols[rm_idx];
            }
            mols[--size] = nullptr;
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
                *mol = (Square_mol (1, 10, Color (255, 0, 0, 255), 800, 800));
                add (mol);
            }
            break;
        }
        case ROUND_MOL:
        {   
            for (int i = 0; i < size; ++i)
            {
                Mol *mol = new Round_mol();
                *mol = (Round_mol (1, 10, Color (255, 0, 0, 255), 800, 800));
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

Mol *Mol_manager::collide (Mol *mol1, Mol *mol2)
{
    Mol *mol = (Mol *)calloc (1, sizeof (Mol));
    *mol = *mol1;
    mol->color_ = Color (0, 255, 0, 255);
    return mol;
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