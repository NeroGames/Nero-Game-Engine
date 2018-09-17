////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/engine/EngineSetting.h>
#include <Nero/utility/ConstantPool.h>

namespace nero
{
    EngineSetting::EngineSetting()
    {
        autoSave        = true;
        autoSaveTime    = 30.f;
        lastScene       = DEFAULT_SCENE;
    }

    nlohmann::json EngineSetting::toJson()
    {
        nlohmann::json setting;

        setting["auto-save"]        = autoSave;
        setting["auto-save-time"]   = autoSaveTime;
        setting["last-scene"]       = lastScene;

        return setting;
    }

    EngineSetting EngineSetting::fromJson(nlohmann::json json)
    {
        EngineSetting setting;

        setting.autoSave = json["auto-save"];
        setting.autoSaveTime = json["auto-save-time"];

        std::string scene = json["last-scene"].get<std::string>();
        setting.lastScene = scene == "default" ? DEFAULT_SCENE : scene;

        return setting;
    }

}
