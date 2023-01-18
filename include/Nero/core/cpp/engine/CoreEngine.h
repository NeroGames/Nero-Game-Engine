////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef COREENGINE_H
#define COREENGINE_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/engine/EngineConstant.h>
// SFML
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
// JSON
#include <json/json.hpp>
// Profiler
#include <easy/profiler.h>
#include <easy/arbitrary_value.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class CoreEngine {
      public:
        CoreEngine(bool createWindow = true);
        virtual ~CoreEngine();

        virtual void run();

      protected:
        // game loop
        virtual void handleEvent()                    = 0;
        virtual void update(const sf::Time& timeStep) = 0;
        virtual void render()                         = 0;
        // quit engine+
        virtual void quitEngine();
        // utility
        // get
        std::string  getWindowTitle() const;
        unsigned int getWindowWidth() const;
        unsigned int getWindowHeight() const;
        sf::Vector2f getWindowSize() const;
        sf::Vector2f getWindowPosition() const;
        float        getFrameRate() const;
        float        getFrameTime() const;
        // set
        void         setWindowTitle(const std::string& title);
        void         setWindowIcon(const std::string& icon);

      protected:
        void createEngineWindow();
        void computeFrameRate(sf::Time timeStep);

      protected:
        // window
        sf::RenderWindow m_RenderWindow;
        std::string      m_WindowTitle;
        sf::Image        m_WindowIcon;
        // framerate
        sf::Time         m_ElapsedTime;
        int              m_FrameCount;
        float            m_FramePerSecond;
        float            m_TimePerFrame;
    };
} // namespace nero
#endif // COREENGINE_H
