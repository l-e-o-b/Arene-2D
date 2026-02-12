#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>


class VictoryScreen
{
public:
    VictoryScreen();
    bool run();

private:
    sf::RenderWindow window;
    sf::Font font;

    sf::Text title;
    sf::Text menuButton;
    sf::Text quitButton;

    void setup();
    void processEvents(bool& goMenu, bool& quit);
    void render();
};
