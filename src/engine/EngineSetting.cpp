////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2022 sk-landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/engine/EngineSetting.h>
#include <Nero/utility/ConstantPool.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    EngineSetting::EngineSetting()
    {
        autoSave        = true;
        autoSaveTime    = 30.f;
        maxScene        = 10;
        previewAlpha    = 180;
        carriageReturn  = "|";
        lastScene       = DEFAULT_SCENE;
    }

    ////////////////////////////////////////////////////////////
    nlohmann::json EngineSetting::toJson()
    {
        nlohmann::json json;

        json["auto_save"]        = autoSave;
        json["auto_save_time"]   = autoSaveTime;
        json["last_scene"]       = lastScene;
        json["max_scene"]        = maxScene;
        json["preview_alpha"]    = previewAlpha;
        json["carriage_return"]  = carriageReturn;

        return json;
    }

    ////////////////////////////////////////////////////////////
    EngineSetting EngineSetting::fromJson(nlohmann::json json)
    {
        EngineSetting setting;

        setting.autoSave        = json["auto_save"];
        setting.autoSaveTime    = json["auto_save_time"];
        setting.maxScene        = json["max_scene"];
        setting.previewAlpha    = json["preview_alpha"];
        setting.lastScene       = json["last_scene"].get<std::string>();
        setting.carriageReturn  = json["carriage_return"].get<std::string>();

        return setting;
    }
}
