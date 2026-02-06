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

void resolveRectCircleCollision(
    sf::RectangleShape& rect,
    const sf::CircleShape& circle)
{
    sf::Vector2f rectPos = rect.getPosition();
    sf::Vector2f half = rect.getSize() / 2.f;

    sf::Vector2f circlePos = circle.getPosition();
    float radius = circle.getRadius();

    float closestX = std::clamp(
        circlePos.x,
        rectPos.x - half.x,
        rectPos.x + half.x
    );

    float closestY = std::clamp(
        circlePos.y,
        rectPos.y - half.y,
        rectPos.y + half.y
    );

    sf::Vector2f closestPoint{ closestX, closestY };
    sf::Vector2f diff = rectPos - closestPoint;

    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if (dist == 0.f || dist >= radius)
        return;

    sf::Vector2f normal = diff / dist;
    float penetration = radius - dist;

    rect.move(normal * penetration);
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

    for (const auto& barrel : map.getObstacleColliders())
    {
        resolveRectCircleCollision(player.getHitbox(),barrel
        );
    }

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

    for (const auto& barrel : map.getObstacleColliders())
    {
        sf::CircleShape debug = barrel;
        debug.setFillColor(sf::Color::Transparent);
        debug.setOutlineThickness(2.f);
        debug.setOutlineColor(sf::Color::Green);
        window.draw(debug);
    }
    window.display();
}

