#pragma once

#include "NpcContext.h"

namespace NpcAi
{
    class Conditions
    {
    public:
        static float Distance(const NpcContext& context);
        static bool IsSeeingPlayer(const NpcContext& context);
        static bool CanEnterAttack(const NpcContext& context);
        static bool AlwaysTrue(const NpcContext&);
    };
}
