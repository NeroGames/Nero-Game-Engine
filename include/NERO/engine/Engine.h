////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ENGINE_H
#define ENGINE_H
///////////////////////////HEADERS//////////////////////////
//SFML
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    class Engine
    {
        public: //Method
                                Engine(const float& winWidth, const float& winHeight, const sf::String& winTitle);
            virtual            ~Engine();
            virtual void        run();

        protected: //Method
            virtual void        handleEvent()                          = 0;
            virtual void        update(const sf::Time& timeStep)        = 0;
            virtual void        render()                                = 0;

            //Getter
            sf::String          getWinTitle()   const;
            float               getWinWidth()   const;
            float               getWinHeight()  const;

            //Setter
            void                setWinTitle(const sf::String& winTitle);

        protected: //Attribute
            sf::RenderWindow    m_Window;
            sf::String          m_WinTitle;
            float               m_WinWidth;
            float               m_WinHeight;
    };
}

#endif // ENGINE_H
