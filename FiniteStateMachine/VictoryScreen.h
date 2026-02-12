#pragma once
#include <SFML/Graphics.hpp>

class VictoryScreen
{
public:
    VictoryScreen();
    bool run(); // true = retour menu, false = quitter

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
