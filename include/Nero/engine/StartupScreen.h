////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef STARTUPSCREEN_H
#define STARTUPSCREEN_H
///////////////////////////HEADERS//////////////////////////
//SFML
#include <SFML/Graphics/RenderWindow.hpp>
//STD
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class StartupScreen
    {
        public:
            typedef std::shared_ptr<StartupScreen> Ptr;

                                            StartupScreen();
            virtual                        ~StartupScreen();

            virtual void                    init()                                     = 0;
            virtual void                    handleEvent(sf::Event& event)               = 0;
            virtual void                    update(const sf::Time& timeStep)            = 0;
            virtual void                    render()                                    = 0;

            virtual const sf::Color         getBackgroundColor()    const               = 0;
            virtual const float             getMinTime()            const               = 0;

            void                            setRenderWindow(sf::RenderWindow* renderWindow);

        protected:
             sf::RenderWindow*              m_RenderWindow;
    };
}
#endif // STARTUPSCREEN_H
