#pragma once
#include "ship.h"

const long int DEFAULT_CELL_COUNT = 10;
const long int MAX_SHIP_SIZE = 4;

class BattleField
{
public:
    std::vector<std::vector<bool>> matrix;
    std::vector<Ship> ships;

    BattleField();
    bool place_ship(long int sid, long int x, long int y, char dir);
    bool place_ship(long int sid, long int x, long int y);
    bool rotate_ship(long int sid);
    uint8_t punch_cell(long int x, long int y);
    bool is_out_of_matrix(Ship rotated_ship);
};