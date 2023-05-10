////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/PhysicsInteractor.h>
#include <Nero/core/cpp/utility/String.h>
#include <Nero/core/cpp/utility/Graphics.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
// Box2d
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
// Std
#include <cstring>
////////////////////////////////////////////////////////////
namespace nero
{
    void DestructionListener::SayGoodbye(b2Fixture* fixture)
    {
        B2_NOT_USED(fixture);
    }

    void DestructionListener::SayGoodbye(b2Joint* joint)
    {
        if(physicsInteractor->m_MouseJoint == joint)
        {
            physicsInteractor->m_MouseJoint = nullptr;
        }
        else
        {
            physicsInteractor->jointDestroyed(joint);
        }
    }

    QueryCallback::QueryCallback(const b2Vec2& point)
    {
        m_Point   = point;
        m_Fixture = nullptr;
    }

    bool QueryCallback::ReportFixture(b2Fixture* fixture)
    {
        b2Body* body = fixture->GetBody();

        if(body->GetType() == b2_dynamicBody)
        {
            bool inside = fixture->TestPoint(m_Point);

            if(inside)
            {
                m_Fixture = fixture;

                // Clean pointer
                body      = nullptr;

                // We are done, terminate the query.
                return false;
            }
        }

        // Clean pointer
        body = nullptr;

        // Continue the query.
        return true;
    }

    QueryCallback::~QueryCallback()
    {
        m_Fixture = nullptr;
    }

    PhysicsInteractor::PhysicsInteractor(const FontHolder::Ptr& editorFontHolder)
        : m_DestructionListener()
        , m_Message("")
        , m_StatMessage("")
        , m_ProfileMessage("")
        , m_Bomb(nullptr)
        , m_BombSpawnPoint(0.f, 0.f)
        , m_BombSpawning(false)
        , m_GroundBody(nullptr)
        , m_WorldAABB()
        , m_MouseJoint(nullptr)
        , m_MouseWorld(0.f, 0.f)
        , m_StepCount(0)
        , m_MaxProfile()
        , m_TotalProfile()
        , m_IsLeftShift(false)
        , m_IsMouseRightButton(false)
        , m_PhysicsDataText()
        , m_EditorFontHolder(editorFontHolder)
    {
        m_DestructionListener.physicsInteractor = PhysicsInteractor::Ptr(this);

        // void * memset (void * ptr, int value, size_t num);
        // Sets the first num bytes of the block of memory pointed by ptr to the specified value
        // (interpreted as an unsigned char).
        memset(&m_MaxProfile, 0, sizeof(b2Profile));
        memset(&m_TotalProfile, 0, sizeof(b2Profile));

        // Physics Data Text
        m_PhysicsDataText.setFont(m_EditorFontHolder->getDefaultFont());
        m_PhysicsDataText.setCharacterSize(18.f);
        m_PhysicsDataText.setFillColor(sf::Color::White);
        m_PhysicsDataText.setPosition(sf::Vector2f(60.f, 20.f));
    }

    void PhysicsInteractor::initialize(std::shared_ptr<b2World>           physicsWorld,
                                       ShapeRenderer::Ptr                 shapeRenderer,
                                       Setting::Ptr                       levelSetting,
                                       RenderContext::Ptr                 renderContext,
                                       std::shared_ptr<sf::RenderTexture> renderTexture,
                                       std::shared_ptr<sf::RenderTexture> frontScreenTexture,
                                       AdvancedCamera::Ptr                editorCamera)
    {
        m_Bomb               = nullptr;
        m_GroundBody         = nullptr;
        m_MouseJoint         = nullptr;
        m_PhysicsWorld       = physicsWorld;
        m_ShapeRenderer      = shapeRenderer;
        m_RenderContext      = renderContext;
        m_RenderTexture      = renderTexture;
        m_FrontScreenTexture = frontScreenTexture;
        m_EditorCamera       = editorCamera;
        m_LevelSetting       = levelSetting;

        m_PhysicsWorld->SetDestructionListener(&m_DestructionListener);

        // Setup the ground
        b2BodyDef bodyDef;
        m_GroundBody = m_PhysicsWorld->CreateBody(&bodyDef);
    }

    void PhysicsInteractor::update(const sf::Time&)
    {
        if(!m_PhysicsWorld)
            return;

        const auto frequence       = m_LevelSetting->getSetting("physics").getFloat("frequency");
        float32    physicsTimeStep = frequence > 0.f ? 1.f / frequence : float32(0.0f);

        if(physicsTimeStep > 0.0f)
        {
            ++m_StepCount;
        }

        m_Message = m_StatMessage + m_ProfileMessage;
        m_PhysicsDataText.setString(m_Message);
    }

    void PhysicsInteractor::handleEvent(const sf::Event& event)
    {
        if(!m_PhysicsWorld)
            return;

        switch(event.type)
        {
            // Keyboard events
            case sf::Event::KeyPressed:
                handleKeyboardInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handleKeyboardInput(event.key.code, false);
                break;

                // Mouse buttons events
            case sf::Event::MouseButtonPressed:
                handleMouseButtonsInput(event.mouseButton, true);
                break;
            case sf::Event::MouseButtonReleased:
                handleMouseButtonsInput(event.mouseButton, false);
                break;

                // Mouse mouvements event
            case sf::Event::MouseMoved:
                handleMouseMoveInput(event.mouseMove);
                break;
        }
    }

    void PhysicsInteractor::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {
        // Handle only key pressed inputs
        if(isPressed)
        {
            if(key == sf::Keyboard::B)
                launchBomb();
        }

        if(key == sf::Keyboard::LShift)
            m_IsLeftShift = isPressed;
    }

    void PhysicsInteractor::handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse,
                                                    const bool&                        isPressed)
    {
        sf::Vector2f world_pos = m_RenderTexture->mapPixelToCoords(
            sf::Vector2i(m_RenderContext->mousePosition.x, m_RenderContext->mousePosition.y),
            m_RenderTexture->getView());

        b2Vec2 p = graphics::sf_to_b2(world_pos, EngineConstant.SCALE);

        if(mouse.button == sf::Mouse::Left && isPressed == true)
        {
            if(m_IsLeftShift)
                shiftMouseDown(p);
            else
                mouseDown(p);
        }
        else if(mouse.button == sf::Mouse::Left && isPressed == false)
        {
            mouseUp(p);
        }
        else if(mouse.button == sf::Mouse::Right)
        {
            if(isPressed)
            {
                m_LastMousePosition = p;
            }

            m_IsMouseRightButton = isPressed;
        }
    }

    void PhysicsInteractor::handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse)
    {
        sf::Vector2f world_pos = m_RenderTexture->mapPixelToCoords(
            sf::Vector2i(m_RenderContext->mousePosition.x, m_RenderContext->mousePosition.y),
            m_RenderTexture->getView());

        b2Vec2 p = graphics::sf_to_b2(world_pos, EngineConstant.SCALE);

        mouseMove(p);
    }

    void PhysicsInteractor::renderFrontScreen()
    {
        m_FrontScreenTexture->draw(m_PhysicsDataText);
    }

    void PhysicsInteractor::render()
    {
        if(!m_PhysicsWorld)
            return;

        auto physicsSetting = m_LevelSetting->getSetting("physics");

        if(physicsSetting.getBool("draw_statistics"))
        {
            // bodies/contacts/joints
            int32   bodyCount    = m_PhysicsWorld->GetBodyCount();
            int32   contactCount = m_PhysicsWorld->GetContactCount();
            int32   jointCount   = m_PhysicsWorld->GetJointCount();

            // proxies/height/balance/quality
            int32   proxyCount   = m_PhysicsWorld->GetProxyCount();
            int32   height       = m_PhysicsWorld->GetTreeHeight();
            int32   balance      = m_PhysicsWorld->GetTreeBalance();
            float32 quality      = m_PhysicsWorld->GetTreeQuality();

            m_StatMessage        = "body/contact/joint = " + toString(bodyCount) + " / " +
                            toString(contactCount) + " / " + toString(jointCount) +
                            "\n"
                            "proxy/height/balance/quality = " +
                            toString(proxyCount) + " / " + toString(height) + " / " +
                            toString(balance) + " / " + toString(quality) + "\n";
        }
        else
        {
            m_StatMessage = "";
        }

        // Profile data
        {
            const b2Profile& p           = m_PhysicsWorld->GetProfile();

            m_MaxProfile.step            = b2Max(m_MaxProfile.step, p.step);
            m_MaxProfile.collide         = b2Max(m_MaxProfile.collide, p.collide);
            m_MaxProfile.solve           = b2Max(m_MaxProfile.solve, p.solve);
            m_MaxProfile.solveInit       = b2Max(m_MaxProfile.solveInit, p.solveInit);
            m_MaxProfile.solveVelocity   = b2Max(m_MaxProfile.solveVelocity, p.solveVelocity);
            m_MaxProfile.solvePosition   = b2Max(m_MaxProfile.solvePosition, p.solvePosition);
            m_MaxProfile.solveTOI        = b2Max(m_MaxProfile.solveTOI, p.solveTOI);
            m_MaxProfile.broadphase      = b2Max(m_MaxProfile.broadphase, p.broadphase);

            m_TotalProfile.step          += p.step;
            m_TotalProfile.collide       += p.collide;
            m_TotalProfile.solve         += p.solve;
            m_TotalProfile.solveInit     += p.solveInit;
            m_TotalProfile.solveVelocity += p.solveVelocity;
            m_TotalProfile.solvePosition += p.solvePosition;
            m_TotalProfile.solveTOI      += p.solveTOI;
            m_TotalProfile.broadphase    += p.broadphase;
        }

        if(physicsSetting.getBool("draw_profile"))
        {
            const b2Profile& p = m_PhysicsWorld->GetProfile();

            b2Profile        aveProfile;
            memset(&aveProfile, 0, sizeof(b2Profile));

            if(m_StepCount > 0)
            {
                float32 scale            = 1.0f / m_StepCount;

                aveProfile.step          = scale * m_TotalProfile.step;
                aveProfile.collide       = scale * m_TotalProfile.collide;
                aveProfile.solve         = scale * m_TotalProfile.solve;
                aveProfile.solveInit     = scale * m_TotalProfile.solveInit;
                aveProfile.solveVelocity = scale * m_TotalProfile.solveVelocity;
                aveProfile.solvePosition = scale * m_TotalProfile.solvePosition;
                aveProfile.solveTOI      = scale * m_TotalProfile.solveTOI;
                aveProfile.broadphase    = scale * m_TotalProfile.broadphase;
            }

            m_ProfileMessage =
                "step [ave] (max) = " + toString(p.step) + " [" + toString(aveProfile.step) + "]" +
                "(" + toString(m_MaxProfile.step) + ") " +
                "\n"
                "collide [ave] (max) = " +
                toString(p.collide) + " [" + toString(aveProfile.collide) + "]" + "(" +
                toString(m_MaxProfile.collide) + ")" +
                "\n"
                "solve [ave] (max) = " +
                toString(p.solve) + " [" + toString(aveProfile.solve) + "]" + "(" +
                toString(m_MaxProfile.solve) + ")" +
                "\n"
                "solve init [ave] (max) = " +
                toString(p.solveInit) + " [" + toString(aveProfile.solveInit) + "]" + "(" +
                toString(m_MaxProfile.solveInit) + ")" +
                "\n"
                "solve velocity [ave] (max) = " +
                toString(p.solveVelocity) + " [" + toString(aveProfile.solveVelocity) + "]" + "(" +
                toString(m_MaxProfile.solveVelocity) + ")" +
                "\n"
                "solve position [ave] (max) = " +
                toString(p.solvePosition) + " [" + toString(aveProfile.solvePosition) + "]" + "(" +
                toString(m_MaxProfile.solvePosition) + ")" +
                "\n"
                "solveTOI [ave] (max) = " +
                toString(p.solveTOI) + " [" + toString(aveProfile.solveTOI) + "]" + "(" +
                toString(m_MaxProfile.solveTOI) + ")" +
                "\n"
                "broad-phase [ave] (max) = " +
                toString(p.broadphase) + " [" + toString(aveProfile.broadphase) + "]" + "(" +
                toString(m_MaxProfile.broadphase) + ")" + "\n";
        }
        else
        {
            m_ProfileMessage = "";
        }

        if(m_MouseJoint)
        {
            b2Vec2  p1 = m_MouseJoint->GetAnchorB();
            b2Vec2  p2 = m_MouseJoint->GetTarget();

            b2Color c;
            c.Set(0.0f, 1.0f, 0.0f);
            m_ShapeRenderer->DrawPoint(p1, 4.0f, c);
            m_ShapeRenderer->DrawPoint(p2, 4.0f, c);

            c.Set(0.8f, 0.8f, 0.8f);
            m_ShapeRenderer->DrawSegment(p1, p2, c);
        }

        if(m_BombSpawning)
        {
            b2Color c;
            c.Set(0.0f, 0.0f, 1.0f);
            m_ShapeRenderer->DrawPoint(m_BombSpawnPoint, 4.0f, c);

            c.Set(0.8f, 0.8f, 0.8f);
            m_ShapeRenderer->DrawSegment(m_MouseWorld, m_BombSpawnPoint, c);
        }
    }

    void PhysicsInteractor::shiftMouseDown(const b2Vec2& p)
    {
        m_MouseWorld = p;

        if(m_MouseJoint != nullptr)
        {
            return;
        }

        spawnBomb(p);
    }

    void PhysicsInteractor::mouseDown(const b2Vec2& p)
    {
        m_MouseWorld = p;

        if(m_MouseJoint != nullptr)
        {
            return;
        }

        // Make a small box.
        b2AABB aabb;
        b2Vec2 d;
        d.Set(0.001f, 0.001f);
        aabb.lowerBound = p - d;
        aabb.upperBound = p + d;

        // Query the world for overlapping shapes.
        QueryCallback callback(p);
        m_PhysicsWorld->QueryAABB(&callback, aabb);

        if(callback.m_Fixture)
        {
            b2Body*         body = callback.m_Fixture->GetBody();
            b2MouseJointDef md;
            md.bodyA     = m_GroundBody;
            md.bodyB     = body;
            md.target    = p;
            md.maxForce  = 1000.0f * body->GetMass();
            m_MouseJoint = (b2MouseJoint*)m_PhysicsWorld->CreateJoint(&md);
            body->SetAwake(true);
        }
    }

    void PhysicsInteractor::mouseUp(const b2Vec2& p)
    {
        if(m_MouseJoint)
        {
            m_PhysicsWorld->DestroyJoint(m_MouseJoint);
            m_MouseJoint = nullptr;
        }

        if(m_BombSpawning)
        {
            completeBombSpawn(p);
        }
    }

    void PhysicsInteractor::mouseMove(const b2Vec2& p)
    {
        m_MouseWorld = p;

        if(m_MouseJoint)
        {
            m_MouseJoint->SetTarget(p);
        }
    }

    void PhysicsInteractor::spawnBomb(const b2Vec2& worldPt)
    {
        m_BombSpawnPoint = worldPt;
        m_BombSpawning   = true;
    }

    void PhysicsInteractor::completeBombSpawn(const b2Vec2& p)
    {
        if(m_BombSpawning == false)
        {
            return;
        }

        const float multiplier = 30.0f;
        b2Vec2      vel        = m_BombSpawnPoint - p;
        vel                    *= multiplier;
        launchBomb(m_BombSpawnPoint, vel);
        m_BombSpawning = false;
    }

    void PhysicsInteractor::launchBomb()
    {
        sf::Vector2f spaw_pos = sf::Vector2f(m_EditorCamera->getPosition().x,
                                             m_EditorCamera->getPosition().y -
                                                 m_RenderTexture->getView().getSize().y / 2.f);

        b2Vec2       p        = graphics::sf_to_b2(spaw_pos, EngineConstant.SCALE);
        p.x                   = p.x + math::randomFloat(-30.0f, 30.0f);
        b2Vec2 v              = -5.0f * b2Vec2(math::randomFloat(-15.0f, 15.0f), -30.0f);
        launchBomb(p, v);
    }

    void PhysicsInteractor::destroyBomb()
    {
        if(m_Bomb)
        {
            m_PhysicsWorld->DestroyBody(m_Bomb);
            m_Bomb = nullptr;
        }
    }

    void PhysicsInteractor::launchBomb(const b2Vec2& position, const b2Vec2& velocity)
    {
        if(m_Bomb)
        {
            m_PhysicsWorld->DestroyBody(m_Bomb);
            m_Bomb = nullptr;
        }

        b2BodyDef bd;
        bd.type     = b2_dynamicBody;
        bd.position = position;
        bd.bullet   = true;
        m_Bomb      = m_PhysicsWorld->CreateBody(&bd);
        m_Bomb->SetLinearVelocity(velocity);

        b2CircleShape circle;
        circle.m_radius = 0.2f;

        b2FixtureDef fd;
        fd.shape       = &circle;
        fd.density     = 20.0f;
        fd.restitution = 0.0f;

        b2Vec2 minV    = position - b2Vec2(0.3f, 0.3f);
        b2Vec2 maxV    = position + b2Vec2(0.3f, 0.3f);

        b2AABB aabb;
        aabb.lowerBound = minV;
        aabb.upperBound = maxV;

        m_Bomb->CreateFixture(&fd);
    }

    void PhysicsInteractor::jointDestroyed(b2Joint* joint)
    {
        B2_NOT_USED(joint);
    }

} // namespace nero
