#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <memory>

class Player;

// consider removing pos field it's handled by chamber.cc
class Item {
protected:
    const std::pair<int, int> pos;
    const char icon;
    Item(std::pair<int, int>&& pos, char icon) : pos{std::move(pos)}, icon{icon} {}

public:
    virtual ~Item() = default;
    static std::unique_ptr<Item> make_item(char, std::pair<int, int>&&) noexcept;
    virtual void use(const std::unique_ptr<Player>&) = 0;
    char get_icon() const noexcept { return icon; }
};

// ContactItem should not inherit publicly from Item because when
// Items are used they disappear (this is not true of ContactItems)
class ContactItem {
protected:
    const std::pair<int, int> pos;
    const char icon;
    ContactItem(std::pair<int, int>&& pos, char icon) : pos{std::move(pos)}, icon{icon} {}

public:
    virtual ~ContactItem() = default;
    static std::unique_ptr<ContactItem> make_contact_item(char, std::pair<int, int>&&, bool) noexcept;
    virtual void trigger(const std::unique_ptr<Player>&) = 0;
    char get_icon() const noexcept { return icon; }
};

#endif