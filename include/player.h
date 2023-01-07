#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include "direction.h"
#include "stats.h"

class Player {
protected:
    Stats stats;
    char race;
    int gold;
    Player(int, int, int, char, std::pair<int, int>&&);
    // virtual void trigger(); This will be for gold pickups

public:
    virtual ~Player() = 0;
    static std::shared_ptr<Player> make_player(char, std::pair<int, int>&&) noexcept;
    std::tuple<int, int, int, char, int> get_stats() const noexcept;
    virtual void change_gold(int amount) { gold + amount > 0 ? gold += amount : gold = 0; }
    int get_gold() const noexcept { return gold; } // only used by decorators

    virtual void move(Direction dir) noexcept { stats.move(dir); }

    // these forwarding functions are so pointless, just use inheritance
    // and get rid of the stats object...
    // note to self: prefer inheritance over composition when I want to
    //   manipulate the data fields directly from the outside class
    void change_hp(int change) noexcept { stats.change_hp(change); }
    void change_atk(int change) noexcept { stats.change_atk(change); }
    void change_def(int change) noexcept { stats.change_def(change); }
    std::pair<int, int> get_pos() const noexcept { return stats.get_pos(); }

    // Potion logic
    virtual std::shared_ptr<Player> remove_effects() noexcept = 0;

    // Combat logic
    virtual void tank(int) noexcept;
};

// I will allow the decorator constructors to be public because
// they require a reference to an existing playerptr, which the user
// needs to initialize to a player created through the factory method
class PlayerDecorator : public Player {
protected:
    // Decorators should not have unique_ptrs to objects, think ownership semantics
    // 1. if the object was stack allocated, unique_ptr ends up calling delete on a stack object
    // 2. the decorator is not responsible for deleting the inside objects (decorators can be stripped off)
    std::shared_ptr<Player> playerptr;

public:
    PlayerDecorator(std::shared_ptr<Player> playerptr) : Player(*playerptr), playerptr{playerptr} {}
    std::shared_ptr<Player> remove_effects() noexcept override { return playerptr->remove_effects(); } // no const since I don't want to return a const &
    void move(Direction dir) noexcept override { stats.move(dir); playerptr->move(dir); }
    void tank(int) noexcept override;
    void change_gold(int amount) override {
        playerptr->change_gold(amount);
        gold = playerptr->get_gold();
    } // assuming races aren't changed during the game
};

class TempPotionedPlayer : public PlayerDecorator {
public:
    TempPotionedPlayer(std::shared_ptr<Player> playerptr, int atkChange, int defChange) : PlayerDecorator(playerptr) {
        change_atk(atkChange);
        change_def(defChange);
    }
};

class BarrierSuitPlayer : public PlayerDecorator {
public:
    using PlayerDecorator::PlayerDecorator;
    void tank(int) noexcept override;
};

#endif