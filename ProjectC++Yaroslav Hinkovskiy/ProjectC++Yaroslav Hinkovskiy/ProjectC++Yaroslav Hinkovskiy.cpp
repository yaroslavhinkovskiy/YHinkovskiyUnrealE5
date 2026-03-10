// ProjectC++Yaroslav Hinkovskiy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <limits>
#include <memory>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef std::map<int, int> Army;


struct Vector3 {
    double x, y, z;

    Vector3 operator-(const Vector3& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }

    double length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 normalized() const {
        double len = length();
        return (len > 0) ? Vector3{ x / len, y / len, z / len } : Vector3{ 0, 0, 0 };
    }
};

struct TargetData {
    Vector3 direction;
    double angle;
    std::string side;
};

class Character;

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onNotify(const Character& character, const std::string& event) = 0;
};

class Item {
public:
    std::string icon;
    virtual ~Item() = default;
    virtual void applyStats(int& str, int& intel, int& agi) {}
    virtual int speedBonus() const { return 0; }
};

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

class SpeedItem : public Item {
    int bonus;
public:
    SpeedItem(std::string ic, int b) : bonus(b) {
        icon = ic;
    }
    int speedBonus() const override { return bonus; }
};

class Weapon {
public:
    std::string icon;
    int damage;
    int cooldown;
    int currentCooldown;

    Weapon(std::string ic, int dmg, int cd)
        : icon(ic), damage(dmg), cooldown(cd), currentCooldown(0) {
    }

    bool ready() const { return currentCooldown == 0; }

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
        if (currentCooldown > 0) currentCooldown--;
    }
};

class Character {
public:
    std::string name;
    std::string character_class;
    float health;
    int x, y; 
    Vector3 forwardVector; 

    int baseSpeed;
    int currentSpeed;
    Army army;
    bool took_damage;
    int strength, intelligence, agility;

    std::shared_ptr<Weapon> weapon;
    std::vector<std::shared_ptr<Item>> inventory;

private:
    std::vector<IObserver*> observers;

public:
    Character()
        : health(0), x(0), y(0), forwardVector({ 1, 0, 0 }),
        baseSpeed(1), currentSpeed(1),
        took_damage(false),
        strength(10), intelligence(10), agility(10) {
    }

    void addObserver(IObserver* obs) { observers.push_back(obs); }

    void notify(const std::string& event) {
        for (auto* obs : observers) obs->onNotify(*this, event);
    }

    void takeDamage(float dmg) {
        health -= dmg;
        took_damage = true;
        notify("damage");
    }

    
    TargetData analyzeTarget(const Character& target) {
        Vector3 myPos = { (double)x, (double)y, 0 };
        Vector3 targetPos = { (double)target.x, (double)target.y, 0 };

        Vector3 dirToTarget = (targetPos - myPos).normalized();
        Vector3 fwdNorm = forwardVector.normalized();

        
        double dot = fwdNorm.x * dirToTarget.x + fwdNorm.y * dirToTarget.y + fwdNorm.z * dirToTarget.z;
        if (dot > 1.0) dot = 1.0;
        if (dot < -1.0) dot = -1.0;
        double angleDeg = std::acos(dot) * (180.0 / M_PI);

        
        double crossZ = fwdNorm.x * dirToTarget.y - fwdNorm.y * dirToTarget.x;
        std::string side = (crossZ >= 0) ? "Right" : "Left";

        return { dirToTarget, angleDeg, side };
    }

    void addItemToInventory(std::shared_ptr<Item> item) {
        inventory.push_back(item);
        item->applyStats(strength, intelligence, agility);
        recalculateSpeed();
    }

    void recalculateSpeed() {
        currentSpeed = baseSpeed;
        for (const auto& item : inventory) currentSpeed += item->speedBonus();
    }

    void input(int index) {
        std::cout << "\n--- Player " << index + 1 << " ---\n";
        std::cout << "Name: "; std::cin >> name;
        std::cout << "Health: "; std::cin >> health;
        std::cout << "Position (x y): "; std::cin >> x >> y;
        std::cout << "Base speed: "; std::cin >> baseSpeed;
        currentSpeed = baseSpeed;
        
        forwardVector = { 1, 0, 0 };
    }
};

class DamageLogger : public IObserver {
public:
    void onNotify(const Character& character, const std::string& event) override {
        if (event == "damage") {
            std::cout << "[LOGGER] " << character.name << " wounded! HP: " << character.health << "\n";
        }
    }
};

int main() {
    int n;
    std::cout << "Players count: "; std::cin >> n;
    if (n < 2) n = 2; 

    std::vector<Character> characters(n);
    for (int i = 0; i < n; i++) characters[i].input(i);

    
    TargetData info = characters[0].analyzeTarget(characters[1]);

    std::cout << "\n--- TARGET ANALYSIS (Player 1 to Player 2) ---\n";
    std::cout << "Direction: (" << info.direction.x << ", " << info.direction.y << ")\n";
    std::cout << "Angle: " << info.angle << " degrees\n";
    std::cout << "Side: " << info.side << "\n";
    std::cout << "------------------------------------------\n";

    return 0;
}