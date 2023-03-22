////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/scene/GameLevel.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
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
        // TODO get gravity from m_LevelContext.levelSetting
        , m_PhysicsWorld(std::make_shared<b2World>(b2Vec2(0.f, 9.8f)))
        , m_ShapeRenderer(std::make_shared<ShapeRenderer>(m_LevelContext.renderTexture))
    {
        m_LightManager->create({-1000.f, -1000.f, 2000.f, 2000.f},
                               m_LevelContext.renderTexture->getSize());
        // Add a sun light
        auto lightSetting = m_LevelContext.levelSetting->getSetting("lighting");
        ltbl::LightDirectionEmission* ambientLight = m_LightManager->createLightDirectionEmission();
        ambientLight->setCastDirection(lightSetting.getVector("cast_direction"));
        ambientLight->setCastAngle(lightSetting.getFloat("cast_angle"));
        ambientLight->setSourceDistance(lightSetting.getFloat("source_distance"));
        ambientLight->setColor(lightSetting.getColor("ambient_color"));
        ambientLight->setSourceRadius(lightSetting.getFloat("source_radius"));
        ambientLight->setTurnedOn(lightSetting.getBool("enable_ambient_light"));

        // m_PhysicsWorld->SetContactListener(m_ContactListener);
        m_PhysicsWorld->SetDebugDraw(m_ShapeRenderer.get());
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
        const auto frequence       = 40.f;
        float32    physicsTimeStep = frequence > 0.f ? 1.f / frequence : float32(0.0f);

        if(physicsTimeStep > 0.f)
        {
            physicsTimeStep = (physicsTimeStep * timeStep.asSeconds()) /
                              EngineConstant.TIME_PER_FRAME.asSeconds();
        }

        uint32 flags = 0;
        flags        += b2Draw::e_shapeBit;
        flags        += b2Draw::e_jointBit;
        // flags        += b2Draw::e_aabbBit;
        flags        += b2Draw::e_centerOfMassBit;

        m_ShapeRenderer->SetFlags(flags);

        m_PhysicsWorld->SetAllowSleeping(true);
        m_PhysicsWorld->SetWarmStarting(true);
        m_PhysicsWorld->SetContinuousPhysics(true);
        m_PhysicsWorld->SetSubStepping(true);

        const int velocityIterations = 8;
        const int positionIterations = 3;
        m_PhysicsWorld->Step(physicsTimeStep, velocityIterations, positionIterations);
    }

    void GameLevel::render()
    {
    }

    void GameLevel::renderShape()
    {
        m_PhysicsWorld->DrawDebugData();
    }

    void GameLevel::renderLight()
    {
        if(m_LevelContext.levelSetting->getBool("enable_light"))
        {
            m_LightManager->render(*m_LevelContext.renderTexture.get());
        }
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

    std::shared_ptr<ltbl::LightSystem> GameLevel::getLightManager() const
    {
        return m_LightManager;
    }

    std::shared_ptr<b2World> GameLevel::getPhysicsWorld() const
    {
        return m_PhysicsWorld;
    }

    ShapeRenderer::Ptr GameLevel::getShapeRenderer() const
    {
        return m_ShapeRenderer;
    }

} // namespace nero
