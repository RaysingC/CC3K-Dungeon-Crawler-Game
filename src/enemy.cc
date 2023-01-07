#include "enemy.h"
#include <cmath>
#include "direction.h"
#include "player.h"

Enemy::Enemy(int maxhp, int atk, int def, char type, std::pair<int, int>&& pos)
    : stats{maxhp, atk, def, std::move(pos)}, type{type}, playerInRangeLastTurn{false},
    playerInRangeNow{false} {}

Enemy::~Enemy() {}

void Enemy::tank(int playerAttack) noexcept {
    auto [ hp, atk, def] = stats.get_tuple();
    int totalDamage = ceil(100.0 / (100.0 + static_cast<double>(def) * static_cast<double>(atk)));
    stats.change_hp(totalDamage);
}

void Enemy::check_player_in_range(const std::unique_ptr<Player>& playerptr) noexcept {
    playerInRangeLastTurn = playerInRangeNow;
    const auto [ myx, myy ] = get_pos();
    const auto [ playerx, playery ] = playerptr->get_pos();
    if (myx - playerx <= 1 && myx - playerx >= -1 &&
        myy - playery <= 1 && myy - playery >= -1) {
        playerInRangeNow = true;
    } else {
        playerInRangeNow = false;
    }
}

void Enemy::attempt_attack(const std::unique_ptr<Player>& playerptr) /*const*/ noexcept {
    if (playerInRangeLastTurn && playerInRangeNow) {
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
    // add gold stealing method
};

class Merchant : public Enemy {
public:
    Merchant(std::pair<int, int>&& pos) : Enemy(30, 70, 5, 'M', std::move(pos)) {}
    std::unique_ptr<ContactItem> drop_item() const override;
};

std::unique_ptr<ContactItem> Merchant::drop_item() const {
    auto pos = get_pos();
    return ContactItem::make_goldpile(4, std::move(pos));
}

class Dragon : public Enemy {
public:
    Dragon(std::pair<int, int>&& pos) : Enemy(150, 20, 20, 'D', std::move(pos)) {}
    void move(Direction) noexcept override {}
};

class Phoenix : public Enemy {
public:
    Phoenix(std::pair<int, int>&& pos) : Enemy(50, 35, 20, 'X', std::move(pos)) {}
};