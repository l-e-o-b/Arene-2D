#include "Bot.h"

Bot::Bot(const sf::Vector2f& startPos)
{
    shape.setSize({ 40.f, 40.f });
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(shape.getSize() / 2.f);
    shape.setPosition(startPos);

    // FSM pas encore initialisée
}

void Bot::Init()
{
    PatrolState* patrolState = fsm.CreateState<PatrolState>();
    ChaseState* chaseState = fsm.CreateState<ChaseState>();

    patrolState->AddTransition(Conditions::IsSeeingPlayer, chaseState);

    chaseState->AddTransition(
        [](const NpcContext& context)
        {
            return !Conditions::IsSeeingPlayer(context);
        },
        patrolState
    );

    fsm.Init(patrolState, context);
}

void Bot::Update(float)
{
    // FSM désactivée pour l’instant
    // fsm.Update(context);
}

void Bot::Render(sf::RenderWindow& window)
{
    window.draw(shape);
}
