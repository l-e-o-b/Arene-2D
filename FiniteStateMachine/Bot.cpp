#include "Bot.h"

Bot::Bot(const sf::Vector2f& startPos, BotType type)
    : type(type)
{
    shape.setSize({ 40.f, 40.f });
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(shape.getSize() / 2.f);
    shape.setPosition(startPos);
    hp = 20;
    dmg = 1;
    damaged = false;
    // FSM pas encore initialisée

    context.bot = this;
}

void Bot::startAttackCooldown()
{
    attackTimer = attackCooldown;
}

bool Bot::canAttack() const
{
    return attackTimer <= 0.f;
}


void Bot::move(const sf::Vector2f& direction, float dt)
{
    shape.move(direction * speed * dt);
}


const sf::Vector2f& Bot::getPosition() const
{
    return shape.getPosition();
}

NpcContext& Bot::getContext()
{
    return context;
}

BotType Bot::getType() const
{
    return type;
}


void Bot::Init()
{
    auto* idle = fsm.CreateState<IdleState>();
    auto* chase = fsm.CreateState<ChaseState>();
    auto* attack = fsm.CreateState<AttackState>();

    // --- Transitions Idle → Chase ---
    if (type == BotType::Aggressive)
    {
        idle->AddTransition(Conditions::AlwaysTrue, chase);
    }
    else // ZoneGuard
    {
        idle->AddTransition(Conditions::IsSeeingPlayer, chase);

        chase->AddTransition(
            [](const NpcContext& ctx)
            {
                return !Conditions::IsSeeingPlayer(ctx);
            },
            idle
        );
    }

    // --- Transitions Chase → Attack ---
    chase->AddTransition(Conditions::CanEnterAttack, attack);

    // --- Attack → Chase ---
    attack->AddTransition(
        [](const NpcContext&)
        {
            return true; // sortie immédiate
        },
        chase
    );
    fsm.Init(idle, context);
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
void Bot::clampToMap(const sf::FloatRect& bounds)
{
    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f half = shape.getSize() / 2.f;

    pos.x = std::clamp(pos.x, (bounds.position.x + half.x), bounds.position.x + (bounds.size.x - half.x));
    pos.y = std::clamp(pos.y, (bounds.position.y + half.y), bounds.position.y + (bounds.size.y - half.y));

    shape.setPosition(pos);
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
void Bot::Update(float dt)
{
    // Mise à jour du timer d’attaque
    if (attackTimer > 0.f)
        attackTimer -= dt;

    context.BotPosition = shape.getPosition();
    fsm.Update(context);
}

void Bot::Render(sf::RenderWindow& window)
{
    window.draw(shape);
}
