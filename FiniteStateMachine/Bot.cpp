#include "Bot.h"

Bot::Bot(const sf::Vector2f& startPos, BotType type)
    : type(type)
{
    shape.setSize({ 40.f, 40.f });
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(shape.getSize() / 2.f);
    shape.setPosition(startPos);

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
