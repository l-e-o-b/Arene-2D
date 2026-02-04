#pragma once
#include <SFML/Graphics.hpp>
#include "StateMachine/StateMachine.h"

#include "NpcStates/ChaseState.h"
#include "NpcStates/Conditions.h"
#include "NpcStates/NpcContext.h"
#include "NpcStates/PatrolState.h"
#include "AttackState.h"
#include "IdleState.h"

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

private:
    // --- Partie JEU ---
    sf::RectangleShape shape;
    BotType type;

    // --- Partie IA ---
    FSM::StateMachine<NpcContext> fsm;
    NpcContext context{};
    float speed = 120.f;

};