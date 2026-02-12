#pragma once
#include <SFML/Graphics.hpp>

class MainMenu
{
public:
    MainMenu();
    bool run();

private:
    sf::RenderWindow window;
    sf::Font font;

    sf::Text title;
    sf::Text playButton;
    sf::Text quitButton;

    void setup();
    void processEvents(bool& playSelected, bool& quitSelected);
    void render();
};
