////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef STARTUPSCREEN_H
#define STARTUPSCREEN_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/resource/ResourceManager.h>
// SFML
#include <SFML/Graphics/RenderWindow.hpp>
// STD
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class StartupScreen {
      public:
        typedef std::unique_ptr<StartupScreen> Ptr;

        StartupScreen();
        virtual ~StartupScreen();

      public: // method to override
        virtual void            init()                           = 0;
        virtual void            handleEvent(sf::Event& event)    = 0;
        virtual void            update(const sf::Time& timeStep) = 0;
        virtual void            render()                         = 0;
        virtual const sf::Color getCanvasColor() const           = 0;
        virtual float           getDuration() const              = 0;

      public:
        void setRenderWindow(sf::RenderWindow* renderWindow);
        void setResourceManager(ResourceManager::Ptr resourceManager);

      private:
        virtual void destroy();

      protected:
        sf::RenderWindow*    m_RenderWindow;
        ResourceManager::Ptr m_ResourceManager;
    };
} // namespace nero
#endif // STARTUPSCREEN_H
