#include "HurtState.h"
#include "Bot.h"

namespace NpcAi
{
    void HurtState::Enter(NpcContext ctx)
    {
        timer = sf::Time::Zero;
        ctx.bot->setHurt(false);
        ctx.bot->setAnimation("Assets/VampHurt.png");
        if (ctx.bot == nullptr)
            return;

        sf::Vector2f direction = ctx.playerPosition - ctx.BotPosition;
        ctx.bot->setDirection(direction);
    }

    void HurtState::Execute(NpcContext ctx)
    {
        timer += sf::seconds(ctx.dt);

        if (timer >= sf::seconds(1.f))
        {
            int dmg = ctx.bot->getPendingDamage();

            if (dmg > 0)
            {
                ctx.bot->sethp(ctx.bot->gethp() - dmg);
                ctx.bot->clearPendingDamage();
                ctx.bot->setHurt(true);
            }
        }
    }

    void HurtState::Exit(NpcContext ctx)
    {
        if (ctx.bot->gethp() > 1) {
            ctx.bot->setAnimation("Assets/VampIdle.png");
            ctx.bot->resetHit();
            ctx.bot->wasJustHit();
        }
            ctx.bot->setAnimation("Assets/VampDead.png");
    }
}
