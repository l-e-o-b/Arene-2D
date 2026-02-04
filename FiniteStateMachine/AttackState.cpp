#include "AttackState.h"
#include <iostream>

void AttackState::Enter(NpcContext ctx)
{
    if (ctx.bot && ctx.bot->canAttack())
    {
        std::cout << "ATTACK !" << std::endl;
        ctx.bot->startAttackCooldown();
    }
}



void AttackState::Execute(NpcContext ctx)
{
    // Pour l’instant : simple feedback console
    std::cout << "Bot attaque le joueur !" << std::endl;
}