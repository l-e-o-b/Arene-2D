#include "VictoryScreen.h"
#include <iostream>

VictoryScreen::VictoryScreen()
    : window(sf::VideoMode({ 800, 600 }), "Victory"),
    title(font),
    menuButton(font),
    quitButton(font)
{
}

bool VictoryScreen::run()
{
    if (!font.openFromFile("Assets/arial.ttf"))
    {
        std::cout << "Erreur chargement font\n";
        return false;
    }

    setup();

    while (window.isOpen())
    {
        bool goMenu = false;
        bool quit = false;

        processEvents(goMenu, quit);

        if (goMenu)
        {
            window.close();
            return true;
        }

        if (quit)
        {
            window.close();
            return false;
        }

        render();
    }

    return false;
}

void VictoryScreen::setup()
{
    title.setString("VICTOIRE !");
    title.setCharacterSize(70);
    title.setFillColor(sf::Color::Green);
    title.setPosition({ 250.f, 150.f });

    menuButton.setString("MENU");
    menuButton.setCharacterSize(40);
    menuButton.setFillColor(sf::Color::White);
    menuButton.setPosition({ 350.f, 350.f });

    quitButton.setString("QUIT");
    quitButton.setCharacterSize(40);
    quitButton.setFillColor(sf::Color::White);
    quitButton.setPosition({ 350.f, 420.f });
}

void VictoryScreen::processEvents(bool& goMenu, bool& quit)
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto* mouse =
            event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mousePos =
                    window.mapPixelToCoords(mouse->position);

                if (menuButton.getGlobalBounds().contains(mousePos))
                    goMenu = true;

                if (quitButton.getGlobalBounds().contains(mousePos))
                    quit = true;
            }
        }
    }
}

void VictoryScreen::render()
{
    animationTime += 0.016f; // ~60fps approximation
    float progress = std::min(animationTime / animationDuration, 1.f);

    window.clear(sf::Color(30, 30, 30));

    sf::Vector2f mousePos =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // --- MENU ---
    if (menuButton.getGlobalBounds().contains(mousePos))
    {
        menuButton.setFillColor(sf::Color::Yellow);
        menuButton.setScale({ 1.1f, 1.1f });
    }
    else
    {
        menuButton.setFillColor(sf::Color::White);
        menuButton.setScale({ 1.f, 1.f });
    }

    // --- QUIT ---
    if (quitButton.getGlobalBounds().contains(mousePos))
    {
        quitButton.setFillColor(sf::Color::Yellow);
        quitButton.setScale({ 1.1f, 1.1f });
    }
    else
    {
        quitButton.setFillColor(sf::Color::White);
        quitButton.setScale({ 1.f, 1.f });
    }
    // Fade (alpha)
    std::uint8_t alpha = static_cast<std::uint8_t>(255 * progress);
    title.setFillColor(sf::Color(0, 255, 0, alpha)); // Vert avec alpha

    // Zoom progressif
    float scale = 0.8f + (0.2f * progress); // de 0.8 → 1.0
    title.setScale({ scale, scale });

    window.draw(title);
    window.draw(menuButton);
    window.draw(quitButton);

    window.display();
}
