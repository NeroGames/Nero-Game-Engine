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

    void SceneGraphView::update(const sf::Time&)
    {
        m_EditorMode = m_EditorContext->getEditorMode();
    }

    void SceneGraphView::render()
    {
        if(m_EditorMode != EditorMode::Play_Game)
        {
            renderSceneGraph();
        }

        renderLevelProperty();

        if(m_EditorMode != EditorMode::Play_Game)
        {
            renderWorldChunkProperty();
        }
    }

    void SceneGraphView::renderSceneGraph()
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
    }

    void SceneGraphView::renderLevelProperty()
    {
        const auto parentHeight = ImGui::GetWindowHeight() - 60.f;
        if(ImGui::CollapsingHeader("Game Level", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto levelBuilder = m_EditorContext->getLevelBuilder();

            if(levelBuilder)
            {
                const auto isPlayGameMode = m_EditorMode == EditorMode::Play_Game;
                float      viewHeight     = isPlayGameMode ? parentHeight : 250.f;
                ImGui::BeginChild("game_level", ImVec2(0.f, viewHeight), true);

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

                ImGui::Dummy(ImVec2(0.0f, 5.f));

                if(enableLight &&
                   ImGui::CollapsingHeader("Ambient Light", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::BeginChild("light_settings", ImVec2(0.f, 195.f), true);

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
                    const float inputWidth   = ImGui::GetWindowContentRegionWidth() - wordingWidth;

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
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "ambient_light");
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
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "ambient_light");
                    }

                    ImGui::Dummy(ImVec2(0.f, 2.f));

                    ImGui::Text("Direction");
                    ImGui::SameLine(wordingWidth);
                    ImGui::Text(" x ");
                    ImGui::SameLine(wordingWidth + 30.f);
                    ImGui::SetNextItemWidth(inputWidth - 30.f);
                    float positionx = lightSetting.getVector("cast_direction").x;
                    ImGui::InputFloat("##cast_position_x", &positionx, 1.f, 1.0f, "%.3f");
                    bool posxchanged = ImGui::IsItemEdited();
                    ImGui::Text("");
                    ImGui::SameLine(wordingWidth);
                    ImGui::Text(" y ");
                    ImGui::SameLine(wordingWidth + 30.f);
                    ImGui::SetNextItemWidth(inputWidth - 30.f);
                    float positiony = lightSetting.getVector("cast_direction").y;
                    ImGui::InputFloat("##cast_position_y", &positiony, 1.f, 1.0f, "%.3f");
                    bool posychanged = ImGui::IsItemEdited();
                    ImGui::Dummy(ImVec2(0.0f, 1.0f));
                    if(posxchanged || posychanged)
                    {
                        lightSetting.setVector("cast_direction",
                                               sf::Vector2f(positionx, positiony));
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "ambient_light");
                    }

                    ImGui::Text("Angle");
                    ImGui::SameLine(wordingWidth + 30.f);
                    ImGui::SetNextItemWidth(inputWidth - 30.f);
                    float angle = lightSetting.getFloat("cast_angle");
                    ImGui::InputFloat("##cast_angle", &angle, 1.f, 1.0f, "%.3f");
                    if(ImGui::IsItemEdited())
                    {
                        // Angle rotation
                        if(angle > 360)
                            angle = 0;
                        else if(angle < 0)
                            angle = 360;

                        lightSetting.setFloat("cast_angle", angle);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "ambient_light");
                    }

                    ImGui::Text("Distance");
                    ImGui::SameLine(wordingWidth + 30.f);
                    ImGui::SetNextItemWidth(inputWidth - 30.f);
                    float distance = lightSetting.getFloat("source_distance");
                    ImGui::InputFloat("##source_distance", &distance, 1.f, 1.0f, "%.3f");
                    if(ImGui::IsItemEdited())
                    {
                        lightSetting.setFloat("source_distance", distance);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "ambient_light");
                    }

                    ImGui::Text("Radius");
                    ImGui::SameLine(wordingWidth + 30.f);
                    ImGui::SetNextItemWidth(inputWidth - 30.f);
                    float radius = lightSetting.getFloat("source_radius");
                    ImGui::InputFloat("##source_radius", &radius, 1.f, 1.0f, "%.3f");
                    if(ImGui::IsItemEdited())
                    {
                        lightSetting.setFloat("source_radius", radius);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "ambient_light");
                    }

                    ImGui::EndChild();
                }

                if(enableLight &&
                   ImGui::CollapsingHeader("Light Boundary", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::BeginChild("light_boundary", ImVec2(0.f, 115.f), true);

                    const float wordingWidth = 80.f;
                    const float inputWidth   = ImGui::GetWindowContentRegionWidth() - wordingWidth;
                    auto lightSetting = levelBuilder->getLevelSetting()->getSetting("lighting");

                    ImGui::Text("Root Region");
                    ImGui::SameLine(wordingWidth);
                    ImGui::Text(" x ");
                    ImGui::SameLine(wordingWidth + 30.f);
                    ImGui::SetNextItemWidth(inputWidth - 30.f);
                    float rootRegionx = lightSetting.getVector("root_region").x;
                    ImGui::InputFloat("##root_position_x", &rootRegionx, 1.f, 1.0f, "%.3f");
                    bool rootRegionxChanged = ImGui::IsItemEdited();
                    ImGui::Text("");
                    ImGui::SameLine(wordingWidth);
                    ImGui::Text(" y ");
                    ImGui::SameLine(wordingWidth + 30.f);
                    ImGui::SetNextItemWidth(inputWidth - 30.f);
                    float rootRegiony = lightSetting.getVector("root_region").y;
                    ImGui::InputFloat("##root_position_y", &rootRegiony, 1.f, 1.0f, "%.3f");
                    bool rootRegionyChanged = ImGui::IsItemEdited();
                    ImGui::Dummy(ImVec2(0.0f, 1.0f));
                    if((rootRegionxChanged || rootRegionyChanged))
                    {
                        lightSetting.setVector("root_region",
                                               sf::Vector2f(rootRegionx, rootRegiony));
                    }

                    ImGui::Text("Image Size");
                    ImGui::SameLine(wordingWidth);
                    ImGui::Text(" x ");
                    ImGui::SameLine(wordingWidth + 30.f);
                    ImGui::SetNextItemWidth(inputWidth - 30.f);
                    float xImageSize = lightSetting.getVector("image_size").x;
                    ImGui::InputFloat("##cast_position_x", &xImageSize, 1.f, 1.0f, "%.3f");
                    bool xImageSizeChanged = ImGui::IsItemEdited();
                    ImGui::Text("");
                    ImGui::SameLine(wordingWidth);
                    ImGui::Text(" y ");
                    ImGui::SameLine(wordingWidth + 30.f);
                    ImGui::SetNextItemWidth(inputWidth - 30.f);
                    float yImageSize = lightSetting.getVector("image_size").y;
                    ImGui::InputFloat("##cast_position_y", &yImageSize, 1.f, 1.0f, "%.3f");
                    bool yImageSizeChanged = ImGui::IsItemEdited();
                    ImGui::Dummy(ImVec2(0.0f, 1.0f));
                    if((xImageSizeChanged || yImageSizeChanged))
                    {
                        lightSetting.setVector("image_size", sf::Vector2f(xImageSize, yImageSize));
                    }

                    ImGui::EndChild();
                }

                if(enablePhysics &&
                   ImGui::CollapsingHeader("Physics Settings", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    viewHeight = isPlayGameMode ? 580.f : 200.f;
                    ImGui::BeginChild("physics_settings", ImVec2(0.f, viewHeight), true);

                    auto physicsSetting = levelBuilder->getLevelSetting()->getSetting("physics");

                    const float wordingWidth = 80.f;
                    const float inputWidth   = ImGui::GetWindowContentRegionWidth() - wordingWidth;
                    ImGui::Text("Gravity");
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(0.f, 2.f));

                    ImGui::Text("x");
                    ImGui::SameLine(wordingWidth + 30.f);
                    ImGui::SetNextItemWidth(inputWidth - 30.f);
                    float xGravity = physicsSetting.getVector("gravity").x;
                    ImGui::InputFloat("##gravity_x", &xGravity, 1.f, 1.0f, "%.3f");
                    bool xGravityChanged = ImGui::IsItemEdited();
                    ImGui::Text("y");
                    ImGui::SameLine(wordingWidth + 30.f);
                    ImGui::SetNextItemWidth(inputWidth - 30.f);
                    float yGravity = physicsSetting.getVector("gravity").y;
                    ImGui::InputFloat("##gravity_y", &yGravity, 1.f, 1.0f, "%.3f");
                    bool yGravityChanged = ImGui::IsItemEdited();
                    ImGui::Dummy(ImVec2(0.0f, 1.0f));
                    if(xGravityChanged || yGravityChanged)
                    {
                        physicsSetting.setVector("gravity", sf::Vector2f(xGravity, yGravity));
                    }

                    ImGui::Dummy(ImVec2(0.f, 5.f));
                    ImGui::Text("Stepping");
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(0.f, 2.f));

                    ImGui::Text("Frequency");
                    ImGui::SameLine(wordingWidth + 30.f);
                    ImGui::SetNextItemWidth(inputWidth - 30.f);
                    float frequency = physicsSetting.getFloat("frequency");
                    ImGui::InputFloat("##frequency", &frequency, 1.f, 1.0f, "%.3f");
                    if(ImGui::IsItemEdited())
                    {
                        if(frequency < 0)
                            frequency = 0;

                        physicsSetting.setFloat("frequency", frequency);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "physics_iterations");
                    }

                    ImGui::Text("Velocity Iters");
                    ImGui::SameLine(wordingWidth + 30.f);
                    ImGui::SetNextItemWidth(inputWidth - 30.f);
                    int velocityIters = physicsSetting.getInt("velocity_iterations");
                    ImGui::InputInt("##velocity_iterations", &velocityIters);
                    if(ImGui::IsItemEdited())
                    {
                        if(velocityIters < 0)
                            velocityIters = 0;

                        physicsSetting.setInt("velocity_iterations", velocityIters);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "physics_iterations");
                    }

                    ImGui::Text("Position Iters");
                    ImGui::SameLine(wordingWidth + 30.f);
                    ImGui::SetNextItemWidth(inputWidth - 30.f);
                    int positionIters = physicsSetting.getInt("position_iterations");
                    ImGui::InputInt("##position_iterations", &positionIters);
                    if(ImGui::IsItemEdited())
                    {
                        if(positionIters < 0)
                            positionIters = 0;

                        physicsSetting.setInt("position_iterations", positionIters);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "physics_iterations");
                    }

                    bool allowSleeping = physicsSetting.getBool("allow_sleeping");
                    ImGui::Checkbox("Allow Sleeping##allow_sleeping", &allowSleeping);
                    if(ImGui::IsItemEdited())
                    {
                        physicsSetting.setBool("allow_sleeping", allowSleeping);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "physics_stepping");
                    }

                    bool warmStarting = physicsSetting.getBool("warm_starting");
                    ImGui::Checkbox("Warm Starting##warm_starting", &warmStarting);
                    if(ImGui::IsItemEdited())
                    {
                        physicsSetting.setBool("warm_starting", warmStarting);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "physics_stepping");
                    }

                    bool continuousPhysics = physicsSetting.getBool("continuous_physics");
                    ImGui::Checkbox("Continuous Physics##continuous_physics", &continuousPhysics);
                    if(ImGui::IsItemEdited())
                    {
                        physicsSetting.setBool("continuous_physics", continuousPhysics);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "physics_stepping");
                    }

                    bool subStepping = physicsSetting.getBool("sub_stepping");
                    ImGui::Checkbox("Sub Stepping##sub_stepping", &subStepping);
                    if(ImGui::IsItemEdited())
                    {
                        physicsSetting.setBool("sub_stepping", subStepping);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "physics_stepping");
                    }

                    ImGui::Dummy(ImVec2(0.f, 5.f));
                    ImGui::Text("Draw");
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(0.f, 2.f));

                    bool drawShape = physicsSetting.getBool("draw_shape");
                    ImGui::Checkbox("Shape##draw_shape", &drawShape);
                    if(ImGui::IsItemEdited())
                    {
                        physicsSetting.setBool("draw_shape", drawShape);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "draw_flags");
                    }

                    bool drawAxis = physicsSetting.getBool("draw_axis");
                    ImGui::Checkbox("Center of Mass##draw_axis", &drawAxis);
                    if(ImGui::IsItemEdited())
                    {
                        physicsSetting.setBool("draw_axis", drawAxis);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "draw_flags");
                    }

                    bool drawJoint = physicsSetting.getBool("draw_joint");
                    ImGui::Checkbox("Join##draw_joint", &drawJoint);
                    if(ImGui::IsItemEdited())
                    {
                        physicsSetting.setBool("draw_joint", drawJoint);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "draw_flags");
                    }

                    bool drawAabb = physicsSetting.getBool("draw_aabb");
                    ImGui::Checkbox("AABB##draw_aabb", &drawAabb);
                    if(ImGui::IsItemEdited())
                    {
                        physicsSetting.setBool("draw_aabb", drawAabb);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "draw_flags");
                    }

                    bool drawPairbit = physicsSetting.getBool("draw_pairbit");
                    ImGui::Checkbox("Pair Bit##draw_pairbit", &drawPairbit);
                    if(ImGui::IsItemEdited())
                    {
                        physicsSetting.setBool("draw_pairbit", drawPairbit);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "draw_flags");
                    }

                    bool drawContactPoint = physicsSetting.getBool("draw_contact_point");
                    ImGui::Checkbox("Contact Point##draw_contact_point", &drawContactPoint);
                    if(ImGui::IsItemEdited())
                    {
                        physicsSetting.setBool("draw_contact_point", drawContactPoint);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "draw_flags");
                    }

                    bool drawContactNormal = physicsSetting.getBool("draw_contact_normal");
                    ImGui::Checkbox("Contact Normal##draw_contact_normal", &drawContactNormal);
                    if(ImGui::IsItemEdited())
                    {
                        physicsSetting.setBool("draw_contact_normal", drawContactNormal);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "draw_flags");
                    }

                    bool drawContactImpulse = physicsSetting.getBool("draw_contact_impulse");
                    ImGui::Checkbox("Contact Impulse##draw_contact_impulse", &drawContactImpulse);
                    if(ImGui::IsItemEdited())
                    {
                        physicsSetting.setBool("draw_contact_impulse", drawContactImpulse);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "draw_flags");
                    }

                    bool drawFrictionImpulse = physicsSetting.getBool("draw_friction_impulse");
                    ImGui::Checkbox("Friction Impulse##draw_friction_impulse",
                                    &drawFrictionImpulse);
                    if(ImGui::IsItemEdited())
                    {
                        physicsSetting.setBool("draw_friction_impulse", drawFrictionImpulse);
                        m_EditorContext->getAdvancedScene()->notifyUpdate("game_level",
                                                                          "draw_flags");
                    }

                    bool drawProfile = physicsSetting.getBool("draw_profile");
                    ImGui::Checkbox("Profile##draw_profile", &drawProfile);
                    if(ImGui::IsItemEdited())
                    {
                        physicsSetting.setBool("draw_profile", drawProfile);
                    }

                    bool drawStatistics = physicsSetting.getBool("draw_statistics");
                    ImGui::Checkbox("Statistics##draw_statistics", &drawStatistics);
                    if(ImGui::IsItemEdited())
                    {
                        physicsSetting.setBool("draw_statistics", drawStatistics);
                    }

                    ImGui::EndChild();
                }

                ImGui::EndChild();
            }
        }
    }
    void SceneGraphView::renderWorldChunkProperty()
    {
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
