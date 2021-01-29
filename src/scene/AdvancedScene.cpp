////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/scene/AdvancedScene.h>
//BOOST
#include <boost/algorithm/string.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    //DestructionListener
    void DestructionListener::SayGoodbye(b2Fixture* fixture)
    {
        B2_NOT_USED(fixture);
    }

    void DestructionListener::SayGoodbye(b2Joint* joint)
    {
        if (scene->m_MouseJoint == joint)
        {
            scene->m_MouseJoint = NULL;
        }
        else
        {
            scene->jointDestroyed(joint);
        }
    }

    ////////////////////////////////////////////////////////////
    //AdvancedScene
    AdvancedScene::AdvancedScene(Scene::Context context):
         m_Scene(nullptr)
        ,m_Context(context)
        ,m_DestructionListener()
        //
        ,m_Message("")
        ,m_StatMessage("")
        ,m_ProfileMessage("")
        ,m_Bomb(nullptr)
        ,m_BombSpawning(false)
        ,m_BombSpawnPoint(0.f, 0.f)
        ,m_MouseJoint(nullptr)
        ,m_GroundBody(nullptr)
        ,m_WorldAABB()
        ,m_MouseWorld(0.f, 0.f)
        ,m_StepCount(0)
        ,m_MaxProfile()
        ,m_TotalProfile()
        ,m_SceneSetting()
        ,m_CameraSetting()
        ,m_CanvasColor(sf::Color::Black)
    {
        //Manager
        m_DestructionListener.scene = AdvancedScene::Ptr(this);
        //Scene
        m_SoundManager              = SoundManager::Ptr(new SoundManager(context.resourceManager->music, context.resourceManager->sound));
        //World
        m_SceneBuilder              = SceneBuilder::Ptr(new SceneBuilder(context.renderCanvas, context.resourceManager, m_SceneSetting));
        m_UndoManager               = UndoManager::Ptr(new UndoManager());
        m_Grid                      = Grid::Ptr(new Grid());

        //Front Screen
        addScreen(DEFAULT_FRONTSREEN);
        selectScreen(DEFAULT_FRONTSREEN);
        m_CurrentScreen = DEFAULT_FRONTSREEN;

        //Add the default layer
        m_SceneBuilder->addLayer();

        //void * memset (void * ptr, int value, size_t num);
        //Sets the first num bytes of the block of memory pointed by ptr to the specified value (interpreted as an unsigned char).
        memset(&m_MaxProfile, 0, sizeof(b2Profile));
        memset(&m_TotalProfile, 0, sizeof(b2Profile));

        //callbacks
        m_UpdateUI      = [](){};
        m_UpdateUndo    = [](){};
        m_UpdateLog     = [](const std::string&, int){};
        m_UpdateLogIf   = [](const std::string&, bool, int){};
    }

    ////////////////////////////////////////////////////////////
    AdvancedScene::~AdvancedScene()
    {

    }

    ////////////////////////////////////////////////////////////
    void AdvancedScene::init()
    {
        m_Scene->m_PhysicWorld->SetDestructionListener(&m_DestructionListener);

        //Setup the ground
        b2BodyDef bodyDef;
        m_GroundBody = m_Scene->m_PhysicWorld->CreateBody(&bodyDef);
    }

    ////////////////////////////////////////////////////////////
    void AdvancedScene::setScene(Scene::Ptr scene)
    {
        m_Scene = scene;
    }

    ////////////////////////////////////////////////////////////
    SceneBuilder::Ptr AdvancedScene::getSceneBuilder()
    {
        return m_SceneBuilder;
    }

    ////////////////////////////////////////////////////////////
    SceneBuilder::Ptr AdvancedScene::getScreenBuilder()
    {
        auto frontSceen = std::find_if(m_FrontScreenTable.begin(), m_FrontScreenTable.end(), [&](FrontScreen screen){return screen.name == m_CurrentScreen;});
        return frontSceen->screenBuilder;
    }

    ////////////////////////////////////////////////////////////
    UndoManager::Ptr AdvancedScene::getUndoManager()
    {
        return m_UndoManager;
    }

    ////////////////////////////////////////////////////////////
    UndoManager::Ptr AdvancedScene::getScreenUndoManager()
    {
        auto frontSceen = std::find_if(m_FrontScreenTable.begin(), m_FrontScreenTable.end(), [&](FrontScreen screen){return screen.name == m_CurrentScreen;});
        return frontSceen->undoManager;
    }

    ////////////////////////////////////////////////////////////
    Grid::Ptr AdvancedScene::getFrontScreenGrid()
    {
        auto frontSceen = std::find_if(m_FrontScreenTable.begin(), m_FrontScreenTable.end(), [&](FrontScreen screen){return screen.name == m_CurrentScreen;});
        return frontSceen->grid;
    }

     CameraSetting& AdvancedScene::getScreenCameraSetting()
     {
        auto frontSceen = std::find_if(m_FrontScreenTable.begin(), m_FrontScreenTable.end(), [&](FrontScreen screen){return screen.name == m_CurrentScreen;});
        return frontSceen->cameraSetting;
     }

    sf::Color AdvancedScene::getScreenCanvasColor()
    {
        auto frontSceen = std::find_if(m_FrontScreenTable.begin(), m_FrontScreenTable.end(), [&](FrontScreen screen){return screen.name == m_CurrentScreen;});
        return frontSceen->canvasColor;
    }


    ////////////////////////////////////////////////////////////
    void AdvancedScene::setName(std::string name)
    {
        m_SceneName = name;
    }

    ////////////////////////////////////////////////////////////
    void AdvancedScene::update(const sf::Time& timeStep)
    {
        if(m_Scene)
        {
            m_Scene->m_SceneSetting = m_SceneSetting;
            m_Scene->m_PhysicWorld->SetGravity(m_SceneSetting.gravity);
            m_Scene->update(timeStep);

            if(m_SceneSetting.pause && m_SceneSetting.singleStep)
                m_SceneSetting.singleStep = false;
        }

        float32 b2TimeStep = m_SceneSetting.hz > 0.0f ? 1.0f / m_SceneSetting.hz : float32(0.0f);

        if (b2TimeStep > 0.0f)
        {
            ++m_StepCount;
        }

        m_Message = m_StatMessage + m_ProfileMessage + m_Scene->m_PauseMessage;
        m_Scene->m_Text.setString(m_Message);
    }

    ////////////////////////////////////////////////////////////
    void AdvancedScene::renderDebug()
    {
        if (m_SceneSetting.drawStats)
        {
            //bodies/contacts/joints
            int32 bodyCount     = m_Scene->m_PhysicWorld->GetBodyCount();
            int32 contactCount  = m_Scene->m_PhysicWorld->GetContactCount();
            int32 jointCount    = m_Scene->m_PhysicWorld->GetJointCount();

            //proxies/height/balance/quality
            int32 proxyCount    = m_Scene->m_PhysicWorld->GetProxyCount();
            int32 height        = m_Scene->m_PhysicWorld->GetTreeHeight();
            int32 balance       = m_Scene->m_PhysicWorld->GetTreeBalance();
            float32 quality     = m_Scene->m_PhysicWorld->GetTreeQuality();

            m_StatMessage = "body/contact/joint = " + toString(bodyCount) + " / " +  toString(contactCount) + " / " + toString(jointCount) + "\n" \
                            "proxy/height/balance/quality = " + toString(proxyCount) + " / " + toString(height) + " / " +  toString(balance) + " / " + toString(quality) + "\n";
        }
        else
        {
            m_StatMessage = "";
        }

        // Track maximum profile times
        {
            const b2Profile& p = m_Scene->m_PhysicWorld->GetProfile();

            m_MaxProfile.step               = b2Max(m_MaxProfile.step, p.step);
            m_MaxProfile.collide            = b2Max(m_MaxProfile.collide, p.collide);
            m_MaxProfile.solve              = b2Max(m_MaxProfile.solve, p.solve);
            m_MaxProfile.solveInit          = b2Max(m_MaxProfile.solveInit, p.solveInit);
            m_MaxProfile.solveVelocity      = b2Max(m_MaxProfile.solveVelocity, p.solveVelocity);
            m_MaxProfile.solvePosition      = b2Max(m_MaxProfile.solvePosition, p.solvePosition);
            m_MaxProfile.solveTOI           = b2Max(m_MaxProfile.solveTOI, p.solveTOI);
            m_MaxProfile.broadphase         = b2Max(m_MaxProfile.broadphase, p.broadphase);

            m_TotalProfile.step             += p.step;
            m_TotalProfile.collide          += p.collide;
            m_TotalProfile.solve            += p.solve;
            m_TotalProfile.solveInit        += p.solveInit;
            m_TotalProfile.solveVelocity    += p.solveVelocity;
            m_TotalProfile.solvePosition    += p.solvePosition;
            m_TotalProfile.solveTOI         += p.solveTOI;
            m_TotalProfile.broadphase       += p.broadphase;
        }

        if (m_SceneSetting.drawProfile)
        {
            const b2Profile& p = m_Scene->m_PhysicWorld->GetProfile();

            b2Profile aveProfile;
            memset(&aveProfile, 0, sizeof(b2Profile));

            if (m_StepCount > 0)
            {
                float32 scale = 1.0f / m_StepCount;

                aveProfile.step             = scale * m_TotalProfile.step;
                aveProfile.collide          = scale * m_TotalProfile.collide;
                aveProfile.solve            = scale * m_TotalProfile.solve;
                aveProfile.solveInit        = scale * m_TotalProfile.solveInit;
                aveProfile.solveVelocity    = scale * m_TotalProfile.solveVelocity;
                aveProfile.solvePosition    = scale * m_TotalProfile.solvePosition;
                aveProfile.solveTOI         = scale * m_TotalProfile.solveTOI;
                aveProfile.broadphase       = scale * m_TotalProfile.broadphase;
            }

            m_ProfileMessage =  "step [ave] (max) = "           + toString(p.step)          + " [" + toString(aveProfile.step)          + "]" +  "(" + toString(m_MaxProfile.step)          + ") " + "\n" \
                                "collide [ave] (max) = "        + toString(p.collide)       + " [" + toString(aveProfile.collide)       + "]" +  "(" + toString(m_MaxProfile.collide)       + ")" + "\n" \
                                "solve [ave] (max) = "          + toString(p.solve)         + " [" + toString(aveProfile.solve)         + "]" +  "(" + toString(m_MaxProfile.solve)         + ")" + "\n" \
                                "solve init [ave] (max) = "     + toString(p.solveInit)     + " [" + toString(aveProfile.solveInit)     + "]" +  "(" + toString(m_MaxProfile.solveInit)     + ")" + "\n" \
                                "solve velocity [ave] (max) = " + toString(p.solveVelocity) + " [" + toString(aveProfile.solveVelocity) + "]" +  "(" + toString(m_MaxProfile.solveVelocity) + ")" + "\n" \
                                "solve position [ave] (max) = " + toString(p.solvePosition) + " [" + toString(aveProfile.solvePosition) + "]" +  "(" + toString(m_MaxProfile.solvePosition) + ")" + "\n" \
                                "solveTOI [ave] (max) = "       + toString(p.solveTOI)      + " [" + toString(aveProfile.solveTOI)      + "]" +  "(" + toString(m_MaxProfile.solveTOI)      + ")" + "\n" \
                                "broad-phase [ave] (max) = "    + toString(p.broadphase)    + " [" + toString(aveProfile.broadphase)    + "]" +  "(" + toString(m_MaxProfile.broadphase)    + ")" + "\n";
        }
        else
        {
            m_ProfileMessage = "";
        }


        if (m_MouseJoint)
        {
            b2Vec2 p1 = m_MouseJoint->GetAnchorB();
            b2Vec2 p2 = m_MouseJoint->GetTarget();

            b2Color c;
            c.Set(0.0f, 1.0f, 0.0f);
            m_Scene->m_ShapeRenderer.DrawPoint(p1, 4.0f, c);
            m_Scene->m_ShapeRenderer.DrawPoint(p2, 4.0f, c);

            c.Set(0.8f, 0.8f, 0.8f);
            m_Scene->m_ShapeRenderer.DrawSegment(p1, p2, c);
        }

        if (m_BombSpawning)
        {
            b2Color c;
            c.Set(0.0f, 0.0f, 1.0f);
            m_Scene->m_ShapeRenderer.DrawPoint(m_BombSpawnPoint, 4.0f, c);

            c.Set(0.8f, 0.8f, 0.8f);
            m_Scene->m_ShapeRenderer.DrawSegment(m_MouseWorld, m_BombSpawnPoint, c);
        }

        if (m_SceneSetting.drawContactPoints)
        {
            const float32 k_impulseScale = 0.1f;
            const float32 k_axisScale = 0.3f;

            for (int32 i = 0; i < m_Scene->m_PointCount; ++i)
            {
                ContactPoint* point = m_Scene->m_Points + i;

                if (point->state == b2_addState)
                {
                    // Add
                    m_Scene->m_ShapeRenderer.DrawPoint(point->position, 10.0f, b2Color(0.3f, 0.95f, 0.3f));
                }
                else if (point->state == b2_persistState)
                {
                    // Persist
                    m_Scene->m_ShapeRenderer.DrawPoint(point->position, 5.0f, b2Color(0.3f, 0.3f, 0.95f));
                }

                if (m_SceneSetting.drawContactNormals == 1)
                {
                    b2Vec2 p1 = point->position;
                    b2Vec2 p2 = p1 + k_axisScale * point->normal;
                    m_Scene->m_ShapeRenderer.DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.9f));
                }
                else if (m_SceneSetting.drawContactImpulse == 1)
                {
                    b2Vec2 p1 = point->position;
                    b2Vec2 p2 = p1 + k_impulseScale * point->normalImpulse * point->normal;
                    m_Scene->m_ShapeRenderer.DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.3f));
                }

                if (m_SceneSetting.drawFrictionImpulse == 1)
                {
                    b2Vec2 tangent = b2Cross(point->normal, 1.0f);
                    b2Vec2 p1 = point->position;
                    b2Vec2 p2 = p1 + k_impulseScale * point->tangentImpulse * tangent;
                    m_Scene->m_ShapeRenderer.DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.3f));
                }
            }
        }
    }

     ////////////////////////////////////////////////////////////
    void AdvancedScene::shiftMouseDown(const b2Vec2& p)
    {
        m_MouseWorld = p;

        if (m_MouseJoint != NULL)
        {
            return;
        }

        spawnBomb(p);
    }

    ////////////////////////////////////////////////////////////
    void AdvancedScene::mouseDown(const b2Vec2& p)
    {
        m_MouseWorld = p;

        if (m_MouseJoint != NULL)
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
        m_Scene->m_PhysicWorld->QueryAABB(&callback, aabb);

        if (callback.m_Fixture)
        {
            b2Body* body = callback.m_Fixture->GetBody();
            b2MouseJointDef md;
            md.bodyA = m_GroundBody;
            md.bodyB = body;
            md.target = p;
            md.maxForce = 1000.0f * body->GetMass();
            m_MouseJoint = (b2MouseJoint*)m_Scene->m_PhysicWorld->CreateJoint(&md);
            body->SetAwake(true);
        }
    }

    ////////////////////////////////////////////////////////////
    void AdvancedScene::mouseUp(const b2Vec2& p)
    {
        if (m_MouseJoint)
        {
            m_Scene->m_PhysicWorld->DestroyJoint(m_MouseJoint);
            m_MouseJoint = NULL;
        }

        if (m_BombSpawning)
        {
            completeBombSpawn(p);
        }
    }

    ////////////////////////////////////////////////////////////
    void AdvancedScene::mouseMove(const b2Vec2& p)
    {
        m_MouseWorld = p;

        if (m_MouseJoint)
        {
            m_MouseJoint->SetTarget(p);
        }
    }

    ////////////////////////////////////////////////////////////
    void AdvancedScene::spawnBomb(const b2Vec2& worldPt)
    {
        m_BombSpawnPoint = worldPt;
        m_BombSpawning = true;
    }

    ////////////////////////////////////////////////////////////
    void AdvancedScene::completeBombSpawn(const b2Vec2& p)
    {
        if (m_BombSpawning == false)
        {
            return;
        }

        const float multiplier = 30.0f;
        b2Vec2 vel = m_BombSpawnPoint - p;
        vel *= multiplier;
        launchBomb(m_BombSpawnPoint,vel);
        m_BombSpawning = false;
    }

    ////////////////////////////////////////////////////////////
    void AdvancedScene::launchBomb()
    {
        sf::Vector2f spaw_pos = sf::Vector2f(m_Context.camera->getPosition().x, m_Context.camera->getPosition().y - m_Context.renderCanvas->GetView().getSize().y / 2.f);
        b2Vec2 p = sf_to_b2(spaw_pos, SCALE);
        p.x = p.x + randomFloat(-30.0f, 30.0f);
        b2Vec2 v = -5.0f * b2Vec2(randomFloat(-15.0f, 15.0f), -30.0f);
        launchBomb(p, v);
    }
    ////////////////////////////////////////////////////////////
    void AdvancedScene::destroyBomb()
    {
        if (m_Bomb)
        {
            m_Scene->m_PhysicWorld->DestroyBody(m_Bomb);
            m_Bomb = nullptr;
        }
    }

    ////////////////////////////////////////////////////////////
    void AdvancedScene::launchBomb(const b2Vec2& position, const b2Vec2& velocity)
    {
        if (m_Bomb)
        {
            m_Scene->m_PhysicWorld->DestroyBody(m_Bomb);
            m_Bomb = nullptr;
        }

        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.position = position;
        bd.bullet = true;
        m_Bomb = m_Scene->m_PhysicWorld->CreateBody(&bd);
        m_Bomb->SetLinearVelocity(velocity);

        b2CircleShape circle;
        circle.m_radius = 0.2f;

        b2FixtureDef fd;
        fd.shape = &circle;
        fd.density = 20.0f;
        fd.restitution = 0.0f;

        b2Vec2 minV = position - b2Vec2(0.3f,0.3f);
        b2Vec2 maxV = position + b2Vec2(0.3f,0.3f);

        b2AABB aabb;
        aabb.lowerBound = minV;
        aabb.upperBound = maxV;

        m_Bomb->CreateFixture(&fd);
    }

    ////////////////////////////////////////////////////////////
    void AdvancedScene::jointDestroyed(b2Joint* joint)
    {
        B2_NOT_USED(joint);
    }

    void AdvancedScene::selectScreen(const std::string& name)
    {
        m_CurrentScreen = name;
    }

    bool AdvancedScene::loadScreen(const std::string& name)
    {
        addScreen(name, true);
    }


    bool AdvancedScene::addScreen(const std::string& _name, bool loading)
    {
        std::string name = _name;
        boost::algorithm::trim(name);

        //Cannot have two screen with the same name
        for(auto screen : m_FrontScreenTable)
        {
            if(screen.name == name || name == "")
                return false;
        }

        if(m_UpdateLog && !loading)
        {
            m_UpdateLog("adding new Screen [" + name +"]", nero::Info);
        }

        FrontScreen frontScreen;
        frontScreen.screenBuilder                   = SceneBuilder::Ptr(new SceneBuilder(m_Context.renderCanvas, m_Context.resourceManager, m_SceneSetting));
        frontScreen.undoManager                     = UndoManager::Ptr(new UndoManager());
        frontScreen.grid                            = Grid::Ptr(new Grid());
        frontScreen.name                            = name;
        frontScreen.cameraSetting.position          = sf::Vector2f(400.f, 303.f);
        frontScreen.cameraSetting.defaultPosition   = sf::Vector2f(400.f, 303.f);
        frontScreen.cameraSetting.zoom              = -36;

        m_FrontScreenTable.push_back(frontScreen);

        m_FrontScreenTable.back().screenBuilder->addLayer();

        if(m_UpdateUI && m_UpdateUndo && m_UpdateLog && m_UpdateLogIf && !loading)
        {
            frontScreen.screenBuilder->setUpdateUI(m_UpdateUI);
            frontScreen.screenBuilder->setUpdateUndo(m_UpdateUndo);
            frontScreen.screenBuilder->setUpdateLog(m_UpdateLog);
            frontScreen.screenBuilder->setUpdateLogIf(m_UpdateLogIf);
            frontScreen.undoManager->add(frontScreen.screenBuilder->saveScene());
        }

        return true;
    }

    std::vector<std::string> AdvancedScene::getScreenTable()
    {
        std::vector<std::string> screenTable;

        for(auto screen : m_FrontScreenTable)
        {
           screenTable.push_back(screen.name);
        }

        return screenTable;
    }

    SoundManager::Ptr AdvancedScene::getSoundManager()
    {
        return m_SoundManager;
    }

    bool AdvancedScene::deleteScreen(const std::string& name)
    {
        //At least one Screen my stay, the current screen cannot be delete
        if(m_FrontScreenTable.size() == 1 || name == m_CurrentScreen)
            return false;;

        for(auto it = m_FrontScreenTable.begin(); it != m_FrontScreenTable.end(); it++)
        {
            if(it->name == name)
            {
                m_UpdateLog("removing Screen [" + name +"]", nero::Info);

                m_FrontScreenTable.erase(it);
                return true;
            }
        }

        return false;
    }

    bool AdvancedScene::renameScreen(const std::string& name, const std::string& _newName)
    {
        std::string newName = _newName;
        boost::algorithm::trim(newName);

        //Cannot have two screen with the same name
        for(auto screen : m_FrontScreenTable)
        {
            if(screen.name == newName || newName == "")
                return false;
        }

        bool result = false;
        for(auto it = m_FrontScreenTable.begin(); it != m_FrontScreenTable.end(); it++)
        {
            if(it->name == name)
            {
                m_UpdateLog("renaming Screen [" + name +"] to [" + newName +"]", nero::Info);

                it->name = newName;
                result = true;
                break;
            }
        }

        if(m_CurrentScreen == name)
            m_CurrentScreen = newName;

        return result;
    }

    void AdvancedScene::setScreenCanvasColor(const sf::Color& color)
    {
        auto frontSceen = std::find_if(m_FrontScreenTable.begin(), m_FrontScreenTable.end(), [&](FrontScreen screen){return screen.name == m_CurrentScreen;});
        frontSceen->canvasColor = color;
    }

     void AdvancedScene::setUpdateUI(std::function<void()>  fn)
    {
        m_UpdateUI = fn;
    }

    void AdvancedScene::setUpdateUndo(std::function<void()>  fn)
    {
        m_UpdateUndo = fn;
    }

    void AdvancedScene::setUpdateLog(std::function<void(const std::string&, int)>  fn)
    {
        m_UpdateLog = fn;
    }

    void AdvancedScene::setUpdateLogIf(std::function<void(const std::string&, bool, int)>  fn)
    {
        m_UpdateLogIf = fn;
    }
}
