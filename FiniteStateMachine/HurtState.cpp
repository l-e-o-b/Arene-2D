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

        if (timer >= sf::seconds(0.5f))
        {
            ctx.bot->setHurt(true);
        }

    }

    void HurtState::Exit(NpcContext ctx)
    {
        ctx.bot->setAnimation("Assets/VampIdle.png");
    }
}
