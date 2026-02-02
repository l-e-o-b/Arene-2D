#include "Player.h"

Player::Player()
{
    shape.setSize({ 40.f, 40.f });
    shape.setFillColor(sf::Color::Cyan);
    shape.setOrigin(shape.getSize() / 2.f);
    shape.setPosition({ 400.f, 300.f });

    speed = 250.f;
}

void Player::update(float dt)
{
    sf::Vector2f movement{ 0.f, 0.f };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
        movement.y -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
        movement.x -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += speed;

    shape.move(movement * dt);
}

void Player::clampToWindow(const sf::Vector2u& windowSize)
{
    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f half = shape.getSize() / 2.f;

    pos.x = std::clamp(pos.x, half.x, windowSize.x - half.x);
    pos.y = std::clamp(pos.y, half.y, windowSize.y - half.y);

    shape.setPosition(pos);
}

void Player::render(sf::RenderWindow& window)
{
    window.draw(shape);
}
