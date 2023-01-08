#include "enemy.h"
#include <cmath>
#include "direction.h"
#include "player.h"

Enemy::Enemy(int maxhp, int atk, int def, char type, std::pair<int, int>&& pos, bool hostile = true)
    : stats{maxhp, atk, def, std::move(pos)}, type{type}, playerInRangeLastTurn{false},
    playerInRangeNow{false}, hostile{hostile} {}

Enemy::~Enemy() {}

void Enemy::tank(int playerAttack) noexcept {
    auto [ hp, atk, def] = stats.get_tuple();
    int totalDamage = ceil(100.0 / (100.0 + static_cast<double>(def) * static_cast<double>(atk)));
    stats.change_hp(totalDamage);
}

bool Enemy::player_in_range(const std::shared_ptr<Player>& playerptr) noexcept {
    playerInRangeLastTurn = playerInRangeNow;
    const auto [ myx, myy ] = get_pos();
    const auto [ playerx, playery ] = playerptr->get_pos();
    if (myx - playerx <= 1 && myx - playerx >= -1 &&
        myy - playery <= 1 && myy - playery >= -1) {
        playerInRangeNow = true;
        return true;
    } else {
        playerInRangeNow = false;
        return false;
    }
}

void Enemy::attempt_attack(const std::shared_ptr<Player>& playerptr) /*const*/ noexcept {
    if (hostile && playerInRangeLastTurn && playerInRangeNow) {
        const auto [ hp, atk, def ] = stats.get_tuple();
        playerptr->tank(atk);
    }
}

class Vampire : public Enemy {
public:
    Vampire(std::pair<int, int>&& pos) : Enemy(50, 25, 25, 'V', std::move(pos)) {}
};

class Werewolf : public Enemy {
public:
    Werewolf(std::pair<int, int>&& pos) : Enemy(120, 30, 5, 'W', std::move(pos)) {}
};

class Troll : public Enemy {
public:
    Troll(std::pair<int, int>&& pos) : Enemy(120, 25, 15, 'T', std::move(pos)) {}
    void passive() noexcept override;
};

void Troll::passive() noexcept {
    const auto [ hp, atk, def ] = stats.get_tuple();
    if (hp + 2 <= 120) // hardcoding the troll's hp is terrible
        stats.change_hp(2);
}

class Goblin : public Enemy {
public:
    Goblin(std::pair<int, int>&& pos) : Enemy(70, 5, 10, 'N', std::move(pos)) {}
    void attempt_attack(const std::shared_ptr<Player>&) noexcept override;
};

void Goblin::attempt_attack(const std::shared_ptr<Player>& playerptr) noexcept {
    if (hostile && playerInRangeLastTurn && playerInRangeNow) {
        const auto [ hp, atk, def ] = stats.get_tuple();
        playerptr->tank(atk);
        playerptr->change_gold(-1);
    }
}

std::unique_ptr<ContactItem> Merchant::drop_item() const {
    return ContactItem::make_goldpile(4, get_pos());
}

void Merchant::tank(int playerAttack) noexcept {
    Merchant::hostile = true;
    Enemy::tank(playerAttack);
}

void Merchant::attempt_attack(const std::shared_ptr<Player>& playerptr) noexcept {
    if (Merchant::hostile && playerInRangeLastTurn && playerInRangeNow) {
        const auto [ hp, atk, def ] = stats.get_tuple();
        playerptr->tank(atk);
    }
}

class Dragon : public Enemy {
public:
    Dragon(std::pair<int, int>&& pos) : Enemy(150, 20, 20, 'D', std::move(pos), false) {}
    void move(Direction) noexcept override {}
    void tank(int playerAttack) noexcept override {
        hostile = true;
        Enemy::tank(playerAttack);
    }
};

class Phoenix : public Enemy {
public:
    Phoenix(std::pair<int, int>&& pos) : Enemy(50, 35, 20, 'X', std::move(pos)) {}
};