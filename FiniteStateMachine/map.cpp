#include "map.h"

Map::Map()
{
    const float thickness = 20.f;
    const float width = 700.f;
    const float height = 500.f;

    top.setSize({ width, thickness });
    bottom.setSize({ width, thickness });
    left.setSize({ thickness, height });
    right.setSize({ thickness, height });

    top.setPosition({ 50.f, 50.f });
    bottom.setPosition({ 50.f, height + 30.f });
    left.setPosition({ 50.f, 50.f });
    right.setPosition({ width + 30.f, 50.f });

    top.setFillColor(sf::Color::White);
    bottom.setFillColor(sf::Color::White);
    left.setFillColor(sf::Color::White);
    right.setFillColor(sf::Color::White);
}

void Map::render(sf::RenderWindow& window)
{
    window.draw(top);
    window.draw(bottom);
    window.draw(left);
    window.draw(right);
}

sf::FloatRect Map::getInnerBounds() const
{
    float leftBound = left.getPosition().x + left.getSize().x;
    float topBound = top.getPosition().y + top.getSize().y;
    float rightBound = right.getPosition().x;
    float bottomBound = bottom.getPosition().y;

    return sf::FloatRect(
        { leftBound, topBound },
        { rightBound - leftBound, bottomBound - topBound }
    );
}
