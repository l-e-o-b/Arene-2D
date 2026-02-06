#include "NpcStates/ChaseState.h"
#include <cmath>
#include <iostream>
#include "Bot.h"

namespace NpcAi
{
    void ChaseState::Enter(NpcContext _context)
    {
        std::cout << "Enter Chase State" << std::endl;
        _context.bot->setAnimation("Assets/VampChase.png");

    }

    void ChaseState::Execute(NpcContext _context)
    {
        if (_context.bot == nullptr)
            return;

        sf::Vector2f direction = _context.playerPosition - _context.BotPosition;

        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0.f)
            direction /= length;

        _context.bot->setDirection(direction);
        _context.bot->move(direction, 1.f / 60.f);
    }

    void ChaseState::Exit(NpcContext)
    {
        std::cout << "Exit Chase State" << std::endl;
    }
}
