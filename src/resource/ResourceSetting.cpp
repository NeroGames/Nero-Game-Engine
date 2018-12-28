#include <Nero/resource/ResourceSetting.h>

namespace nero
{
    nlohmann::json ResourceSetting::toJson()
    {
        nlohmann::json json;

        json["font"]["folder"]          = "resource/font";
        json["font"]["extension"]       = {"ttf", "otf"};
        json["font"]["default"]         = "Sansation";
        json["sound"]["folder"]         = "resource/sound";
        json["sound"]["extension"]      = {"wav", "ogg", "flac"};
        json["music"]["folder"]         = "resource/music";
        json["music"]["extension"]      = {"wav", "ogg", "flac"};
        json["shader"]["folder"]        = "resource/shader";
        json["shader"]["extension"]     = {"vert", "frag"};
        json["shader"]["descriptor"]    = "resource/shader/shader.json";
        json["script"]["folder"]        = "resource/script";
        json["script"]["extension"]     = {"lua"};
        json["texture"]["folder"]       = "resource/texture";
        json["texture"]["extension"]    = {"png", "jpg", "bmp", "dds", "tga", "psd"};
        json["texture"]["separator"]    = "-";
        json["animation"]["folder"]     = "resource/animation";
        json["animation"]["extension"]  = {"png", "jpg", "bmp", "dds", "tga", "psd"};

        return json;
    }
}
