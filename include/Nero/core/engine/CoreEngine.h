////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef COREENGINE_H
#define COREENGINE_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/utility/LogUtil.h>


#include<Nero/core/engine/EngineConstant.h>
//SFML
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
//JSON
#include <json/json.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    class CoreEngine
    {
        public:
                                CoreEngine();
            virtual            ~CoreEngine();

            virtual void        run();

        protected:
            //game loop
            virtual void        handleEvent()                       = 0;
            virtual void        update(const sf::Time& timeStep)    = 0;
            virtual void        render()                            = 0;
            //quit engine
            virtual void        quitEngine();
            //utility
                //get
            std::string         getWindowTitle()    const;
            unsigned int        getWindowWidth();
            unsigned int        getWindowHeight();
            float               getFrameRate()      const;
            float               getFrameTime()      const;
                //set
            void                setWindowTitle(const std::string& title);


        private:
            void                buildDirectory();
            void                loadLoggingSetting();
            void                loadEngineSetting();
            void                createEngineWindow();
            void                computeFrameRate(sf::Time timeStep);

        protected:
            //window
            sf::RenderWindow    m_Window;           /*!< The engine window */
            std::string         m_WindowTitle;      /*!< The engine window title */
            unsigned int        m_WindowWidth;      /*!< The engine window width */
            unsigned int        m_WindowHeight;     /*!< the engine window height */
            //framerate
            sf::Time			m_ElapsedTime;      /*!< Temporary variable to store the amount of elapsed time*/
            int 	            m_FrameCount;       /*!< Temporary variable to store the amount of frame*/
            float               m_FramePerSecond;   /*!< Number of frame per second */
            float               m_TimePerFrame;     /*!< Duration of one frame */
            //setting
            nlohmann::json      m_Setting;
    };
}
#endif // COREENGINE_H
