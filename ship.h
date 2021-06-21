#pragma once
#include <cstdint>
#include <vector>

class Ship
{
public:
    std::vector<bool> cells;
    long int x;
    long int y;
    char dir;

    Ship(long int size, long int x, long int y, char dir);

    void rotate();
    void punch_me(long int cell_id);
    bool is_killed();

    Ship operator=(Ship& other_ship);
};