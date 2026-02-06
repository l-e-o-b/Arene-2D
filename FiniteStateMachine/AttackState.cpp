#include "AttackState.h"
#include <iostream>

void AttackState::Enter(NpcContext ctx)
{
    if (ctx.bot && ctx.bot->canAttack())
    {
        std::cout << "ATTACK !" << std::endl;
        ctx.bot->startAttackCooldown();
        ctx.bot->setAnimation("Assets/VampAttack.png");
        ctx.bot->startAttackAnim();
        ctx.bot->Attacked();
        if (ctx.bot == nullptr)
            return;

        sf::Vector2f direction = ctx.playerPosition - ctx.BotPosition;
        ctx.bot->setDirection(direction);
    }
}



void AttackState::Execute(NpcContext ctx)
{
}
