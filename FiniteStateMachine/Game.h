#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Bot.h"
#include "map.h"
#include "GameOverScreen.h"
#include "VictoryScreen.h"
class Game
{

public:
    Game();
    void run();
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
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

};
