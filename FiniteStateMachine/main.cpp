#include "StateMachine/NPC.h"
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "MainMenu.h"
int main()
{
    while (true)
    {
        MainMenu menu;

        if (!menu.run())
            break;

        Game game;

        bool returnToMenu = game.run();

        if (!returnToMenu)
            break;
    }

    return 0;
}

