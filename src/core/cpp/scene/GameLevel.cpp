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
        , m_ObjectManager(
              std::make_shared<ObjectManager>(m_LevelRoot, m_PhysicsWorld, m_ScreenTable))
        , m_ContactListener(std::make_shared<ContactListener>(m_ObjectManager))
    {
        auto       lightSetting = m_LevelContext.levelSetting->getSetting("lighting");
        const auto rootRegion   = lightSetting.getVector("root_region");
        const auto imageSize    = lightSetting.getVector("image_size");
        m_LightManager->create({rootRegion.x, rootRegion.y, imageSize.x, imageSize.y},
                               m_LevelContext.renderTexture->getSize());
        // Add a sun light
        m_AmbientLight = m_LightManager->createLightDirectionEmission();
        m_AmbientLight->setCastDirection(lightSetting.getVector("cast_direction"));
        m_AmbientLight->setCastAngle(lightSetting.getFloat("cast_angle"));
        m_AmbientLight->setSourceDistance(lightSetting.getFloat("source_distance"));
        m_AmbientLight->setColor(lightSetting.getColor("ambient_color"));
        m_AmbientLight->setSourceRadius(lightSetting.getFloat("source_radius"));
        m_AmbientLight->setTurnedOn(lightSetting.getBool("enable_ambient_light"));

        m_PhysicsWorld->SetContactListener(m_ContactListener.get());
        m_PhysicsWorld->SetDebugDraw(m_ShapeRenderer.get());

        m_ContactListener->registerCallback(
            "beginContact",
            std::bind(&GameLevel::onCollisionContactBegin, this, std::placeholders::_1));
        m_ContactListener->registerCallback(
            "endContact",
            std::bind(&GameLevel::onCollisionContactEnd, this, std::placeholders::_1));
        m_ContactListener->registerCallback(
            "preSolveContact",
            std::bind(&GameLevel::onCollisionPreSolveContact, this, std::placeholders::_1));
        m_ContactListener->registerCallback(
            "postSolveContact",
            std::bind(&GameLevel::onCollisionPostSolveContact, this, std::placeholders::_1));
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
        auto lightSetting = m_LevelContext.levelSetting->getSetting("lighting");
        m_AmbientLight->setCastDirection(lightSetting.getVector("cast_direction"));
        m_AmbientLight->setCastAngle(lightSetting.getFloat("cast_angle"));
        m_AmbientLight->setSourceDistance(lightSetting.getFloat("source_distance"));
        m_AmbientLight->setColor(lightSetting.getColor("ambient_color"));
        m_AmbientLight->setSourceRadius(lightSetting.getFloat("source_radius"));

        const auto frequence       = 40.f;
        float32    physicsTimeStep = frequence > 0.f ? 1.f / frequence : float32(0.0f);

        if(physicsTimeStep > 0.f)
        {
            physicsTimeStep = (physicsTimeStep * timeStep.asSeconds()) /
                              EngineConstant.TIME_PER_FRAME.asSeconds();
        }

        auto   physicsSetting = m_LevelContext.levelSetting->getSetting("physics");
        uint32 flags          = 0;
        flags                 += physicsSetting.getBool("draw_shape") * b2Draw::e_shapeBit;
        flags                 += physicsSetting.getBool("draw_joint") * b2Draw::e_jointBit;
        flags                 += physicsSetting.getBool("draw_aabb") * b2Draw::e_aabbBit;
        flags                 += physicsSetting.getBool("draw_axis") * b2Draw::e_centerOfMassBit;
        flags                 += physicsSetting.getBool("draw_pairbit") * b2Draw::e_pairBit;

        m_ShapeRenderer->SetFlags(flags);

        m_PhysicsWorld->SetAllowSleeping(physicsSetting.getBool("allow_sleeping"));
        m_PhysicsWorld->SetWarmStarting(physicsSetting.getBool("warm_starting"));
        m_PhysicsWorld->SetContinuousPhysics(physicsSetting.getBool("continuous_physics"));
        m_PhysicsWorld->SetSubStepping(physicsSetting.getBool("sub_stepping"));

        const int velocityIterations = physicsSetting.getInt("velocity_iterations");
        const int positionIterations = physicsSetting.getInt("position_iterations");
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
