#include "AttackState.h"
#include <iostream>

void AttackState::Enter(NpcContext ctx)
{
    if (ctx.bot && ctx.bot->canAttack())
    {
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
    if (!ctx.player->isAlive())
        return;

    if (!ctx.bot || !ctx.player)
        return;

    if (!ctx.bot->canAttack())
        return;

    // Distance simple (hitbox rect-rect)
    if (ctx.bot->getAttackHitbox().getGlobalBounds()
        .findIntersection(ctx.player->getHitbox().getGlobalBounds()))
    {
        if (ctx.player->canBeHit())
        {
            ctx.player->takeDamage(ctx.bot->getdmg());
            ctx.bot->startAttackCooldown();
        }
    }

}

