// ProjectC++Yaroslav Hinkovskiy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <limits>
#include <string>


void setPlayersData(const int& player_count, std::vector<std::string>& name, std::vector<float>& health, std::vector<std::string>& character_class)
{
    std::string player_name;
    float player_health;
    std::string player_class;

    for (int i = 0; i < player_count; i++)
    {
        std::cout << "\nEnter nickname for player " << (i + 1) << ": ";
        std::cin >> player_name;

        std::cout << "Enter player's health for " << player_name << ": ";
        while (!(std::cin >> player_health))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid value. Enter health: ";
        }

        std::cout << "Enter class for " << player_name << " (e.g., magician): ";
        std::cin >> player_class;

        name.push_back(player_name);
        health.push_back(player_health);
        character_class.push_back(player_class);
    }
}


void getPlayersState(std::vector<std::string>& name, std::vector<float>& health, std::vector<std::string>& character_class)
{
    std::cout << "\n--- Current Players State ---" << std::endl;
    for (int i = 0; i < health.size(); i++)
    {
        if (health[i] <= 0) health[i] = 0;

        std::cout << "Name: " << name[i] << " (" << (i + 1) << ")" << std::endl;
        std::cout << "Class: " << character_class[i] << std::endl;

        if (health[i] <= 0)
            std::cout << "Status: DEAD" << std::endl;
        else
            std::cout << "Health: " << health[i] << " hp" << std::endl;

        std::cout << "---------------------------" << std::endl;
    }
}


void setPlayerCount(int& player_count) {
    std::cout << "Enter count of players: ";
    while (!(std::cin >> player_count) || player_count <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid. Enter count: ";
    }
}

void causeDamage(std::vector<float>& health) {
    float damage;
    std::cout << "\nEnter amount of damage to all players (0-9): ";
    while (!(std::cin >> damage) || damage < 0 || damage > 9) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid. Enter 0-9: ";
    }
    for (float& hp : health) hp -= damage;
}

int main()
{
    int player_count;
    std::vector<std::string> name;
    std::vector<float> health;
    std::vector<std::string> character_class; 

    setPlayerCount(player_count);
    
    setPlayersData(player_count, name, health, character_class);

    causeDamage(health);

    getPlayersState(name, health, character_class);

    return 0;
}


