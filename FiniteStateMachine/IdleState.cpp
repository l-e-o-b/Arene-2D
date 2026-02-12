#include "IdleState.h"
#include "Bot.h"

void IdleState::Enter(NpcContext ctx)
{
    ctx.bot->setAnimation("Assets/VampIdle.png");
    if (ctx.bot == nullptr)
        return;

    sf::Vector2f direction = ctx.playerPosition - ctx.BotPosition;
    ctx.bot->setDirection(direction);
    ctx.bot->setVelocity({0.f, 0.f});
}

void IdleState::Execute(NpcContext ctx)
{
    if (ctx.player->isAttacking() &&
        ctx.bot->isInsideCone(*ctx.player))
    {
        ctx.bot->markHit(ctx.player->getdmg());
    }
}
