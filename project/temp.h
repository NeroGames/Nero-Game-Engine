#ifndef TEMP_H
#define TEMP_H

/*#include "library/imgui/imgui.h"
#include "library/imgui/imgui-SFML.h"
#include <libssh2/libssh2.h>
#include <git2/git2.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include <iostream>

int main()
{
    git_libgit2_init();

    git_repository *repo = nullptr;
    const char *url = "http://github.com/sk-landry/nero-game-engine.git";
    const char *path = "boo";
    //int error = git_clone(&repo, url, path, nullptr);

    //git_clone_options clone_options;
    //clone_options.checkout_opts.checkout_strategy = ;

    /*if (error < 0) {
      const git_error *e = giterr_last();
      printf("Error %d/%d: %s\n", error, e->klass, e->message);
      exit(error);
    }*/

    /*sf::RenderWindow window(sf::VideoMode(1024, 600), "Nero Game Engine 2");
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

    //Test Libssh2
    LIBSSH2_SESSION * session;
    /* Create a session instance */
    /*session = libssh2_session_init();

    if(!session)
    {
        std::cout << "Could not initialize SSH session ! " << std::endl;
    }
    else
    {
        std::cout << " SSH  session initialize correctly ! " << std::endl;
    }

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
    git_libgit2_shutdown();
}*/


#endif // TEMP_H
