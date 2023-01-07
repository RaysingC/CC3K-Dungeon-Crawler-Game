#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include "direction.h"
#include "stats.h"
#include <cmath>

class Player {
protected:
    Stats stats;
    char race;
    int gold;
    Player(int, int, int, char, std::pair<int, int>&&);
    // virtual void trigger(); This will be for gold pickups

public:
    virtual ~Player() = 0;
    static std::unique_ptr<Player> make_player(char, std::pair<int, int>&&) noexcept;
    virtual std::tuple<int, int, int, char, int> get_stats() const noexcept;
    virtual void change_gold(int amount) { gold + amount > 0 ? gold += amount : gold = 0; }

    void move(Direction dir) noexcept { stats.move(dir); }

    // these forwarding functions are so pointless, just use inheritance
    // and get rid of the stats object...
    // note to self: prefer inheritance over composition when I want to
    //   manipulate the data fields directly from the outside class
    void change_hp(int change) noexcept { stats.change_hp(change); }
    void change_atk(int change) noexcept { stats.change_atk(change); }
    void change_def(int change) noexcept { stats.change_def(change); }
    const std::pair<int, int>& get_pos() noexcept { return stats.get_pos(); }

    // Potion logic
    virtual Player& remove_effects() noexcept { return *this; };

    // Combat logic
    virtual void tank(int) noexcept;
};

// I will allow the decorator constructors to be public because
// they require a reference to an existing playerptr, which the user
// needs to initialize to a player created through the factory method
class PlayerDecorator : public Player {
protected:
    const std::unique_ptr<Player>& playerptr;

public:
    PlayerDecorator(const std::unique_ptr<Player>& playerptr)
        : Player(*playerptr), playerptr{playerptr} {}
    Player& remove_effects() noexcept { return *playerptr; }
};

class TempPotionedPlayer : public PlayerDecorator {
    int atkChange, defChange;

public:
    TempPotionedPlayer(const std::unique_ptr<Player>& playerptr, int atkChange, int defChange)
        : PlayerDecorator(playerptr), atkChange{atkChange}, defChange{defChange} {}
    std::tuple<int, int, int, char, int> get_stats() const noexcept override;
};

class BarrierSuitPlayer : public PlayerDecorator {
public:
    using PlayerDecorator::PlayerDecorator;
    void tank(int) noexcept override;
};

#endif