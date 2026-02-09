#pragma once
#include <SFML/Graphics.hpp>

class Barrel
{
public:
    Barrel(const sf::Vector2f& position, sf::Texture& texture);

    void render(sf::RenderWindow& window) const;

    sf::RectangleShape& getHitbox();
    const sf::RectangleShape& getHitbox() const;

private:
    sf::Sprite sprite;
    sf::RectangleShape hitbox;
};
