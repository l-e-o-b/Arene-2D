#include <iostream>
#include <cmath>
#include <algorithm>
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
        player_enemy(aggressiveBot);
    }
}

void Game::player_enemy(Bot& bot)
{
    if (bot.gethp() > 0 &&
        player.isAttacking() &&
        bot.canBeHit() &&
        bot.checkHit(player.getAtkCircle()))
    {
        bot.sethp(bot.gethp() - player.getdmg());
        std::cout << "dealt " << player.getdmg() << " dmg\n";
        bot.setHit();
    }

    if (!player.isAttacking())
        bot.resetHit();
}

/* ========================================================= */
/* ================= RECT ↔ RECT COLLISION ================= */
/* ========================================================= */

void resolveRectCollision(
    sf::RectangleShape& mover,
    const sf::RectangleShape& obstacle
)
{
    sf::FloatRect a = mover.getGlobalBounds();
    sf::FloatRect b = obstacle.getGlobalBounds();

    if (!a.findIntersection(b))
        return;

    sf::Vector2f diff = mover.getPosition() - obstacle.getPosition();

    float overlapX = (a.size.x / 2.f + b.size.x / 2.f) - std::abs(diff.x);
    float overlapY = (a.size.y / 2.f + b.size.y / 2.f) - std::abs(diff.y);

    if (overlapX < overlapY)
    {
        mover.move({ (diff.x < 0.f ? -overlapX : overlapX), 0.f });
    }
    else
    {
        mover.move({ 0.f, (diff.y < 0.f ? -overlapY : overlapY) });
    }
}


/* ========================================================= */

void Game::processEvents()
{
    while (const auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void Game::update(float dt)
{
    auto bounds = map.getInnerBounds();

    player.update(dt);

    sf::Vector2f velocity = player.getVelocity();
    sf::RectangleShape& playerHitbox = player.getHitbox();

    // ===== AXE X =====
    playerHitbox.move({ velocity.x * dt, 0.f });

    for (const auto& barrel : map.getBarrels())
    {
        if (playerHitbox.getGlobalBounds()
            .findIntersection(barrel.getHitbox().getGlobalBounds()))
        {
            playerHitbox.move({ -velocity.x * dt, 0.f });
            break;
        }
    }

    // ===== AXE Y =====
    playerHitbox.move({ 0.f, velocity.y * dt });

    for (const auto& barrel : map.getBarrels())
    {
        if (playerHitbox.getGlobalBounds()
            .findIntersection(barrel.getHitbox().getGlobalBounds()))
        {
            playerHitbox.move({ 0.f, -velocity.y * dt });
            break;
        }
    }

    player.syncSpriteToHitbox();
    player.clampToMap(bounds);
}



void Game::render()
{
    window.clear(sf::Color(30, 30, 30));

    map.render(window);
    player.render(window);

    if (aggressiveBot.gethp() > 0)
        aggressiveBot.Render(window);

    if (zoneBot.gethp() > 0)
        zoneBot.Render(window);

    window.display();
}
