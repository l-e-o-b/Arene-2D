#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>


class Player
{
public:
    Player();

    void update(float dt);
    void render(sf::RenderWindow& window);
    const sf::CircleShape& getAtkCircle() const;
    bool isAttacking() const;
    int gethp();
    int getdmg();
    void sethp(int new_hp);
    void clampToMap(const sf::FloatRect& bounds);
    const sf::Vector2f& getPosition() const;
    void movement(float dt);
    void following_circle(float dt);
    void Attack();
private:
    sf::RectangleShape shape;
    sf::CircleShape atkCircle;

    float speed;
    float atk_speed;
    sf::Time atkAcc;
    bool atk_state;
    sf::Time atkDuration;
    int hp;
    int dmg;
};
