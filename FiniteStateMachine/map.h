#pragma once
#include <SFML/Graphics.hpp>

class Map
{
public:
    Map();
    void render(sf::RenderWindow& window);
    sf::FloatRect getInnerBounds() const;
    std::vector<sf::RectangleShape>& getObstacleColliders();

private:
    sf::RectangleShape top, bottom, left, right;
    std::vector<sf::RectangleShape> obstacleColliders;
    sf::Texture obstacleTexture;
    sf::Sprite obstacleSprite;
    sf::Texture obstacleTexture2;
    sf::Sprite obstacleSprite2;
    sf::Texture obstacleTexture3;
    sf::Sprite obstacleSprite3;
    sf::RectangleShape b1;
    sf::RectangleShape b2;
    sf::RectangleShape b3;
};