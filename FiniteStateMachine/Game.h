#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Bot.h"
#include "map.h"
class Game
{

public:
    Game();
    void run();
    void player_enemy(Bot& bot);
    void botupdate(Bot& bot, sf::Rect<float> bounds, float dt);
private:
    void processEvents();
    void update(float dt);
    void render();
    sf::RenderWindow window;
    sf::Clock clock;

    Player player;

    Bot aggressiveBot;
    Bot zoneBot;
    Map map;
};
