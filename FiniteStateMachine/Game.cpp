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
        player_enemy(zoneBot);
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

void Game::player_enemy(Bot& bot) {
    if (bot.gethp() > 0 &&
        player.isAttacking() &&
        bot.canBeHit() &&
        bot.checkHit(player.getAtkCircle()))
    {
        bot.sethp(bot.gethp() - player.getdmg());
        std::cout << "dealt " << player.getdmg() << "dmg" << std::endl;
        bot.setHit();
    }
    if (!player.isAttacking()) {
        bot.resetHit();

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
    auto bounds = map.getInnerBounds();
    player.update(dt);
    player.clampToMap(bounds);

    if (aggressiveBot.gethp() > 0){
        aggressiveBot.getContext().playerPosition = player.getPosition();
        aggressiveBot.Update(dt);
        aggressiveBot.clampToMap(bounds);
    }

    if (aggressiveBot.gethp() > 0) {
        zoneBot.getContext().playerPosition = player.getPosition();
        zoneBot.Update(dt);
        zoneBot.clampToMap(bounds);
    }
    
}

void Game::render()
{
    window.clear(sf::Color(30, 30, 30));

    player.render(window);
    if (aggressiveBot.gethp() > 0)
        aggressiveBot.Render(window);
    if (zoneBot.gethp() > 0)
        zoneBot.Render(window);
    map.render(window);
    window.display();
}

