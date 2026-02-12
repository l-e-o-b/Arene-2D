#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "Map.h"
#include <cmath>
#include <iostream>


enum class Direction
{
    Down,
    Left,
    Right,
    Up
};

class Player
{
public:
    Player();

    void update(float dt);
    void render(sf::RenderWindow& window);
    bool isAttacking() const;
    int gethp();
    int getdmg();
    void sethp(int new_hp);
    void clampToMap(const sf::FloatRect& bounds);
    const sf::Vector2f& getPosition() const;
    void movement(float dt);
    void Attack(float dt);
    float getCollisionRadius() const;
    sf::RectangleShape& getHitbox();
    sf::Time getAtkAcc();
    float getAtkSpeed();

    void takeDamage(int dmg);
    bool canBeHit() const;
    void setHit();
    void resetHit();
    bool isAlive() const;
    sf::Vector2f getForwardVector() const;
    float getAttackRange() const;
    float getAttackAngle() const;
private:
    sf::RectangleShape shape;
    float attackRange = 50.f;
    float attackAngle = 45.f;
    int directionRow = 0;
    float speed;
    float atk_speed;
    sf::Time atkAcc;
    bool atk_state;
    sf::Time atkDuration;
    int hp;
    int dmg;

    sf::Texture texture;
    sf::Sprite sprite;

    // --- Animation Idle ---
    Direction currentDirection;

    int idleFrameCount = 6;
    int currentIdleFrame = 0;

    float idleFrameTime = 0.2f;   // temps entre frames
    float idleTimer = 0.f;

    sf::Vector2i frameSize = { 48, 48 };

    // --- Animation Walk ---
    int walkFrameCount = 6;        // adapte au nombre réel de frames
    int currentWalkFrame = 0;

    float walkFrameTime = 0.2f;   // plus rapide que l'idle
    float walkTimer = 0.f;

    int idleStartRow = 0;
    int walkStartRow = 4;
    int attackStartRow = 16;

    // --- Animation Attack ---
    int attackFrameCount = 6;      // adapte à ta spritesheet
    int currentAttackFrame = 0;

    float attackFrameTime = 0.12f;
    float attackTimer = 0.f;

    bool damaged = false;
    sf::Time invincibilityTimer = sf::Time::Zero;
    sf::Time invincibilityDuration = sf::seconds(0.5f);

    bool alive = true;
    sf::RectangleShape hpBarBackground;
    sf::RectangleShape hpBarFill;

    int maxHp = 20;   // valeur max fixe


};
