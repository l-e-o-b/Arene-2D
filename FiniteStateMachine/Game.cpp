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
    aggressiveBot.Render(window);
    zoneBot.Render(window);

    window.display();
}

