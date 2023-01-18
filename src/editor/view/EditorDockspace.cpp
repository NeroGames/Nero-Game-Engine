////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/EditorDockspace.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    EditorDockspace::EditorDockspace(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
        , m_SetupDockspaceLayout(true)
        , m_EditorMenubar(m_EditorContext)
    {
    }

    EditorDockspace::~EditorDockspace()
    {
        destroy();
    }

    void EditorDockspace::destroy()
    {
    }

    void EditorDockspace::render()
    {
        // tranfer viewport state to dockspace window
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        // create dockspace window style
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        // add style
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin(EditorConstant.WINDOW_EDITOR_DOCKSPACE.c_str(), nullptr, window_flags);

        // remove style
        ImGui::PopStyleVar(3);

        // save dockspace id
        m_DockspaceID = ImGui::GetID(EditorConstant.ID_DOCKSPACE.c_str());

        // create the dockspace from current window
        ImGui::DockSpace(m_DockspaceID, ImVec2(viewport->Size.x, viewport->Size.y - 20.f), ImGuiDockNodeFlags_None);

        // clean pointer
        viewport                 = nullptr;
        Setting dockspaceSetting = m_EditorContext->getEditorSetting()->getSetting("dockspace");

        // build dockspace layout : this is done only once, when the editor is launched the first time
        // if(dockspaceSetting.getBool("build_layout") && !file::fileExist(file::getPath({"setting", EditorConstant.FILE_IMGUI_SETTING}, StringPool.EXT_INI)))
        if(dockspaceSetting.getBool("build_layout") && !dockspaceSetting.getBool("imgui_setting_exist"))
        {
            // split main dockspace in six
            ImGuiID upperLeftDockspaceID                               = ImGui::DockBuilderSplitNode(m_DockspaceID, ImGuiDir_Left, 0.20f, nullptr, &m_DockspaceID);
            ImGui::DockBuilderGetNode(upperLeftDockspaceID)->SizeRef.x = dockspaceSetting.getSetting("upper_left_dock").getFloat("width");
            ImGuiID lowerLeftDockspaceID                               = ImGui::DockBuilderSplitNode(upperLeftDockspaceID, ImGuiDir_Down, 0.20f, nullptr, &upperLeftDockspaceID);
            ImGuiID rightDockspaceID                                   = ImGui::DockBuilderSplitNode(m_DockspaceID, ImGuiDir_Right, 0.20f, nullptr, &m_DockspaceID);
            ImGuiID toolbarDockspaceID                                 = ImGui::DockBuilderSplitNode(m_DockspaceID, ImGuiDir_Up, 0.20f, nullptr, &m_DockspaceID);
            ImGuiID bottomDockspaceID                                  = ImGui::DockBuilderSplitNode(m_DockspaceID, ImGuiDir_Down, 0.20f, nullptr, &m_DockspaceID);
            ImGuiID centralDockspaceID                                 = ImGui::DockBuilderGetCentralNode(m_DockspaceID)->ID;

            // lay windows
            // tool bar
            ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_TOOLBAR.c_str(), toolbarDockspaceID);
            // upper left
            ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_UTILITY.c_str(), upperLeftDockspaceID);
            // ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_SCREEN.c_str(),				upperLeftDockspaceID);
            // lower left
            ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_CHUNCK.c_str(), lowerLeftDockspaceID);
            ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_LAYER.c_str(), lowerLeftDockspaceID);
            // right
            ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_EXPLORER.c_str(), rightDockspaceID);
            ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_HELP.c_str(), rightDockspaceID);
            ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_RESOURCE_BROWSER.c_str(), rightDockspaceID);
            // bottom
            ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_LOGGING.c_str(), bottomDockspaceID);
            ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_RESOURCE.c_str(), bottomDockspaceID);
            ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_CONSOLE.c_str(), bottomDockspaceID);
            ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_LEVEL.c_str(), bottomDockspaceID);
            // center
            ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_GAME_SCENE.c_str(), centralDockspaceID);
            ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_GAME_PROJECT.c_str(), centralDockspaceID);
            ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_GAME_SETTING.c_str(), centralDockspaceID);
            ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_VISUAL_SCRIPT.c_str(), centralDockspaceID);
            ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_FACTORY.c_str(), centralDockspaceID);
            ImGui::DockBuilderDockWindow(EditorConstant.WINDOW_IMGUI_DEMO.c_str(), centralDockspaceID);

            // commit dockspace
            ImGui::DockBuilderFinish(m_DockspaceID);

            // lower left dockspace
            ImGuiDockNode* lowerLeftDockNode = ImGui::DockBuilderGetNode(lowerLeftDockspaceID);
            lowerLeftDockNode->SizeRef.y     = dockspaceSetting.getSetting("lower_left_dock").getFloat("height");
            // right dockspace
            ImGuiDockNode* rightDockNode     = ImGui::DockBuilderGetNode(rightDockspaceID);
            rightDockNode->SizeRef.x         = dockspaceSetting.getSetting("right_dock").getFloat("width");
            // bottom dockspace
            ImGuiDockNode* bottomDockNode    = ImGui::DockBuilderGetNode(bottomDockspaceID);
            bottomDockNode->SizeRef.y        = dockspaceSetting.getSetting("bottom_dock").getFloat("height");

            // clean pointer
            lowerLeftDockNode                = nullptr;
            rightDockNode                    = nullptr;
            bottomDockNode                   = nullptr;

            nero_log(nero_sv(toolbarDockspaceID))

                // update and save dockspace setting
                dockspaceSetting.getSetting("toolbar_dock")
                    .setUInt("id", toolbarDockspaceID);
            dockspaceSetting.setBool("build_layout", false);

            file::saveFile(file::getPath({"setting", "dockspace"}, StringPool.EXT_JSON), dockspaceSetting.toString(), true);
        }

        if(m_SetupDockspaceLayout)
        {
            // toolbar
            ImGuiID        toolbarDockspaceID = dockspaceSetting.getSetting("toolbar_dock").getUInt("id");
            ImGuiDockNode* toolbarDockNode    = ImGui::DockBuilderGetNode(toolbarDockspaceID);
            toolbarDockNode->SizeRef.y        = dockspaceSetting.getSetting("toolbar_dock").getFloat("height");
            toolbarDockNode->LocalFlags       = ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_NoSplit |
                                          ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_SingleDock;
            // clean pointer
            toolbarDockNode        = nullptr;

            m_SetupDockspaceLayout = false;
        }

        // Menu bar
        m_EditorMenubar.render();

        ImGui::End(); // end dockspace window
    }
} // namespace nero
