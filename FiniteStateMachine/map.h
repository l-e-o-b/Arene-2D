#pragma once
#include <SFML/Graphics.hpp>

class Map
{
public:
    Map();
    void render(sf::RenderWindow& window);
    sf::FloatRect getInnerBounds() const;
    const std::vector<sf::CircleShape>& getObstacleColliders() const;

private:
    sf::RectangleShape top, bottom, left, right;
    std::vector<sf::CircleShape> obstacleColliders;
    sf::Texture obstacleTexture;
    sf::Sprite obstacleSprite;
    sf::Texture obstacleTexture2;
    sf::Sprite obstacleSprite2;
    sf::Texture obstacleTexture3;
    sf::Sprite obstacleSprite3;

};