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

    backgroundSprite.setTextureRect(
        sf::IntRect(
            { 0, 0 },
        {
            static_cast<int>(backgroundTexture.getSize().x),
            static_cast<int>(backgroundTexture.getSize().y)
        }
        )
    );



    backgroundSprite.setPosition({ 0.f, 0.f });

    // Adaptation � la fen�tre
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    backgroundSprite.setScale({ scaleX, scaleY });

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

    return false;
}

void resolveBarrelCollision(
    sf::RectangleShape& dynamic,
    const sf::RectangleShape& solid
)
{
    sf::FloatRect ra = dynamic.getGlobalBounds();
    sf::FloatRect rb = solid.getGlobalBounds();

    std::optional<sf::FloatRect> intersection = ra.findIntersection(rb);

    if (!intersection)
        return;

    sf::FloatRect inter = *intersection;

    if (inter.size.x < inter.size.y)
    {
        if (dynamic.getPosition().x < solid.getPosition().x)
            dynamic.move({ -inter.size.x, 0.f });
        else
            dynamic.move({ inter.size.x, 0.f });
    }
    else
    {
        if (dynamic.getPosition().y < solid.getPosition().y)
            dynamic.move({ 0.f, -inter.size.y });
        else
            dynamic.move({ 0.f, inter.size.y });
    }
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
    sf::Vector2f zvelocity = zoneBot.getVelocity();

    zoneBot.move({ zvelocity.x * dt, 0.f });

    for ( auto& barrel : map.getObstacleColliders())
        resolveBarrelCollision(zoneBot.getHitbox(), barrel);

    zoneBot.move({ 0.f, zvelocity.y * dt });

    for ( auto& barrel : map.getObstacleColliders())
        resolveBarrelCollision(zoneBot.getHitbox(), barrel);

    sf::Vector2f avelocity = aggressiveBot.getVelocity();

    aggressiveBot.move({ avelocity.x * dt, 0.f });

    for ( auto& barrel : map.getObstacleColliders())
        resolveBarrelCollision(aggressiveBot.getHitbox(), barrel);

    aggressiveBot.move({ 0.f, avelocity.y * dt });

    for ( auto& barrel : map.getObstacleColliders())
        resolveBarrelCollision(aggressiveBot.getHitbox(), barrel);

    for ( auto& barrel : map.getObstacleColliders())
    {
        resolveBarrelCollision(player.getHitbox(), barrel);
    }
    botupdate(aggressiveBot, bounds, dt);
    botupdate(zoneBot, bounds, dt);

    resolveRectCollision(player.getHitbox(), aggressiveBot.getHitbox());
    resolveRectCollision(player.getHitbox(), zoneBot.getHitbox());
    resolveRectCollision(aggressiveBot.getHitbox(), zoneBot.getHitbox());

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

    window.draw(backgroundSprite); 

    map.render(window);
    player.render(window);

    if (aggressiveBot.gethp() > 0)
        aggressiveBot.Render(window);

    if (zoneBot.gethp() > 0)
        zoneBot.Render(window);

    window.display();
}
