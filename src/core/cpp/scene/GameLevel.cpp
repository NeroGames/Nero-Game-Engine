////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/scene/GameLevel.h>
/////////////////////////////////////////////////////////////
namespace nero
{
    GameLevel::Context::Context(std::string                        levelName,
                                Setting::Ptr                       levelSetting,
                                std::string                        levelDirectory,
                                std::shared_ptr<sf::RenderTexture> renderTexture,
                                Camera::Ptr                        camera)
        : levelName(levelName)
        , levelSetting(levelSetting)
        , levelDirectory(levelDirectory)
        , renderTexture(renderTexture)
        , camera(camera)
    {
    }

    GameLevel::Context::Context()
    {
    }

    GameLevel::GameLevel(GameLevel::Context context)
        : m_LevelContext(context)
        , m_ResourceManager(std::make_shared<ResourceManager>(
              m_LevelContext.levelSetting->getSetting("resource")))
        , m_LevelRoot(std::make_shared<Object>())
        , m_LightManager(std::make_shared<ltbl::LightSystem>(false))
    {
        m_LightManager->create({-1000.f, -1000.f, 2000.f, 2000.f},
                               m_LevelContext.renderTexture->getSize());

        // just of testing
        sf::Texture& pointLightTexture =
            m_LevelContext.resourceManager->getLightmapHolder()->getTexture("pointLightTexture");
        // Add a sun light
        ltbl::LightDirectionEmission* sun = m_LightManager->createLightDirectionEmission();
        sun->setColor(sf::Color(0, 51, 102, 50));

        // Add a light point
        /*ltbl::LightPointEmission* mlight = m_LightManager->createLightPointEmission();
        mlight->setOrigin(sf::Vector2f(pointLightTexture.getSize().x * 0.5f,
                                       pointLightTexture.getSize().y * 0.5f));
        mlight->setTexture(pointLightTexture);
        mlight->setScale(3.f, 3.f);
        mlight->setColor(sf::Color::White);*/

        /*std::vector<sf::RectangleShape> shapes;
        sf::RectangleShape              blocker;
        blocker.setSize({200.f, 50.f});
        blocker.setPosition({0.f, -300.f});
        blocker.setFillColor(sf::Color::Red);
        shapes.push_back(blocker);
        m_LightManager->createLightShape(blocker);*/
    }

    GameLevel::~GameLevel()
    {
        destroy();
    }

    void GameLevel::init()
    {
    }

    void GameLevel::destroy()
    {
    }

    void GameLevel::handleEvent(const sf::Event& event)
    {
        if(event.type == sf::Event::MouseButtonPressed &&
           event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Texture& pointLightTexture =
                m_LevelContext.resourceManager->getLightmapHolder()->getTexture(
                    "pointLightTexture");
            auto light = m_LightManager->createLightPointEmission();
            light->setOrigin(sf::Vector2f(pointLightTexture.getSize().x * 0.5f,
                                          pointLightTexture.getSize().y * 0.5f));
            light->setTexture(pointLightTexture);
            light->setScale(6.f, 6.f);
            light->setColor(sf::Color::White);
            light->setPosition(m_LevelContext.renderTexture->mapPixelToCoords(
                {event.mouseButton.x, event.mouseButton.y}));
        }
    }

    void GameLevel::update(const sf::Time& timeStep)
    {
    }

    void GameLevel::render()
    {
        m_LightManager->render(*m_LevelContext.renderTexture.get());
    }

    void GameLevel::renderShape()
    {
    }

    void GameLevel::loadLevel()
    {
    }

    void GameLevel::loadChunk()
    {
    }

    void GameLevel::loadLayer()
    {
    }

    void GameLevel::loadObject()
    {
    }

    void GameLevel::onWindowClosed()
    {
    }

    void GameLevel::onWindowResized(const sf::Vector2u& size)
    {
    }

    void GameLevel::onWindowFocusChanged(const bool& gainedFocus)
    {
    }

    void GameLevel::onKeyboardButton(const sf::Keyboard::Key& key,
                                     const bool&              isPressed,
                                     const ModifierKey&       modifier)
    {
    }

    void GameLevel::onTextEntered(const std::string& c)
    {
    }

    void GameLevel::onMouseMoved(const sf::Vector2f& position)
    {
    }

    void GameLevel::onMouseButton(const sf::Mouse::Button& button,
                                  const bool&              isPressed,
                                  const sf::Vector2f&      position)
    {
    }

    void GameLevel::onMouseWheel(const sf::Mouse::Wheel& wheel,
                                 const float&            delta,
                                 const sf::Vector2f&     position)
    {
    }

    void GameLevel::onMouseWindowSurface(const bool& mouseEntered)
    {
    }

    void GameLevel::onJoystickConnection(const unsigned int& joystickId, const bool& connected)
    {
    }

    void GameLevel::onJoystickButton(const unsigned int& joystickId,
                                     const unsigned int& button,
                                     const bool&         isPressed)
    {
    }

    void GameLevel::onJoystickAxis(const unsigned int&       joystickId,
                                   const sf::Joystick::Axis& axis,
                                   const float&              position)
    {
    }

    void GameLevel::onCollisionContactBegin(Collision collision)
    {
    }

    void GameLevel::onCollisionContactEnd(Collision collision)
    {
    }

    void GameLevel::onCollisionPreSolveContact(Collision collision)
    {
    }

    void GameLevel::onCollisionPostSolveContact(Collision collision)
    {
    }

    ResourceManager::Ptr GameLevel::getResourceManager() const
    {
        return m_ResourceManager;
    }

    Setting::Ptr GameLevel::getSetting() const
    {
        return m_LevelContext.levelSetting;
    }

    std::string GameLevel::getLevelDirectory() const
    {
        return m_LevelContext.levelDirectory;
    }

    std::shared_ptr<sf::RenderTexture> GameLevel::getRenderTexture()
    {
        return m_LevelContext.renderTexture;
    }

    Camera::Ptr GameLevel::getCamera()
    {
        return m_LevelContext.camera;
    }

    Object::Ptr GameLevel::getLevelRoot() const
    {
        return m_LevelRoot;
    }

    std::shared_ptr<ltbl::LightSystem> GameLevel::getLightManager()
    {
        return m_LightManager;
    }

} // namespace nero
