#pragma once
#include <SFML/Graphics.hpp>
#include "StateMachine/StateMachine.h"

#include "NpcStates/ChaseState.h"
#include "NpcStates/Conditions.h"
#include "NpcStates/NpcContext.h"
#include "NpcStates/PatrolState.h"

using namespace NpcAi;

class Bot
{
public:
    Bot(const sf::Vector2f& startPos);

    void Init();                 // FSM (plus tard)
    void Update(float dt);
    void Render(sf::RenderWindow& window);

    void resetHit();
    bool canBeHit() const;
    void setHit();
    bool checkHit(const sf::CircleShape& atkCircle) const;
    int gethp();
    int getdmg();
    void sethp(int new_hp);
private:
    // --- Partie JEU ---
    sf::RectangleShape shape;
    int hp;
    int dmg;
    bool damaged;

    // --- Partie IA ---
    FSM::StateMachine<NpcContext> fsm;
    NpcContext context{};
};
