#include <gtest/gtest.h>
#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <iostream>


TEST(ImGUI, Window)
{
    sf::RenderWindow window(sf::VideoMode(1024, 600), "Nero Game Engine 2");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::Texture texture;
    texture.loadFromFile("form_9.png");

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    sf::Vector2f canvasWindowSize  = sf::Vector2f(0.f, 0.);
    sf::Vector2f canvasWindowPosition  = sf::Vector2f(0.f, 0.);


    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(sprite.getPosition() - sf::Vector2f(-10.f, 0.f));
    sprite.setScale(1.f, -1.f);

    sf::RenderTexture sceneTexture;
    sceneTexture.create(800.f, 500.f, sf::ContextSettings());

    sceneTexture.clear(sf::Color::Black);
    sceneTexture.draw(sprite);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Right)
                {
                    sprite.move(sf::Vector2f(2.f, 0.f));
                }

                if(event.key.code == sf::Keyboard::Left)
                {
                    sprite.move(sf::Vector2f(-2.f, 0.f));
                }

                if(event.key.code == sf::Keyboard::Up)
                {
                    sprite.move(sf::Vector2f(0.f, 2.f));
                }

                if(event.key.code == sf::Keyboard::Down)
                {
                    sprite.move(sf::Vector2f(0.f, -2.f));
                }

                sceneTexture.clear(sf::Color::Black);
                sceneTexture.draw(sprite);
            }
        }

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::ShowDemoWindow();

        ImGui::Begin("   Scene   ");
            canvasWindowSize = ImGui::GetWindowSize();
            canvasWindowPosition = ImGui::GetWindowSize();

            //std::cout << " Size : " << canvasWindowSize.x << " -  " << canvasWindowSize.y << " " << std::endl;
            //std::cout << " Position : " << canvasWindowPosition.x << " -  " << canvasWindowPosition.y << " " << std::endl;

            ImGui::Image(sceneTexture.getTexture());

        ImGui::End();

        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        window.clear();
        //window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}
