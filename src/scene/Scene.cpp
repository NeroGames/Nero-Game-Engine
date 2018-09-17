////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/scene/Scene.h>
#include <Nero/utility/Utility.h>
#include <Nero/object/LayerObject.h>
//SFML
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
//STD
#include <stdio.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    Scene::Scene(Context context):
         m_Context(context)
        ,m_RenderCanvas(context.renderCanvas)
        ,m_RootObject(Object::Ptr(new Object))
        ,m_CameraTarget(nullptr)
        ,m_SceneName("")
        ,L(nullptr)
        //
        ,m_SceneSetting()
        ,m_CameraSetting()
        ,m_SoundSetting()
        //
        ,m_ShapeRenderer(context.renderCanvas)
        ,m_ObjectManager(ObjectManager::Ptr(new ObjectManager(m_RootObject, m_PhysicWorld)))
        ,m_SoundManager(SoundManager::Ptr(new SoundManager(context.resourceManager->Music, context.resourceManager->Sound)))
        ,m_ScriptManager(ScriptManager::Ptr(new ScriptManager()))
        //
        ,m_PointCount(0)
        ,m_Text()
        ,m_PauseMessage("")
        ,m_CameraTargetOffset()
        ,m_CameraFollowTarget(false)
    {
        //Setup the world
        m_PhysicWorld = new b2World(m_SceneSetting.gravity);
        m_PhysicWorld->SetContactListener(this);
        m_PhysicWorld->SetDebugDraw(&m_ShapeRenderer);

        //Text
        m_Text.setFont(context.resourceManager->Font.getDefaultFont());
        m_Text.setCharacterSize(15.f);
        m_Text.setFillColor(sf::Color::White);
        m_Text.setPosition(sf::Vector2f(10.f, 10.f));

        //Script
        L = luaL_newstate();
        luaL_openlibs(L);
    }

    ////////////////////////////////////////////////////////////
    Scene::~Scene()
    {
        //Close lua state
        lua_close(L);

        //Delete the world
        delete m_PhysicWorld;
        m_PhysicWorld = nullptr;
    }

    ////////////////////////////////////////////////////////////
    void Scene::handleEvent(const sf::Event& event)
    {
        if(m_SceneSetting.pause && !m_SceneSetting.singleStep)
            return;

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

    ////////////////////////////////////////////////////////////
    void Scene::update(const sf::Time& timeStep)
    {
        m_ObjectManager->setWorld(m_PhysicWorld);

        if(!m_PhysicWorld->IsLocked())
            m_ObjectManager->removeDeadPhysicObject();

        float32 b2TimeStep = m_SceneSetting.hz > 0.0f ? 1.0f / m_SceneSetting.hz : float32(0.0f);

        if(b2TimeStep > 0.f)
        {
            b2TimeStep = (b2TimeStep * timeStep.asSeconds())/TIME_PER_FRAME.asSeconds();
        }

        if(m_SceneSetting.pause && !m_SceneSetting.singleStep)
        {
            b2TimeStep = 0.0f;
            m_PauseMessage = "#-- PAUSED --#";
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

        if(!m_SceneSetting.pause || m_SceneSetting.singleStep)
            m_RootObject->update(sf::seconds(b2TimeStep));

        m_Text.setString(m_PauseMessage);

        cameraFollowTarget();
    }

    ////////////////////////////////////////////////////////////
    void Scene::render()
    {
        auto childTable = m_RootObject->getAllChild();

        for(auto it = childTable->begin(); it != childTable->end(); it++)
        {
            LayerObject::Ptr layer_object = LayerObject::Cast(*it);

            if(layer_object->isVisible())
                m_RenderCanvas->Draw(*layer_object);
        }
    }

    ////////////////////////////////////////////////////////////
    void Scene::renderShape()
    {
        m_PhysicWorld->DrawDebugData();
    }

    ////////////////////////////////////////////////////////////
    void Scene::renderFrontScreen()
    {
        m_RenderCanvas->Draw(m_Text);
    }

    ////////////////////////////////////////////////////////////
    void Scene::preSolve(b2Contact* contact, const b2Manifold* oldManifold)
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

        collision.setObjectA(PhysicObject::Cast(m_ObjectManager->findObject(id_A)));
        collision.setObjectB(PhysicObject::Cast(m_ObjectManager->findObject(id_B)));

        handleCollisionPreSolveContact(collision);
    }

    ////////////////////////////////////////////////////////////
    void Scene::postSolve(b2Contact* contact, const b2ContactImpulse* impulse)
    {
        Collision collision(contact, nullptr, impulse);

        int id_A = (int)contact->GetFixtureA()->GetBody()->GetUserData();
        int id_B = (int)contact->GetFixtureB()->GetBody()->GetUserData();

        collision.setObjectA(PhysicObject::Cast(m_ObjectManager->findObject(id_A)));
        collision.setObjectB(PhysicObject::Cast(m_ObjectManager->findObject(id_B)));

        handleCollisionPostSolveContact(collision);
    }

    ////////////////////////////////////////////////////////////
    void Scene::beginContact(b2Contact* contact)
    {
        Collision collision(contact, nullptr, nullptr);

        int id_A = (int)contact->GetFixtureA()->GetBody()->GetUserData();
        int id_B = (int)contact->GetFixtureB()->GetBody()->GetUserData();

        collision.setObjectA(PhysicObject::Cast(m_ObjectManager->findObject(id_A)));
        collision.setObjectB(PhysicObject::Cast(m_ObjectManager->findObject(id_B)));

        handleCollisionContactBegin(collision);
    }

    ////////////////////////////////////////////////////////////
    void Scene::endContact(b2Contact* contact)
    {
        Collision collision(contact, nullptr, nullptr);

        int id_A = (int)contact->GetFixtureA()->GetBody()->GetUserData();
        int id_B = (int)contact->GetFixtureB()->GetBody()->GetUserData();

        collision.setObjectA(PhysicObject::Cast(m_ObjectManager->findObject(id_A)));
        collision.setObjectB(PhysicObject::Cast(m_ObjectManager->findObject(id_B)));

        handleCollisionContactEnd(collision);
    }

    ////////////////////////////////////////////////////////////
    void Scene::setGravity(const sf::Vector2f& gravity)
    {
        m_PhysicWorld->SetGravity(sf_to_b2(gravity));
    }

    ////////////////////////////////////////////////////////////
    void Scene::setGravity(float gravity)
    {
        m_PhysicWorld->SetGravity(sf_to_b2(sf::Vector2f(0.f, gravity)));
    }

    ////////////////////////////////////////////////////////////
    void Scene::setCameraTarget(PhysicObject::Ptr target)
    {
        m_CameraTarget              = target;
        m_CameraFollowTarget        = true;
    }

    ////////////////////////////////////////////////////////////
    void Scene::setCameraTargetOffset(const float left, const float right, const float up, const float down)
    {
        m_CameraTargetOffset.left = left;
        m_CameraTargetOffset.right = right;
        m_CameraTargetOffset.up = up;
        m_CameraTargetOffset.down = down;
    }

    ////////////////////////////////////////////////////////////
    void Scene::cameraFollowTarget()
    {
        if(!m_CameraTarget || !m_CameraFollowTarget)
            return;

        float offset;

        //Horizontal offset
        sf::Vector2f camera_position = m_Context.camera->getPosition();
        sf::Vector2f target_position = m_CameraTarget->getWorldCenter();
        offset = target_position.x - camera_position.x;

        if(offset > m_CameraTargetOffset.right)
            m_Context.camera->setPosition(camera_position + sf::Vector2f(offset - m_CameraTargetOffset.right, 0.f));
        else if(offset < - m_CameraTargetOffset.left)
            m_Context.camera->setPosition(camera_position + sf::Vector2f(offset + m_CameraTargetOffset.left, 0.f));

        //Vertical offset
        camera_position = m_Context.camera->getPosition();
        offset = target_position.y - camera_position.y;

        if(offset > m_CameraTargetOffset.up)
            m_Context.camera->setPosition(camera_position + sf::Vector2f(0.f, offset - m_CameraTargetOffset.up));
        else if(offset < - m_CameraTargetOffset.down)
            m_Context.camera->setPosition(camera_position + sf::Vector2f(0.f, offset + m_CameraTargetOffset.down));

    }

    ////////////////////////////////////////////////////////////
    void Scene::setCameraFollowTarget(bool flag)
    {
        m_CameraFollowTarget = flag;
    }

        void Scene::init()
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

}
