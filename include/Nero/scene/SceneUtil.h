////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef SCENE_UTIL_H
#define SCENE_UTIL_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/resource/ResourceManager.h>
#include <Nero/camera/Camera.h>
//SFGUI
#include <SFGUI/Canvas.hpp>
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
    ///\brief Store resources provided by the engine
    struct Context
    {
        Context(sfg::Canvas::Ptr renderCanvas, sf::View& frontView, Camera::Ptr camera, ResourceManager::Ptr resourceManager);

        sfg::Canvas::Ptr        renderCanvas;
        sf::View&               frontView;
        Camera::Ptr             camera;
        ResourceManager::Ptr    resourceManager;
    };


    //
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

    //
    class QueryCallback : public b2QueryCallback
    {
        public:
                            QueryCallback(const b2Vec2& point);
                            ~QueryCallback();

            bool            ReportFixture(b2Fixture* fixture);

            b2Vec2          m_Point;
            b2Fixture*      m_Fixture;
    };


    ///\brief Store Scene parameters
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

        sf::Color   canvasColor;

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
        nlohmann::json      toJson();
    };

    ///\brief Store the Scene current camera state
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
        nlohmann::json      toJson();
    };


    ///\brief Scene sound setting
    struct SoundSetting
    {
        //Constructor
        SoundSetting();

        //Parameter
        float soundVolume;
        float musicVolume;

        //Function
        nlohmann::json      toJson();
    };

    struct CameraTargetOffset
    {
        //Constructor
        CameraTargetOffset();

        //Parameter
        float left;
        float right;
        float up;
        float down;

        //Function
        nlohmann::json      toJson();
    };

    //
    const int32 k_maxContactPoints = 2048;
}



#endif // SCENE_UTIL_H
