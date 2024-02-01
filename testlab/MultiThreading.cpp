#include <gtest/gtest.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <future>
void renderingThread(sf::RenderWindow* window)
{
    // the rendering loop
    while(window->isOpen())
    {
        // draw...

        // end the current frame
        window->display();
    }
}
void handleEvent(sf::RenderWindow* window)
{
    sf::Event event;
    while(window->pollEvent(event))
    {
    }
}

void update(const sf::Time& timeStep)
{
}

TEST(MultiThreading, Rendering)
{
    // create the window (remember: it's safer to create it in the main thread due to OS
    // limitations)
    sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL");

    // deactivate its OpenGL context
    window.setActive(false);

    // launch the rendering thread
    std::thread thread(&renderingThread, &window);

    sf::Clock   clock;
    auto        timeSinceLastUpdate = sf::Time::Zero;
    auto        TIME_PER_FRAME      = sf::seconds(1.f / 60.f);

    // the event/logic/whatever loop
    while(window.isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate  += elapsedTime;

        // Update as many times as neccessary
        while(timeSinceLastUpdate > TIME_PER_FRAME)
        {

            timeSinceLastUpdate -= TIME_PER_FRAME;

            // 1... handle user inputs
            handleEvent(&window);

            // 2... update the game
            update(TIME_PER_FRAME);
        }
    }
}
