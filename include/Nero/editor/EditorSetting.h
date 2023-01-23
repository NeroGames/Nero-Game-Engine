////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef EDITORSETTING_H
#define EDITORSETTING_H
///////////////////////////HEADERS///////////////////////////
// JSON
#include <json/json.hpp>
/////////////////////////////////////////////////////////////
namespace nero
{
    const struct
    {
        const nlohmann::json windowSetting = {
            {     "width",       1245},
            {    "height",        700},
            {"position_x",         20},
            {"position_y",         20},
            { "icon_file", "icon.png"}
        };
        const nlohmann::json resourceSetting = {};

    } EditorSetting;

} // namespace nero

#endif // EDITORSETTING_H
