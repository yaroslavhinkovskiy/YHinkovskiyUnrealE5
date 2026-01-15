// ProjectC++Yaroslav Hinkovskiy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <limits>

typedef std::map<int, int> Army;

/* ================== PASSIVE ITEM ================== */
class PassiveItem {
public:
    std::string icon;
    int strBonus, intBonus, agiBonus;

    PassiveItem(std::string ic, int s, int i, int a)
        : icon(ic), strBonus(s), intBonus(i), agiBonus(a) {
    }

    void apply(int& str, int& intel, int& agi) {
        str += strBonus;
        intel += intBonus;
        agi += agiBonus;
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
    int speed;
    Army army;
    bool took_damage;

    
    int strength, intelligence, agility;

    Weapon* weapon;
    PassiveItem* passives[3];

    Character()
        : health(0), x(0), y(0), speed(1),
        took_damage(false),
        strength(10), intelligence(10), agility(10),
        weapon(nullptr)
    {
        for (int i = 0; i < 3; i++) passives[i] = nullptr;
    }

    void Move(int dx, int dy) {
        x += dx * speed;
        y += dy * speed;
        std::cout << name << " moved to (" << x << ", " << y << ")\n";
    }

    void equipWeapon(Weapon* w) {
        if (weapon)
            std::cout << name << " replaced weapon\n";
        else
            std::cout << name << " picked up weapon\n";
        weapon = w;
    }

    void attack() {
        if (!weapon) {
            std::cout << name << " has no weapon!\n";
            return;
        }
        weapon->use();
    }

    void addPassive(PassiveItem* item, int slot) {
        if (slot < 0 || slot >= 3) return;
        passives[slot] = item;
        item->apply(strength, intelligence, agility);
    }

    void input(int index) {
        std::cout << "\n--- Player " << index + 1 << " ---\n";
        std::cout << "Name: ";
        std::cin >> name;

        std::cout << "Health: ";
        while (!(std::cin >> health)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::cout << "Class: ";
        std::cin >> character_class;

        std::cout << "Position (x y): ";
        std::cin >> x >> y;

        std::cout << "Speed: ";
        std::cin >> speed;

        int n;
        std::cout << "Army entries: ";
        std::cin >> n;

        for (int i = 0; i < n; i++) {
            int type, count;
            std::cin >> type >> count;
            army[type] += count;
        }
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

    Weapon sword("sword.png", 30, 2);
    PassiveItem ring("ring.png", 2, 3, 1);

    characters[0].equipWeapon(&sword);
    characters[0].addPassive(&ring, 0);

    characters[0].attack();
    characters[0].attack();
    sword.tick();
    sword.tick();
    characters[0].attack();

    processMeteor(characters);
    getPlayersState(characters);

    return 0;
}