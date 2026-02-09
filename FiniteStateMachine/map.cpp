#include "map.h"
#include <iostream>

Map::Map()
{
    float radius = 21.f;
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


    if (!barrelTexture.loadFromFile("Assets/Barrel1.png"))
    {
        std::cerr << "Erreur chargement barrel\n";
    }
    barrelTexture.setSmooth(false);


    barrels.emplace_back(sf::Vector2f{ 420.f, 320.f }, barrelTexture);
    barrels.emplace_back(sf::Vector2f{ 320.f, 170.f }, barrelTexture);
    barrels.emplace_back(sf::Vector2f{ 120.f, 220.f }, barrelTexture);

}

void Map::render(sf::RenderWindow& window)
{
    window.draw(top);
    window.draw(bottom);
    window.draw(left);
    window.draw(right);

    for (const auto& barrel : barrels)
        barrel.render(window);
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


const std::vector<Barrel>& Map::getBarrels() const
{
    return barrels;
}
