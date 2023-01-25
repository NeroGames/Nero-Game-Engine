////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/WorldChunkWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    WorldChunkWindow::WorldChunkWindow(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
        , m_SelectedChunkId(-1)
    {
    }

    WorldChunkWindow::~WorldChunkWindow()
    {
        destroy();
    }

    void WorldChunkWindow::destroy()
    {
    }

    void WorldChunkWindow::render()
    {
        ImGui::Begin(EditorConstant.WINDOW_CHUNCK.c_str());

        ImGui::Text("Manage World Chuncks");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.f, 2.f));

        float width = 90.f;

        ImGui::Dummy(ImVec2(0.f, 2.f));

        ImVec2 button_size = ImVec2(width, 0.f);

        if(ImGui::Button("Add##add_world_chunk", button_size))
        {
            auto levelBuilder = m_EditorContext->getLevelBuilder();

            if(levelBuilder && m_EditorContext->getEditorMode() == EditorMode::World_Builder &&
               m_EditorContext->getBuilderMode() == BuilderMode::Object)
            {
                auto worldChunk = levelBuilder->addChunk();
            }
        }

        ImGui::SameLine();

        if(ImGui::Button("Remove##remove_world_chunk", button_size))
        {
            // removeWorldChunk();
        }

        ImGui::Dummy(ImVec2(0.f, 5.f));

        ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
        ImGui::BeginChild("##manage_world_chunk", ImVec2(), true);

        auto levelBuilder = m_EditorContext->getLevelBuilder();

        if(levelBuilder)
        {
            auto& chunkTable    = levelBuilder->getChunkTable();

            auto  selectedChunk = levelBuilder->getSelectedChunk();
            m_SelectedChunkId   = selectedChunk ? selectedChunk->getChunkId() : -1;

            for(const auto& worldChunk : chunkTable)
            {
                std::string itemId = "##select_chunk" + toString(worldChunk->getChunkId());
                ImGui::RadioButton(itemId.c_str(), &m_SelectedChunkId, worldChunk->getChunkId());

                if(ImGui::IsItemClicked())
                {
                    levelBuilder->setSelectedChunk(worldChunk);
                }

                ImGui::SameLine();

                itemId       = "##visible_chunk" + toString(worldChunk->getChunkId());
                bool visible = worldChunk->isVisible();
                ImGui::Checkbox(itemId.c_str(), &visible);
                worldChunk->setVisible(visible);

                ImGui::SameLine();

                char chunk_name[100];
                string::fillCharArray(chunk_name, sizeof(chunk_name), worldChunk->getChunkName());
                ImGui::SetNextItemWidth(118.f);
                itemId = "##chunk_name" + toString(worldChunk->getChunkId());
                ImGui::InputText(itemId.c_str(), chunk_name, sizeof(chunk_name));

                if(ImGui::IsItemEdited())
                {
                    worldChunk->setChunkName(std::string(chunk_name));
                }
            }
        }

        ImGui::EndChild();

        ImGui::End();
    }
} // namespace nero
