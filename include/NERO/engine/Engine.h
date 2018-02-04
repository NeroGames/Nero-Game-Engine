////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
// All rights reserved
//
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
                                Engine(const sf::String& winTitle, const float& winWidth, const float& winHeight);
            virtual            ~Engine();
            virtual void        run();

        protected: //Method
            virtual void        handleEvents()                         = 0;
            virtual void        update(const sf::Time& elapsedTime)    = 0;
            virtual void        render()                               = 0;

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
