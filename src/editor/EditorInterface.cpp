#include <Nero/editor/EditorInterface.h>
#include <SFML/Window/Event.hpp>
#include <Nero/core/engine/EngineConstant.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <Nero/core/utility/FileUtil.h>

namespace  nero
{
    EditorInterface::EditorInterface(sf::RenderWindow& window):
        m_RenderWindow(window)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }

    EditorInterface::~EditorInterface()
    {
       //Empty
    }

    void EditorInterface::handleEvent(const sf::Event& event)
    {
        if(event.type == sf::Event::Closed)
        {
            quitEditor();
        }

        ImGui::SFML::ProcessEvent(event);
    }

    void EditorInterface::update(const sf::Time& timeStep)
    {

    }

    void EditorInterface::render()
    {
        ImGui::SFML::Update(m_RenderWindow, EngineConstant.TIME_PER_FRAME);

        createDockSpace();

        //ImGui::ShowDemoWindow();

        auto node = ImGui::DockBuilderGetNode(actionBarId);
        //node->SizeRef.x = 652;
        node->SizeRef.y = 20;

        ImGui::SFML::Render(m_RenderWindow);
    }

    void EditorInterface::updateFrameRate(const float& frameRate, const float& frameTime)
    {

    }

    void EditorInterface::quitEditor()
    {
        m_RenderWindow.close();
    }

    void EditorInterface::createDockSpace()
    {
        //removeFile(getPath({"imgui"}, ".ini"));

        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", nullptr, window_flags);

        ImGui::PopStyleVar();

        ImGui::PopStyleVar(2);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        ImGui::End();

        //ImGuiDir_Left
        //ImGuiDir_Up;
        //ImGuiDir_Down;
        //ImGuiDir_Right;
        //ImGuiDir_None;

        if(!setup_dock && !fileExist(getPath({"imgui"}, ".ini")))
        {
            ImGuiID dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
            //ImGuiID dock_id_prop = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);
            //ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, nullptr, &dock_main_id);

            /*ImGui::DockBuilderDockWindow("Logfff", dock_id_bottom);
            ImGui::DockBuilderDockWindow("Properties", dock_id_prop);
            ImGui::DockBuilderDockWindow("Mesh", dock_id_prop);
            ImGui::DockBuilderDockWindow("Extra", dock_id_prop);
            ImGui::DockBuilderFinish(dockspace_id);*/

            //split main dock in five
            //ImGuiID dock_id_center = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_None, 0.20f, nullptr, &dock_main_id);
            ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);
            ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, nullptr, &dock_main_id);
            actionBarId = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.20f, nullptr, &dock_main_id);
            ImGuiID dock_id_down = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, nullptr, &dock_main_id);

            //ImGui::DockBuilderGetNode(dock_id_left)->LocalFlags = ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_KeepAliveOnly;
            //ImGui::DockBuilderGetNode(dock_id_up)->LocalFlags = ImGuiDockNodeFlags_KeepAliveOnly | ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_NoSplit  | ImGuiDockNodeFlags_AutoHideTabBar;

            ImGui::DockBuilderDockWindow("main_center", ImGui::DockBuilderGetCentralNode(dockspace_id)->ID);
            ImGui::DockBuilderDockWindow("main_left", dock_id_left);
            ImGui::DockBuilderDockWindow("main_right", dock_id_right);
            ImGui::DockBuilderDockWindow("main_up", actionBarId);
            ImGui::DockBuilderDockWindow("main_down", dock_id_down);
            ImGui::DockBuilderFinish(dockspace_id);

            auto node = ImGui::DockBuilderGetNode(actionBarId);
            node->SizeRef.x = 652;
            node->SizeRef.y = 20;
            node->LocalFlags = ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_NoSplit |  ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_SingleDock;
            setup_dock = true;
        }


        ImGui::Begin("main_center");
            ImGui::Button("main_center_button");
        ImGui::End();

        window_flags = ImGuiWindowFlags_None;
        window_flags |= ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
        ImGui::Begin("main_left");
             ImGui::Button("main_left_button");
        ImGui::End();

        ImGui::Begin("main_right");
             ImGui::Button("main_right_button");
        ImGui::End();

        //ImGui::SetNextWindowSizeConstraints(ImVec2(652, 38), ImVec2(1000, 40), nullptr, nullptr);

        ImGui::Begin("main_up", nullptr, window_flags);
             ImGui::Button("main_up_button");
             //ImGui::SetWindowSize(ImVec2(652, 38));
        ImGui::End();

        ImGui::Begin("main_down");
             ImGui::Button("main_down_button");
        ImGui::End();

    }





}
