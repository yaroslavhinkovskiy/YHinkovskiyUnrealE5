// ProjectC++Yaroslav Hinkovskiy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <limits>

typedef std::map<int, int> Army;

class Character {
public:
    std::string name;
    std::string character_class;
    float health;
    int x, y;
    int speed;
    Army army;
    bool took_damage;

    Character() : health(0), x(0), y(0), speed(1), took_damage(false) {}

    void Move(int dx, int dy) {
        x += dx * speed;
        y += dy * speed;
        std::cout << "Character " << name << " moved to (" << x << ", " << y << ")\n";
    }

    void input(int index) {
        std::cout << "\n--- Setting data for player " << (index + 1) << " ---\n";
        std::cout << "Enter nickname: ";
        std::cin >> name;

        std::cout << "Enter health: ";
        while (!(std::cin >> health)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid. Enter health: ";
        }

        std::cout << "Enter class: ";
        std::cin >> character_class;

        std::cout << "Enter position (x y): ";
        std::cin >> x >> y;

        std::cout << "Enter speed: ";
        std::cin >> speed;

        int n_units;
        std::cout << "Enter number of unit entries: ";
        std::cin >> n_units;

        for (int j = 0; j < n_units; j++) {
            int unit_type, count;
            std::cout << "  Entry " << j + 1 << " (Type and Count): ";
            while (!(std::cin >> unit_type >> count)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "  Error! Enter two numbers: ";
            }
            army[unit_type] += count;
        }
    }
};

void processMeteor(std::vector<Character>& characters) {
    int mx, my;
    float m_damage, m_power;

    std::cout << "\n--- Meteor Event ---\n";
    std::cout << "Enter meteor coordinates (x y): ";
    std::cin >> mx >> my;
    std::cout << "Enter meteor damage and power: ";
    std::cin >> m_damage >> m_power;

    float affect_radius = 3.0f * m_power;

    for (size_t i = 0; i < characters.size(); ++i) {
        float dx = (float)characters[i].x - mx;
        float dy = (float)characters[i].y - my;
        float distance = (float)std::sqrt(dx * dx + dy * dy);

        if (distance <= affect_radius) {
            characters[i].health -= m_damage;
            characters[i].took_damage = true;
        }
    }
}

void getPlayersState(const std::vector<Character>& characters) {
    std::cout << "\n--- Final Results ---" << std::endl;
    bool anyone_dead = false;

    for (size_t i = 0; i < characters.size(); ++i) {
        const Character& c = characters[i];
        std::cout << "Name: " << c.name << " [" << c.character_class << "]" << std::endl;
        std::cout << "Position: (" << c.x << ", " << c.y << ")" << std::endl;

        float display_hp = (c.health > 0) ? c.health : 0;
        if (c.health <= 0) {
            std::cout << "Status: DEAD (0 hp)" << std::endl;
            anyone_dead = true;
        }
        else {
            std::cout << "Health: " << display_hp << " hp" << (c.took_damage ? " (WOUNDED)" : "") << std::endl;
        }

        if (!c.army.empty()) {
            int min_t = c.army.begin()->first;
            int max_t = c.army.begin()->first;
            
            for (Army::const_iterator it = c.army.begin(); it != c.army.end(); ++it) {
                int type = it->first;
                int count = it->second;
                if (count < c.army.at(min_t)) min_t = type;
                if (count > c.army.at(max_t)) max_t = type;
            }
            std::cout << "Army - Max: Type " << max_t << ", Min: Type " << min_t << std::endl;
        }
        std::cout << "---------------------------" << std::endl;
    }

    if (anyone_dead) {
        std::cout << "DEAD LIST: ";
        for (const auto& c : characters) {
            if (c.health <= 0) std::cout << c.name << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    int n;
    std::cout << "Enter count of players: ";
    if (!(std::cin >> n) || n <= 0) return 0;

    std::vector<Character> characters(n);
    for (int i = 0; i < n; ++i) {
        characters[i].input(i);
    }

    int m;
    std::cout << "\nEnter number of movement commands: ";
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        int id, moveX, moveY;
        std::cout << "Command (Character_ID x y): ";
        std::cin >> id >> moveX >> moveY;

        if (id >= 0 && id < (int)characters.size()) {
            characters[id].Move(moveX, moveY);
        }
        else {
            std::cout << "Invalid Character ID!\n";
        }
    }

    processMeteor(characters);
    getPlayersState(characters);

    std::cout << "\nPress Enter to exit...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    return 0;
}