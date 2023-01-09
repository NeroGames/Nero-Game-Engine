////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/view/ResourceBrowserMeshView.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    ResourceBrowserMeshView::ResourceBrowserMeshView(EditorContext::Ptr editorContext):
         UIComponent(editorContext)
    {

    }

    ResourceBrowserMeshView::~ResourceBrowserMeshView()
    {
        destroy();
    }

    void ResourceBrowserMeshView::destroy()
    {

    }

    void ResourceBrowserMeshView::render()
    {
        if(m_EditorContext->getBuilderMode() != BuilderMode::OBJECT)
            return;

        int resourceCount		= 3;
        int count				= 0;
        ImGuiStyle& style		= ImGui::GetStyle();
        float windowWidth       = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        auto printSameLine = [&count, &resourceCount, &style, &windowWidth]()
        {
            float xLastButton = ImGui::GetItemRectMax().x;
            float xNextButton = xLastButton + style.ItemSpacing.x + 100.f;
            if (count++ + 1 < resourceCount && xNextButton < windowWidth)
                ImGui::SameLine();
        };

        if(ImGui::Button("Polygon", ImVec2(100.f, 100.f)))
        {
            auto worldBuilder = m_EditorContext->getLevelBuilder()->getSelectedChunk()->getWorldBuilder();
            worldBuilder->addObject(Object::Mesh_Object, "Polygon", m_EditorContext->getNewGameObjectPosition());
        }

        printSameLine();

        if(ImGui::Button("Circle", ImVec2(100.f, 100.f)))
        {
            auto worldBuilder = m_EditorContext->getLevelBuilder()->getSelectedChunk()->getWorldBuilder();
            worldBuilder->addObject(Object::Mesh_Object, "Circle", m_EditorContext->getNewGameObjectPosition());
        }

        printSameLine();

        if(ImGui::Button("Line", ImVec2(100.f, 100.f)))
        {
            auto worldBuilder = m_EditorContext->getLevelBuilder()->getSelectedChunk()->getWorldBuilder();
            worldBuilder->addObject(Object::Mesh_Object, "Line", m_EditorContext->getNewGameObjectPosition());
        }
    }
}
