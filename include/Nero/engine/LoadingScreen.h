////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/engine/StartupScreen.h>
//SFML
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
//STD
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class LoadingScreen : public StartupScreen
    {
        public:
            typedef std::unique_ptr<LoadingScreen> Ptr;

        public:
                                    LoadingScreen();

            void                    init();
            void                    handleEvent(sf::Event& event);
            void                    update(const sf::Time& timeStep);
            void                    render();

            const sf::Color         getBackgroundColor()    const;
            const float             getMinTime()            const;

        private:
            sf::Texture             m_LogoTexture;
            sf::Sprite              m_LogoSprite;
            sf::Texture             m_CopyrightsTexture;
            sf::Sprite              m_CopyrightsSprite;

            sf::RectangleShape      m_LoadingRectRight;
            sf::RectangleShape      m_LoadingRectLeft;
            sf::CircleShape         m_LoadingBall;
            int                     m_Direction;
            float                   m_MaxRight;
            float                   m_MaxLeft;
    };
}
#endif // LOADINGSCREEN_H
