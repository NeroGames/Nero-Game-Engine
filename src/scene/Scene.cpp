#include <NERO/scene/Scene.h>

//STD
#include <stdio.h>
//NERO
#include <NERO/Utility.h>
#include <NERO/object/LayerObject.h>
//SFML
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>


namespace nero
{
    Scene::Context::Context(sfg::Canvas::Ptr& renderCanvas, sf::View& canvasDefaultView, Camera& camera, ResourceManager& resourceManager):
        renderCanvas(renderCanvas),
        canvasDefaultView(canvasDefaultView),
        camera(camera),
        resourceManager(resourceManager)
    {
        //
    }

    Scene::Scene(Context context):
        m_Context(context),
        m_RenderCanvas(context.renderCanvas),
        m_ShapeRender(context.renderCanvas),
        m_PointCount(0),
        m_Text(),
        m_PauseMessage(""),
        m_RootObject(Object::Ptr(new Object)),
        m_ObjectManager(m_RootObject, m_World),
        m_CameraTargetOffsetLeft(0.f),
        m_CameraTargetOffsetRight(0.f),
        m_CameraTargetOffsetUp(0.f),
        m_CameraTargetOffsetDown(0.f),
        m_CameraFollowTarget(false)
    {
        //Setup the world
        m_World = new b2World(b2Vec2(0.0f, GRAVITY));
        m_World->SetContactListener(this);
        m_World->SetDebugDraw(&m_ShapeRender);

        //Text
        m_Text.setFont(context.resourceManager.Font.get("Sansation"));
        m_Text.setCharacterSize(20.f);
        m_Text.setFillColor(sf::Color::White);
        m_Text.setPosition(sf::Vector2f(10.f, 10.f));
    }

    Scene::~Scene()
    {
        // By deleting the world, we delete the bomb, mouse joint, etc.
        delete m_World;
        m_World = nullptr;
    }

    void Scene::renderOnFrontScreen()
    {
            m_RenderCanvas->Draw(m_Text);
    }

    void Scene::update(SceneSettings* settings)
    {
        m_ObjectManager.setWorld(m_World);

        if(!m_World->IsLocked())
            m_ObjectManager.removeDeadPhysicObject();

        float32 timeStep = settings->hz > 0.0f ? 1.0f / settings->hz : float32(0.0f);

        if (settings->pause)
        {
            if (settings->singleStep)
            {
                settings->singleStep = 0;
            }
            else
            {
                timeStep = 0.0f;
            }

            m_PauseMessage = "<-- PAUSED -->";
        }
        else
        {
            m_PauseMessage = "";
        }

        uint32 flags = 0;
        flags += settings->drawShapes			* b2Draw::e_shapeBit;
        flags += settings->drawJoints			* b2Draw::e_jointBit;
        flags += settings->drawAABBs			* b2Draw::e_aabbBit;
        flags += settings->drawCOMs				* b2Draw::e_centerOfMassBit;
        m_ShapeRender.SetFlags(flags);

        m_World->SetAllowSleeping(settings->enableSleep > 0);
        m_World->SetWarmStarting(settings->enableWarmStarting > 0);
        m_World->SetContinuousPhysics(settings->enableContinuous > 0);
        m_World->SetSubStepping(settings->enableSubStepping > 0);

        m_PointCount = 0;

        m_World->Step(timeStep, settings->velocityIterations, settings->positionIterations);
        m_RootObject->update(sf::seconds(timeStep));


        m_Text.setString(m_PauseMessage);

        cameraFollowTarget();
    }

    void Scene::render()
    {
        auto children = m_RootObject->getAllChild();

        for(auto it = children->begin(); it != children->end(); it++)
        {
            LayerObject::Ptr layer_object = std::static_pointer_cast<LayerObject>(*it);

            if(layer_object->isVisible())
                m_RenderCanvas->Draw(*layer_object);
        }
    }

    void Scene::renderShape()
    {
        m_World->DrawDebugData();
    }

    void Scene::handleEvent(const sf::Event& event)
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
                handleMouseButtonsInput(event.mouseButton, true);
                break;
            case sf::Event::MouseButtonReleased:
                handleMouseButtonsInput(event.mouseButton, false);
                break;

            //Mouse mouvements event
            case sf::Event::MouseMoved:
                handleMouseMoveInput(event.mouseMove);
                break;
        }
    }

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

        collision.setObjectA(std::static_pointer_cast<nero::PhysicObject>(m_ObjectManager.findObject(id_A)));
        collision.setObjectB(std::static_pointer_cast<nero::PhysicObject>(m_ObjectManager.findObject(id_B)));

        handleCollisionPreSolveContact(collision);

    }

    void Scene::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
    {
//        B2_NOT_USED(contact);
//        B2_NOT_USED(impulse);

        Collision collision(contact, nullptr, impulse);

        int id_A = (int)contact->GetFixtureA()->GetBody()->GetUserData();
        int id_B = (int)contact->GetFixtureB()->GetBody()->GetUserData();

        collision.setObjectA(nero::PhysicObject::Cast(m_ObjectManager.findObject(id_A)));
        collision.setObjectB(nero::PhysicObject::Cast(m_ObjectManager.findObject(id_B)));

        handleCollisionPostSolveContact(collision);
    }

    void Scene::BeginContact(b2Contact* contact)
    {
        //B2_NOT_USED(contact);

        Collision collision(contact, nullptr, nullptr);

        int id_A = (int)contact->GetFixtureA()->GetBody()->GetUserData();
        int id_B = (int)contact->GetFixtureB()->GetBody()->GetUserData();

        collision.setObjectA(nero::PhysicObject::Cast(m_ObjectManager.findObject(id_A)));
        collision.setObjectB(nero::PhysicObject::Cast(m_ObjectManager.findObject(id_B)));

        handleCollisionContactBegin(collision);
    }

    void Scene::EndContact(b2Contact* contact)
    {

        //B2_NOT_USED(contact);

        Collision collision(contact, nullptr, nullptr);

        int id_A = (int)contact->GetFixtureA()->GetBody()->GetUserData();
        int id_B = (int)contact->GetFixtureB()->GetBody()->GetUserData();

        collision.setObjectA(nero::PhysicObject::Cast(m_ObjectManager.findObject(id_A)));
        collision.setObjectB(nero::PhysicObject::Cast(m_ObjectManager.findObject(id_B)));

        handleCollisionContactEnd(collision);
    }

    void Scene::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {
        //
    }

    void Scene::handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed)
    {
        //
    }

    void Scene::handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse)
    {
        //
    }

    void Scene::init()
    {
        //
    }

    void Scene::handleCollisionContactBegin(Collision collision)
    {
        //
    }

    void Scene::handleCollisionContactEnd(Collision collision)
    {
        //
    }

    void Scene::handleCollisionPreSolveContact(Collision collision)
    {
        //
    }

    void Scene::handleCollisionPostSolveContact(Collision collision)
    {
        //
    }

    void Scene::setGravity(const sf::Vector2f& gravity)
    {
        m_World->SetGravity(sf_to_b2(gravity));
    }

    void Scene::setGravity(float gravity)
    {
        m_World->SetGravity(sf_to_b2(sf::Vector2f(0.f, gravity)));
    }

    void Scene::setCameraTarget(PhysicObject::Ptr target, float offset_left, float offset_right, float offset_up, float offset_down)
    {
        m_CameraTarget              = target;
        m_CameraTargetOffsetLeft    = offset_left;
        m_CameraTargetOffsetRight   = offset_right;
        m_CameraTargetOffsetUp      = offset_up;
        m_CameraTargetOffsetDown    = offset_down;
        m_CameraFollowTarget        = true;
    }

    void Scene::cameraFollowTarget()
    {
        if(!m_CameraTarget || !m_CameraFollowTarget)
            return;

        sf::Vector2f camera_position = m_Context.camera.getPosition();
        sf::Vector2f target_position = m_CameraTarget->getWorldCenter();
        float offset;

        offset = target_position.x - camera_position.x;

        if(offset > m_CameraTargetOffsetRight)
            m_Context.camera.setPosition(camera_position + sf::Vector2f(offset - m_CameraTargetOffsetRight, 0.f));
        else if(offset < - m_CameraTargetOffsetLeft)
            m_Context.camera.setPosition(camera_position + sf::Vector2f(offset + m_CameraTargetOffsetLeft, 0.f));

        camera_position = m_Context.camera.getPosition();
        offset = target_position.y - camera_position.y;

        if(offset > m_CameraTargetOffsetUp)
            m_Context.camera.setPosition(camera_position + sf::Vector2f(0.f, offset - m_CameraTargetOffsetUp));
        else if(offset < - m_CameraTargetOffsetDown)
            m_Context.camera.setPosition(camera_position + sf::Vector2f(0.f, offset + m_CameraTargetOffsetDown));

    }

    void Scene::setCameraFollowTarget(bool flag)
    {
        m_CameraFollowTarget = flag;
    }



}
