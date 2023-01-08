#include "item.h"
#include "chambersettings.h"
#include "player.h"
#include <iostream>

// potion class doesn't actually need all those fields,
// consider moving some to derived classes
class Potion : public Item {
protected:
    enum PotionType { RH, BA, BD, PH, WA, WD, numUniquePotions };
    int hpChange, atkChange, defChange;
    Potion(std::pair<int, int>&& pos, int hpChange, int atkChange, int defChange)
        : Item(std::move(pos), 'P'), hpChange{hpChange}, atkChange{atkChange}, defChange{defChange} {}

public:
    static std::unique_ptr<Potion> make_potion(std::pair<int, int>&&);
    void use(std::shared_ptr<Player>&) override;
    std::tuple<int, int, int> get_effects() {
        return std::make_tuple(hpChange, atkChange, defChange);
    }
};

// should this be polymorphic to Potion?
class TempPotion : public Potion {
public:
    // I will allow this because we're inside a .cc file and
    // other classes can't see this class and call the constructor
    using Potion::Potion;
    void use(std::shared_ptr<Player>&) override;
};

std::unique_ptr<Potion> Potion::make_potion(std::pair<int, int>&& pos) {
    std::default_random_engine& rng{ChamberSettings::get_generator()};
    PotionType pt = static_cast<PotionType>(rng() % PotionType::numUniquePotions);
    // I have to initialize unique_ptrs like this because all the constructors are inaccessible
    // the forwarding function make_unique can't access their constructors
    /*
    switch (pt) {
        case RH:
            return std::unique_ptr<Potion>(new Potion{std::move(pos), static_cast<int>(rng() % 10) + 1, 0, 0});
        case BA:
            return std::unique_ptr<TempPotion>(new TempPotion{std::move(pos), 0, 5, 0});
        case BD:
            return std::unique_ptr<TempPotion>(new TempPotion{std::move(pos), 0, 0, 5});
        case PH:
            return std::unique_ptr<Potion>(new Potion{std::move(pos), static_cast<int>(-(rng() % 10)) + 1, 0, 0});
        case WA:
            return std::unique_ptr<TempPotion>(new TempPotion{std::move(pos), 0, -5, 0});
        case WD:
            return std::unique_ptr<TempPotion>(new TempPotion{std::move(pos), 0, 0, -5});
        default:
            break;
    }
    */
            return std::unique_ptr<TempPotion>(new TempPotion{std::move(pos), 0, 5, 0});
    std::cerr << "Error: Making a potion that doesn't exist\n";
    return nullptr;
}

void Potion::use(std::shared_ptr<Player>& playerptr) {
    const auto [ hp, atk, def, race, gold ] = playerptr->get_stats();
    if (race == 'e') {
        hpChange = abs(hpChange);
        atkChange = abs(atkChange);
        defChange = abs(defChange);
    }
    playerptr->change_hp(hpChange);
    playerptr->change_atk(atkChange);
    playerptr->change_def(defChange);
}

void TempPotion::use(std::shared_ptr<Player>& playerptr) {
    const auto [ hp, atk, def, race, gold ] = playerptr->get_stats();
    if (race == 'e') {
        hpChange = abs(hpChange);
        atkChange = abs(atkChange);
        defChange = abs(defChange);
    }
    playerptr->change_hp(hpChange);
    // I can't reassign *playerptr by value because playerptr was initialized with a Player*
    // so it's still using the Player vptr! I have to overwrite playerptr to use a different vptr
    playerptr = std::shared_ptr<Player>(new TempPotionedPlayer{playerptr, atkChange, defChange});
}

std::unique_ptr<Item> Item::make_item(char item, std::pair<int, int>&& pos) noexcept {
    if (item == 'p') {
        return Potion::make_potion(std::move(pos));
    } else {
        return nullptr;
    }
}

class GoldPile : public ContactItem {
    int value;
    GoldPile(std::pair<int, int>&& pos, int value) : ContactItem(std::move(pos), 'G'), value{value} {}
    friend std::unique_ptr<ContactItem>
        ContactItem::make_goldpile(int, std::pair<int, int>&&); // really?

public:
    static std::unique_ptr<ContactItem> make_goldpile(std::pair<int, int>&&, bool);
    void trigger(std::shared_ptr<Player>& playerptr) override {
        // logic based on race is handled by polymorphism see player.cc
        playerptr->change_gold(value);
    }
};

class Stairs : public ContactItem {
    Stairs(std::pair<int, int>&& pos) : ContactItem(std::move(pos), '\\') {}

public:
    void trigger(std::shared_ptr<Player>& playerptr) override {
        playerptr = playerptr->remove_effects();
        ++ChamberSettings::floor_number();
        ChamberSettings::reset_next_turn() = true;
    }
    friend class ContactItem;
};

class BarrierSuit : public ContactItem {
    BarrierSuit(std::pair<int, int>&& pos) : ContactItem(std::move(pos), 'B') {}

public:
    void trigger(std::shared_ptr<Player>& playerptr) override {
        playerptr = std::shared_ptr<Player>(new BarrierSuitPlayer{playerptr});
    }
    friend class GoldPile;
};

std::unique_ptr<ContactItem> GoldPile::make_goldpile(std::pair<int, int>&& pos, bool makeBarrierSuits) {
    std::default_random_engine& rng = ChamberSettings::get_generator();
    int randInt = rng() % 8;
    if (randInt < 5) {
        return std::unique_ptr<GoldPile>(new GoldPile{std::move(pos), 1});
    } else if (randInt < 7) {
        return std::unique_ptr<GoldPile>(new GoldPile{std::move(pos), 2});
    } else if (makeBarrierSuits) {
        return std::unique_ptr<BarrierSuit>(new BarrierSuit{std::move(pos)});
    } else {
        return std::unique_ptr<GoldPile>(new GoldPile{std::move(pos), 6});
    }
}

std::unique_ptr<ContactItem>
ContactItem::make_contact_item(char item, std::pair<int, int>&& pos, bool makeBarrierSuits = false) {
    if (item == 'g') {
        return GoldPile::make_goldpile(std::move(pos), makeBarrierSuits);
    } else if (item == 's') {
        return std::unique_ptr<Stairs>(new Stairs{std::move(pos)});
    } else {
        return nullptr;
    }
}

std::unique_ptr<ContactItem>
ContactItem::make_goldpile(int value, std::pair<int, int>&& pos) {
    return std::unique_ptr<GoldPile>(new GoldPile{std::move(pos), value});
}