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
    Potion(int hpChange, int atkChange, int defChange)
        : hpChange{hpChange}, atkChange{atkChange}, defChange{defChange} {}

public:
    static std::unique_ptr<Potion> make_potion();
    void use(const std::unique_ptr<Player>&) override;
    std::tuple<int, int, int> get_effects() {
        return std::make_tuple(hpChange, atkChange, defChange);
    }
};

std::unique_ptr<Potion> Potion::make_potion() {
    std::default_random_engine& rng{ChamberSettings::get_generator()};
    PotionType pt = static_cast<PotionType>(rng() % PotionType::numUniquePotions);
    switch (pt) {
        case RH:
            return std::make_unique<Potion>(rng() % 11, 0, 0);
        case BA:
            return std::make_unique<TempPotion>(0, 5, 0);
        case BD:
            return std::make_unique<TempPotion>(0, 0, 5);
        case PH:
            return std::make_unique<Potion>(-(rng() % 11), 0, 0);
        case WA:
            return std::make_unique<TempPotion>(0, -5, 0);
        case WD:
            return std::make_unique<TempPotion>(0, 0, -5);
    }
    std::cerr << "Error: Making a potion that doesn't exist\n";
    return nullptr;
}

void Potion::use(const std::unique_ptr<Player>& playerptr) {
    playerptr->change_hp(hpChange);
    playerptr->change_atk(atkChange);
    playerptr->change_def(defChange);
}

class TempPotion : public Potion {
public:
    // I will allow this because we're inside a .cc file and
    // other classes can't see this class and call the constructor
    using Potion::Potion;
    void use(const std::unique_ptr<Player>&) override;
};

void TempPotion::use(const std::unique_ptr<Player>& playerptr) {
    playerptr->change_hp(hpChange);
    *playerptr = TempPotionedPlayer{playerptr, atkChange, defChange};
}

class GoldPile : public ContactItem {
public:
    static std::unique_ptr<Item> make_goldpile(bool makeBarrierSuits);
    void trigger(const std::unique_ptr<Player>&) override;
};

std::unique_ptr<Item> Item::make_item(char item, bool makeBarrierSuits = false) noexcept {
    if (item == 'p') {
        return Potion::make_potion();
    } else if (item == 'g') {
        return GoldPile::make_goldpile(makeBarrierSuits);
    } else {
        return nullptr;
    }
}