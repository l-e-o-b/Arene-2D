#include "Player.h"
Player::Player()
{
    shape.setSize({ 40.f, 40.f });
    shape.setFillColor(sf::Color::Cyan);
    shape.setOrigin(shape.getSize() / 2.f);
    shape.setPosition({ 400.f, 300.f });

    atkCircle.setRadius(40.f);
    atkCircle.setOrigin({ 40.f, 40.f });
    atkCircle.setFillColor(sf::Color::Transparent);

    speed = 250.f;
    atk_speed = 1;
    atk_state = false;
    atkDuration = sf::Time::Zero;
    atkAcc = sf::Time::Zero;
    hp = 20;
    dmg = 2;
}


const sf::Vector2f& Player::getPosition() const
{
    return shape.getPosition();
}


void Player::update(float dt)
{
    sf::Vector2f movement{ 0.f, 0.f };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movement.x -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += speed;

    shape.move(movement * dt);

    atkAcc += sf::seconds(dt);
    if (atk_state)
    {
        atkCircle.setPosition(shape.getPosition());

        atkDuration += sf::seconds(dt);
        if (atkDuration >= sf::seconds(0.1f))
        {
            atk_state = false;
            atkDuration = sf::Time::Zero;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
    {
        sf::Time interval = sf::seconds(1.f / atk_speed);

        if (atkAcc >= interval)
        {
            atkCircle.setPosition(shape.getPosition());
            atk_state = true;
            atkAcc -= interval;
        }
    }
}
const sf::CircleShape& Player::getAtkCircle() const
{
    return atkCircle;
}

bool Player::isAttacking() const
{
    return atk_state;
}
int Player::gethp() {
    return hp;
}
int Player::getdmg() {
    return dmg;
}
void Player ::sethp(int new_hp) {
    hp = new_hp;
}
void Player::clampToMap(const sf::FloatRect& bounds)
{
    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f half = shape.getSize() / 2.f;

    pos.x = std::clamp(pos.x, (bounds.position.x + half.x), bounds.position.x + (bounds.size.x - half.x));
    pos.y = std::clamp(pos.y, (bounds.position.y + half.y), bounds.position.y + (bounds.size.y - half.y));

    shape.setPosition(pos);
}

void Player::render(sf::RenderWindow& window)
{
    window.draw(shape);
}
