#pragma once
#include "StateMachine/State.h"
#include "NpcStates/NpcContext.h"
#include "SFML/System.hpp"

namespace NpcAi
{
    class HurtState : public FSM::State<NpcContext>
    {
    public:
        void Enter(NpcContext ctx) override;
        void Execute(NpcContext ctx) override;
        void Exit(NpcContext ctx) override;

    private:
        sf::Time timer = sf::Time::Zero;
        const sf::Time duration = sf::seconds(0.5f);
    };
}
