#pragma once
#include "StateMachine/State.h"
#include "NpcStates/NpcContext.h"

class IdleState : public FSM::State<NpcContext>
{
public:
    void Enter(NpcContext ctx) override;
    void Execute(NpcContext ctx) override;
};
