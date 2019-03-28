////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef CONSTANTPOOL_H_INCLUDED
#define CONSTANTPOOL_H_INCLUDED
///////////////////////////HEADERS//////////////////////////
//STD
#include <string>
//SFML
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Color.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    //INT
    const int           LOAD_RESOURCE_MIN_TIME                  = 5; //seconds
    const int           MAX_SCENE                               = 20;
    const int           DEFAULT_ALPHA                           = 255;
    const int           MESH_MODE_ALPHA                         = 100;
    //FLOAT
    const float         MAX_SOUND                               = 256;
    const float         MAX_SOUND_POOL                          = 10;
    //COLOR
    const sf::Color     BACKGROUND_CLEAR_COLOR                  = sf::Color(100.f, 100.f, 100.f);
    //STRING
    const std::string   CONFIGURATION_FOLDER                    = "config";
    const std::string   RESOURCE_CONFIGURATION                  = "resource_config";
    const std::string   ENGINE_CONFIGURATION                    = "engine_config";
    const std::string   LOG_CONFIGURATION                       = "log_config.txt";
    const std::string   WORKSPACE_FOLDER                        = "Nero";
    const std::string   DEFAULT_SCENE                           = "## + default + ##";
    const std::string   DEFAULT_FRONTSREEN                      = "Start Screen";
    const std::string   DEFAULT_FONT                            = "Sansation.ttf";
    const std::string   LOADING_LOGO                            = "nero_logo.png";
    const std::string   ENGINE_COPYRIGHTS                       = "copyrights.png";
    const std::string   RESOURCE_FOLDER                         = "resource";
    const std::string   FONT_FOLDER                             = "font";
    const std::string   SOUND_FOLDER                            = "sound";
    const std::string   MUSIC_FOLDER                            = "music";
    const std::string   SHADER_FOLDER                           = "shader";
    const std::string   SCRIPT_FOLDER                           = "script";
    const std::string   TEXTURE_FOLDER                          = "texture";
    const std::string   ANIMATION_FOLDER                        = "animation";
    const std::string   STARTUP_FOLDER                          = "startup";
    const std::string   CONCEPTION_FOLDER                       = "conception";
    const std::string   LOG_FOLDER                              = "log";
    const std::string   SHADER_LIST                             = "shader.json";
    const std::string   SIMPLE_FRAGMENT                         = "simpleFragment.frag";
    const std::string   SIMPLE_VERTEX                           = "simpleVertex.ver";
    const std::string   ENGINE_VERSION                          = "v1.0.0";
    const std::string   ENGINE_COPYRIGHT                        = "Copyright (c) 2016-2019 SANOU A. K. Landry";
}
#endif // CONSTANTPOOL_H_INCLUDED
