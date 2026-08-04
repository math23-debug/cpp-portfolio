#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <cstdlib>
#include <cctype>
#include <limits>
#include <vector>

int main()
{
    float speedX = 0, speedY = 0;
    float friction = 0.9998f;

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "My First Window", sf::Style::Titlebar | sf::Style::Close);

    sf::Clock clock;

    sf::CircleShape circle(50.f);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition({ 300.f,200.f });

    bool launched = false;
    bool wasPressed = false; 
    bool relaunch = true;

    sf::Vector2f startPos = circle.getPosition();
    startPos.x += circle.getRadius();
    startPos.y += circle.getRadius();

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();
        sf::Vector2f circPos = circle.getPosition();
        sf::Vector2u windSize = window.getSize();

        sf::VertexArray line(sf::PrimitiveType::Lines, 2);

        if (!launched)
        {
            relaunch = false;

            bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (isPressed)
            {
                line[0].position = startPos;
                line[0].color = sf::Color::White;
                line[1].position = mousePos;
                line[1].color = sf::Color::White;
            }
            if(wasPressed && !isPressed)
            {
                sf::Vector2f dragVector = startPos - mousePos;
                speedX = dragVector.x * 3;
                speedY = dragVector.y * 3;
                launched = true;
            }
            wasPressed = isPressed;
        }
        else
        {
            if (circPos.x < 0)
            {
                circle.setPosition({ 0.f, circPos.y });
                speedX *= -1;
            }
            if (circPos.x + (circle.getRadius() * 2) > windSize.x)
            {
                circle.setPosition({ windSize.x - (circle.getRadius() * 2), circPos.y });
                speedX *= -1;
            }
            if (circPos.y < 0)
            {
                circle.setPosition({ circPos.x, 0.f });
                speedY *= -1;
            }
            if (circPos.y + (circle.getRadius() * 2) > windSize.y)
            {
                circle.setPosition({ circPos.x, windSize.y - (circle.getRadius() * 2) });
                speedY *= -1;
            }

            circle.move({ speedX * deltaTime, speedY * deltaTime });

            if (std::abs(speedX) < 15.f && std::abs(speedY) < 15.f)
            {
                launched = false;
                wasPressed = false;
                relaunch = true;
                startPos = circle.getPosition();
                startPos.x += circle.getRadius();
                startPos.y += circle.getRadius();
            }
            if (std::abs(speedX) < 225.f && std::abs(speedY) < 225.f) friction = 0.9995f;
            else friction = 0.9998;

            speedX *= friction;
            speedY *= friction;


        }

        window.clear();
        if (!launched && !relaunch) window.draw(line);
        window.draw(circle);
        window.display();
    }
}