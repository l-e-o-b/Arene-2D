#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>


class Player
{
public:
    Player();

    void update(float dt);
    void render(sf::RenderWindow& window);

    void clampToWindow(const sf::Vector2u& windowSize);
    const sf::Vector2f& getPosition() const;

private:
    sf::RectangleShape shape;
    float speed;
};
