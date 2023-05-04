////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef GAMELEVELWINDOW_H
#define GAMELEVELWINDOW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class GameLevelWindow : public UIComponent
    {
      public:
        GameLevelWindow(EditorContext::Ptr editorContext);
        virtual ~GameLevelWindow() override;

        virtual void destroy() override;
        virtual void render() override;
        virtual void update(const sf::Time& timeStep) override;

      private:
        void printSameLine(unsigned int&      index,
                           const float        windowWidth,
                           const ImGuiStyle&  imguiStyle,
                           const unsigned int leveCount);

      private:
        AdvancedScene::Ptr       m_AdvancedScene;
        ImVec2                   m_ButtonZize;
        bool                     m_OpenGameLevel;
        bool                     m_CloseGameLevel;
        bool                     m_DeleteGameLevel;
        std::vector<std::string> m_LevelNameTable;
        std::string              m_SelectedGameLevel;
        std::string              m_OpenedGameLevel;
        ImGuiStyle&              m_ImguiStyle;
    };
} // namespace nero
#endif // GAMELEVELWINDOW_H
