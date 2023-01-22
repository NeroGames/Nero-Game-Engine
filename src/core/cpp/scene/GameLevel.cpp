////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
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
    {
    }

    GameLevel::GameLevel()
    {
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
    }

    void GameLevel::update(const sf::Time& timeStep)
    {
    }

    void GameLevel::render()
    {
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
} // namespace nero
