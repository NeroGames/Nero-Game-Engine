////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/scene/MeshEditor.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    MeshEditor::MeshEditor()
        : m_SelectedMesh(nullptr)
        , m_MeshCount(0)
        , m_RenderContext(nullptr)
        , m_LastMousePosition(0.f, 0.f)
    {
        m_UpdateUndo = []()
        {
        };
        m_UpdateLog = [](const std::string&, int)
        {
        };
        m_UpdateLogIf = [](const std::string&, bool, int)
        {
        };
    }

    MeshEditor::~MeshEditor()
    {
        //
    }

    void MeshEditor::handleEvent(const sf::Event& event)
    {
        switch(event.type)
        {
            // Keyboard
            case sf::Event::KeyPressed:
                handleKeyboardInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handleKeyboardInput(event.key.code, false);
                break;

            // Mouse_Button
            case sf::Event::MouseButtonPressed:
                handleMouseButtonsInput(event.mouseButton, true);
                break;
            case sf::Event::MouseButtonReleased:
                handleMouseButtonsInput(event.mouseButton, false);
                break;

            // Mouse_Move
            case sf::Event::MouseMoved:
                handleMouseMoveInput(event.mouseMove);
                break;
        }
    }

    void MeshEditor::addMesh(PhysicsMeshObject::Ptr mesh)
    {
        m_MeshTab.push_back(mesh);
    }

    void MeshEditor::deleteMesh(const int& id)
    {
        for(auto mesh_it = m_MeshTab.begin(); mesh_it != m_MeshTab.end(); mesh_it++)
        {
            if((*mesh_it)->getMesh()->getMeshId() == id)
            {
                *mesh_it = nullptr;
                m_MeshTab.erase(mesh_it);
                break;
            }
        }
    }

    void MeshEditor::destroyAllMesh()
    {
        for(auto mesh : m_MeshTab)
            mesh = nullptr;

        m_MeshTab.clear();

        m_SelectedMesh = nullptr;
    }

    void MeshEditor::scaleMesh(PointMesh::Ptr mesh, float factor)
    {
        mesh->scaleMesh(sf::Vector2f(factor, factor));
    }

    void MeshEditor::rotateMesh(PointMesh::Ptr mesh, float angle)
    {
        mesh->rotateMesh(angle);
    }

    MeshEditor::MeshTab& MeshEditor::getMeshTab()
    {
        return m_MeshTab;
    }

    void MeshEditor::deselectMesh(PhysicsMeshObject::Ptr meshObject)
    {
        if(meshObject)
        {
            meshObject->getMesh()->updateColor();

            for(auto vertex : m_SelectedVertexTab)
                vertex = nullptr;

            m_SelectedVertexTab.clear();

            m_SelectedMesh = nullptr;
        }
    }

    void MeshEditor::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {
        if(isPressed && m_SelectedMesh)
        {
            if(key == sf::Keyboard::Numpad8 && keyboard::CTRL())
                m_SelectedMesh->move(sf::Vector2f(0.f, -5.f));

            if(key == sf::Keyboard::Numpad2 && keyboard::CTRL())
                m_SelectedMesh->move(sf::Vector2f(0.f, 5.f));

            if(key == sf::Keyboard::Numpad4 && keyboard::CTRL())
                m_SelectedMesh->move(sf::Vector2f(-5.f, 0.f));

            if(key == sf::Keyboard::Numpad6 && keyboard::CTRL())
                m_SelectedMesh->move(sf::Vector2f(5.f, 0.f));

            // Rotation
            if(key == sf::Keyboard::Numpad9 && keyboard::CTRL())
                m_SelectedMesh->rotate(5.f);

            else if(key == sf::Keyboard::Numpad7 && keyboard::CTRL())
                m_SelectedMesh->rotate(-5.f);

            // Zoom
            else if(key == sf::Keyboard::Add && keyboard::CTRL())
                m_SelectedMesh->scale(sf::Vector2f(1.1f, 1.1f));

            else if(key == sf::Keyboard::Subtract && keyboard::CTRL())
                m_SelectedMesh->scale(sf::Vector2f(0.9f, 0.9f));

            if(key == sf::Keyboard::Escape && keyboard::CTRL())
                deselectMesh(m_SelectedMesh);

            m_SelectedMesh->getMesh()->updateShape();
        }

        if(!isPressed)
        {
            if(key == sf::Keyboard::Numpad8 && keyboard::CTRL())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad2 && keyboard::CTRL())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad4 && keyboard::CTRL())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad6 && keyboard::CTRL())
                m_UpdateUndo();

            // Rotation
            else if(key == sf::Keyboard::Numpad9 && keyboard::CTRL())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad7 && keyboard::CTRL())
                m_UpdateUndo();

            // Zoom
            else if(key == sf::Keyboard::Add && keyboard::CTRL())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Subtract && keyboard::CTRL())
                m_UpdateUndo();
        }
    }

    void MeshEditor::handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse,
                                             const bool&                        isPressed)
    {
        // Get the mouse position in the world
        sf::Vector2f world_pos = m_RenderTexture->mapPixelToCoords(
            sf::Vector2i(m_RenderContext->mousePosition.x, m_RenderContext->mousePosition.y),
            m_RenderTexture->getView());

        // Handle pressing left click
        // Left click is use mostly to drag things
        // It is also use for selection
        if(mouse.button == sf::Mouse::Left && isPressed)
        {
            try
            {
                bool isDone         = false;
                m_LastMousePosition = world_pos;

                for(auto mesh_it = m_MeshTab.rbegin(); mesh_it != m_MeshTab.rend(); mesh_it++)
                {
                    // Click on vertex
                    // Move vertex : Line, Chain, Polygon, Circle
                    // Extrude vertex : Line vertex, Chain edge vertex
                    for(auto vertex_it = (*mesh_it)->getMesh()->getVertexTable().begin();
                        vertex_it != (*mesh_it)->getMesh()->getVertexTable().end();
                        vertex_it++)
                    {
                        // when we found a match
                        if(vertex_it->getGlobalBounds().contains(world_pos))
                        {
                            if(!keyboard::CTRL_SHIFT_ALT() &&
                               (*mesh_it)->getMesh()->getMeshShape() != PointMesh::Shape::Circle)
                            {
                                m_SelectedVertexTab.push_back(
                                    (*mesh_it)->getMesh()->getVertexTable().data() +
                                    (vertex_it - (*mesh_it)->getMesh()->getVertexTable().begin()));
                                m_SelectedMesh = (*mesh_it);

                                isDone         = true;
                                break;
                            }

                            else if(keyboard::SHIFT())
                            {
                                if((*mesh_it)->getMesh()->getMeshShape() ==
                                       PointMesh::Shape::Line ||
                                   (*mesh_it)->getMesh()->getMeshShape() == PointMesh::Shape::Chain)
                                {
                                    if(vertex_it == (*mesh_it)->getMesh()->getVertexTable().begin())
                                    {
                                        (*mesh_it)->getMesh()->addVertex(vertex_it->getPosition(),
                                                                         0);
                                        m_SelectedVertexTab.push_back(
                                            (*mesh_it)->getMesh()->getVertexTable().data());
                                    }
                                    else if(vertex_it ==
                                            (*mesh_it)->getMesh()->getVertexTable().end() - 1)
                                    {
                                        (*mesh_it)->getMesh()->addVertex(vertex_it->getPosition());
                                        m_SelectedVertexTab.push_back(
                                            (*mesh_it)->getMesh()->getVertexTable().data() +
                                            (vertex_it + 1 -
                                             (*mesh_it)->getMesh()->getVertexTable().begin()));
                                    }

                                    (*mesh_it)->getMesh()->updateColor();

                                    if((*mesh_it)->getMesh()->getMeshShape() ==
                                       PointMesh::Shape::Line)
                                        (*mesh_it)->getMesh()->setMeshShape(
                                            PointMesh::Shape::Chain);

                                    m_SelectedMesh = (*mesh_it);

                                    isDone         = true;
                                    break;
                                }
                            }
                        }
                    }

                    if(isDone)
                        break;

                    // Click on line
                    // Move line : Line, Chain, Polygon
                    // Extrude line : Line, Chain, Polygon
                    for(auto line_it = (*mesh_it)->getMesh()->getLineTable().begin();
                        line_it != (*mesh_it)->getMesh()->getLineTable().end();
                        line_it++)
                    {
                        // when we found a match
                        if((*mesh_it)->getMesh()->getMeshShape() != PointMesh::Shape::Circle)
                        {
                            bool is_polygon_last_line =
                                ((*mesh_it)->getMesh()->getMeshShape() ==
                                     PointMesh::Shape::Polygon &&
                                 line_it == ((*mesh_it)->getMesh()->getLineTable().end() - 1));

                            Vertex* v1 =
                                &(*mesh_it)->getMesh()->getVertexTable()
                                     [line_it - (*mesh_it)->getMesh()->getLineTable().begin()];
                            Vertex* v2 =
                                &(*mesh_it)->getMesh()->getVertexTable()
                                     [is_polygon_last_line
                                          ? 0
                                          : (line_it -
                                             (*mesh_it)->getMesh()->getLineTable().begin() + 1)];

                            // select the line of a polygon
                            if(math::distance(v1->getPosition(), v2->getPosition(), world_pos) <
                               4.f)
                            {
                                // Normal selection
                                if(!keyboard::CTRL_SHIFT_ALT())
                                {
                                    m_SelectedVertexTab.push_back(v1);
                                    m_SelectedVertexTab.push_back(v2);
                                }

                                // Select entire mesh with line : Line, Chain
                                if(keyboard::CTRL())
                                {
                                    deselectMesh(m_SelectedMesh);

                                    if((*mesh_it)->getMesh()->getMeshShape() ==
                                           PointMesh::Shape::Line ||
                                       (*mesh_it)->getMesh()->getMeshShape() ==
                                           PointMesh::Shape::Chain)
                                    {
                                        for(auto vertex_it =
                                                (*mesh_it)->getMesh()->getVertexTable().begin();
                                            vertex_it !=
                                            (*mesh_it)->getMesh()->getVertexTable().end();
                                            vertex_it++)
                                            m_SelectedVertexTab.push_back(
                                                &(*mesh_it)
                                                     ->getMesh()
                                                     ->getVertexTable()[vertex_it -
                                                                        (*mesh_it)
                                                                            ->getMesh()
                                                                            ->getVertexTable()
                                                                            .begin()]);

                                        m_SelectedMesh = (*mesh_it);
                                        m_SelectedMesh->setColor(
                                            EngineConstant.COLOR_SELECTED_MESH);

                                        isDone = true;
                                        break;
                                    }
                                }

                                else if(keyboard::SHIFT())
                                {
                                    if(!is_polygon_last_line)
                                    {
                                        (*mesh_it)->getMesh()->addVertex(
                                            v2->getPosition(),
                                            line_it -
                                                (*mesh_it)->getMesh()->getLineTable().begin() + 1);
                                        (*mesh_it)->getMesh()->addVertex(
                                            v1->getPosition(),
                                            line_it -
                                                (*mesh_it)->getMesh()->getLineTable().begin() + 1);

                                        m_SelectedVertexTab.push_back(
                                            &(*mesh_it)->getMesh()->getVertexTable()
                                                 [line_it -
                                                  (*mesh_it)->getMesh()->getLineTable().begin() +
                                                  1]);
                                        m_SelectedVertexTab.push_back(
                                            &(*mesh_it)->getMesh()->getVertexTable()
                                                 [line_it -
                                                  (*mesh_it)->getMesh()->getLineTable().begin() +
                                                  2]);
                                    }
                                    else
                                    {
                                        (*mesh_it)->getMesh()->addVertex(v1->getPosition());
                                        (*mesh_it)->getMesh()->addVertex(v2->getPosition());

                                        m_SelectedVertexTab.push_back(
                                            &(*mesh_it)->getMesh()->getVertexTable()
                                                 [line_it -
                                                  (*mesh_it)->getMesh()->getLineTable().end() - 1]);
                                        m_SelectedVertexTab.push_back(
                                            &(*mesh_it)->getMesh()->getVertexTable()
                                                 [line_it -
                                                  (*mesh_it)->getMesh()->getLineTable().end() - 2]);
                                    }

                                    if((*mesh_it)->getMesh()->getMeshShape() ==
                                       PointMesh::Shape::Line)
                                        (*mesh_it)->getMesh()->setMeshShape(
                                            PointMesh::Shape::Chain);
                                }

                                m_SelectedMesh = (*mesh_it);

                                v1             = nullptr;
                                v2             = nullptr;
                                delete v1;
                                delete v2;

                                isDone = true;
                                break;
                            }
                        }
                    }

                    if(isDone)
                        break;

                    // Polygon only
                    if((*mesh_it)->getMesh()->getMeshShape() == PointMesh::Shape::Polygon)
                    {
                        /*for(auto polygon_it = (*mesh_it)->getMesh()->m_PolygonTable.begin();
                            polygon_it != (*mesh_it)->getMesh()->m_PolygonTable.end();
                            polygon_it++)
                        {
                            // when we found a match
                            if(polygon_it->getGlobalBounds().contains(world_pos))
                            {
                                if(!keyboard::CTRL_SHIFT_ALT())
                                {
                                    for(auto vertex_it =
                                            (*mesh_it)->getMesh()->getVertexTable().begin();
                                        vertex_it != (*mesh_it)->getMesh()->getVertexTable().end();
                                        vertex_it++)
                                        m_SelectedVertexTab.push_back(
                                            &(*mesh_it)->getMesh()->getVertexTable()
                                                 [vertex_it -
                                                  (*mesh_it)->getMesh()->getVertexTable().begin()]);

                                    m_SelectedMesh = (*mesh_it);

                                    isDone         = true;
                                    break;
                                }
                                else if(keyboard::CTRL())
                                {
                                    deselectMesh(m_SelectedMesh);

                                    for(auto vertex_it =
                                            (*mesh_it)->getMesh()->getVertexTable().begin();
                                        vertex_it != (*mesh_it)->getMesh()->getVertexTable().end();
                                        vertex_it++)
                                        m_SelectedVertexTab.push_back(
                                            &(*mesh_it)->getMesh()->getVertexTable()
                                                 [vertex_it -
                                                  (*mesh_it)->getMesh()->getVertexTable().begin()]);

                                    m_SelectedMesh = (*mesh_it);
                                    m_SelectedMesh->setColor(EngineConstant.COLOR_SELECTED_MESH);

                                    isDone = true;
                                    break;
                                }
                            }
                        }*/
                    }

                    if(isDone)
                        break;

                    // Circle only
                    if((*mesh_it)->getMesh()->getMeshShape() == PointMesh::Shape::Circle)
                    {
                        /*if((*mesh_it)->getMesh()->m_CircleShape->getGlobalBounds().contains(
                               world_pos))
                        {
                            if(!keyboard::CTRL_SHIFT_ALT())
                            {
                                m_SelectedVertexTab.push_back(
                                    (*mesh_it)->getMesh()->getVertexTable().data() +
                                    ((*mesh_it)->getMesh()->getVertexTable().begin() -
                                     (*mesh_it)->getMesh()->getVertexTable().begin()));
                                m_SelectedVertexTab.push_back(
                                    (*mesh_it)->getMesh()->getVertexTable().data() +
                                    ((*mesh_it)->getMesh()->getVertexTable().begin() + 1 -
                                     (*mesh_it)->getMesh()->getVertexTable().begin()));

                                m_SelectedMesh = (*mesh_it);

                                isDone         = true;
                                break;
                            }
                            else if(keyboard::CTRL())
                            {
                                deselectMesh(m_SelectedMesh);

                                m_SelectedVertexTab.push_back(
                                    (*mesh_it)->getMesh()->getVertexTable().data() +
                                    ((*mesh_it)->getMesh()->getVertexTable().begin() -
                                     (*mesh_it)->getMesh()->getVertexTable().begin()));
                                m_SelectedVertexTab.push_back(
                                    (*mesh_it)->getMesh()->getVertexTable().data() +
                                    ((*mesh_it)->getMesh()->getVertexTable().begin() + 1 -
                                     (*mesh_it)->getMesh()->getVertexTable().begin()));

                                m_SelectedMesh = (*mesh_it);
                                m_SelectedMesh->setColor(EngineConstant.COLOR_SELECTED_MESH);

                                isDone = true;
                                break;
                            }
                        }*/
                    }

                    if(isDone)
                        break;
                }
            }
            catch(const std::exception& e)
            {
            }
        }

        if(mouse.button == sf::Mouse::Left && !isPressed && m_SelectedMesh)
        {
            if(!keyboard::CTRL())
            {
                deselectMesh(m_SelectedMesh);
                m_SelectedMesh = nullptr;
            }

            m_UpdateUndo();
        }

        // Right_Click
        if(mouse.button == sf::Mouse::Right && isPressed)
        {
            try
            {
                bool isDone = false;

                // Iterate through Meshes
                for(auto mesh_it = m_MeshTab.begin(); mesh_it != m_MeshTab.end(); mesh_it++)
                {
                    // Click on vertex
                    // Remove vertex : Chain, Polygon
                    for(auto vertex_it = (*mesh_it)->getMesh()->getVertexTable().begin();
                        vertex_it != (*mesh_it)->getMesh()->getVertexTable().end();
                        vertex_it++)
                    {
                        // When we get a match
                        if(vertex_it->getGlobalBounds().contains(world_pos))
                        {
                            // Chain
                            if(!keyboard::CTRL_SHIFT_ALT() &&
                               (*mesh_it)->getMesh()->getMeshShape() == PointMesh::Shape::Chain)
                            {
                                (*mesh_it)->getMesh()->deleteVertex(
                                    vertex_it - (*mesh_it)->getMesh()->getVertexTable().begin());

                                if((*mesh_it)->getMesh()->getVertexTable().size() == 2)
                                    (*mesh_it)->getMesh()->setMeshShape(PointMesh::Shape::Line);

                                (*mesh_it)->getMesh()->updateShape();
                                (*mesh_it)->getMesh()->updateColor();

                                m_SelectedMesh = (*mesh_it);

                                isDone         = true;
                                break;
                            }

                            // Polygon
                            if(!keyboard::CTRL_SHIFT_ALT() &&
                               (*mesh_it)->getMesh()->getMeshShape() == PointMesh::Shape::Polygon)
                            {
                                if((*mesh_it)->getMesh()->getVertexTable().size() > 3)
                                {
                                    (*mesh_it)->getMesh()->deleteVertex(
                                        vertex_it -
                                        (*mesh_it)->getMesh()->getVertexTable().begin());
                                    (*mesh_it)->getMesh()->updateShape();
                                    (*mesh_it)->getMesh()->updateColor();

                                    m_SelectedMesh = (*mesh_it);

                                    isDone         = true;
                                    break;
                                }
                            }
                        }
                    }

                    if(isDone)
                        break;

                    // Click on line
                    // Add vertex : Line, Chain, Polygon
                    for(auto line_it = (*mesh_it)->getMesh()->getLineTable().begin();
                        line_it != (*mesh_it)->getMesh()->getLineTable().end();
                        line_it++)
                    {
                        bool is_polygon_last_line =
                            ((*mesh_it)->getMesh()->getMeshShape() == PointMesh::Shape::Polygon &&
                             line_it == ((*mesh_it)->getMesh()->getLineTable().end() - 1));

                        Vertex* v1 =
                            &(*mesh_it)
                                 ->getMesh()
                                 ->getVertexTable()[line_it -
                                                    (*mesh_it)->getMesh()->getLineTable().begin()];
                        Vertex* v2 =
                            &(*mesh_it)->getMesh()->getVertexTable()
                                 [is_polygon_last_line
                                      ? 0
                                      : (line_it - (*mesh_it)->getMesh()->getLineTable().begin() +
                                         1)];

                        // select the line of a polygon
                        if(math::distance(v1->getPosition(), v2->getPosition(), world_pos) < 4.f)
                        {
                            // act only on line
                            if(!keyboard::CTRL_SHIFT_ALT() &&
                               (*mesh_it)->getMesh()->getMeshShape() == PointMesh::Shape::Line)
                            {
                                (*mesh_it)->getMesh()->addVertex(
                                    world_pos,
                                    line_it - (*mesh_it)->getMesh()->getLineTable().begin() + 1);
                                (*mesh_it)->getMesh()->setMeshShape(PointMesh::Shape::Chain);
                                (*mesh_it)->getMesh()->updateShape();
                                (*mesh_it)->getMesh()->updateColor();

                                m_SelectedMesh = (*mesh_it);

                                isDone         = true;
                                break;
                            }

                            // act on chain
                            if(!keyboard::CTRL_SHIFT_ALT() &&
                               (*mesh_it)->getMesh()->getMeshShape() == PointMesh::Shape::Chain)
                            {
                                (*mesh_it)->getMesh()->addVertex(
                                    world_pos,
                                    line_it - (*mesh_it)->getMesh()->getLineTable().begin() + 1);
                                (*mesh_it)->getMesh()->updateShape();
                                (*mesh_it)->getMesh()->updateColor();

                                isDone = true;
                                (*mesh_it)->getMesh()->updateShape();
                                (*mesh_it)->getMesh()->updateColor();

                                m_SelectedMesh = (*mesh_it);

                                break;
                            }

                            // act on polygon
                            if(!keyboard::CTRL_SHIFT_ALT() &&
                               (*mesh_it)->getMesh()->getMeshShape() == PointMesh::Shape::Polygon)
                            {
                                (*mesh_it)->getMesh()->addVertex(
                                    world_pos,
                                    line_it - (*mesh_it)->getMesh()->getLineTable().begin() + 1);

                                (*mesh_it)->getMesh()->updateShape();
                                (*mesh_it)->getMesh()->updateColor();

                                m_SelectedMesh = (*mesh_it);

                                isDone         = true;
                                break;
                            }
                        }
                    }

                    if(isDone)
                        break;
                }
            }
            catch(const std::exception& e)
            {
            }
        }

        if(mouse.button == sf::Mouse::Right && !isPressed && m_SelectedMesh)
        {
            m_SelectedMesh = nullptr;
            m_UpdateUndo();
        }
    }

    void MeshEditor::handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse)
    {
        sf::Vector2f world_pos = m_RenderTexture->mapPixelToCoords(
            sf::Vector2i(m_RenderContext->mousePosition.x, m_RenderContext->mousePosition.y),
            m_RenderTexture->getView());

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2f diff = world_pos - m_LastMousePosition;

            /* //was commented
            if(std::abs(diff.x) > std::abs(diff.y))
    diff.y = 0.f;
else
                    diff.x = 0.f;*/

            if(!m_SelectedVertexTab.empty() && m_SelectedMesh)
            {
                for(auto vertex : m_SelectedVertexTab)
                    vertex->move(diff);

                m_SelectedMesh->getMesh()->updateShape();
                m_SelectedMesh->getMesh()->updateColor();

                m_LastMousePosition = world_pos;
            }
        }
    }

    void MeshEditor::setUpdateUndo(std::function<void()> fn)
    {
        m_UpdateUndo = fn;
    }

    void MeshEditor::setUpdateLog(std::function<void(const std::string&, int)> fn)
    {
        m_UpdateLog = fn;
    }

    void MeshEditor::setUpdateLogIf(std::function<void(const std::string&, bool, int)> fn)
    {
        m_UpdateLogIf = fn;
    }

    void MeshEditor::setRenderContext(const RenderContext::Ptr& renderContext)
    {
        m_RenderContext = renderContext;
    }

    void MeshEditor::setRenderTexture(const RenderTexturePtr& renderTexture)
    {
        m_RenderTexture = renderTexture;
    }

} // namespace nero
