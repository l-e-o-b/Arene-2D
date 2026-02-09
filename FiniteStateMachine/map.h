#pragma once
#include "Barrel.h"
#include <vector>

class Map
{
public:
    Map();
    void render(sf::RenderWindow& window);
    sf::FloatRect getInnerBounds() const;
    const std::vector<Barrel>& getBarrels() const;

private:
    sf::Texture barrelTexture;
    std::vector<Barrel> barrels;

    sf::RectangleShape top, bottom, left, right;
};

