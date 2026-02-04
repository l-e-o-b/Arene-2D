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
    const sf::CircleShape& getAtkCircle() const;
    bool isAttacking() const;
    int gethp();
    int getdmg();
    void sethp(int new_hp);
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
