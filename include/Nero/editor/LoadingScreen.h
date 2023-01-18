////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/engine/StartupScreen.h>
// SFML
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
// STD
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class LoadingScreen : public StartupScreen {
      public:
        typedef std::unique_ptr<LoadingScreen> Ptr;

      public:
        LoadingScreen();

        void            init();
        void            handleEvent(sf::Event& event);
        void            update(const sf::Time& timeStep);
        void            render();

        const sf::Color getCanvasColor() const;
        float           getDuration() const;

      private:
        sf::Sprite         m_LogoSprite;
        sf::Sprite         m_CopyrightsSprite;

        sf::RectangleShape m_LoadingRectRight;
        sf::RectangleShape m_LoadingRectLeft;
        sf::CircleShape    m_LoadingBall;
        int                m_Direction;
        float              m_MaxRight;
        float              m_MaxLeft;
    };
} // namespace nero
#endif // LOADINGSCREEN_H
