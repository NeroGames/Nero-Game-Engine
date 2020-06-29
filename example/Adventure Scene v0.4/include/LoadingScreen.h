////////////////////////////////////////////////////////////
// Project Adventure Scene
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/engine/StartupScreen.h>
#include <SFML/Graphics/Texture.hpp>
//SFML
#include <SFML/Graphics/Sprite.hpp>
////////////////////////////////////////////////////////////
namespace ng
{
    class LoadingScreen : public nero::StartupScreen
    {
        public:
            typedef std::shared_ptr<LoadingScreen> Ptr;

                                    LoadingScreen();

            void                    init();
            void                    handleEvent(sf::Event& event);
            void                    update(const sf::Time& timeStep);
            void                    render();

            const sf::Color         getBackgroundColor()    const;
            const float             getMinTime()            const;

        private:
            sf::Texture             mTitleTexture;
            sf::Sprite              mTitleSprite;

            sf::Texture             mLoadingTexture;
            sf::Sprite              mLoadingSprite;

            sf::Texture             mLogoTexture;
            sf::Sprite              mLogoSprite;

            float                   mTimeCount;
    };
}
#endif // LOADINGSCREEN_H
