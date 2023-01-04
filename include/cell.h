#ifndef CELL_H
#define CELL_H

#include <memory>
#include "direction.h"

class Cell {
    char type;
    bool occupied, hasContactItem;

public:
    Cell() : type{' '}, occupied{true} {}
    Cell(char type) : type{type},
        occupied{type == '.' || type == '#' || type == '+' ? false : true} {}
    char get_type() const noexcept { return type; }

    // movement logic
    bool is_occupied() const noexcept { return occupied; }
    void occupy() noexcept { occupied = true; }
    void unoccupy() noexcept { occupied = false; }

    // item logic
    bool has_contact_item() const noexcept { return hasContactItem; }
    void place_contact_item() noexcept { hasContactItem = true; }
    void remove_contact_item() noexcept { hasContactItem = false; }

};

#endif