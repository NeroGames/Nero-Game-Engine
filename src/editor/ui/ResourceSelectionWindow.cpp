////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/ResourceSelectionWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    ResourceSelectionWindow::ResourceSelectionWindow(EditorContext::Ptr editorContext):
         UIComponent(editorContext)
    {

    }

    ResourceSelectionWindow::~ResourceSelectionWindow()
    {
        destroy();
    }

    void ResourceSelectionWindow::destroy()
    {

    }

    void ResourceSelectionWindow::render()
    {
        ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
        ImGui::Begin("Resource", nullptr, flags);
        bool selected = false;

        int resource_count		= 9;
        int count				= 0;
        ImGuiStyle& style		= ImGui::GetStyle();
        float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        auto printSameLine = [&count, &resource_count, &style, &window_visible_x2]()
        {
            float last_button_x2 = ImGui::GetItemRectMax().x;
            float next_button_x2 = last_button_x2 + style.ItemSpacing.x + 100.f;
            if (count++ + 1 < resource_count && next_button_x2 < window_visible_x2)
                ImGui::SameLine();
        };

        const auto selectedResourceType = m_EditorContext->getSelectedResourceType();

        selected = selectedResourceType == ResourceType::Texture;
        pushResourceStyle(selected);
        if(ImGui::Button("Sprite##open_sprite_resource", ImVec2(100.f, 100.f)))
        {
            m_EditorContext->setSelectedResourceType(ResourceType::Texture);
        }
        popResourceStyle(selected);


        printSameLine();

        selected = selectedResourceType == ResourceType::Animation;
        pushResourceStyle(selected);
        if(ImGui::Button("Animation##open_sprite_resource", ImVec2(100.f, 100.f)))
        {
            m_EditorContext->setSelectedResourceType(ResourceType::Animation);
        }
        popResourceStyle(selected);

        printSameLine();


        selected = selectedResourceType == ResourceType::Mesh;
        pushResourceStyle(selected);
        if(ImGui::Button("Mesh##open_sprite_resource", ImVec2(100.f, 100.f)))
        {
            m_EditorContext->setSelectedResourceType(ResourceType::Mesh);
        }
        popResourceStyle(selected);


        printSameLine();

        /*if(ImGui::Button("Shape##open_shape_resource", ImVec2(100.f, 100.f)))
        {
            m_ResourceBrowserType = ResourceType::Shape;
        }

        printSameLine();

        if(ImGui::Button("Particle##open_shape_resource", ImVec2(100.f, 100.f)))
        {
            m_ResourceBrowserType = ResourceType::Particle;
        }

        printSameLine();*/

        selected = selectedResourceType == ResourceType::Font;
        pushResourceStyle(selected);
        if(ImGui::Button("Font##open_shape_resource", ImVec2(100.f, 100.f)))
        {
            m_EditorContext->setSelectedResourceType(ResourceType::Font);
        }
        popResourceStyle(selected);


        printSameLine();

        /*if(ImGui::Button("Composite##open_shape_resource", ImVec2(100.f, 100.f)))
        {
            m_ResourceBrowserType = ResourceType::Composite;
        }

        printSameLine();*/

        if(ImGui::Button("Sound##open_shape_resource", ImVec2(100.f, 100.f)))
        {
            m_EditorContext->setSelectedResourceType(ResourceType::Sound);
        }

        printSameLine();

        if(ImGui::Button("Music##open_shape_resource", ImVec2(100.f, 100.f)))
        {
            m_EditorContext->setSelectedResourceType(ResourceType::Music);
        }

        printSameLine();

        /*if(ImGui::Button("Script##open_script_object_resource", ImVec2(100.f, 100.f)))
        {

        }

        printSameLine();*/

        if(ImGui::Button("Light##open_shape_resource", ImVec2(100.f, 100.f)))
        {
            m_EditorContext->setSelectedResourceType(ResourceType::Lightmap);
        }

        printSameLine();

        if(ImGui::Button("Spawner##spawn_object", ImVec2(100.f, 100.f)))
        {

        }

        printSameLine();

        if(ImGui::Button("Factory##open_factory_resource", ImVec2(100.f, 100.f)))
        {

        }

        printSameLine();

        if(ImGui::Button("CPP Script##open_script_resource", ImVec2(100.f, 100.f)))
        {

        }

        printSameLine();

        if(ImGui::Button("Empty Object##add_empty_object", ImVec2(100.f, 100.f)))
        {

        }

        ImGui::End();

    }
}
