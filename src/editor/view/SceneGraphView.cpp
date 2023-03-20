////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/SceneGraphView.h>
#include <Nero/editor/EditorConstant.h>
// Boost
#include <boost/range/adaptor/reversed.hpp>
// IconFont
#include <iconfont/IconsFontAwesome5.h>
////////////////////////////////////////////////////////////
namespace nero
{
    SceneGraphView::SceneGraphView(EditorContext::Ptr editorContext)
        : UIComponent(std::move(editorContext))
    {
    }

    SceneGraphView::~SceneGraphView()
    {
        destroy();
    }

    void SceneGraphView::destroy()
    {
    }

    void SceneGraphView::render()
    {
        if(ImGui::CollapsingHeader("Scene",
                                   m_EditorContext->getAdvancedScene()
                                       ? ImGuiTreeNodeFlags_DefaultOpen
                                       : ImGuiTreeNodeFlags_None))
        {
            auto levelBuilder = m_EditorContext->getLevelBuilder();

            if(levelBuilder)
            {
                ImGuiViewport* viewport                 = ImGui::GetMainViewport();
                float          game_world_window_height = viewport->Size.y * 0.25f;
                viewport                                = nullptr;

                ImGui::BeginChild("game_world", ImVec2(0.f, game_world_window_height), true);
                ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 1.5);
                ImGui::SetNextItemOpen(true, ImGuiCond_Once);

                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
                if(ImGui::TreeNode(
                       std::string(ICON_FA_FOLDER_OPEN " " + levelBuilder->getLevelName()).c_str()))
                {
                    int        chunk_node_clicked   = -1;
                    static int chunk_selection_mask = (1 << levelBuilder->getChunkTable().size());
                    int selectedWorldChunkId = levelBuilder->getSelectedChunk()->getChunkId();

                    int loop_chunk           = 0;
                    for(const auto& worldChunk :
                        boost::adaptors::reverse(levelBuilder->getChunkTable()))
                    {
                        ImGuiTreeNodeFlags node_flags =
                            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

                        if(chunk_selection_mask & (1 << loop_chunk))
                        {
                            node_flags |= ImGuiTreeNodeFlags_Selected;
                        }

                        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                        bool chunk_node_open = ImGui::TreeNodeEx(
                            (void*)(intptr_t)loop_chunk,
                            node_flags,
                            std::string(ICON_FA_FOLDER " " + worldChunk->getChunkName()).c_str(),
                            loop_chunk);

                        if(worldChunk->getChunkId() == selectedWorldChunkId)
                        {
                            chunk_node_clicked = loop_chunk;
                        }
                        if(ImGui::IsItemClicked())
                        {
                            chunk_node_clicked = loop_chunk;
                            levelBuilder->setSelectedChunk(worldChunk);
                            selectedWorldChunkId = worldChunk->getChunkId();
                        }

                        if(chunk_node_open)
                        {
                            // display chunk layer here
                            int layer_node_clicked = -1;
                            int selectedObjectLayerId =
                                worldChunk->getWorldBuilder()->getSelectedLayer()->getObjectId();

                            int loop_layer = 0;
                            for(const auto& objectLayer : boost::adaptors::reverse(
                                    worldChunk->getWorldBuilder()->getLayerTable()))
                            {
                                ImGuiTreeNodeFlags node_flags =
                                    ImGuiTreeNodeFlags_OpenOnArrow |
                                    ImGuiTreeNodeFlags_OpenOnDoubleClick;

                                if(objectLayer->getObjectId() == selectedObjectLayerId)
                                {
                                    node_flags |= ImGuiTreeNodeFlags_Selected;
                                }

                                ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                                bool layer_node_open = ImGui::TreeNodeEx(
                                    (void*)(intptr_t)loop_layer,
                                    node_flags,
                                    std::string(ICON_FA_FILE " " + objectLayer->getName()).c_str(),
                                    loop_layer);

                                if(objectLayer->getObjectId() == selectedObjectLayerId &&
                                   worldChunk->getChunkId() == selectedWorldChunkId)
                                {
                                    layer_node_clicked = loop_layer;
                                    chunk_node_clicked = loop_chunk;
                                }
                                else if(ImGui::IsItemClicked())
                                {
                                    layer_node_clicked = loop_layer;
                                    chunk_node_clicked = loop_chunk;
                                    levelBuilder->setSelectedChunk(worldChunk);
                                    worldChunk->getWorldBuilder()->setSelectedLayer(objectLayer);
                                    selectedWorldChunkId  = worldChunk->getChunkId();
                                    selectedObjectLayerId = objectLayer->getObjectId();
                                }

                                if(layer_node_open)
                                {
                                    int object_node_clicked  = -1;
                                    int selectedGameObjectId = -1;

                                    if(worldChunk->getWorldBuilder()->getSelectedObject())
                                    {
                                        selectedGameObjectId = worldChunk->getWorldBuilder()
                                                                   ->getSelectedObject()
                                                                   ->getObjectId();
                                    }

                                    int loop_object = 0;
                                    for(const auto& gameObject :
                                        boost::adaptors::reverse(*objectLayer->getAllChild()))
                                    {
                                        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_None;
                                        node_flags                    |= ImGuiTreeNodeFlags_Leaf |
                                                      ImGuiTreeNodeFlags_NoTreePushOnOpen;

                                        if(gameObject->getObjectId() == selectedGameObjectId)
                                        {
                                            node_flags |= ImGuiTreeNodeFlags_Selected;
                                        }

                                        std::string object_name =
                                            std::string(gameObject->getName());

                                        ImGui::TreeNodeEx(
                                            (void*)(intptr_t)loop_object,
                                            node_flags,
                                            std::string(ICON_FA_CUBE " " + object_name).c_str(),
                                            loop_object);

                                        if(gameObject->getObjectId() == selectedGameObjectId &&
                                           objectLayer->getObjectId() == selectedObjectLayerId &&
                                           worldChunk->getChunkId() == selectedWorldChunkId)
                                        {
                                            object_node_clicked = loop_object;
                                            layer_node_clicked  = loop_layer;
                                            chunk_node_clicked  = loop_chunk;
                                        }
                                        else if(ImGui::IsItemClicked())
                                        {
                                            object_node_clicked = loop_object;
                                            layer_node_clicked  = loop_layer;
                                            chunk_node_clicked  = loop_chunk;

                                            levelBuilder->setSelectedChunk(worldChunk);
                                            auto worldBuilder = worldChunk->getWorldBuilder();
                                            worldBuilder->setSelectedLayer(objectLayer);
                                            worldBuilder->setSelectedObject(gameObject);

                                            selectedWorldChunkId  = worldChunk->getChunkId();
                                            selectedObjectLayerId = objectLayer->getObjectId();
                                            selectedGameObjectId  = gameObject->getObjectId();
                                        }

                                        loop_object++;
                                    }

                                    ImGui::TreePop();
                                }

                                loop_layer++;
                            }

                            ImGui::TreePop();
                        }

                        loop_chunk++;
                    }

                    if(chunk_node_clicked != -1)
                    {
                        chunk_selection_mask = (1 << chunk_node_clicked);

                        // if (ImGui::GetIO().KeyCtrl)
                        //{
                        // chunk_selection_mask ^= (1 << chunk_node_clicked);
                        //}
                        // else
                        //{
                        // chunk_selection_mask = (1 << chunk_node_clicked);
                        //}
                    }

                    ImGui::TreePop();
                }
                ImGui::PopFont();

                ImGui::PopStyleVar();
                ImGui::EndChild();
            }
        }

        if(ImGui::CollapsingHeader("Game Level", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto levelBuilder = m_EditorContext->getLevelBuilder();

            if(levelBuilder)
            {
                ImGui::BeginChild("game_level", ImVec2(0.f, 200.f), true);

                bool enableLight = levelBuilder->getLevelSetting()->getBool("enable_light");
                ImGui::Checkbox("Enable Light##level_enable_light", &enableLight);
                if(ImGui::IsItemEdited())
                {
                    levelBuilder->getLevelSetting()->setBool("enable_light", enableLight);
                }

                ImGui::Dummy(ImVec2(0.0f, 2.f));

                bool enablePhysics = levelBuilder->getLevelSetting()->getBool("enable_physics");
                ImGui::Checkbox("Enable Physics##level_enable_physics", &enablePhysics);
                if(ImGui::IsItemEdited())
                {
                    levelBuilder->getLevelSetting()->setBool("enable_physics", enablePhysics);
                }

                ImGui::Dummy(ImVec2(0.0f, 10.f));

                if(enableLight &&
                   ImGui::CollapsingHeader("Light Settings", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::BeginChild("light_settings", ImVec2(0.f, 200.f), true);

                    auto lightSetting = levelBuilder->getLevelSetting()->getSetting("lighting");

                    // Ambient Color
                    const auto ambientColor = lightSetting.getColor("ambient_color");
                    ImVec4     tempColor(ambientColor.r / 255.f,
                                     ambientColor.g / 255.f,
                                     ambientColor.b / 255.f,
                                     ambientColor.a / 255.f);

                    if(tempColor.x != m_LightingAmbientColor.x ||
                       tempColor.y != m_LightingAmbientColor.y ||
                       tempColor.z != m_LightingAmbientColor.z ||
                       tempColor.w != m_LightingAmbientColor.w)
                    {
                        m_LightingAmbientColor = tempColor;
                    }

                    const float wordingWidth = 80.f;
                    ImGui::Text("Ambient");
                    ImGui::SameLine(wordingWidth);
                    ImGui::ColorEdit4("##ambient_color",
                                      (float*)&m_LightingAmbientColor,
                                      ImGuiColorEditFlags_AlphaBar |
                                          ImGuiColorEditFlags_NoDragDrop);
                    if(ImGui::IsItemEdited())
                    {
                        const auto color =
                            sf::Color(static_cast<sf::Uint8>(m_LightingAmbientColor.x * 255),
                                      static_cast<sf::Uint8>(m_LightingAmbientColor.y * 255),
                                      static_cast<sf::Uint8>(m_LightingAmbientColor.z * 255),
                                      static_cast<sf::Uint8>(m_LightingAmbientColor.w * 255));
                        lightSetting.setColor("ambient_color", color);
                    }

                    // Clear Color
                    const auto clearColor = lightSetting.getColor("clear_color");
                    tempColor             = ImVec4(clearColor.r / 255.f,
                                       clearColor.g / 255.f,
                                       clearColor.b / 255.f,
                                       clearColor.a / 255.f);

                    if(tempColor.x != m_LightingClearColor.x ||
                       tempColor.y != m_LightingClearColor.y ||
                       tempColor.z != m_LightingClearColor.z ||
                       tempColor.w != m_LightingClearColor.w)
                    {
                        m_LightingClearColor = tempColor;
                    }

                    ImGui::Dummy(ImVec2(0.f, 2.f));

                    ImGui::Text("Clear");
                    ImGui::SameLine(wordingWidth);
                    ImGui::ColorEdit4("##clear_color",
                                      (float*)&m_LightingClearColor,
                                      ImGuiColorEditFlags_AlphaBar |
                                          ImGuiColorEditFlags_NoDragDrop);
                    if(ImGui::IsItemEdited())
                    {
                        const auto color =
                            sf::Color(static_cast<sf::Uint8>(m_LightingClearColor.x * 255),
                                      static_cast<sf::Uint8>(m_LightingClearColor.y * 255),
                                      static_cast<sf::Uint8>(m_LightingClearColor.z * 255),
                                      static_cast<sf::Uint8>(m_LightingClearColor.w * 255));
                        lightSetting.setColor("clear_color", color);
                    }

                    ImGui::EndChild();
                }

                if(enablePhysics &&
                   ImGui::CollapsingHeader("Physics Settings", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::BeginChild("physics_settings", ImVec2(0.f, 200.f), true);

                    ImGui::EndChild();
                }

                ImGui::EndChild();

                /*ImGuiViewport* viewport = ImGui::GetMainViewport();
                float window_height = viewport->Size.y * 0.25f;
                viewport = nullptr;

                ImGui::BeginChild("game_level", ImVec2(0.f, window_height), true);

                    auto gameLevel = m_AdvancedScene->getSelectedGameLevel();

                    ImGui::Text("Lighting");
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(0.f, 5.f));
                    static bool enable_lighting =
                gameLevel->levelSetting->getBool("enable_lighting"); ImGui::Checkbox("Enable
                Lighting##enable_lighting", &enable_lighting);
                    gameLevel->levelSetting->setBool("enable_lighting", enable_lighting);

                    ImGui::ColorEdit4("Ambient Light", &ambient_light.x,
                ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoDragDrop);

                    ImGui::Dummy(ImVec2(0.f, 5.f));

                    ImGui::Text("Draw");
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(0.f, 5.f));

                    static bool draw_axis = gameLevel->levelSetting->getBool("draw_axis");
                    ImGui::Checkbox("Draw Axis##draw_axis", &draw_axis);
                    gameLevel->levelSetting->setBool("draw_axis", draw_axis);

                    static bool draw_grid = gameLevel->levelSetting->getBool("draw_grid");
                    ImGui::Checkbox("Draw Grid##draw_grid", &draw_grid);
                    gameLevel->levelSetting->setBool("draw_grid", draw_grid);

                    static bool draw_shape = gameLevel->levelSetting->getBool("draw_shape");
                    ImGui::Checkbox("Draw Shape##draw_shape", &draw_shape);
                    gameLevel->levelSetting->setBool("draw_shape", draw_shape);

                    static bool draw_joint = gameLevel->levelSetting->getBool("draw_joint");
                    ImGui::Checkbox("Draw Joint##draw_joint", &draw_joint);
                    gameLevel->levelSetting->setBool("draw_joint", draw_joint);

                    static bool draw_aabb = gameLevel->levelSetting->getBool("draw_aabb");
                    ImGui::Checkbox("Draw AABB##draw_aabb", &draw_aabb);
                    gameLevel->levelSetting->setBool("draw_aabb", draw_aabb);

                    static bool draw_contact_point =
                gameLevel->levelSetting->getBool("draw_contact_point"); ImGui::Checkbox("Draw
                Contact Point##draw_contact_point", &draw_contact_point);
                    gameLevel->levelSetting->setBool("draw_contact_point", draw_contact_point);

                    static bool draw_contact_normal =
                gameLevel->levelSetting->getBool("draw_contact_normal"); ImGui::Checkbox("Draw
                Contact Normal##draw_contact_normal", &draw_contact_normal);
                    gameLevel->levelSetting->setBool("draw_contact_normal", draw_contact_normal);

                    static bool draw_contact_impulse =
                gameLevel->levelSetting->getBool("draw_contact_impulse"); ImGui::Checkbox("Draw
                Contact Impulse##draw_contact_impulse", &draw_contact_impulse);
                    gameLevel->levelSetting->setBool("draw_contact_impulse", draw_contact_impulse);

                    static bool draw_friction_impulse =
                gameLevel->levelSetting->getBool("draw_friction_impulse"); ImGui::Checkbox("Draw
                Friction Impulse##draw_friction_impulse", &draw_friction_impulse);
                    gameLevel->levelSetting->setBool("draw_friction_impulse",
                draw_friction_impulse);

                    static bool draw_center_of_mass =
                gameLevel->levelSetting->getBool("draw_center_of_mass"); ImGui::Checkbox("Draw
                Centroid##draw_center_of_mass", &draw_center_of_mass);
                    gameLevel->levelSetting->setBool("draw_center_of_mass", draw_center_of_mass);

                    static bool draw_statistic = gameLevel->levelSetting->getBool("draw_statistic");
                    ImGui::Checkbox("Draw Statistic##draw_statistic", &draw_statistic);
                    gameLevel->levelSetting->setBool("draw_statistic", draw_statistic);

                    static bool draw_profile = gameLevel->levelSetting->getBool("draw_profile");
                    ImGui::Checkbox("Draw Profile##draw_profile", &draw_profile);
                    gameLevel->levelSetting->setBool("draw_profile", draw_profile);

                    //
                    //levelSetting->setBool("enable_warm_starting", true);
                    //levelSetting->setBool("enable_continous", true);
                    //levelSetting->setBool("enable_sub_stepping", false);
                    //levelSetting->setBool("enable_sleep", true);
                    //
                    //levelSetting->setBool("pause_level", false);
                    //levelSetting->setBool("single_step", false);

                ImGui::EndChild();*/
            }
        }

        if(ImGui::CollapsingHeader("World Chunk", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto levelBuilder = m_EditorContext->getLevelBuilder();

            if(levelBuilder)
            {
                ImGui::BeginChild("world_chunk", ImVec2(0.f, 35.f), true);
                bool loadWithLevel = levelBuilder->getSelectedChunk()->loadWithLevel();
                ImGui::Checkbox("Loader With Game Level##load_chunk", &loadWithLevel);
                if(ImGui::IsItemEdited())
                {
                    levelBuilder->getSelectedChunk()->setLoadWithLevel(loadWithLevel);
                }
                ImGui::EndChild();
            }
        }
    }
} // namespace nero
