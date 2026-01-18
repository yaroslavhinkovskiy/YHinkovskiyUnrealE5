// ProjectC++Yaroslav Hinkovskiy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <limits>
#include <memory>

typedef std::map<int, int> Army;

/* ================== BASE ITEM ================== */
class Item {
public:
    std::string icon;
    virtual ~Item() = default;

    virtual void applyStats(int& str, int& intel, int& agi) {}
    virtual int speedBonus() const { return 0; }
};

/* ================== PASSIVE ITEM ================== */
class PassiveItem : public Item {
public:
    int strBonus, intBonus, agiBonus;

    PassiveItem(std::string ic, int s, int i, int a)
        : strBonus(s), intBonus(i), agiBonus(a) {
        icon = ic;
    }

    void applyStats(int& str, int& intel, int& agi) override {
        str += strBonus;
        intel += intBonus;
        agi += agiBonus;
    }
};

/* ================== SPEED ITEM ================== */
class SpeedItem : public Item {
    int bonus;
public:
    SpeedItem(std::string ic, int b) : bonus(b) {
        icon = ic;
    }

    int speedBonus() const override {
        return bonus;
    }
};

/* ================== WEAPON ================== */
class Weapon {
public:
    std::string icon;
    int damage;
    int cooldown;
    int currentCooldown;

    Weapon(std::string ic, int dmg, int cd)
        : icon(ic), damage(dmg), cooldown(cd), currentCooldown(0) {
    }

    bool ready() const {
        return currentCooldown == 0;
    }

    void use() {
        if (ready()) {
            std::cout << "Weapon attack! Damage: " << damage << "\n";
            currentCooldown = cooldown;
        }
        else {
            std::cout << "Weapon on cooldown: " << currentCooldown << "\n";
        }
    }

    void tick() {
        if (currentCooldown > 0)
            currentCooldown--;
    }
};

/* ================== CHARACTER ================== */
class Character {
public:
    std::string name;
    std::string character_class;
    float health;
    int x, y;

    int baseSpeed;
    int currentSpeed;

    Army army;
    bool took_damage;

    int strength, intelligence, agility;

    std::shared_ptr<Weapon> weapon;
    std::vector<std::shared_ptr<Item>> inventory;

    Character()
        : health(0), x(0), y(0),
        baseSpeed(1), currentSpeed(1),
        took_damage(false),
        strength(10), intelligence(10), agility(10) {
    }

    void move(int dx, int dy) {
        x += dx * currentSpeed;
        y += dy * currentSpeed;
        std::cout << name << " moved to (" << x << ", " << y << ")\n";
    }

    void equipWeapon(std::shared_ptr<Weapon> w) {
        weapon = w;
    }

    void attack() {
        if (!weapon) {
            std::cout << name << " has no weapon!\n";
            return;
        }
        weapon->use();
    }

    /* === INVENTORY === */
    void addItemToInventory(std::shared_ptr<Item> item) {
        inventory.push_back(item);
        item->applyStats(strength, intelligence, agility);
        recalculateSpeed();
    }

    void recalculateSpeed() {
        currentSpeed = baseSpeed;
        for (const auto& item : inventory) {
            currentSpeed += item->speedBonus();
        }
    }

    void input(int index) {
        std::cout << "\n--- Player " << index + 1 << " ---\n";
        std::cout << "Name: ";
        std::cin >> name;

        std::cout << "Health: ";
        std::cin >> health;

        std::cout << "Class: ";
        std::cin >> character_class;

        std::cout << "Position (x y): ";
        std::cin >> x >> y;

        std::cout << "Base speed: ";
        std::cin >> baseSpeed;
        currentSpeed = baseSpeed;
    }
};

/* ================== METEOR ================== */
void processMeteor(std::vector<Character>& characters) {
    int mx, my;
    float dmg, power;

    std::cout << "\nMeteor (x y dmg power): ";
    std::cin >> mx >> my >> dmg >> power;

    float radius = 3.0f * power;

    for (auto& c : characters) {
        float dist = std::sqrt((c.x - mx) * (c.x - mx) + (c.y - my) * (c.y - my));
        if (dist <= radius) {
            c.health -= dmg;
            c.took_damage = true;
        }
    }
}

/* ================== STATE ================== */
void getPlayersState(const std::vector<Character>& chars) {
    std::cout << "\n--- RESULTS ---\n";
    for (const auto& c : chars) {
        std::cout << c.name << " [" << c.character_class << "]\n";
        std::cout << "HP: " << (c.health > 0 ? c.health : 0)
            << (c.took_damage ? " WOUNDED\n" : "\n");
        std::cout << "STR: " << c.strength
            << " INT: " << c.intelligence
            << " AGI: " << c.agility << "\n";
        std::cout << "Speed: " << c.currentSpeed << "\n";
        std::cout << "----------------------\n";
    }
}

/* ================== MAIN ================== */
int main() {
    int n;
    std::cout << "Players count: ";
    std::cin >> n;

    std::vector<Character> characters(n);
    for (int i = 0; i < n; i++)
        characters[i].input(i);

    auto sword = std::make_shared<Weapon>("sword.png", 30, 2);
    auto ring = std::make_shared<PassiveItem>("ring.png", 2, 3, 1);
    auto boots = std::make_shared<SpeedItem>("boots.png", 2);

    characters[0].equipWeapon(sword);
    characters[0].addItemToInventory(ring);
    characters[0].addItemToInventory(boots);

    characters[0].attack();
    sword->tick();
    sword->tick();
    characters[0].attack();

    processMeteor(characters);
    getPlayersState(characters);

    return 0;
}