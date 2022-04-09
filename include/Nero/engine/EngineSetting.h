////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2022 sk-landry
////////////////////////////////////////////////////////////
#ifndef ENGINESETTING_H_INCLUDED
#define ENGINESETTING_H_INCLUDED
///////////////////////////HEADERS//////////////////////////
//JSON
#include <json/json.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    /*!
        \ingroup engine
        \brief Store the engine settings

        The nero::EngineSetting structure is initialized with the content of the configuration file `config/engine_config`.
        If the configuration file does not exist, a new one is created automatically when the engine start.
    */
    struct EngineSetting
    {
        ///\brief construction, initialize all settings
        EngineSetting();

        bool                        autoSave;           /*!< default value true */
        float                       autoSaveTime;       /*!<brief default value 30 seconds*/
        int                         maxScene;           /*!< default value 10 scenes */
        int                         previewAlpha;       /*!< default value 255. This value must be in the interval [0 - 255]*/
        std::string                 lastScene;          /*!< default value the default scene. */
        std::string                 carriageReturn;     /*!< default value pipe character ( | )*/

        ///\brief save the settings to JSON
        nlohmann::json              toJson();
        ///\brief load the settings from JSON
        static EngineSetting        fromJson(nlohmann::json json);
    };
}
#endif // ENGINESETTING_H_INCLUDED
