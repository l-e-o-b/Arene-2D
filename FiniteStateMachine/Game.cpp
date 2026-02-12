#include <iostream>
#include "Game.h"
Game::Game()
    : window(sf::VideoMode({ 800, 600 }), "Mini Arene 2D")
    , aggressiveBot({ 200.f, 300.f }, BotType::Aggressive)
    , zoneBot({ 600.f, 300.f }, BotType::ZoneGuard)
    , backgroundTexture()
    , backgroundSprite(backgroundTexture)
{
    if (!backgroundTexture.loadFromFile("Assets/background.png"))
    {
        std::cout << "Erreur chargement background\n";
    }

    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setPosition({ 0.f, 0.f });

    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    backgroundSprite.setScale(sf::Vector2f(scaleX, scaleY));

    window.setFramerateLimit(60);

    aggressiveBot.Init();
    zoneBot.Init();
}



bool Game::run()
{
    while (window.isOpen())
    {
        processEvents();

        float dt = clock.restart().asSeconds();
        update(dt);

        render();

        // --- GAME OVER ---
        if (!player.isAlive())
        {
            window.close();
            GameOverScreen screen;
            return screen.run();  // true = menu, false = quit
        }

        // --- VICTORY ---
        if (aggressiveBot.gethp() <= 0 &&
            zoneBot.gethp() <= 0)
        {
            window.close();
            VictoryScreen screen;
            return screen.run();  // true = menu, false = quit
        }
    }


void resolveRectCircleCollision(
    sf::RectangleShape& rect,
    const sf::CircleShape& circle)
{
    sf::Vector2f rectPos = rect.getPosition();
    sf::Vector2f half = rect.getSize() / 2.f;

    sf::Vector2f circlePos = circle.getPosition();
    float radius = circle.getRadius() + 2.f; // padding


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

void resolveRectCollision(
    sf::RectangleShape& a,
    sf::RectangleShape& b
)
{
    sf::FloatRect ra = a.getGlobalBounds();
    sf::FloatRect rb = b.getGlobalBounds();

    if (!ra.findIntersection(rb))
        return;

    sf::Vector2f diff = a.getPosition() - b.getPosition();
    float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    if (length == 0.f)
        return;

    sf::Vector2f normal = diff / length;

    float overlapX = (ra.size.x / 2.f + rb.size.x / 2.f) - std::abs(diff.x);
    float overlapY = (ra.size.y / 2.f + rb.size.y / 2.f) - std::abs(diff.y);

    if (overlapX <= 0.f || overlapY <= 0.f)
        return;

    if (overlapX < overlapY)
    {
        a.move({ normal.x * overlapX * 0.5f, 0.f });
        b.move({ -normal.x * overlapX * 0.5f, 0.f });
    }
    else
    {
        a.move({ 0.f, normal.y * overlapY * 0.5f });
        b.move({ 0.f, -normal.y * overlapY * 0.5f });
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

void Game::botupdate(Bot& bot, sf::Rect<float> bounds, float dt)
{
    if (bot.gethp() <= 0)
        return;

    bot.getContext().player = &player;
    bot.getContext().playerPosition = player.getPosition();

    bot.Update(dt);
    bot.clampToMap(bounds);
}



void Game::update(float dt)
{
    auto bounds = map.getInnerBounds();
    if (player.isAlive())
    {
        player.update(dt);
        player.clampToMap(bounds);
    }

    for (const auto& barrel : map.getObstacleColliders())
    {
        resolveRectCircleCollision(player.getHitbox(),barrel
        );
    }
    botupdate(aggressiveBot, bounds, dt);
    botupdate(zoneBot, bounds, dt);

    resolveRectCollision(player.getHitbox(), aggressiveBot.getHitbox());
    resolveRectCollision(player.getHitbox(), zoneBot.getHitbox());
    resolveRectCollision(aggressiveBot.getHitbox(), zoneBot.getHitbox());
}

    static bool lastAtkState = true;

    if (player.isAttacking() && !lastAtkState)
    {
        aggressiveBot.resetHit();
        aggressiveBot.wasJustHit();
        zoneBot.resetHit();
        zoneBot.wasJustHit();
    }

    lastAtkState = player.isAttacking();

    player_enemy(zoneBot);
    player_enemy(aggressiveBot);

    if (!player.isAlive())
    {
        window.close();
        GameOverScreen gameOver;
        bool goMenu = gameOver.run();
        return;
    }

    if (aggressiveBot.gethp() <= 0 &&
        zoneBot.gethp() <= 0)
    {
        window.close();
        VictoryScreen victory;
        bool goMenu = victory.run();
        return;
    }

}

void Game::render()
{
    window.clear();
    backgroundSprite.setColor(sf::Color::Green);


    window.draw(backgroundSprite); 
    player.render(window);
    aggressiveBot.Render(window);
    zoneBot.Render(window);
    map.render(window);

    window.display();
}