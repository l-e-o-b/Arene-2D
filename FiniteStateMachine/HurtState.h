#pragma once
#include "StateMachine/State.h"
#include "NpcStates/NpcContext.h"

namespace NpcAi
{
    class HurtState : public FSM::State<NpcContext>
    {
    public:
        void Enter(NpcContext _context) override;
        void Execute(NpcContext _context) override;
        void Exit(NpcContext _context) override;
    };
}
