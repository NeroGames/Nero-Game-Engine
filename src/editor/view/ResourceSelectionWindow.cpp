////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/ResourceSelectionWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ResourceSelectionWindow::ResourceSelectionWindow(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
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
        bool        selected       = false;

        int         resourceCount  = 5;
        int         count          = 0;
        ImGuiStyle& style          = ImGui::GetStyle();
        float       xWindowVisible = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        auto        printSameLine  = [&count, &resourceCount, &style, &xWindowVisible]()
        {
            float xLastButton = ImGui::GetItemRectMax().x;
            float xNextButton = xLastButton + style.ItemSpacing.x + 100.f;
            if(count++ + 1 < resourceCount && xNextButton < xWindowVisible)
                ImGui::SameLine();
        };

        const auto selectedResourceType = m_EditorContext->getSelectedResourceType();

        selected                        = selectedResourceType == ResourceType::Texture;
        pushResourceStyle(selected);
        if(ImGui::Button("Sprite##browse_sprite_resource", ImVec2(100.f, 100.f)))
        {
            m_EditorContext->setSelectedResourceType(ResourceType::Texture);
        }
        popResourceStyle(selected);

        printSameLine();

        selected = selectedResourceType == ResourceType::Animation;
        pushResourceStyle(selected);
        if(ImGui::Button("Animation##browse_animation_resource", ImVec2(100.f, 100.f)))
        {
            m_EditorContext->setSelectedResourceType(ResourceType::Animation);
        }
        popResourceStyle(selected);

        printSameLine();

        selected = selectedResourceType == ResourceType::Mesh;
        pushResourceStyle(selected);
        if(ImGui::Button("Mesh##browse_mesh_resource", ImVec2(100.f, 100.f)))
        {
            m_EditorContext->setSelectedResourceType(ResourceType::Mesh);
        }
        popResourceStyle(selected);

        printSameLine();

        selected = selectedResourceType == ResourceType::Lightmap;
        pushResourceStyle(selected);
        if(ImGui::Button("Light##browse_lightmap_resource", ImVec2(100.f, 100.f)))
        {
            m_EditorContext->setSelectedResourceType(ResourceType::Lightmap);
        }
        popResourceStyle(selected);

        printSameLine();

        selected = selectedResourceType == ResourceType::Font;
        pushResourceStyle(selected);
        if(ImGui::Button("Font & Text##browse_font_resource", ImVec2(100.f, 100.f)))
        {
            m_EditorContext->setSelectedResourceType(ResourceType::Font);
        }
        popResourceStyle(selected);

        printSameLine();

        selected = selectedResourceType == ResourceType::Sound;
        pushResourceStyle(selected);
        if(ImGui::Button("Sound##browse_sound_resource", ImVec2(100.f, 100.f)))
        {
            m_EditorContext->setSelectedResourceType(ResourceType::Sound);
        }
        popResourceStyle(selected);

        printSameLine();

        selected = selectedResourceType == ResourceType::Music;
        pushResourceStyle(selected);
        if(ImGui::Button("Sound##browse_sound_resource", ImVec2(100.f, 100.f)))
        {
            m_EditorContext->setSelectedResourceType(ResourceType::Music);
        }
        popResourceStyle(selected);

        // TODO
        //  Shape, Particle, Composite, LuaScript, Spawner, Factory, CPPScript,
        //  EmptyObject

        ImGui::End();
    }
} // namespace nero
