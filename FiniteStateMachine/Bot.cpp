#include "Bot.h"

Bot::Bot(const sf::Vector2f& startPos)
{
    shape.setSize({ 40.f, 40.f });
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(shape.getSize() / 2.f);
    shape.setPosition(startPos);
    hp = 20;
    dmg = 1;
    damaged = false;
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

bool Bot::checkHit(const sf::CircleShape& atkCircle) const
{
    sf::Vector2f cPos = atkCircle.getPosition();
    float radius = atkCircle.getRadius();

    sf::Vector2f rPos = shape.getPosition();
    sf::Vector2f half = shape.getSize() / 2.f;

    float closestX = std::clamp(cPos.x, rPos.x - half.x, rPos.x + half.x);
    float closestY = std::clamp(cPos.y, rPos.y - half.y, rPos.y + half.y);

    float dx = cPos.x - closestX;
    float dy = cPos.y - closestY;

    return (dx * dx + dy * dy) <= (radius * radius);
}
bool Bot::canBeHit() const
{
    return !damaged;
}

void Bot::resetHit()
{
    damaged = false;
}

void Bot::setHit() {
    damaged = true;
}
int Bot::gethp() {
    return hp;
}
int Bot::getdmg() {
    return dmg;
}
void Bot::sethp(int new_hp) {
    hp = new_hp;
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
