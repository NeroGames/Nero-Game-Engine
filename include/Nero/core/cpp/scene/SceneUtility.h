////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SCENE_UTIL_H
#define SCENE_UTIL_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/cpp/resource/ResourceManager.h>
#include <Nero/core/cpp/camera/Camera.h>
#include <Nero/core/cpp/object/Object.h>
//SFML
#include <SFML/Graphics/Color.hpp>
//Box2D
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
//JSON
#include <json/json.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
	//constant
	const int32 MAX_CONTACT_POINT = 2048;

	//contact point
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

	//query callback
    class QueryCallback : public b2QueryCallback
    {
        public:
                            QueryCallback(const b2Vec2& point);
						   ~QueryCallback();

            bool            ReportFixture(b2Fixture* fixture);

            b2Vec2          m_Point;
            b2Fixture*      m_Fixture;
    };


	//leve setting
    struct SceneSetting
    {
        //Constructor
        SceneSetting();

        //Parameter
        float32 hz;
        b2Vec2  viewCenter;
        b2Vec2  gravity;
        int32   velocityIterations;
        int32   positionIterations;

        bool pause;
        bool singleStep;

        bool enableSleep;
        bool enableWarmStarting;
        bool enableContinuous;
        bool enableSubStepping;

        bool drawAxis;
        bool drawGrid;
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

        //Function
        nlohmann::json toJson();
        static  SceneSetting fromJson(nlohmann::json setting);
    };

	//camera setting
    struct CameraSetting
    {
        //Constructor
        CameraSetting();

        //Parameter
        sf::Vector2f        defaultPosition;
        float               defaultRotation;
        float               defaultZoom;

        sf::Vector2f        position;
        float               rotation;
        float               zoom;

        //Function
        nlohmann::json toJson();
        static  CameraSetting fromJson(nlohmann::json setting);
    };

	//camera target
    struct CameraTarget
    {
        //Constructor
        CameraTarget();

        //Parameter
        Object::Ptr     target;
        float           offsetLeft;
        float           offsetRight;
        float           offsetUp;
        float           offsetDown;
        bool            followTarget;
    };

	struct RenderContext
	{
		typedef std::shared_ptr<RenderContext> Ptr;

        sf::Vector2f	canvasPosition;
        sf::Vector2f	canvasSize;
        sf::Vector2f	mousePosition;
		bool			focus;
	};
}
#endif // SCENE_UTIL_H
