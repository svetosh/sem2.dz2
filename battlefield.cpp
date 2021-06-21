#include "battlefield.h"

BattleField::BattleField()
{
    matrix.resize(DEFAULT_CELL_COUNT);
    for (long int i = 0; i < matrix.size(); ++i) 
    {
        matrix[i].resize(DEFAULT_CELL_COUNT);
        for (long int j = 0; j < matrix[i].size(); ++j) 
        {
            matrix[i][j] = true;
        }
    }
    for (long int i = 4; i > 0; --i) {
        for (long int j = 5 - i; j > 0; --j) 
        {
            ships.push_back(Ship(i, 0, 0, 'h'));
        }
    }
}

bool
BattleField::is_out_of_matrix(Ship rotated_ship)
{
    long int end_position = rotated_ship.cells.size() - 1 + (rotated_ship.dir == 'h') * (rotated_ship.x) + (rotated_ship.dir == 'v') * (rotated_ship.y);
    if (end_position > matrix.size() - 1) 
    {
        return true;
    }
    return false;
}

bool
BattleField::place_ship(long int sid, long int x, long int y, char dir)
{
    if (sid < ships.size()) 
    {
        Ship backup = ships[sid];
        ships[sid].x = x;
        ships[sid].y = y;
        ships[sid].dir = dir;
        if (is_out_of_matrix(ships[sid]))
        {
            ships[sid] = backup;
        }
        else
        {
            return true;
        }
    }
    return false;
}
bool
BattleField::place_ship(long int sid, long int x, long int y)
{
    return place_ship(sid, x, y, ships[sid].dir);
}

bool
BattleField::rotate_ship(long int sid)
{

    if (sid < ships.size())
    {
        ships[sid].rotate();
        if (is_out_of_matrix(ships[sid])) 
        {
            ships[sid].rotate();
            return false;
        }
    }
    return true;
}

uint8_t
BattleField::punch_cell(long int x, long int y)
{
    if (x < matrix.size() && y < matrix.size())
    {
        if (matrix[x][y])
        {
            matrix[x][y] = false;

            uint8_t sid = 0;
            for (Ship& s : ships)
            {
                if (!s.is_killed()) 
                {
                    for (long int i = 0; i < s.cells.size(); ++i) 
                    {
                        if (((s.x + (s.dir == 'h') * i) == x)
                            && ((s.y + (s.dir == 'v') * i) == y)) 
                        {
                            s.punch_me(i);
                            if (s.is_killed())
                            {
                                return sid + 3;
                            }
                            else 
                            {
                                return 2;
                            }
                        }
                    }
                }
                ++sid;
            }
            return 1;
        }
    }
    return 0;
}