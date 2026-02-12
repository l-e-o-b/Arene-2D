#pragma once
#include <SFML/Graphics.hpp>
#include "StateMachine/StateMachine.h"

#include "NpcStates/ChaseState.h"
#include "NpcStates/Conditions.h"
#include "NpcStates/NpcContext.h"
#include "HurtState.h"
#include "AttackState.h"
#include "IdleState.h"

#include <iostream>

using namespace NpcAi;

enum class BotType
{
    Aggressive,
    ZoneGuard
};


class Bot
{
public:
    Bot(const sf::Vector2f& startPos, BotType type);

    void Init();                 // FSM (plus tard)
    void Update(float dt);
    void Render(sf::RenderWindow& window);
    const sf::Vector2f& getPosition() const;
    NpcContext& getContext();
    BotType getType() const;
    void move(const sf::Vector2f& direction, float dt);
    bool canAttack() const;
    void startAttackCooldown();

    void resetHit();
    bool canBeHit() const;
    void setHit();
    void clampToMap(const sf::FloatRect& bounds);
    int gethp();
    int getdmg();
    void sethp(int new_hp);
    void setDirection(const sf::Vector2f& dir);
    void setAnimation(const std::string& file);
    bool isAttackFinished() const { return attackAnimFinished; }
    void startAttackAnim();
    void Attacked();
    sf::RectangleShape& getHitbox();
    sf::RectangleShape& getAttackHitbox();

    void markHit();
    void markHit(int damage);
    int getPendingDamage() const;
    void clearPendingDamage();
    bool wasJustHit();
    bool isHurtFinished() const;
    void setHurt(bool hurt);
    bool isInsideCone(const Player& player) const;
private:
    // --- Partie JEU ---
    sf::RectangleShape shape;
    int hp;
    int dmg;
    bool damaged;
    bool attacking;
    bool attackAnimFinished = false;
    int attackFrameCount = 12;    bool immune = false;

    BotType type;

    // --- Partie Animation ---
    sf::Sprite sprite;
    sf::Texture texture;
    int currentFrame;
    int currentRow;
    int framerowcount;
    sf::Time animTimer;
    sf::Time frameDuration;
    sf::Vector2i frameSize{ 64, 64 };
    // --- Partie IA ---
    FSM::StateMachine<NpcContext> fsm;
    NpcContext context{};
    float speed = 120.f;

    float attackCooldown = 1.0f;
    float attackTimer = 0.0f;

    int pendingDamage = 0;
    sf::RectangleShape attackHitbox;
    bool hitThisFrame = false;

    bool hurtFinished = false;

};