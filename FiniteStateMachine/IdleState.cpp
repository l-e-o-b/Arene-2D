#include "IdleState.h"
#include "Bot.h"

void IdleState::Enter(NpcContext _context)
{
    // État idle : rien à faire pour l’instant
    _context.bot->setAnimation("Assets/VampIdle.png");
    if (_context.bot == nullptr)
        return;

    sf::Vector2f direction = _context.playerPosition - _context.BotPosition;
    _context.bot->setDirection(direction);
}

void IdleState::Execute(NpcContext _context)
{
    if (_context.player->isAttacking() &&
        _context.bot->isInsideCone(*_context.player))
    {
        _context.bot->markHit(_context.player->getdmg());
    }
}
