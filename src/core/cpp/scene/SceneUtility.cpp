////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/scene/SceneUtility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    // QueryCallback
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

    ////////////////////////////////////////////////////////////
    // SceneSetting
    SceneSetting::SceneSetting()
    {
        hz = 40.0f;
        viewCenter.Set(0.0f, 0.0f);
        gravity.Set(0.f, 10.f);

        velocityIterations  = 8;
        positionIterations  = 3;

        drawAxis            = true;
        drawGrid            = true;
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

        scene_setting["frequency"]            = hz;
        scene_setting["view_center"]          = graphics::vectorToJson<b2Vec2>(viewCenter);
        scene_setting["gravity"]              = graphics::vectorToJson<b2Vec2>(gravity);

        scene_setting["velocity_iterations"]  = velocityIterations;
        scene_setting["position_iterations"]  = positionIterations;

        scene_setting["enable_warm_starting"] = enableWarmStarting;
        scene_setting["enable_continuous"]    = enableContinuous;
        scene_setting["enable_sub_stepping"]  = enableSubStepping;
        scene_setting["enable_sleep"]         = enableSleep;

        scene_setting["draw_axis"]            = drawAxis;
        scene_setting["draw_grid"]            = drawGrid;
        scene_setting["draw_shapes"]          = drawShapes;
        scene_setting["draw_joints"]          = drawJoints;
        scene_setting["draw_aabbs"]           = drawAABBs;
        scene_setting["draw_contact_points"]  = drawContactPoints;
        scene_setting["draw_contact_normals"] = drawContactNormals;
        scene_setting["draw_contact_impulse"] = drawContactImpulse;
        scene_setting["draw_coms"]            = drawCOMs;
        scene_setting["draw_stats"]           = drawStats;
        scene_setting["draw_profile"]         = drawProfile;

        return scene_setting;
    }

    SceneSetting SceneSetting::fromJson(nlohmann::json setting)
    {
        SceneSetting scene_setting;

        scene_setting.hz                 = setting["frequency"];
        scene_setting.gravity            = graphics::vectorFromJson<b2Vec2>(setting["gravity"]);
        scene_setting.viewCenter         = graphics::vectorFromJson<b2Vec2>(setting["view_center"]);

        scene_setting.velocityIterations = setting["velocity_iterations"];
        scene_setting.positionIterations = setting["position_iterations"];

        scene_setting.enableWarmStarting = setting["enable_warm_starting"];
        scene_setting.enableContinuous   = setting["enable_continuous"];
        scene_setting.enableSubStepping  = setting["enable_sub_stepping"];
        scene_setting.enableSleep        = setting["enable_sleep"];

        scene_setting.drawAxis           = setting["draw_axis"];
        scene_setting.drawGrid           = setting["draw_grid"];
        scene_setting.drawShapes         = setting["draw_shapes"];
        scene_setting.drawJoints         = setting["draw_joints"];
        scene_setting.drawAABBs          = setting["draw_aabbs"];
        scene_setting.drawContactPoints  = setting["draw_contact_points"];
        scene_setting.drawContactNormals = setting["draw_contact_normals"];
        scene_setting.drawContactImpulse = setting["draw_contact_impulse"];
        scene_setting.drawCOMs           = setting["draw_coms"];
        scene_setting.drawStats          = setting["draw_stats"];
        scene_setting.drawProfile        = setting["draw_profile"];

        return scene_setting;
    }

    ////////////////////////////////////////////////////////////
    // CameraSetting
    CameraSetting::CameraSetting()
    {
        defaultPosition.x = 0.f;
        defaultPosition.y = 0.f;
        defaultRotation   = 0.f;
        defaultZoom       = 0.f;

        position.x        = 0.f;
        position.y        = 0.f;
        rotation          = 0.f;
        zoom              = 0.f;
    };

    nlohmann::json CameraSetting::toJson()
    {
        nlohmann::json camera_setting;

        camera_setting["default_position"] = {{"x", defaultPosition.x}, {"y", defaultPosition.y}};
        camera_setting["default_rotation"] = defaultRotation;
        camera_setting["default_zoom"]     = defaultZoom;

        camera_setting["position"]         = {{"x", position.x}, {"y", position.y}};
        camera_setting["rotation"]         = rotation;
        camera_setting["zoom"]             = zoom;

        return camera_setting;
    }

    CameraSetting CameraSetting::fromJson(nlohmann::json setting)
    {
        CameraSetting camera_setting;

        camera_setting.defaultPosition.x = setting["default_position"]["x"];
        camera_setting.defaultPosition.y = setting["default_position"]["y"];
        camera_setting.defaultRotation   = setting["default_rotation"];
        camera_setting.defaultZoom       = setting["default_zoom"];

        camera_setting.position.x        = setting["position"]["x"];
        camera_setting.position.y        = setting["position"]["y"];
        camera_setting.rotation          = setting["rotation"];
        camera_setting.zoom              = setting["zoom"];

        return camera_setting;
    }

    ////////////////////////////////////////////////////////////
    // Target
    CameraTarget::CameraTarget()
    {
        target       = nullptr;
        offsetLeft   = 150.f;
        offsetRight  = 0.f;
        offsetUp     = 250.f;
        offsetDown   = 0.f;
        followTarget = false;
    }
} // namespace nero
