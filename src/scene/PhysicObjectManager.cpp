////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/scene/PhysicObjectManager.h>
#include <Nero/utility/Utility.h>
//BOX2D
#include <Box2D/Collision/Shapes/b2EdgeShape.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Collision/Shapes/b2ChainShape.h>
////////////////////////////////////////////////////////////
namespace nero
{
    PhysicObjectManager::PhysicObjectManager():
         m_World(nullptr)
        ,m_ObjectCount(0)
    {
        //ctor
    }

    PhysicObjectManager::~PhysicObjectManager()
    {
        m_World = nullptr;
        delete m_World;
    }

    void PhysicObjectManager::destroyAllObject()
    {
        for(auto it = m_ObjectTab.begin(); it != m_ObjectTab.end(); it++)
            m_World->DestroyBody(it->getBody());

        m_ObjectTab.clear();
    }

    void PhysicObjectManager::setupBodyDef(Mesh& mesh)
    {
        switch(mesh.getType())
        {
            case Mesh::Static_Mesh:
                m_BodyDef.type = b2_staticBody;
                break;

            case Mesh::Dynamic_Mesh:
                m_BodyDef.type = b2_dynamicBody;
                break;

            case Mesh::Kinematic_Mesh:
                m_BodyDef.type = b2_kinematicBody;
                break;
        }

        m_BodyDef.position      = b2Vec2(0.f, 0.f);
        m_BodyDef.angle         = 0.f;
        m_BodyDef.allowSleep    = true;
        m_BodyDef.fixedRotation = mesh.getFixedRotation();
        m_BodyDef.gravityScale  = mesh.getGravityScale();
    }

    void PhysicObjectManager::setupVertexTab(b2Vec2* tab, Mesh& mesh)
    {
        for(int i = 0; i < mesh.getAllVertex()->size(); i++)
            tab[i] = sf_to_b2(mesh.getAllVertex()->at(i).getPosition() - mesh.getCenter(), SCALE);
    }

    void PhysicObjectManager::setupFixtureDef(Mesh& mesh)
    {
        m_FixtureDef.density        = mesh.getDensity();
        m_FixtureDef.isSensor       = mesh.getIsSensor();
        m_FixtureDef.friction       = mesh.getFriction();
        m_FixtureDef.restitution    = mesh.getRestitution();
    }

    void PhysicObjectManager::computePolygonBody(b2Body* pBody, b2FixtureDef* pFixtureDef, std::vector<b2Vec2>* pVerticesVec, int scale)
    {
        int i, n=pVerticesVec->size(), j, m;
        std::vector<b2Vec2> vec;
        b2Vec2 *vertices;
        std::vector<std::vector<b2Vec2> > figsVec;
        b2PolygonShape* polyShape = new b2PolygonShape();

        for (i=0; i<n; i++) {
            vec.push_back(b2Vec2(pVerticesVec->at(i).x*scale,pVerticesVec->at(i).y*scale));
        }

        //calcShapes(vec, figsVec);
        std::vector<sf::Vector2f> convex;
        for(auto v : vec)
            convex.push_back(b2_to_sf(v));

        auto concav = concave_to_convex(convex);

        for(auto tab : concav)
        {
            std::vector<b2Vec2> tab_tab;
            for(auto v: tab)
                tab_tab.push_back(sf_to_b2(v));

            figsVec.push_back(tab_tab);
        }


        n = figsVec.size();

        for (i=0; i<n; i++) {
            vec = figsVec[i];
            m = vec.size();
            vertices = new b2Vec2[m];
            for (j=0; j<m; j++) {
                vertices[j] = b2Vec2(vec[j].x/scale,vec[j].y/scale);
            }
            polyShape->Set(vertices, m);
            delete[] vertices;
            pFixtureDef->shape=polyShape;
            pBody->CreateFixture(pFixtureDef);
        }
    }

    PhysicObject::Ptr PhysicObjectManager::createObject(Mesh* mesh)
    {
        switch(mesh->getShape())
        {
            case Mesh::Line_Mesh:
            {
                setupBodyDef(*mesh);
                PhysicObject object(m_World->CreateBody(&m_BodyDef));

                b2EdgeShape shape;
                b2Vec2 vertexTab[2];
                setupVertexTab(vertexTab, *mesh);
                shape.Set(vertexTab[0], vertexTab[1]);

                setupFixtureDef(*mesh);
                m_FixtureDef.shape = &shape;
                object.getBody()->CreateFixture(&m_FixtureDef);

                object.getBody()->SetTransform(sf_to_b2(mesh->getCenter(), SCALE), 0.f);
                object.setSize(mesh->getSize());

                return std::make_shared<PhysicObject>(object);
            }
            break;

            case Mesh::Circle_Mesh:
            {
                setupBodyDef(*mesh);
                PhysicObject object(m_World->CreateBody(&m_BodyDef));

                b2CircleShape shape;
                shape.m_radius = mesh->getRadius()/SCALE;

                setupFixtureDef(*mesh);
                m_FixtureDef.shape = &shape;
                object.getBody()->CreateFixture(&m_FixtureDef);

                object.getBody()->SetTransform(sf_to_b2(mesh->getCenter(), SCALE), 0.f);
                object.setSize(mesh->getSize());

                return std::make_shared<PhysicObject>(object);
            }
            break;

            case Mesh::Polygon_Mesh:
            {
                setupBodyDef(*mesh);
                PhysicObject object(m_World->CreateBody(&m_BodyDef));

                b2Vec2 vertexTab[mesh->getPointCount()];
                setupVertexTab(vertexTab, *mesh);

                std::vector<b2Vec2> vectTab;
                for(int i = 0; i < mesh->getPointCount(); i++)
                    vectTab.push_back(vertexTab[i]);

                setupFixtureDef(*mesh);

                computePolygonBody(object.getBody(), &m_FixtureDef, &vectTab, SCALE);

                object.getBody()->SetTransform(sf_to_b2(mesh->getCenter(), SCALE), 0.f);
                object.setSize(mesh->getSize());

                return std::make_shared<PhysicObject>(object);

            }
            break;

            case Mesh::Chain_Mesh:
            {
                setupBodyDef(*mesh);
                PhysicObject object(m_World->CreateBody(&m_BodyDef));

                b2ChainShape shape;
                b2Vec2 vertexTab[mesh->getPointCount()];
                setupVertexTab(vertexTab, *mesh);
                shape.CreateChain(vertexTab, mesh->getPointCount());

                setupFixtureDef(*mesh);
                m_FixtureDef.shape = &shape;
                object.getBody()->CreateFixture(&m_FixtureDef);

                object.getBody()->SetTransform(sf_to_b2(mesh->getCenter(), SCALE), 0.f);
                object.setSize(mesh->getSize());

                return std::make_shared<PhysicObject>(object);

            }
            break;
        }
    }

    void PhysicObjectManager::setWorld(b2World* world)
    {
        m_World = world;
    }
}


