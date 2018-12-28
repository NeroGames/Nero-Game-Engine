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
    /*!
        \defgroup engine engine
        \brief main module
    */
    /*!
        \ingroup engine
        \brief Base engine class. Provide the game loop

        The nero::Engine class provide three features
        - The render window
        - The game loop
        - The frame rate computation

        The nero::Engine class is a abstract class. It is mean to be inherited by more specialized class

        \todo Rename nero::Engine to nero::CoreEngine
    */
    class Engine
    {
        public:
            //Method
            /*!
                \brief The engine is created with three parameters. The window width and height, and the window title
                @param windowWidth the engine window width
                @param windowHeight the engine window height
                @param windowTitle the engine window title
            */
            Engine(const unsigned int& windowWidth, const unsigned int& windowHeight, const std::string& windowTitle);
            /*!
                \brief destructor
            */
            virtual            ~Engine();
            /*!
                \brief game loop method

                The game loop perform four actions at each step
                - Handle the game events
                - Update the game
                - Compute the frame rate
                - Render the game

                The default frame rate is 60 frame per second
            */
            virtual void        run();

        protected:
            //Method
            /*!
                \brief handle events method

                This method is empty and means to be override
            */
            virtual void        handleEvent()                       = 0;
             /*!
                \brief update method

                This method is empty and means to be override
            */
            virtual void        update(const sf::Time& timeStep)    = 0;
             /*!
                \brief render method

                This method is empty and means to be override
            */
            virtual void        render()                            = 0;
            //Getter
            /*!
                \brief return the window title
                \return m_WindowTitle
            */
            std::string         getWindowTitle()    const;
             /*!
                \brief return the window width
                \return m_WindowWidth
            */
            unsigned int        getWindowWidth()    const;
             /*!
                \brief return the window height
                \return m_WindowHeight
            */
            unsigned int        getWindowHeight()   const;
            /*!
                \brief return the engine frame rate
                \return m_FramePerSecond
            */
            float               getFrameRate()      const;
            /*!
                \brief return the engine frame duration
                \return m_TimePerFrame
            */
            float               getFrameTime()      const;
            //Setter
            /*!
                \brief update the window title
                \param windowTitle the new window title
            */
            void                setWindowTitle(const std::string& windowTitle);

        private:
            //Method
            /*!
                \brief compute engine frame rate

                The frame rate is computed in a base of 1 second
            */
            void                computeFrameRate(sf::Time timeStep);

        protected:
            //Attribute
            //Window
            sf::RenderWindow    m_Window;           /*!< The render window provided by SFML */
            std::string         m_WindowTitle;      /*!< The engine window title */
            unsigned int        m_WindowWidth;      /*!< The engine window width */
            unsigned int        m_WindowHeight;     /*!< the engine window height */
            //FrameRate
            sf::Time			m_ElapsedTime;      /*!< Temporary variable to store the amount of elapsed time*/
            int 	            m_FrameCount;       /*!< Temporary variable to store the amount of frame*/
            float               m_FramePerSecond;   /*!< Number of frame per second */
            float               m_TimePerFrame;     /*!< Duration of one frame */
    };
}

#endif // ENGINE_H
