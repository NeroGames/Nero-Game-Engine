////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/EditorMenubar.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    EditorMenubar::EditorMenubar(EditorContext::Ptr editorContext):
         UIComponent(editorContext)
    {

    }

    EditorMenubar::~EditorMenubar()
    {
        destroy();
    }

    void EditorMenubar::destroy()
    {

    }

    void EditorMenubar::render()
    {
        const bool gameProjectOpened = m_EditorContext->getGameProject() != nullptr;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("New Project", nullptr, false))
                {
                    // TODO manager tab focus
                    ImGui::OpenPopup(EditorConstant.WINDOW_PROJECT_MANAGER.c_str());
                }

                if(ImGui::MenuItem("Open Project", nullptr, false))
                {
                    ImGui::OpenPopup(EditorConstant.WINDOW_PROJECT_MANAGER.c_str());
                }

                if(ImGui::MenuItem("Save Project", nullptr, false, gameProjectOpened))
                {
                    //saveProject();
                }

                if(ImGui::MenuItem("Close Project", nullptr, false, gameProjectOpened))
                {
                    //closeProject();
                }

                ImGui::Separator();

                if(ImGui::MenuItem("New Workspace", nullptr, false))
                {
                    ImGui::OpenPopup(EditorConstant.WINDOW_PROJECT_MANAGER.c_str());
                }

                if(ImGui::MenuItem("Import Workspace", nullptr, false))
                {
                    ImGui::OpenPopup(EditorConstant.WINDOW_PROJECT_MANAGER.c_str());
                }

                ImGui::Separator();

                auto recentProjectTable =  file::loadJson(file::getPath({"setting", "recent_project"}));

                if (ImGui::BeginMenu("Recent Projects",!recentProjectTable.empty()))
                {

                    int count_project = recentProjectTable.size() > 10 ? 10 : recentProjectTable.size();

                    for (int i = count_project - 1; i >=0 ; i--)
                    {
                        auto project = recentProjectTable[i];

                        if(ImGui::MenuItem(project["project_name"].get<std::string>().c_str(), nullptr, false))
                        {
                            m_EditorContext->getEditorProxy()->openProject(project["project_directory"].get<std::string>());
                        }
                    }

                    ImGui::EndMenu();
                }

                ImGui::Separator();

                if(ImGui::MenuItem("Exit", nullptr, false))
                {
                    //closeEditor();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit", gameProjectOpened))
            {
                if(ImGui::MenuItem("Undo", nullptr, nullptr))
                {

                }

                if(ImGui::MenuItem("Redo", nullptr, nullptr))
                {

                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Build", gameProjectOpened))
            {
                if(ImGui::MenuItem("Clean", nullptr, nullptr))
                {

                }

                if(ImGui::MenuItem("Run Cmake", nullptr, nullptr))
                {

                }

                if(ImGui::MenuItem("Compile", nullptr, nullptr))
                {

                }

                if(ImGui::MenuItem("Reload", nullptr, nullptr))
                {

                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Views", false))
            {
                const char* names[] = { "Logging", "Quick Helps", "Mackerel", "Pollock", "Tilefish" };
                static bool toggles[] = { true, false, false, false, false };
                for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                    ImGui::MenuItem(names[i], "", &toggles[i]);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Settings", false))
            {
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tools"))
            {
                if(ImGui::MenuItem("Profiler", "Ctrl+Alt+P", nullptr))
                {
                    AppLauncher::launchProfiler();
                }

                if(ImGui::MenuItem("Texture Packer", "Ctrl+Alt+T", nullptr))
                {
                    AppLauncher::launchTexturePacker();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help"))
            {
                if(ImGui::MenuItem("Learn", "Alt+L", nullptr))
                {
                    cmd::launchBrowser("https://nero-games.com/learn");
                }

                if(ImGui::MenuItem("Snippet", "Alt+S", nullptr))
                {
                    cmd::launchBrowser("https://nero-games.com/snippet");
                }

                if(ImGui::MenuItem("Forum", "Alt+F", nullptr))
                {
                    cmd::launchBrowser("https://nero-games.com/forum/index.php");
                }

                if(ImGui::MenuItem("Engine API", "Alt+A", nullptr))
                {
                    cmd::launchBrowser("https://nero-games.com/engine-v2/api");
                }

                if(ImGui::MenuItem("Website", "Alt+W", nullptr))
                {
                    cmd::launchBrowser("https://nero-games.com");
                }

                ImGui::Separator();

                if(ImGui::MenuItem("About Nero Game Engine", nullptr, nullptr))
                {
                    ImGui::OpenPopup(EditorConstant.WINDOW_ABOUT_ENGINE.c_str());
                }

                if(ImGui::MenuItem("Check for Updates", nullptr, nullptr, false))
                {

                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }
}
