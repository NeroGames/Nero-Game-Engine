#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>
#include <ltbl/LightSystem.hpp>
#include <iostream>

TEST(Lighting, LightSystem)
{
    // Create an SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Let There Be Light 2", sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    sf::View          view = window.getDefaultView();

    // Create the LightSystem
    ltbl::LightSystem ls(true);
    ls.create({-1000.f, -1000.f, 2000.f, 2000.f}, window.getSize());

    // Load light texture
    sf::Texture pointLightTexture;
    pointLightTexture.loadFromFile("testdata/pointLightTexture.png");
    pointLightTexture.setSmooth(true);
    sf::Texture spookyLightTexture;
    spookyLightTexture.loadFromFile("testdata/spookyLightTexture.png");
    spookyLightTexture.setSmooth(true);
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("testdata/background.png");
    sf::Texture backgroundTextureNormals;
    backgroundTextureNormals.loadFromFile("testdata/background_NORMALS.png");
    sf::Texture headTexture;
    headTexture.loadFromFile("testdata/head.png");
    sf::Texture headTextureNormals;
    headTextureNormals.loadFromFile("testdata/head_NORMALS.png");

    // Add a sun light
    ltbl::LightDirectionEmission* sun = ls.createLightDirectionEmission();
    sun->setColor(sf::Color(255, 255, 255, 50));

    // Add a light point
    ltbl::LightPointEmission* mlight = ls.createLightPointEmission();
    mlight->setOrigin(
        sf::Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));
    mlight->setTexture(pointLightTexture);
    mlight->setScale(3.f, 3.f);
    mlight->setColor(sf::Color::White);

    // Create a shape
    std::vector<sf::RectangleShape> shapes;
    sf::RectangleShape              blocker;
    blocker.setSize({200.f, 50.f});
    blocker.setPosition({500.f, 300.f});
    blocker.setFillColor(sf::Color::Red);
    shapes.push_back(blocker);

    // Create a light shape with the same shape
    ls.createLightShape(blocker);

    ltbl::Sprite background;
    background.setTexture(backgroundTexture);
    background.setNormalsTexture(backgroundTextureNormals);
    ls.addSprite(background);

    ltbl::Sprite head;
    head.setTexture(headTexture);
    head.setNormalsTexture(headTextureNormals);
    head.setPosition(300.f, 200.f);
    ls.addSprite(head);

    sf::Sprite boo1;
    boo1.setTexture(headTextureNormals);
    boo1.setPosition(300.f, 400.f);

    // Main loop
    float        angle = 0.f;
    sf::Vector2f beginRight, endRight;
    sf::Clock    clock;
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
            // Toggle the mouse light when middle click
            if(event.type == sf::Event::MouseButtonPressed &&
               event.mouseButton.button == sf::Mouse::Middle)
            {
                mlight->toggleTurnedOn();
            }
            if(event.type == sf::Event::MouseButtonPressed &&
               event.mouseButton.button == sf::Mouse::Right)
            {
                beginRight =
                    window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y}, view);
            }
            if(event.type == sf::Event::MouseButtonReleased &&
               event.mouseButton.button == sf::Mouse::Right)
            {
                endRight =
                    window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y}, view);

                sf::RectangleShape blocker;
                blocker.setSize(endRight - beginRight);
                blocker.setPosition(beginRight);
                blocker.setFillColor(sf::Color::Red);
                shapes.push_back(blocker);

                // Add a new light shape on right click
                ls.createLightShape(blocker);
            }
            // Add a point light when left click
            if(event.type == sf::Event::MouseButtonPressed &&
               event.mouseButton.button == sf::Mouse::Left)
            {
                auto light = ls.createLightPointEmission();
                light->setOrigin(sf::Vector2f(pointLightTexture.getSize().x * 0.5f,
                                              pointLightTexture.getSize().y * 0.5f));
                light->setTexture(pointLightTexture);
                light->setScale(6.f, 6.f);
                light->setColor(sf::Color::White);
                light->setPosition(
                    window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y}, view));
            }
        }

        sf::Time dt = clock.restart();

        // Rotate the sun light
        angle       += 10.f * dt.asSeconds();
        sun->setCastAngle(angle);

        // Move the mouse light
        mlight->setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window), view));

        // Move the view
        sf::Vector2f mvt;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            mvt.y--;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            mvt.y++;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            mvt.x--;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            mvt.x++;
        view.move(mvt * dt.asSeconds() * 200.f);

        // Render
        window.clear(sf::Color::White);
        window.setView(view);
        background.render(window);
        for(std::size_t i = 0; i < shapes.size(); i++)
        {
            window.draw(shapes[i]);
        }

        head.render(window);
        // head.renderNormals(window);

        window.draw(boo1);

        sf::CircleShape circle1;
        circle1.setRadius(100.f);
        circle1.setOrigin(100.f, 100.f);
        circle1.setPosition(200.f, 500.f);
        circle1.setFillColor(sf::Color::Yellow);

        window.draw(circle1);

        // Render the lights
        ls.render(window, 1.f);

        window.display();
    }
}
