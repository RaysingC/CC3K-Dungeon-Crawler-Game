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

public:
    virtual ~Player() = 0;
    static std::shared_ptr<Player> make_player(char, std::pair<int, int>&&) noexcept;
    std::tuple<int, int, int, char, int> get_stats() const noexcept;
    virtual void change_gold(int amount) { gold + amount > 0 ? gold += amount : gold = 0; }

    virtual void move(Direction dir) noexcept { stats.move(dir); }

    void change_hp(int change) noexcept { stats.change_hp(change); }
    void change_atk(int change) noexcept { stats.change_atk(change); }
    void change_def(int change) noexcept { stats.change_def(change); }
    std::pair<int, int> get_pos() const noexcept { return stats.get_pos(); }
    void set_pos(std::pair<int, int>&& pos) noexcept { stats.set_pos(std::move(pos)); }

    // Potion logic
    virtual std::shared_ptr<Player> remove_effects() noexcept = 0;

    // Combat logic
    virtual void tank(int) noexcept;
};

class PlayerDecorator : public Player {
protected:
    std::shared_ptr<Player> playerptr;

public:
    PlayerDecorator(const std::shared_ptr<Player>& playerptr) : Player(*playerptr), playerptr{playerptr} {}
    std::shared_ptr<Player> remove_effects() noexcept override { return playerptr->remove_effects(); } // no const since I don't want to return a const &
    void move(Direction dir) noexcept override { stats.move(dir); playerptr->move(dir); }
    void tank(int) noexcept override;
    void change_gold(int amount) override {
        playerptr->change_gold(amount);
        const auto [ hp, atk, def, race, gold ] = playerptr->get_stats();
        this->gold = gold;
    }
};

class TempPotionedPlayer : public PlayerDecorator {
public:
    TempPotionedPlayer(const std::shared_ptr<Player>& playerptr, int atkChange, int defChange) : PlayerDecorator(playerptr) {
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