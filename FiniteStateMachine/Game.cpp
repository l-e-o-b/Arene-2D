#include <iostream>
#include "Game.h"
Game::Game()
    : window(sf::VideoMode{ sf::Vector2u{800, 600} }, "Mini Arene 2D")
    , aggressiveBot({ 200.f, 300.f }, BotType::Aggressive)
    , zoneBot({ 600.f, 300.f }, BotType::ZoneGuard)
{
    window.setFramerateLimit(60);

    aggressiveBot.Init();
    zoneBot.Init();
}


void Game::run()
{
    while (window.isOpen())
    {
        processEvents();

        float dt = clock.restart().asSeconds();
        update(dt);

        render();
        if (zoneBot.gethp() > 0 &&
            player.isAttacking() &&
            zoneBot.canBeHit() &&
            zoneBot.checkHit(player.getAtkCircle()))
        {
            zoneBot.sethp(zoneBot.gethp() - player.getdmg());
            std::cout << "dealt " << player.getdmg() << "dmg" << std::endl;
            zoneBot.setHit();
        }
        if (!player.isAttacking()) {
            zoneBot.resetHit();
            
        }
        if (aggressiveBot.gethp() > 0 &&
            player.isAttacking() &&
            aggressiveBot.canBeHit() &&
            aggressiveBot.checkHit(player.getAtkCircle()))
        {
            aggressiveBot.sethp(aggressiveBot.gethp() - player.getdmg());
            std::cout << "dealt " << player.getdmg() << "dmg" << std::endl;
            aggressiveBot.setHit();
        }
        if (!player.isAttacking()) {
            aggressiveBot.resetHit();

        }
    }
}

void Game::processEvents()
{
    while (const auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
    }
}


void Game::update(float dt)
{
    player.update(dt);
    player.clampToWindow(window.getSize());

    aggressiveBot.getContext().playerPosition = player.getPosition();
    zoneBot.getContext().playerPosition = player.getPosition();

    aggressiveBot.Update(dt);
    zoneBot.Update(dt);
}

void Game::render()
{
    window.clear(sf::Color(30, 30, 30));

    player.render(window);
    if (aggressiveBot.gethp() > 0)
        aggressiveBot.Render(window);
    if (zoneBot.gethp() > 0)
        zoneBot.Render(window);
    window.display();
}

