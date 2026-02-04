#include "NpcStates/Conditions.h"
#include "Bot.h"

#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace NpcAi
{
    float Conditions::Distance(const NpcContext& context)
    {
        sf::Vector2f d = context.BotPosition - context.playerPosition;
        return std::sqrt(d.x * d.x + d.y * d.y);
    }

    bool Conditions::IsSeeingPlayer(const NpcContext& context)
    {
        return Distance(context) <= context.detectionRadius;
    }

    bool Conditions::CanEnterAttack(const NpcContext& ctx)
    {
        if (!ctx.bot || !ctx.bot->canAttack())
            return false;

        return Distance(ctx) <= ctx.attackRange;
    }

    bool Conditions::AlwaysTrue(const NpcContext&)
    {
        return true;
    }
}
