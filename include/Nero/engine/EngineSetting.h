////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ENGINESETTING_H_INCLUDED
#define ENGINESETTING_H_INCLUDED
///////////////////////////HEADERS//////////////////////////
//NERO
//JSON
#include <json/json.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    struct EngineSetting
    {
        EngineSetting();

        bool            autoSave;
        float           autoSaveTime;
        std::string     lastScene;

        nlohmann::json      toJson();

        static EngineSetting      fromJson(nlohmann::json json);
    };
}
#endif // ENGINESETTING_H_INCLUDED
