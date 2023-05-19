////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/renderer/NoGameFound.h>
/////////////////////////////////////////////////////////////
namespace nero
{
    NoGameFound::NoGameFound(GameScene::Context context)
        : GameScene(context)
    {
    }

    NoGameFound::~NoGameFound()
    {
    }

    void NoGameFound::handleEvent(const sf::Event& event)
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                // TODO quiScene()
                break;

            case sf::Event::MouseButtonPressed:
                // TODO handleMouseButtonInput(event.mouseButton, true);
                break;
            case sf::Event::MouseButtonReleased:
                // TODO handleMouseButtonInput(event.mouseButton, false);
                break;
        }
    }

    void NoGameFound::update(const sf::Time& timeStep)
    {
    }

    void NoGameFound::render()
    {
    }
} // namespace nero
