#ifndef SCENE_UTILITY_H_INCLUDED
#define SCENE_UTILITY_H_INCLUDED

//Box2D
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>


namespace nero
{
    const int32 k_maxContactPoints = 2048;
    struct ContactPoint
    {
        b2Fixture*      fixtureA;
        b2Fixture*      fixtureB;
        b2Vec2          normal;
        b2Vec2          position;
        b2PointState    state;
        float32         normalImpulse;
        float32         tangentImpulse;
        float32         separation;
    };

    class QueryCallback : public b2QueryCallback
    {
        public:
            QueryCallback(const b2Vec2& point)
            {
                m_Point     = point;
                m_Fixture   = NULL;
            }

            bool ReportFixture(b2Fixture* fixture)
            {
                b2Body* body = fixture->GetBody();
                if (body->GetType() == b2_dynamicBody)
                {
                    bool inside = fixture->TestPoint(m_Point);
                    if (inside)
                    {
                        m_Fixture = fixture;

                        // We are done, terminate the query.
                        return false;
                    }
                }

                // Continue the query.
                return true;
            }

            b2Vec2          m_Point;
            b2Fixture*      m_Fixture;
    };


    struct SceneSettings
    {
        SceneSettings()
        {
            viewCenter.Set(0.0f, 20.0f);
            hz = 30.0f;
            velocityIterations = 8;
            positionIterations = 3;

            drawShapes          = true;
            drawJoints          = true;
            drawAABBs           = false;
            drawContactPoints   = false;
            drawContactNormals  = false;
            drawContactImpulse  = false;
            drawFrictionImpulse = false;
            drawCOMs            = false;
            drawStats           = false;
            drawProfile         = false;

            enableWarmStarting  = true;
            enableContinuous    = true;
            enableSubStepping   = false;
            enableSleep         = true;

            pause               = false;
            singleStep          = false;
        }

        b2Vec2 viewCenter;
        float32 hz;

        int32 velocityIterations;
        int32 positionIterations;

        bool drawShapes;
        bool drawJoints;
        bool drawAABBs;
        bool drawContactPoints;
        bool drawContactNormals;
        bool drawContactImpulse;
        bool drawFrictionImpulse;
        bool drawCOMs;
        bool drawStats;
        bool drawProfile;

        bool enableWarmStarting;
        bool enableContinuous;
        bool enableSubStepping;
        bool enableSleep;

        bool pause;
        bool singleStep;
    };

    struct CameraSettings
    {
        double      defaultPosition_x   = 0.f;
        double      defaultPosition_y   = 0.f;
        double      defaultRotation     = 0.f;
        double      defaultZoom         = 0.f;

        double      position_x          = 0.f;
        double      position_y          = 0.f;
        double      rotation            = 0;
        double      zoom                = 0;
    };
}



#endif // SCENE_UTILITY_H_INCLUDED
