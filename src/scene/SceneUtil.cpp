////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
#include <Nero/scene/SceneUtil.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    //Context
    Context::Context(sfg::Canvas::Ptr renderCanvas, sf::View& frontView, Camera::Ptr camera, ResourceManager::Ptr resourceManager):
        renderCanvas(renderCanvas)
        ,frontView(frontView)
        ,camera(camera)
        ,resourceManager(resourceManager)
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    //QueryCallback
    QueryCallback::QueryCallback(const b2Vec2& point)
    {
        m_Point     = point;
        m_Fixture   = nullptr;
    }

    bool QueryCallback::ReportFixture(b2Fixture* fixture)
    {
        b2Body* body = fixture->GetBody();

        if (body->GetType() == b2_dynamicBody)
        {
            bool inside = fixture->TestPoint(m_Point);

            if (inside)
            {
                m_Fixture = fixture;

                //Clean pointer
                body = nullptr;

                //We are done, terminate the query.
                return false;
            }
        }

        //Clean pointer
        body = nullptr;

        //Continue the query.
        return true;
    }

    QueryCallback::~QueryCallback()
    {
        m_Fixture = nullptr;
    }

    ////////////////////////////////////////////////////////////
    //SceneSetting
    SceneSetting::SceneSetting()
    {
        hz = 30.0f;
        viewCenter.Set(0.0f, 0.0f);
        gravity.Set(0.f, 10.f);

        velocityIterations  = 8;
        positionIterations  = 3;

        canvasColor         = sf::Color::Black;

        drawAxis            = true;
        drawGrid            = false;
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

    nlohmann::json SceneSetting::toJson()
    {
        nlohmann::json scene_setting;

        return scene_setting;
    }

    ////////////////////////////////////////////////////////////
    //CameraSetting
    CameraSetting::CameraSetting()
    {
       defaultPosition.x   = 0.f;
       defaultPosition.y   = 0.f;
       defaultRotation     = 0.f;
       defaultZoom         = 0.f;

       position.x          = 0.f;
       position.y          = 0.f;
       rotation            = 0.f;
       zoom                = 0.f;
    };

    nlohmann::json CameraSetting::toJson()
    {
        nlohmann::json setting;

        return setting;
    }

    ////////////////////////////////////////////////////////////
    //SoundSetting
    SoundSetting::SoundSetting()
    {
        soundVolume = 10.f;
        musicVolume = 10.f;
    }

    nlohmann::json SoundSetting::toJson()
    {
        nlohmann::json sound_setting;

        return sound_setting;
    }

    ////////////////////////////////////////////////////////////
    //CameraTargetOffset
    CameraTargetOffset::CameraTargetOffset()
    {
        left = 100.f;
        right = 0.f;
        up = 100.f;
        down = 100.f;
    }

    nlohmann::json CameraTargetOffset::toJson()
    {
        nlohmann::json offset;

        return offset;
    }

}
