#ifndef CELL_H
#define CELL_H

#include <memory>
#include "direction.h"

class Cell {
    char type;
    bool occupied;

public:
    Cell() : type{' '}, occupied{true} {}
    Cell(char type) : type{type},
        occupied{type == '.' || type == '#' || type == '+' ? false : true} {}
    char get_type() const noexcept { return type; }
    bool is_occupied() const noexcept { return occupied; }
    void occupy() noexcept { occupied = true; }
    void unoccupy() noexcept { occupied = false; }
};

#endif