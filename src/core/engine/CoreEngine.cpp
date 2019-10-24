////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
///Nero
#include <Nero/core/engine/CoreEngine.h>
#include <Nero/core/utility/FileUtil.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    CoreEngine::CoreEngine():
         m_ElapsedTime()
        ,m_FrameCount(0)
        ,m_FramePerSecond(0.f)
        ,m_TimePerFrame(0.f)
    {
        //build directory
        buildDirectory();

        //load logging setting
        loadLoggingSetting();

        //load engine setting
        loadEngineSetting();

        //create render window
        createEngineWindow();
    }

    void CoreEngine::loadEngineSetting()
    {
        m_Setting = loadSetting(EngineConstant.FILE_SETTING_ENGINE);
    }

    void CoreEngine::loadLoggingSetting()
    {
        el::Configurations setting(getPath({EngineConstant.FOLDER_SETTING, EngineConstant.FILE_SETTING_LOGGING}, StringPool.EXTENSION_TEXT));
        el::Loggers::reconfigureAllLoggers(setting);
    }

    void CoreEngine::createEngineWindow()
    {
        m_WindowTitle  = m_Setting["window_title"].get<std::string>();
        m_WindowWidth  = m_Setting["window_width"].get<unsigned int>();
        m_WindowHeight = m_Setting["window_height"].get<unsigned int>();

		m_Window.create(sf::VideoMode(m_WindowWidth, m_WindowHeight), m_WindowTitle, sf::Style::Default);
    }

    CoreEngine::~CoreEngine()
    {
        //Empty
    }

    void CoreEngine::run()
    {
        //Start a clock
        sf::Clock clock;
        sf::Time timeSinceLastUpdate = sf::Time::Zero;

        while(m_Window.isOpen())
        {
            //Accumulate the time elapsed at each loop
            sf::Time elapsedTime = clock.restart();
            timeSinceLastUpdate += elapsedTime;

            //When the time comes over the value of "TIME_PER_FRAME" do --> 1 --> 2 then do --> 2 --> 3
            //TIME_PER_FRAME is constant with a value of 1/60 second (the game is update 60 time per second)
            while(timeSinceLastUpdate > EngineConstant.TIME_PER_FRAME)
            {
                //retrieve 1/60 second in the accumulated time
                timeSinceLastUpdate -= EngineConstant.TIME_PER_FRAME;

                //1... handle user inputs
                handleEvent();
                //2... update the game
                update(EngineConstant.TIME_PER_FRAME);
            }

            //3... Compute Frame rate
            computeFrameRate(elapsedTime);
            //4... render the game
            render();

            el::Loggers::flushAll();
        }
    }

    void CoreEngine::computeFrameRate(sf::Time timeStep)
    {
        //Accumulate data for on 1 second
        m_ElapsedTime       += timeStep;
        m_FrameCount        += 1;

        //Then compute the frame rate
        if(m_ElapsedTime >= sf::seconds(1.0f))
        {
            m_FramePerSecond    = m_FrameCount;
            m_TimePerFrame      = m_ElapsedTime.asSeconds() / m_FrameCount;

            m_ElapsedTime      -= sf::seconds(1.0f);
            m_FrameCount        = 0;
        }
    }

    std::string CoreEngine::getWindowTitle() const
    {
        return m_WindowTitle;
    }

    void CoreEngine::setWindowTitle(const std::string& title)
    {
        m_WindowTitle = title;

        m_Window.setTitle(title);
    }

    unsigned int CoreEngine::getWindowWidth()
    {
        m_WindowWidth = m_Window.getSize().x;

        return m_WindowWidth;
    }

    unsigned int CoreEngine::getWindowHeight()
    {
        m_WindowHeight = m_Window.getSize().y;

        return m_WindowHeight;
    }

    float CoreEngine::getFrameRate() const
    {
        return m_FramePerSecond;
    }

    float CoreEngine::getFrameTime() const
    {
        return m_TimePerFrame;
    }

    void CoreEngine::buildDirectory()
    {
        //create main folders
        createDirectory(getPath({EngineConstant.FOLDER_SETTING}));
        createDirectory(getPath({EngineConstant.FOLDER_LOGGING}));
        createDirectory(getPath({EngineConstant.FOLDER_RESOURCE}));
        createDirectory(getPath({EngineConstant.FOLDER_WORKSPACE}));
        createDirectory(getPath({EngineConstant.FOLDER_STARTUP}));

        //create resource sub folders
        createDirectory(getPath({EngineConstant.FOLDER_RESOURCE, EngineConstant.FOLDER_RESOURCE_TEXTURE}));
        createDirectory(getPath({EngineConstant.FOLDER_RESOURCE, EngineConstant.FOLDER_RESOURCE_FONT}));
        createDirectory(getPath({EngineConstant.FOLDER_RESOURCE, EngineConstant.FOLDER_RESOURCE_MUSIC}));
        createDirectory(getPath({EngineConstant.FOLDER_RESOURCE, EngineConstant.FOLDER_RESOURCE_SOUND}));
        createDirectory(getPath({EngineConstant.FOLDER_RESOURCE, EngineConstant.FOLDER_RESOURCE_SCRIPT}));
        createDirectory(getPath({EngineConstant.FOLDER_RESOURCE, EngineConstant.FOLDER_RESOURCE_SHADER}));
        createDirectory(getPath({EngineConstant.FOLDER_RESOURCE, EngineConstant.FOLDER_RESOURCE_LANGUAGE}));
        createDirectory(getPath({EngineConstant.FOLDER_RESOURCE, EngineConstant.FOLDER_RESOURCE_ANIMATION}));

        //create main files
        //engine setting
        saveFile(getPath({EngineConstant.FOLDER_SETTING, EngineConstant.FILE_SETTING_ENGINE}, StringPool.EXTENSION_JSON), StringPool.BLANK);
        //logging setting
        saveFile(getPath({EngineConstant.FOLDER_SETTING, EngineConstant.FILE_SETTING_LOGGING}, StringPool.EXTENSION_TEXT), StringPool.BLANK);
        //resource setting
        saveFile(getPath({EngineConstant.FOLDER_SETTING, EngineConstant.FILE_SETTING_RESOURCE}, StringPool.EXTENSION_JSON), StringPool.BLANK);

    }

    void CoreEngine::quitEngine()
    {
        m_Window.close();
    }

	void CoreEngine::setWindowIcon(const std::string& icon)
    {
        if (m_WindowIcon.loadFromFile(icon))
        {
            m_Window.setIcon(m_WindowIcon.getSize().x, m_WindowIcon.getSize().y, m_WindowIcon.getPixelsPtr());
        }
    }

}


