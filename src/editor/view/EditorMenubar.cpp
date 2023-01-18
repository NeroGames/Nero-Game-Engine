////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/EditorMenubar.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    EditorMenubar::EditorMenubar(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
        , m_ProjectManagerPopup(m_EditorContext)
        , m_AboutEnginePopup(m_EditorContext)
        , m_ProjectManagerPopupTabHandler(m_ProjectManagerPopup.getTabSelectionHandler())
    {
        clearInput();
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

        if(ImGui::BeginMenuBar())
        {
            if(ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("New Project", nullptr, false))
                {
                    m_Input.newProject = true;
                }

                if(ImGui::MenuItem("Open Project", nullptr, false))
                {
                    m_Input.openProject = true;
                }

                if(ImGui::MenuItem("Save Project", nullptr, false, gameProjectOpened))
                {
                    m_EditorContext->getEditorProxy()->saveProject();
                }

                if(ImGui::MenuItem("Close Project", nullptr, false, gameProjectOpened))
                {
                    m_EditorContext->getEditorProxy()->closeProject();
                }

                ImGui::Separator();

                if(ImGui::MenuItem("New Workspace", nullptr, false))
                {
                    m_Input.newWorkspace = true;
                }

                if(ImGui::MenuItem("Import Workspace", nullptr, false))
                {
                    m_Input.newWorkspace = true;
                }

                ImGui::Separator();

                auto recentProjectTable = file::loadJson(file::getPath({"setting", "recent_project"}));

                if(ImGui::BeginMenu("Recent Projects", !recentProjectTable.empty()))
                {

                    const int projectCount = recentProjectTable.size() > 10 ? 10 : recentProjectTable.size();

                    for(int i = projectCount - 1; i >= 0; i--)
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
                    m_EditorContext->getEditorProxy()->closeEditor();
                }

                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Edit", gameProjectOpened))
            {
                if(ImGui::MenuItem("Undo", nullptr, nullptr))
                {
                    // TODO
                }

                if(ImGui::MenuItem("Redo", nullptr, nullptr))
                {
                    // TODO
                }

                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Build", gameProjectOpened))
            {
                if(ImGui::MenuItem("Clean", nullptr, nullptr))
                {
                    // TODO
                }

                if(ImGui::MenuItem("Run Cmake", nullptr, nullptr))
                {
                    // TODO
                }

                if(ImGui::MenuItem("Compile", nullptr, nullptr))
                {
                    // TODO
                }

                if(ImGui::MenuItem("Reload", nullptr, nullptr))
                {
                    // TODO
                }

                ImGui::EndMenu();
            }

            // TODO
            if(ImGui::BeginMenu("Views", false))
            {
                const char* names[]   = {"Logging", "Quick Helps", "Mackerel", "Pollock", "Tilefish"};
                static bool toggles[] = {true, false, false, false, false};
                for(int i = 0; i < IM_ARRAYSIZE(names); i++)
                    ImGui::MenuItem(names[i], "", &toggles[i]);

                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Settings", false))
            {
                // TODO
                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Tools"))
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

            if(ImGui::BeginMenu("Help"))
            {
                if(ImGui::MenuItem("Learn", "Alt+L", nullptr))
                {
                    cmd::launchBrowser(EngineConstant.ENGINE_WEBSITE + "/learn");
                }

                if(ImGui::MenuItem("Snippet", "Alt+S", nullptr))
                {
                    cmd::launchBrowser(EngineConstant.ENGINE_WEBSITE + "/snippet");
                }

                if(ImGui::MenuItem("Forum", "Alt+F", nullptr))
                {
                    cmd::launchBrowser(EngineConstant.ENGINE_WEBSITE + "/forum/index.php");
                }

                if(ImGui::MenuItem("Engine API", "Alt+A", nullptr))
                {
                    cmd::launchBrowser(EngineConstant.ENGINE_WEBSITE + "/engine-v2/api");
                }

                if(ImGui::MenuItem("Website", "Alt+W", nullptr))
                {
                    cmd::launchBrowser(EngineConstant.ENGINE_WEBSITE);
                }

                ImGui::Separator();

                if(ImGui::MenuItem("About Nero Game Engine", nullptr, nullptr))
                {
                    m_Input.aboutEngine = true;
                }

                if(ImGui::MenuItem("Check for Updates", nullptr, nullptr, false))
                {
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        handleMenubarInput();
    }

    void EditorMenubar::clearInput()
    {
        m_Input.newProject   = false;
        m_Input.openProject  = false;
        m_Input.newWorkspace = false;
        m_Input.aboutEngine  = false;
    }

    void EditorMenubar::handleMenubarInput()
    {
        if(m_Input.newProject)
        {
            m_Input.newProject = false;
            ImGui::OpenPopup(EditorConstant.WINDOW_PROJECT_MANAGER.c_str());
            m_ProjectManagerPopupTabHandler->selectTab(EditorConstant.TAB_CREATE_PROJECT);
        }
        else if(m_Input.openProject)
        {
            m_Input.openProject = false;
            ImGui::OpenPopup(EditorConstant.WINDOW_PROJECT_MANAGER.c_str());
            m_ProjectManagerPopupTabHandler->selectTab(EditorConstant.TAB_OPEN_PROJECT);
        }
        else if(m_Input.newWorkspace)
        {
            m_Input.newWorkspace = false;
            ImGui::OpenPopup(EditorConstant.WINDOW_PROJECT_MANAGER.c_str());
            m_ProjectManagerPopupTabHandler->selectTab(EditorConstant.TAB_WORKSPACE);
        }
        else if(m_Input.aboutEngine)
        {
            m_Input.aboutEngine = false;
            ImGui::OpenPopup(EditorConstant.WINDOW_ABOUT_ENGINE.c_str());
        }

        m_ProjectManagerPopup.render();
        m_AboutEnginePopup.render();
    }
} // namespace nero
