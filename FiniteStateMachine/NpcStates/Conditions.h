#pragma once

#include "NpcContext.h"
#include <cmath>
#include <SFML/System/Vector2.hpp>


namespace NpcAi
{
    class Conditions
    {
    public:
        // Distance entre le bot et le joueur
        static float Distance(const NpcContext& context)
        {
            sf::Vector2f d = context.BotPosition - context.playerPosition;
            return std::sqrt(d.x * d.x + d.y * d.y);
        }

        // Le bot "voit" le joueur (zone de détection)
        static bool IsSeeingPlayer(const NpcContext& context)
        {
            return Distance(context) <= context.detectionRadius;
        }

        // Le bot peut attaquer en mêlée
        static bool CanMeleeAttack(const NpcContext& context)
        {
            return Distance(context) <= context.attackRange;
        }

        static bool AlwaysTrue(const NpcContext&)
        {
            return true;
        }

    };
}
