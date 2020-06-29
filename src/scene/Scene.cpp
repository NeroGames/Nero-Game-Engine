////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/scene/Scene.h>
#include <Nero/utility/Utility.h>
#include <Nero/object/LayerObject.h>
//SFML
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    //Context
    Scene::Context::Context(sfg::Canvas::Ptr renderCanvas, sf::View& frontView, Camera::Ptr camera, ResourceManager::Ptr resourceManager, bool renderEngine):
        renderCanvas(renderCanvas)
        ,frontView(frontView)
        ,camera(camera)
        ,resourceManager(resourceManager)
        ,renderEngine(renderEngine)
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    Scene::Scene(Scene::Context context):
         m_Context(context)
        ,m_World(Object::Ptr(new Object))
        ,m_CameraTarget()
        //
        ,m_SceneSetting()
        ,m_CameraSetting()
        //
        ,m_ShapeRenderer(context.renderCanvas)
        ,m_ObjectManager(ObjectManager::Ptr(new ObjectManager(m_World, m_PhysicWorld, m_ScreenTable)))
        ,m_SoundManager(SoundManager::Ptr(new SoundManager(context.resourceManager->music, context.resourceManager->sound)))
        ,m_ScriptManager(ScriptManager::Ptr(new ScriptManager()))
        //
        ,m_PointCount(0)
        ,m_Text()
        ,m_PauseMessage("")
        ,m_HideWorld(false)
        ,m_SceenCanvasColor(sf::Color::Transparent)
    {
        //Setup the world
        m_PhysicWorld = new b2World(m_SceneSetting.gravity);
        m_PhysicWorld->SetContactListener(this);
        m_PhysicWorld->SetDebugDraw(&m_ShapeRenderer);

        //Text
        m_Text.setFont(context.resourceManager->font.getDefaultFont());
        m_Text.setCharacterSize(15.f);
        m_Text.setFillColor(sf::Color::White);
        m_Text.setPosition(sf::Vector2f(55.f, 10.f));

        //
        m_QuitEngine = [](){};
        m_ResetScene = [](){};
        m_UpdateLog = [](const std::string& content, int level){};
        m_UpdateLogIf = [](const std::string& content, bool condition, int level){};
    }

    ////////////////////////////////////////////////////////////
    Scene::~Scene()
    {
        //Delete the world
        delete m_PhysicWorld;
        m_PhysicWorld = nullptr;
    }

    ////////////////////////////////////////////////////////////
    void Scene::handleEvent(const sf::Event& event)
    {
        if(!m_HideWorld)
        {
            if(!m_SceneSetting.pause || m_SceneSetting.singleStep)
            {
                switch(event.type)
                {
                    //Keyboard events
                    case sf::Event::KeyPressed:
                        handleKeyboardInput(event.key.code, true);
                        break;
                    case sf::Event::KeyReleased:
                        handleKeyboardInput(event.key.code, false);
                        break;

                    //Mouse buttons events
                    case sf::Event::MouseButtonPressed:
                        handleMouseButtonInput(event.mouseButton, true);
                        break;
                    case sf::Event::MouseButtonReleased:
                        handleMouseButtonInput(event.mouseButton, false);
                        break;

                    //Mouse move event
                    case sf::Event::MouseMoved:
                        handleMouseMoveInput(event.mouseMove);
                        break;

                     //Mouse wheel
                    case sf::Event::MouseWheelScrolled:
                        handleMouseWheelInput(event.mouseWheelScroll);
                        break;
                }
            }
        }

        if(isRenderEngine())
        {
            for(auto screen : m_ScreenStack)
            {
                if(!screen->hide)
                {
                    screen->screenUI->handleEvent(event);
                }
            }
        }

    }

    ////////////////////////////////////////////////////////////
    void Scene::update(const sf::Time& timeStep)
    {
        float32 b2TimeStep = m_SceneSetting.hz > 0.0f ? 1.0f / m_SceneSetting.hz : float32(0.0f);

        if(b2TimeStep > 0.f)
        {
            b2TimeStep = (b2TimeStep * timeStep.asSeconds())/TIME_PER_FRAME.asSeconds();
        }

        if(!m_HideWorld)
        {
            if(!m_PhysicWorld->IsLocked())
                m_ObjectManager->removeDeadPhysicObject();


            if(m_SceneSetting.pause && !m_SceneSetting.singleStep)
            {
                b2TimeStep = 0.0f;
                m_Context.renderEngine ? m_PauseMessage = "" : m_PauseMessage = "#-- PAUSED --#";
            }
            else
            {
                m_PauseMessage = "";
            }

            uint32 flags = 0;
            flags += m_SceneSetting.drawShapes * b2Draw::e_shapeBit;
            flags += m_SceneSetting.drawJoints * b2Draw::e_jointBit;
            flags += m_SceneSetting.drawAABBs * b2Draw::e_aabbBit;
            flags += m_SceneSetting.drawCOMs * b2Draw::e_centerOfMassBit;

            m_ShapeRenderer.SetFlags(flags);

            m_PhysicWorld->SetAllowSleeping(m_SceneSetting.enableSleep > 0);
            m_PhysicWorld->SetWarmStarting(m_SceneSetting.enableWarmStarting > 0);
            m_PhysicWorld->SetContinuousPhysics(m_SceneSetting.enableContinuous > 0);
            m_PhysicWorld->SetSubStepping(m_SceneSetting.enableSubStepping > 0);

            m_PointCount = 0;

            m_PhysicWorld->Step(b2TimeStep, m_SceneSetting.velocityIterations, m_SceneSetting.positionIterations);


            if((!m_SceneSetting.pause || m_SceneSetting.singleStep))
                m_World->update(sf::seconds(b2TimeStep));



            m_Text.setString(m_PauseMessage);

            followTarget();
        }

        if(isRenderEngine())
        {
            for(auto screen : m_ScreenStack)
            {
                if(!screen->hide)
                {
                    screen->screen->update(sf::seconds(b2TimeStep));
                    screen->screenUI->update(sf::seconds(b2TimeStep));
                }
            }
        }
    }

    ////////////////////////////////////////////////////////////
    void Scene::render()
    {
        if(!m_HideWorld)
        {
            auto childTable = m_World->getAllChild();

            for(auto it = childTable->begin(); it != childTable->end(); it++)
            {
                LayerObject::Ptr layer_object = LayerObject::Cast(*it);

                if(layer_object->isVisible())
                    m_Context.renderCanvas->Draw(*layer_object);
            }
        }
    }

    ////////////////////////////////////////////////////////////
    void Scene::renderShape()
    {
        if(!m_HideWorld)
        {
            m_PhysicWorld->DrawDebugData();
        }
    }

    ////////////////////////////////////////////////////////////
    void Scene::renderFrontScreen()
    {
        if(isRenderEngine())
        {
            for(auto screen : m_ScreenStack)
            {
                if(!screen->hide)
                {
                    auto childTable1 = screen->screen->getAllChild();
                    auto childTable2 = screen->screenUI->getAllChild();

                    for(auto it = childTable1->begin(); it != childTable1->end(); it++)
                    {
                        m_Context.renderCanvas->Draw(**it);
                    }

                    for(auto it = childTable2->begin(); it != childTable2->end(); it++)
                    {
                        m_Context.renderCanvas->Draw(**it);
                    }
                }
            }
        }

        if(!m_HideWorld)
        {
            m_Context.renderCanvas->Draw(m_Text);
        }
    }

    ////////////////////////////////////////////////////////////
    void Scene::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
    {
        const b2Manifold* manifold = contact->GetManifold();

        if (manifold->pointCount == 0)
        {
            return;
        }

        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        b2PointState state1[b2_maxManifoldPoints], state2[b2_maxManifoldPoints];
        b2GetPointStates(state1, state2, oldManifold, manifold);

        b2WorldManifold worldManifold;
        contact->GetWorldManifold(&worldManifold);

        for (int32 i = 0; i < manifold->pointCount && m_PointCount < k_maxContactPoints; ++i)
        {
            ContactPoint* cp = m_Points + m_PointCount;
            cp->fixtureA = fixtureA;
            cp->fixtureB = fixtureB;
            cp->position = worldManifold.points[i];
            cp->normal = worldManifold.normal;
            cp->state = state2[i];
            cp->normalImpulse = manifold->points[i].normalImpulse;
            cp->tangentImpulse = manifold->points[i].tangentImpulse;
            cp->separation = worldManifold.separations[i];
            ++m_PointCount;
        }

        Collision collision(contact, oldManifold, nullptr);

        int id_A = (int)contact->GetFixtureA()->GetBody()->GetUserData();
        int id_B = (int)contact->GetFixtureB()->GetBody()->GetUserData();

        auto objectA = PhysicObject::Cast(m_ObjectManager->findObject(id_A));
        auto objectB = PhysicObject::Cast(m_ObjectManager->findObject(id_B));

        if(!objectA || !objectB)
            return;

        if(objectA->isDead() || objectB->isDead())
            return;

        collision.setObjectA(objectA);
        collision.setObjectB(objectB);

        handleCollisionPreSolveContact(collision);
    }

    ////////////////////////////////////////////////////////////
    void Scene::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
    {
        Collision collision(contact, nullptr, impulse);

        int id_A = (int)contact->GetFixtureA()->GetBody()->GetUserData();
        int id_B = (int)contact->GetFixtureB()->GetBody()->GetUserData();

        auto objectA = PhysicObject::Cast(m_ObjectManager->findObject(id_A));
        auto objectB = PhysicObject::Cast(m_ObjectManager->findObject(id_B));

        if(!objectA || !objectB)
            return;

        if(objectA->isDead() || objectB->isDead())
            return;

        collision.setObjectA(objectA);
        collision.setObjectB(objectB);

        handleCollisionPostSolveContact(collision);
    }

    ////////////////////////////////////////////////////////////
    void Scene::BeginContact(b2Contact* contact)
    {
        Collision collision(contact, nullptr, nullptr);

        int id_A = (int)contact->GetFixtureA()->GetBody()->GetUserData();
        int id_B = (int)contact->GetFixtureB()->GetBody()->GetUserData();

        auto objectA = PhysicObject::Cast(m_ObjectManager->findObject(id_A));
        auto objectB = PhysicObject::Cast(m_ObjectManager->findObject(id_B));

        if(!objectA || !objectB)
            return;

        if(objectA->isDead() || objectB->isDead())
            return;

        collision.setObjectA(objectA);
        collision.setObjectB(objectB);

        handleCollisionContactBegin(collision);
    }

    ////////////////////////////////////////////////////////////
    void Scene::EndContact(b2Contact* contact)
    {
        Collision collision(contact, nullptr, nullptr);

        int id_A = (int)contact->GetFixtureA()->GetBody()->GetUserData();
        int id_B = (int)contact->GetFixtureB()->GetBody()->GetUserData();

        auto objectA = PhysicObject::Cast(m_ObjectManager->findObject(id_A));
        auto objectB = PhysicObject::Cast(m_ObjectManager->findObject(id_B));

        if(!objectA || !objectB)
            return;

        if(objectA->isDead() || objectB->isDead())
            return;

        collision.setObjectA(objectA);
        collision.setObjectB(objectB);

        handleCollisionContactEnd(collision);
    }

    ////////////////////////////////////////////////////////////
    void Scene::setCameraTarget(Object::Ptr target)
    {
        m_CameraTarget.target       = target;
        m_CameraTarget.followTarget = true;
    }

      ////////////////////////////////////////////////////////////
    void Scene::enableFollowTaget(bool flag)
    {
        m_CameraTarget.followTarget = flag;
    }

    ////////////////////////////////////////////////////////////
    void Scene::updateTargetOffset(const float left, const float right, const float up, const float down)
    {
        m_CameraTarget.offsetLeft   = left;
        m_CameraTarget.offsetRight  = right;
        m_CameraTarget.offsetUp     = up;
        m_CameraTarget.offsetDown   = down;
    }

    ////////////////////////////////////////////////////////////
    void Scene::followTarget()
    {
        //Check
        if(!m_CameraTarget.target || !m_CameraTarget.followTarget) return;

        //Horizontal offset
        sf::Vector2f cameraPosition = m_Context.camera->getPosition();
        sf::Vector2f targetPosition = m_CameraTarget.target->getCenter();
        float offset                = targetPosition.x - cameraPosition.x;

        if(offset > m_CameraTarget.offsetRight)
        {
            m_Context.camera->setPosition(cameraPosition + sf::Vector2f(offset - m_CameraTarget.offsetRight, 0.f));
        }
        else if(offset < - m_CameraTarget.offsetLeft)
        {
            m_Context.camera->setPosition(cameraPosition + sf::Vector2f(offset + m_CameraTarget.offsetLeft, 0.f));
        }

        //Vertical offset
        cameraPosition  = m_Context.camera->getPosition();
        offset          = targetPosition.y - cameraPosition.y;

        if(offset > m_CameraTarget.offsetUp)
        {
            m_Context.camera->setPosition(cameraPosition + sf::Vector2f(0.f, offset - m_CameraTarget.offsetUp));
        }
        else if(offset < - m_CameraTarget.offsetDown)
        {
            m_Context.camera->setPosition(cameraPosition + sf::Vector2f(0.f, offset + m_CameraTarget.offsetDown));
        }
    }

    ////////////////////////////////////////////////////////////
    void Scene::init()
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void Scene::checkSceneObject()
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void Scene::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void Scene::handleMouseButtonInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed)
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void Scene::handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse)
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void Scene::handleCollisionContactBegin(Collision collision)
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void Scene::handleCollisionContactEnd(Collision collision)
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void Scene::handleCollisionPreSolveContact(Collision collision)
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void Scene::handleCollisionPostSolveContact(Collision collision)
    {
        //Empty
    }

    void Scene::handleMouseWheelInput(const sf::Event::MouseWheelScrollEvent& mouse)
    {
        //Empty
    }

    void Scene::quitScene()
    {
        m_QuitEngine();
    }

    void Scene::enableScreen(const std::string& name, bool flag)
    {
        for(auto& screen : m_ScreenStack)
        {
            if(screen->name == name)
            {
                screen->hide = !flag;

                nero_log_if("show " + _s(name), !screen->hide);

                break;
            }
        }
    }

    const sf::Color& Scene::getScreenCanvasColor(const std::string& name) const
    {
        for(auto& screen : m_ScreenTable)
        {
            if(screen->name == name)
            {
                return screen->canvasColor;
            }
        }

        return sf::Color::White;
    }

    bool Scene::isRenderEngine()
    {
        return m_Context.renderEngine;
    }

     const std::string& Scene::getSceneName() const
     {
         return m_SceneName;
     }

    const Scene::Context& Scene::getContext() const
    {
        return m_Context;
    }

    Object::Ptr Scene::getWorld()
    {
        return m_World;
    }

    void Scene::setCanvasColor(const sf::Color& color)
    {
        m_CanvasColor = color;
    }

    const sf::Color& Scene::getCanvasColor() const
    {
        return m_CanvasColor;
    }

    ObjectManager::Ptr Scene::getObjectManager()
    {
        return m_ObjectManager;
    }

    SoundManager::Ptr Scene::getSoundManager()
    {
        return m_SoundManager;
    }

    ScriptManager::Ptr Scene::getScriptManager()
    {
        return m_ScriptManager;
    }

    SceneSetting& Scene::getSceneSetting()
    {
        return m_SceneSetting;
    }

    CameraSetting& Scene::getCameraSetting()
    {
        return m_CameraSetting;
    }

    void Scene::pauseScene()
    {
        m_SceneSetting.pause = true;
    }

    void Scene::resumeScene()
    {
        m_SceneSetting.pause = false;
    }

    bool Scene::isScenePause()
    {
        return m_SceneSetting.pause;
    }

    void Scene::resetScene()
    {
        m_ResetScene();
    }

    float Scene::getFrameRate()
    {
        return m_FrameRate;
    }

    float Scene::getFrameTime()
    {
        return m_FrameTime;
    }

    void Scene::log(const std::string& content, int level)
    {
        if(!isRenderEngine())
        {
            m_UpdateLog(content, level);
        }
        else
        {
            nero_log(content, level);
        }
    }

    void Scene::logIf(const std::string& content, bool condition, int level)
    {
        if(!isRenderEngine())
        {
            m_UpdateLogIf(content, condition, level);
        }
        else
        {
            nero_log_if(content, condition, level);
        }
    }

    std::function<void(const std::string&, int)> Scene::getLog()
    {
        return [this](const std::string& content, int level)
        {
            nero_log(content, level);

            if(!isRenderEngine())
            {
                m_UpdateLog(content, level);
            }
        };
    }

    std::function<void(const std::string&, bool, int)> Scene::getLogIf()
    {
        return [this](const std::string& content, bool condition, int level)
        {
            nero_log_if(content, condition, level);

            if(!isRenderEngine())
            {
                m_UpdateLogIf(content, condition, level);
            }
        };
    }


    void Scene::hideWorld()
    {
        m_HideWorld = true;
    }

    void Scene::showWorld()
    {
        m_HideWorld = false;
    }

    void Scene::hideScreen(const std::string& name)
    {
        enableScreen(name, false);
    }

    void Scene::showScreen(const std::string& name)
    {
        enableScreen(name, true);
    }

    void Scene::pushScreen(const std::string& name)
    {
        for(Screen::Ptr screen : m_ScreenStack)
        {
            if(screen->name == name)
            {
                return;
            }
        }

        for(Screen::Ptr screen : m_ScreenTable)
        {
            if(screen->name == name)
            {
                m_ScreenStack.push_back(screen);
            }
        }
    }

    void Scene::popScreen()
    {
        if(!m_ScreenStack.empty())
        {
            m_ScreenStack.pop_back();
        }
    }

    sf::Vector2f Scene::getSceneResolution()
    {
       return sf::Vector2f(800.f, 600.f);
    }

    b2World* Scene::getPhysicWorld()
    {
        return m_PhysicWorld;
    }

    void Scene::disableLayer(const std::string& name)
    {
        Object::Ptr layer = m_ObjectManager->findLayerObject(name);

        if(!layer)
            return;

        layer->setIsVisible(false);
        layer->setIsUpdateable(false);

        if(layer->getSecondType() == Object::Physic_Object || layer->getSecondType() == Object::Solid_Object || layer->getSecondType() == Object::Animation_Solid_Object)
        {
            auto childTable = layer->getAllChild();
            for(Object::Ptr object : *childTable)
            {
                PhysicObject::Cast(object)->setActive(false);
            }
        }
    }

    void Scene::enableLayer(const std::string& name)
    {
        Object::Ptr layer = m_ObjectManager->findLayerObject(name);

        if(!layer)
            return;

        layer->setIsVisible(true);
        layer->setIsUpdateable(true);

        if(layer->getSecondType() == Object::Physic_Object || layer->getSecondType() == Object::Solid_Object || layer->getSecondType() == Object::Animation_Solid_Object)
        {
            auto childTable = layer->getAllChild();
            for(Object::Ptr object : *childTable)
            {
                PhysicObject::Cast(object)->setActive(true);
            }
        }
    }
}
