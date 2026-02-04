#include "AttackState.h"
#include <iostream>

void AttackState::Enter(NpcContext ctx)
{
    // plus tard : animation, son, cooldown
}

void AttackState::Execute(NpcContext ctx)
{
    // Pour l’instant : simple feedback console
    std::cout << "Bot attaque le joueur !" << std::endl;
}