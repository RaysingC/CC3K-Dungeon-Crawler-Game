#include "item.h"
#include "chambersettings.h"

class Potion : public Item {
protected:
    enum type { RH, BA, BD, PH, WA, WD };
    int maxhpChange, hpChange, atkChange, defChange;

public:
    static std::unique_ptr<Potion> make_potion();
    std::tuple<int, int, int, int> get_stats() {
        return std::make_tuple(maxhpChange, hpChange, atkChange, defChange);
    }
};

class GoldPile : public ContactItem {
public:
    void trigger(std::unique_ptr<Player>&) override;
};

std::unique_ptr<Item> Item::make_item(char item, bool barrierSuit = false) noexcept {
    if (item == 'p') {
        return make_potion();
    } else if (item == 'g') {
        return make_goldpile(barrierSuit);
    } else {
        return nullptr;
    }
}