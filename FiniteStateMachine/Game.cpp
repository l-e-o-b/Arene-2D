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
        bot.markHit();
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
        showGameOverWindow();
    }

    if (aggressiveBot.gethp() <= 0 &&
        zoneBot.gethp() <= 0)
    {
        window.close();           // ferme le jeu principal
        showVictoryWindow();      
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

void Game::showVictoryWindow()
{
    sf::RenderWindow victoryWindow(
        sf::VideoMode({ 600, 400 }),
        "Victoire"
    );

    sf::Font font;
    if (!font.openFromFile("Assets/arial.ttf"))
    {
        std::cout << "Erreur chargement font\n";
        return;
    }

    sf::Text text(font);
    text.setString("VICTOIRE !");
    text.setCharacterSize(60);
    text.setFillColor(sf::Color::Green);
    text.setStyle(sf::Text::Bold);
    text.setPosition({ 150.f, 150.f });

    while (victoryWindow.isOpen())
    {
        while (const auto event = victoryWindow.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                victoryWindow.close();
        }

        victoryWindow.clear(sf::Color::Black);
        victoryWindow.draw(text);
        victoryWindow.display();
    }
}

void Game::showGameOverWindow()
{
    sf::RenderWindow gameOverWindow(
        sf::VideoMode({ 600, 400 }),
        "Game Over"
    );

    sf::Font font;
    if (!font.openFromFile("Assets/arial.ttf"))
    {
        std::cout << "Erreur chargement font\n";
        return;
    }

    sf::Text text(font);
    text.setString("GAME OVER");
    text.setCharacterSize(60);
    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold);

    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
    text.setPosition({ 300.f, 200.f });

    while (gameOverWindow.isOpen())
    {
        while (const auto event = gameOverWindow.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                gameOverWindow.close();
        }

        gameOverWindow.clear(sf::Color::Black);
        gameOverWindow.draw(text);
        gameOverWindow.display();
    }
}







