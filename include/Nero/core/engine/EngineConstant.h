////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ENGINECONSTANT_H
#define ENGINECONSTANT_H
///////////////////////////HEADERS//////////////////////////
//STD
#include <string>
//SFML
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    struct _EngineConstant
    {
        //engine version
        const int ENGINE_VERSION_MAJOR  = 2;
        const int ENGINE_VERSION_MINOR  = 0;
        const int ENGINE_VERSION_PATCH  = 0;

        //engine window
        const std::string ENGINE_WINDOW_TITLE   = "Nero Game Engine";
        const unsigned int ENGINE_WINDOW_WIDTH  = 1024 + 256;
        const unsigned int ENGINE_WINDOW_HEIGHT = 720;

        //time step
        const sf::Time TIME_PER_FRAME = sf::seconds(1.f/60.f);

        //main folders
        const std::string FOLDER_SETTING     = "setting";
        const std::string FOLDER_LOGGING     = "logging";
        const std::string FOLDER_RESOURCE    = "resource";
        const std::string FOLDER_WORKSPACE   = "workspace";
        const std::string FOLDER_STARTUP     = "startup";

        //resource folders
        const std::string FOLDER_RESOURCE_TEXTURE       = "texture";
        const std::string FOLDER_RESOURCE_ANIMATION     = "animation";
        const std::string FOLDER_RESOURCE_FONT          = "font";
        const std::string FOLDER_RESOURCE_MUSIC         = "music";
        const std::string FOLDER_RESOURCE_SHADER        = "shader";
        const std::string FOLDER_RESOURCE_SOUND         = "sound";
        const std::string FOLDER_RESOURCE_LANGUAGE      = "language";
        const std::string FOLDER_RESOURCE_SCRIPT        = "script";

        //file
        const std::string FILE_SETTING_ENGINE    = "engine_setting";
        const std::string FILE_SETTING_LOGGING   = "logging_setting";
        const std::string FILE_SETTING_RESOURCE  = "resource_setting";
        const std::string FILE_LOGGING_INFO      = "logging_info";
        const std::string FILE_LOGGING_DEBUG     = "logging_debug";
        const std::string FILE_LOGGING_WARNING   = "logging_warning";

        //color
        const sf::Color CANVAS_CLEAR_COLOR = sf::Color::Black;
    };

    const _EngineConstant EngineConstant;
}
#endif // ENGINECONSTANT_H
