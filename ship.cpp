#include "ship.h"

Ship::Ship(long int size, long int x, long int y, char dir)
{
    cells.resize(size);
    for (long int i = 0; i < cells.size(); ++i) 
    {
        cells[i] = true;
    }
    this->x = x;
    this->y = y;
    this->dir = dir;
}

void
Ship::rotate()
{
    if (dir == 'h')
    {
        dir = 'v';
    }
    else 
    {
        dir = 'h';
    }
}

void
Ship::punch_me(long int cell_id)
{
    cells[cell_id] = false;
}

bool
Ship::is_killed()
{
    for (bool i : cells) 
    {
        if (i)
        {
            return false;
        }
    }
    return true;
}
Ship
Ship::operator=(Ship& other_ship)
{
    this->x = other_ship.x;
    this->y = other_ship.y;
    this->dir = other_ship.dir;
    this->cells = other_ship.cells;
    return *this;
}
