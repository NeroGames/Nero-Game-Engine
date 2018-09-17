#ifndef CONSTANTPOOL_H_INCLUDED
#define CONSTANTPOOL_H_INCLUDED

#include <string>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Color.hpp>

namespace nero
{
    //String
    const std::string   CONFIGURATION_PATH                      = "config";
    const std::string   RESOURCE_CONFIGURATION                  = "resource_config";
    const std::string   ENGINE_CONFIGURATION                    = "engine_config";
    const std::string   WORKSPACE_FOLDER                        = "Nero";
    const std::string   DEFAULT_SCENE                           = "## + default + ##";
    //INT
    const int           LOAD_RESOURCE_MIN_TIME                  = 10; //seconds
    const int           MAX_SCENE                               = 20;
    const int           DEFAULT_ALPHA                           = 255;
    const int           MESH_MODE_ALPHA                         = 100;
    //FLOAT
    const float         MAX_SOUND                               = 256;
    const float         MAX_SOUND_POOL                          = 10;
    //COLOR
    const sf::Color     BACKGROUND_CLEAR_COLOR                  = sf::Color(100.f, 100.f, 100.f);
}




#endif // CONSTANTPOOL_H_INCLUDED
