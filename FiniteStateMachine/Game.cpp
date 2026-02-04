#include "Game.h"

Game::Game()
    : window(sf::VideoMode{ sf::Vector2u{800, 600} }, "Mini Arene 2D")
    , bot(sf::Vector2f{ 600.f, 300.f }, BotType::Aggressive)
{
    window.setFramerateLimit(60);
    bot.Init();
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
    bot.Update(dt);

    bot.getContext().playerPosition = player.getPosition();

}

void Game::render()
{
    window.clear(sf::Color(30, 30, 30));
    player.render(window);
    bot.Render(window);
    window.display();
}
