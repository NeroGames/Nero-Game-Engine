////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/scene/PhysicsManager.h>
#include <Nero/core/cpp/utility/Utility.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
// BOX2D
#include <Box2D/Collision/Shapes/b2EdgeShape.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Collision/Shapes/b2ChainShape.h>
////////////////////////////////////////////////////////////
namespace nero
{
    PhysicsManager::PhysicsManager()
        : m_PhysicsWorld(nullptr)
    {
        // ctor
    }

    PhysicsManager::~PhysicsManager()
    {
        m_PhysicsWorld = nullptr;
    }

    void PhysicsManager::setPhysicsWorld(std::shared_ptr<b2World> physicsWorld)
    {
        m_PhysicsWorld = physicsWorld;
    }

    PhysicsObject::Ptr PhysicsManager::createObject(PointMesh::Ptr        pointMesh,
                                                    const PhysicsPoperty& physics)
    {
        if(pointMesh->getMeshShape() == PointMesh::None)
            return nullptr;

        setupBodyDef(pointMesh, physics);

        PhysicsObject::Ptr physicObject =
            std::make_shared<PhysicsObject>(m_PhysicsWorld->CreateBody(&m_BodyDef));

        switch(pointMesh->getMeshShape())
        {
            case PointMesh::Line:
            {
                b2EdgeShape shape;
                b2Vec2      vertexTable[2];
                setupVertexTable(vertexTable, pointMesh);
                shape.Set(vertexTable[0], vertexTable[1]);

                setupFixtureDef(physics);
                m_FixtureDef.shape = &shape;
                physicObject->getBody()->CreateFixture(&m_FixtureDef);

                physicObject->getBody()->SetTransform(
                    graphics::sf_to_b2(pointMesh->getMassCenter(), EngineConstant.SCALE),
                    0.f);
                physicObject->setSize(pointMesh->getMeshSize());
            }
            break;

            case PointMesh::Circle:
            {
                b2CircleShape shape;
                shape.m_radius = pointMesh->getMeshSize().x / 2.f / EngineConstant.SCALE;

                setupFixtureDef(physics);
                m_FixtureDef.shape = &shape;
                physicObject->getBody()->CreateFixture(&m_FixtureDef);

                physicObject->getBody()->SetTransform(
                    graphics::sf_to_b2(pointMesh->getMassCenter(), EngineConstant.SCALE),
                    0.f);
                physicObject->setSize(pointMesh->getMeshSize());
            }
            break;

            case PointMesh::Polygon:
            {
                b2Vec2 vertexTable[pointMesh->getPointCount()];
                setupVertexTable(vertexTable, pointMesh);

                std::vector<b2Vec2> vertexTableVector;
                for(unsigned int i = 0; i < pointMesh->getPointCount(); i++)
                {
                    vertexTableVector.push_back(vertexTable[i]);
                }

                setupFixtureDef(physics);

                computePolygonBody(physicObject->getBody(),
                                   &m_FixtureDef,
                                   &vertexTableVector,
                                   EngineConstant.SCALE);

                physicObject->getBody()->SetTransform(
                    graphics::sf_to_b2(pointMesh->getMassCenter(), EngineConstant.SCALE),
                    0.f);
                physicObject->setSize(pointMesh->getMeshSize());
            }
            break;

            case PointMesh::Chain:
            {
                b2ChainShape shape;
                b2Vec2       vertexTable[pointMesh->getPointCount()];
                setupVertexTable(vertexTable, pointMesh);
                shape.CreateChain(vertexTable, pointMesh->getPointCount());

                setupFixtureDef(physics);
                m_FixtureDef.shape = &shape;
                physicObject->getBody()->CreateFixture(&m_FixtureDef);

                physicObject->getBody()->SetTransform(
                    graphics::sf_to_b2(pointMesh->getMassCenter(), EngineConstant.SCALE),
                    0.f);
                physicObject->setSize(pointMesh->getMeshSize());
            }
            break;
        }

        return physicObject;
    }

    void PhysicsManager::setupBodyDef(PointMesh::Ptr pointMesh, const PhysicsPoperty& physics)
    {
        switch(pointMesh->getMeshType())
        {
            case PointMesh::Static:
                m_BodyDef.type = b2_staticBody;
                break;

            case PointMesh::Dynamic:
                m_BodyDef.type = b2_dynamicBody;
                break;

            case PointMesh::Kinematic:
                m_BodyDef.type = b2_kinematicBody;
                break;
        }

        m_BodyDef.position      = b2Vec2(0.f, 0.f);
        m_BodyDef.angle         = 0.f;
        m_BodyDef.allowSleep    = physics.allowSleep;
        m_BodyDef.fixedRotation = physics.fixedRotation;
        m_BodyDef.gravityScale  = physics.gravityScale;
    }

    void PhysicsManager::setupVertexTable(b2Vec2* table, PointMesh::Ptr pointMesh)
    {
        const auto pointTable = pointMesh->getPointTable();
        const auto massCenter = pointMesh->getMassCenter();

        for(unsigned int i = 0; i < pointTable.size(); i++)
        {
            table[i] = graphics::sf_to_b2(pointTable.at(i) - massCenter, EngineConstant.SCALE);
        }
    }

    void PhysicsManager::setupFixtureDef(const PhysicsPoperty& physics)
    {
        m_FixtureDef.density     = physics.density;
        m_FixtureDef.isSensor    = physics.sensor;
        m_FixtureDef.friction    = physics.friction;
        m_FixtureDef.restitution = physics.restitution;
    };

    void PhysicsManager::computePolygonBody(b2Body*              body,
                                            b2FixtureDef*        fixtureDef,
                                            std::vector<b2Vec2>* verticesVec,
                                            int                  scale)
    {
        int                              i, n = verticesVec->size(), j, m;
        std::vector<b2Vec2>              vec;
        b2Vec2*                          vertices;
        std::vector<std::vector<b2Vec2>> figsVec;
        b2PolygonShape*                  polyShape = new b2PolygonShape();

        for(i = 0; i < n; i++)
        {
            vec.push_back(b2Vec2(verticesVec->at(i).x * scale, verticesVec->at(i).y * scale));
        }

        // calcShapes(vec, figsVec);
        std::vector<sf::Vector2f> convex;
        for(auto v : vec)
            convex.push_back(graphics::b2_to_sf(v));

        auto concav = math::concaveToConvex(convex);

        for(auto tab : concav)
        {
            std::vector<b2Vec2> tab_tab;
            for(auto v : tab)
                tab_tab.push_back(graphics::sf_to_b2(v));

            figsVec.push_back(tab_tab);
        }

        n = figsVec.size();

        for(i = 0; i < n; i++)
        {
            vec      = figsVec[i];
            m        = vec.size();
            vertices = new b2Vec2[m];
            for(j = 0; j < m; j++)
            {
                vertices[j] = b2Vec2(vec[j].x / scale, vec[j].y / scale);
            }
            polyShape->Set(vertices, m);
            delete[] vertices;
            fixtureDef->shape = polyShape;
            body->CreateFixture(fixtureDef);
        }
    }
} // namespace nero
