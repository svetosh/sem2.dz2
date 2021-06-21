#pragma once
#include <iostream>
#include <string>
#include <vector>

class Screen
{

    std::vector<std::vector<char>> buffer;

public:
    Screen();
    Screen(long int res_x, long int res_y);
    void resize(long int res_x, long int res_y);
    void clear();
    char& raw_buffer(long int pos_x, long int pos_y);
    long int xsize() const;
    long int ysize() const;

    void print(std::ostream& out = std::cout);

    void place_text(long int pos_x, long int pos_y, const std::string& text, bool horizontal = true);
    void rel_place_text(float rel_x, float rel_y, const std::string& text, bool horizontal = true);
    void rel_place_text(double rel_x, double rel_y, const std::string& text, bool horizontal = true);

    void place_matrix(long int pos_x, long int pos_y, const std::vector<std::vector<char>>& matrix);
    void rel_place_matrix(float rel_x, float rel_y, const std::vector<std::vector<char>>& matrix);
    void rel_place_matrix(double rel_x, double rel_y, const std::vector<std::vector<char>>& matrix);
};