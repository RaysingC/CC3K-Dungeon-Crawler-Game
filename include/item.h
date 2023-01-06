#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <memory>

class Player;

class Item {
protected:
    Item() = default;

public:
    virtual ~Item() = default;
    static std::unique_ptr<Item> make_item(char, bool) noexcept;
    virtual void use(const std::unique_ptr<Player>&) = 0;
};

// ContactItem should not inherit publicly from Item because when
// Items are used they disappear (this is not true of ContactItems)
class ContactItem {
public:
    virtual void trigger(const std::unique_ptr<Player>&) = 0;
};

#endif