#include "StateMachine/NPC.h"

int main()
{
    Npc npc;

    npc.Init();

    while (true)
    {
        npc.Update();
    }
}
