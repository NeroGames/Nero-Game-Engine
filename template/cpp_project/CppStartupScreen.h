////////////////////////////////////////////////////////////
// Project :ProjectName:
// Copyright (c) :Date: :ProjectLead:
////////////////////////////////////////////////////////////
#ifndef :HeaderGard:_H
#define :HeaderGard:_H

#include <Nero/engine/StartupScreen.h>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace :NameSpace:
{
    class :StartupScreenClass: : public nero::StartupScreen
    {
        public:
                                    :StartupScreenClass:();
            void                    init();
            void                    handleEvent(sf::Event& event);
            void                    update(const sf::Time& timeStep);
            void                    render();
            const sf::Color         getBackgroundColor()    const;
            const float             getMinTime()            const;

        private:
            sf::Texture             m_LogoTexture;
            sf::Sprite              m_LogoSprite;
    };

}

#endif // :HeaderGard:_H
