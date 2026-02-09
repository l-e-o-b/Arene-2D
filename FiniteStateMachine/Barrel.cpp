#include "Barrel.h"

// 🔹 CONSTRUCTEUR
Barrel::Barrel(const sf::Vector2f& position, sf::Texture& texture) :
    sprite(sprite)
{
    // Sprite
    sprite.setTexture(texture);
    sprite.setTextureRect({ {0, 0}, {256, 256} });
    sprite.setOrigin({ 24.f, 24.f });
    sprite.setScale({ 0.2f, 0.2f });
    sprite.setPosition(position);

    // Hitbox RECTANGULAIRE
    hitbox.setSize({ 40.f, 40.f });
    hitbox.setOrigin(hitbox.getSize() / 2.f);
    hitbox.setPosition(position);
    hitbox.setFillColor(sf::Color(150, 80, 50)); // visible
}

void Barrel::render(sf::RenderWindow& window) const
{
    window.draw(sprite);
    window.draw(hitbox);
}

sf::RectangleShape& Barrel::getHitbox()
{
    return hitbox;
}

const sf::RectangleShape& Barrel::getHitbox() const
{
    return hitbox;
}
