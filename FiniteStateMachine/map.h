#pragma once
#include <SFML/Graphics.hpp>

class Map
{
public:
    Map();
    void render(sf::RenderWindow& window);
    sf::FloatRect getInnerBounds() const;

private:
    sf::RectangleShape top, bottom, left, right;
};