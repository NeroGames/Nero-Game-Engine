////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ENGINECONSTANT_H
#define ENGINECONSTANT_H
///////////////////////////HEADERS//////////////////////////
// Std
#include <string>
// SFML
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <Nero/core/cpp/utility/String.h>
////////////////////////////////////////////////////////////
namespace nero
{
    namespace EngineConstant
    {
        // Engine name
        inline const std::string ENGINE_NAME      = "Nero Game Engine";
        inline const std::string ENGINE_COPYRIGHT = "Copyright (C) Nero Games. All rights reserved";
        inline const std::string ENGINE_WEBSITE   = "https://nero-games.com";
        inline const std::string LEARN_MORE_MESSAGE =
            "Lean more at" + StringPool.SPACE + ENGINE_WEBSITE;
        // engine version
        inline const int         ENGINE_VERSION_MAJOR = 2;
        inline const int         ENGINE_VERSION_MINOR = 0;
        inline const int         ENGINE_VERSION_PATCH = 0;
        inline const std::string ENGINE_VERSION = toString(ENGINE_VERSION_MAJOR) + StringPool.DOT +
                                                  toString(ENGINE_VERSION_MINOR) + StringPool.DOT +
                                                  toString(ENGINE_VERSION_PATCH);
        // engine window
        inline const std::string ENGINE_WINDOW_TITLE =
            ENGINE_NAME + StringPool.SPACE + ENGINE_VERSION;
        inline const unsigned int ENGINE_WINDOW_WIDTH       = 800;
        inline const unsigned int ENGINE_WINDOW_HEIGHT      = 600;
        // time step
        inline const sf::Time     TIME_PER_FRAME            = sf::seconds(1.f / 60.f);
        // canvas color
        inline const sf::Color    COLOR_CANVAS              = sf::Color::Black;
        // document type
        inline const std::string  DOCUMENT_TYPE_WORKSPACE   = "nero_game_workspace";
        inline const std::string  DOCUMENT_TYPE_PROJECT     = "nero_game_project";
        //
        inline const std::string  NO_GAME_FOUND             = "No Game Found";

        inline const unsigned int NUMBER_ZERO               = 0;
        inline const std::string  DLL_CREATE_STARTUP_SCREEN = "create_startup_screen";
        inline const std::string  DLL_CREATE_SCENE          = "create_scene";

        inline const std::string  DEFAULT_TASK_CATEGORY     = "Nero";

        inline const float        SCALE                     = 50.f;
        inline const float        GRAVITY                   = 9.8f;
        inline const int          SPRITE_LAYER_MAX          = 1000;

        inline const sf::Color    COLOR_STATIC_MESH =
            sf::Color(0.5f * 255.f, 0.9f * 255.f, 0.5f * 255.f);
        inline const sf::Color COLOR_DYNAMIC_MESH =
            sf::Color(0.9f * 255.f, 0.7f * 255.f, 0.7f * 255.f);
        inline const sf::Color COLOR_KINEMATIC_MESH =
            sf::Color(0.5f * 255.f, 0.5f * 255.f, 0.9f * 255.f);
        inline const sf::Color COLOR_SELECTED_MESH = sf::Color(255, 140, 0);
        inline const sf::Color COLOR_INVALIDE_MESH = sf::Color(255, 0, 0);
        inline const int       DEFAULT_ALPHA       = 255;
        inline const int       MESH_MODE_ALPHA     = 100;
    } // namespace EngineConstant
} // namespace nero
#endif // EngineConstant.h
