#include "libtgraph.h"

Screen::Screen()
{
}

Screen::Screen(long int res_x, long int res_y)
{
    buffer.resize(res_x);
    for (std::vector<char>& i : buffer) 
    {
        i.resize(res_y);
        for (char& j : i) 
        {
            j = ' ';
        }
    }
}

void
Screen::resize(long int res_x, long int res_y)
{
    buffer.resize(res_x);
    for (std::vector<char>& i : buffer) 
    {
        i.resize(res_y);
        for (char& j : i) 
        {
            j = ' ';
        }
    }
}

void
Screen::clear()
{
    for (std::vector<char>& i : buffer)
    {
        for (char& j : i) {
            j = ' ';
        }
    }
}

char&
Screen::raw_buffer(long int pos_x, long int pos_y)
{
    return buffer[pos_x][pos_y];
}

long int
Screen::xsize() const
{
    return buffer.size();
}

long int
Screen::ysize() const
{
    if (buffer.size() > 0)
        return buffer[0].size();
    else
        return 0;
}

void
Screen::print(std::ostream& out)
{
    for (long int y = 0; y < ysize(); ++y)
    {
        for (long int x = 0; x < xsize(); ++x)
        {
            out << buffer[x][y];
        }
        out << std::endl;
    }
}

void
Screen::place_text(long int pos_x, long int pos_y, const std::string& text, bool horizontal)
{
    long int counter = 0;
    if (pos_x < xsize() && pos_y < ysize())
    {
        for (char i : text) 
        {
            if (horizontal && pos_x + counter < xsize())
                buffer[pos_x + counter][pos_y] = i;
            else if (pos_y + counter < ysize())
                buffer[pos_x][pos_y + counter] = i;
            ++counter;
        }
    }
}

void
Screen::rel_place_text(float rel_x, float rel_y, const std::string& text, bool horizontal)
{
    long int pos_x = xsize() * rel_x;
    long int pos_y = ysize() * rel_y;
    place_text(pos_x, pos_y, text, horizontal);
}

void
Screen::rel_place_text(double rel_x, double rel_y, const std::string& text, bool horizontal)
{
    long int pos_x = xsize() * rel_x;
    long int pos_y = ysize() * rel_y;
    place_text(pos_x, pos_y, text, horizontal);
}

void
Screen::place_matrix(long int pos_x, long int pos_y, const std::vector<std::vector<char>>& matrix)
{
    if (pos_x < xsize() && pos_y < ysize())
    {
        for (long int x = 0; x < matrix.size(); ++x) 
        {
            for (long int y = 0; y < matrix[0].size(); ++y)
            {
                if (pos_x + x < xsize() && pos_y + y < ysize())
                    buffer[pos_x + x][pos_y + y] = matrix[x][y];
            }
        }
    }
}

void
Screen::rel_place_matrix(float rel_x, float rel_y, const std::vector<std::vector<char>>& matrix)
{
    long int pos_x = xsize() * rel_x;
    long int pos_y = ysize() * rel_y;
    place_matrix(pos_x, pos_y, matrix);
}

void
Screen::rel_place_matrix(double rel_x, double rel_y, const std::vector<std::vector<char>>& matrix)
{
    long int pos_x = xsize() * rel_x;
    long int pos_y = ysize() * rel_y;
    place_matrix(pos_x, pos_y, matrix);
}