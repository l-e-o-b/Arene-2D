#include "map.h"
#include <iostream>

Map::Map(): obstacleSprite(obstacleTexture), obstacleSprite2(obstacleTexture2) ,obstacleSprite3(obstacleTexture3)
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

    // Barrel 1
    sf::CircleShape b1(radius);
    b1.setOrigin({ radius, radius });
    b1.setPosition({ 420.f, 320.f });
    b1.setFillColor(sf::Color::White);
    obstacleColliders.push_back(b1);

    // Barrel 2
    sf::CircleShape b2(radius);
    b2.setOrigin({ radius, radius });
    b2.setPosition({ 320.f, 170.f });
    b2.setFillColor(sf::Color::White);
    obstacleColliders.push_back(b2);

    // Barrel 3
    sf::CircleShape b3(radius);
    b3.setOrigin({ radius, radius });
    b3.setPosition({ 120.f, 220.f });
    b3.setFillColor(sf::Color::White);
    obstacleColliders.push_back(b3);

    if (!obstacleTexture.loadFromFile("Assets/Barrel1.png"))
    {
        // debug simple
        std::cerr << "Erreur chargement obstacle\n";
    }
    obstacleTexture.setSmooth(false); // pixel art

    obstacleSprite.setTextureRect(
        sf::IntRect({ 0, 0 }, { 256, 256 }) 
    );

    obstacleSprite.setOrigin({ 24.f, 24.f });
    obstacleSprite.setScale({ 0.2f, 0.2f });

    obstacleSprite.setPosition({ 400.f, 300.f });



    if (!obstacleTexture2.loadFromFile("Assets/Barrel1.png"))
    {
        // debug simple
        std::cerr << "Erreur chargement obstacle\n";
    }
    obstacleTexture2.setSmooth(false); // pixel art

    obstacleSprite2.setTextureRect(
        sf::IntRect({ 0, 0 }, { 256, 256 })
    );

    obstacleSprite2.setOrigin({ 24.f, 24.f });
    obstacleSprite2.setScale({ 0.2f, 0.2f });

    obstacleSprite2.setPosition({ 300.f, 150.f });



    if (!obstacleTexture3.loadFromFile("Assets/Barrel1.png"))
    {
        // debug simple
        std::cerr << "Erreur chargement obstacle\n";
    }
    obstacleTexture3.setSmooth(false); // pixel art

    obstacleSprite3.setTextureRect(
        sf::IntRect({ 0, 0 }, { 256, 256 })
    );

    obstacleSprite3.setOrigin({ 24.f, 24.f });
    obstacleSprite3.setScale({ 0.2f, 0.2f });

    obstacleSprite3.setPosition({ 100.f, 200.f });

}

void Map::render(sf::RenderWindow& window)
{
    window.draw(top);
    window.draw(bottom);
    window.draw(left);
    window.draw(right);
    window.draw(obstacleSprite);
    window.draw(obstacleSprite2);
    window.draw(obstacleSprite3);
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


const std::vector<sf::CircleShape>& Map::getObstacleColliders() const
{
    return obstacleColliders;
}
