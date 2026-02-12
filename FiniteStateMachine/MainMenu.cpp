#include "MainMenu.h"
#include <iostream>


MainMenu::MainMenu()
    : window(sf::VideoMode({ 800, 600 }), "Main Menu"),
    title(font),
    playButton(font),
    quitButton(font)
{
}


bool MainMenu::run()
{
    if (!font.openFromFile("Assets/arial.ttf"))
    {
        std::cout << "Erreur chargement font\n";
        return false;
    }

    setup();

    while (window.isOpen())
    {
        bool playSelected = false;
        bool quitSelected = false;

        processEvents(playSelected, quitSelected);

        if (playSelected)
        {
            window.close();
            return true;
        }

        if (quitSelected)
        {
            window.close();
            return false;
        }

        render();
    }

    return false;
}

void MainMenu::setup()
{
    title = sf::Text(font);
    title.setString("Mini Arena 2D");
    title.setCharacterSize(60);
    title.setFillColor(sf::Color::White);
    title.setPosition({ 200.f, 100.f });

    playButton = sf::Text(font);
    playButton.setString("PLAY");
    playButton.setCharacterSize(40);
    playButton.setFillColor(sf::Color::Green);
    playButton.setPosition({ 350.f, 300.f });

    quitButton = sf::Text(font);
    quitButton.setString("QUIT");
    quitButton.setCharacterSize(40);
    quitButton.setFillColor(sf::Color::Red);
    quitButton.setPosition({ 350.f, 380.f });
}

void MainMenu::processEvents(bool& playSelected, bool& quitSelected)
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }

        if (const auto* mousePressed =
            event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mousePressed->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mousePos =
                    window.mapPixelToCoords(mousePressed->position);

                if (playButton.getGlobalBounds().contains(mousePos))
                {
                    playSelected = true;
                }

                if (quitButton.getGlobalBounds().contains(mousePos))
                {
                    quitSelected = true;
                }
            }
        }
    }
}


void MainMenu::render()
{
    window.clear(sf::Color(20, 20, 20));

    // Position actuelle de la souris
    sf::Vector2f mousePos =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (playButton.getGlobalBounds().contains(mousePos))
    {
        playButton.setFillColor(sf::Color::Yellow);
        playButton.setScale({ 1.1f, 1.1f });
    }
    else
    {
        playButton.setFillColor(sf::Color::Green);
        playButton.setScale({ 1.f, 1.f });
    }

    if (quitButton.getGlobalBounds().contains(mousePos))
    {
        quitButton.setFillColor(sf::Color::Yellow);
        quitButton.setScale({ 1.1f, 1.1f });
    }
    else
    {
        quitButton.setFillColor(sf::Color::Green);
        quitButton.setScale({ 1.f, 1.f });
    }


    window.draw(title);
    window.draw(playButton);
    window.draw(quitButton);

    window.display();
}
