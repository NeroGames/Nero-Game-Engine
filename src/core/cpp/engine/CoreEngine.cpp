////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/engine/CoreEngine.h>
// Easy profiler
#include <easy/profiler.h>
////////////////////////////////////////////////////////////
namespace nero
{
    CoreEngine::CoreEngine(bool createWindow)
        : m_ElapsedTime()
        , m_FrameCount(0)
        , m_FramePerSecond(0.f)
        , m_TimePerFrame(0.f)
    {
        // create render window
        if(createWindow)
        {
            createEngineWindow();
        }
    }

    void CoreEngine::createEngineWindow()
    {
        m_RenderWindow.create(
            sf::VideoMode(EngineConstant::ENGINE_WINDOW_WIDTH, EngineConstant::ENGINE_WINDOW_HEIGHT),
            EngineConstant::ENGINE_WINDOW_TITLE,
            sf::Style::Default);
    }

    CoreEngine::~CoreEngine()
    {
        // Empty
    }

    void CoreEngine::run()
    {
        // Start a clock
        sf::Clock clock;
        sf::Time  timeSinceLastUpdate = sf::Time::Zero;

        while(m_RenderWindow.isOpen())
        {
            EASY_BLOCK("CoreEngine::GameLoop")

            // Accumulate the time elapsed at each loop
            sf::Time elapsedTime = clock.restart();
            timeSinceLastUpdate  += elapsedTime;

            // Update as many times as neccessary
            while(timeSinceLastUpdate > EngineConstant::TIME_PER_FRAME)
            {
                EASY_BLOCK("CoreEngine::Event-And-Update")

                // retrieve 1/60 second in the accumulated time
                timeSinceLastUpdate -= EngineConstant::TIME_PER_FRAME;

                // 1... handle user inputs
                EASY_BLOCK("CoreEngine::HandleEvent")
                handleEvent();
                EASY_END_BLOCK

                // 2... update the game
                EASY_BLOCK("CoreEngine::Update")
                update(EngineConstant::TIME_PER_FRAME);
                EASY_END_BLOCK

                EASY_END_BLOCK
            }

            // 3... render the game
            EASY_BLOCK("CoreEngine::Render")
            render();
            EASY_END_BLOCK

            // 4... Compute Frame rate
            EASY_BLOCK("CoreEngine::ComputeFrameRate")
            computeFrameRate(elapsedTime);
            EASY_END_BLOCK

            EASY_END_BLOCK
        }
    }

    void CoreEngine::computeFrameRate(sf::Time timeStep)
    {
        // Accumulate data for on 1 second
        m_ElapsedTime += timeStep;
        m_FrameCount  += 1;

        // Then compute the frame rate
        if(m_ElapsedTime >= sf::seconds(1.0f))
        {
            m_FramePerSecond = m_FrameCount;
            m_TimePerFrame   = m_ElapsedTime.asSeconds() / m_FrameCount;

            m_ElapsedTime    -= sf::seconds(1.0f);
            m_FrameCount     = 0;
        }
    }

    std::string CoreEngine::getWindowTitle() const
    {
        return m_WindowTitle;
    }

    void CoreEngine::setWindowTitle(const std::string& title)
    {
        m_WindowTitle = title;

        m_RenderWindow.setTitle(m_WindowTitle);
    }

    unsigned int CoreEngine::getWindowWidth() const
    {
        return m_RenderWindow.getSize().x;
    }

    unsigned int CoreEngine::getWindowHeight() const
    {
        return m_RenderWindow.getSize().y;
    }

    sf::Vector2f CoreEngine::getWindowSize() const
    {
        return sf::Vector2f(m_RenderWindow.getSize().x, m_RenderWindow.getSize().y);
    }

    sf::Vector2f CoreEngine::getWindowPosition() const
    {
        return sf::Vector2f(m_RenderWindow.getPosition().x, m_RenderWindow.getPosition().y);
    }

    float CoreEngine::getFrameRate() const
    {
        return m_FramePerSecond;
    }

    float CoreEngine::getFrameTime() const
    {
        return m_TimePerFrame;
    }

    void CoreEngine::quitEngine()
    {
        m_RenderWindow.close();
    }

    void CoreEngine::setWindowIcon(const std::string& icon)
    {
        if(m_WindowIcon.loadFromFile(icon))
        {
            m_RenderWindow.setIcon(m_WindowIcon.getSize().x,
                                   m_WindowIcon.getSize().y,
                                   m_WindowIcon.getPixelsPtr());
        }
    }
} // namespace nero
