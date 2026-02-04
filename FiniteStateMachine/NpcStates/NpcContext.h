#pragma once
#include <SFML/System/Vector2.hpp>

class Npc;

class Bot;

class NpcContext
{
public:
    Bot* bot = nullptr;

    sf::Vector2f BotPosition;
    sf::Vector2f playerPosition;

    float detectionRadius = 200.f;
    float attackRange = 40.f;  

};