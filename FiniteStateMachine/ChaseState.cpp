#include "NpcStates/ChaseState.h"
#include <cmath>
#include <iostream>
#include "Bot.h"

namespace NpcAi
{
    void ChaseState::Enter(NpcContext ctx)
    {
        ctx.bot->setAnimation("Assets/VampChase.png");

    }

    void ChaseState::Execute(NpcContext ctx)
    {
        if (ctx.bot == nullptr)
            return;

        sf::Vector2f direction = ctx.playerPosition - ctx.BotPosition;

        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0.f)
            direction /= length;

        ctx.bot->setDirection(direction);
        ctx.bot->setVelocity(direction);

        if (ctx.player->isAttacking() &&
            ctx.bot->isInsideCone(*ctx.player))
        {
            ctx.bot->markHit(ctx.player->getdmg());
        }
    }

    void ChaseState::Exit(NpcContext)
    {
    }
}
