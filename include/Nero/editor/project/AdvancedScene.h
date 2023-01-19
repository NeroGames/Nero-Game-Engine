////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ADVANCEDSCENE_H
#define ADVANCEDSCENE_H
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/engine/Parameter.h>
#include <Nero/core/cpp/engine/Setting.h>
#include <Nero/editor/level/LevelBuilder.h>
#include <Nero/editor/screen/ScreenBuilder.h>
// STD
#include <memory>
#include <vector>
////////////////////////////////////////////////////////////
namespace nero
{
    class AdvancedScene
    {
      public:
        typedef std::shared_ptr<AdvancedScene> Ptr;

      public:
        AdvancedScene();

        void                     init();
        void                     clear();
        // level
        std::string              createLevel(const Parameter& parameter);
        void                     openLevel(const std::string& levelName);
        void                     closeSelectedLevel();
        void                     removeLevel(const std::string& levelName);
        void                     registerLevel(const std::string& levelName);
        void                     unregisterLevel(const std::string& levelName);
        std::vector<std::string> getRegisteredLevelTable();
        // screen
        void                     createScreen(const Parameter& parameter);
        // setting
        void                     setEditorSetting(const Setting::Ptr& setting);
        void                     setProjectSetting(const Setting::Ptr& setting);
        // render
        void                     setRenderContext(const RenderContext::Ptr& renderContext);
        void              setRenderTexture(const std::shared_ptr<sf::RenderTexture>& renderTexture);
        LevelBuilder::Ptr getLevelBuilder() const;

      private:
        // level
        LevelBuilder::Ptr                  m_LevelBuilder;
        std::vector<std::string>           m_RegisteredLevelTable;
        // screen
        ScreenBuilder::Ptr                 m_SelectedScreen;
        // setting
        Setting::Ptr                       m_EditorSetting;
        Setting::Ptr                       m_ProjectSetting;
        Setting::Ptr                       m_SceneSetting;
        // render
        RenderContext::Ptr                 m_RenderContext;
        std::shared_ptr<sf::RenderTexture> m_RenderTexture;
    };
} // namespace nero
#endif // ADVANCEDSCENE_H
