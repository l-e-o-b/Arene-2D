#pragma once
#include "StateMachine/State.h"
#include "NpcStates/NpcContext.h"
#include "Bot.h"
#include "player.h"

class AttackState : public FSM::State<NpcContext>
{
public:
	void Enter(NpcContext ctx) override;
	void Execute(NpcContext ctx) override;
};
