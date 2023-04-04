////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/scene/MeshEditor.h>
#include <Nero/core/cpp/model/PolygonMesh.h>
#include <Nero/core/cpp/model/CircleMesh.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    MeshEditor::MeshEditor()
        : m_SelectedMesh(nullptr)
        , m_MeshCount(0)
        , m_RenderContext(nullptr)
        , m_LastMousePosition(0.f, 0.f)
        // 1.f equals PointMesh::m_MinVertexDistance
        , m_Epsilon(1.f, 1.f)
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

    void MeshEditor::addMesh(PointMesh::Ptr mesh)
    {
        m_MeshTable.push_back(mesh);
    }

    void MeshEditor::deleteMesh(const int& id)
    {
        for(auto meshIt = m_MeshTable.begin(); meshIt != m_MeshTable.end(); ++meshIt)
        {
            if((*meshIt)->getMeshId() == id)
            {
                *meshIt = nullptr;
                m_MeshTable.erase(meshIt);
                break;
            }
        }
    }

    void MeshEditor::clearMeshTable()
    {
        m_MeshTable.clear();
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

    void MeshEditor::unselectMesh(PointMesh::Ptr meshObject)
    {
        if(meshObject)
        {
            meshObject->updateColor();

            m_SelectedVertexTable.clear();
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
                unselectMesh(m_SelectedMesh);

            m_SelectedMesh->updateShape();
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

    sf::Vector2f MeshEditor::getMouseWorldPosition() const
    {
        return m_RenderTexture->mapPixelToCoords(
            sf::Vector2i(m_RenderContext->mousePosition.x, m_RenderContext->mousePosition.y),
            m_RenderTexture->getView());
    }

    bool MeshEditor::handleLeftClickPressOnVertex(PointMesh::Ptr pointMesh)
    {
        // No operation on CircleMesh vertex
        if(pointMesh->getMeshShape() == PointMesh::Shape::Circle)
            return false;

        const auto mousePosition = getMouseWorldPosition();
        auto&      vertexTable   = pointMesh->getVertexTable();

        // Move vertex : Line, Chain, Polygon, Circle
        // Extrude vertex : Line vertex, Chain edge vertex
        for(auto vertexIt = vertexTable.begin(); vertexIt != vertexTable.end(); ++vertexIt)
        {
            auto bound = vertexIt->getGlobalBounds();
            if(vertexIt->getGlobalBounds().contains(mousePosition))
            {
                // When no modifier is pressed,
                // Select the vertex for move operation
                // Cannot select CircleMesh vertex
                if(!keyboard::CTRL_SHIFT_ALT())
                {
                    m_SelectedVertexTable.push_back(vertexTable.data() +
                                                    (vertexIt - vertexTable.begin()));

                    m_SelectedMesh = pointMesh;

                    // Complete action
                    return true;
                }

                // When Shift modifier is pressed
                // Add new vertices and select new vertices for extrusion
                // Cannot select CircleMesh vertex
                else if(keyboard::SHIFT())
                {
                    // First vertex
                    if(vertexIt == vertexTable.begin())
                    {
                        pointMesh->addVertex(vertexIt->getPosition() + m_Epsilon, 0);
                        m_SelectedVertexTable.push_back(vertexTable.data());
                    }
                    // Last vertex
                    else if(vertexIt == vertexTable.end() - 1)
                    {
                        pointMesh->addVertex(vertexIt->getPosition() + m_Epsilon);
                        m_SelectedVertexTable.push_back(vertexTable.data() +
                                                        (vertexIt + 1 - vertexTable.begin()));
                    }
                    // Middle vertex
                    else
                    {
                        auto index = vertexIt - vertexTable.begin() + 1;
                        pointMesh->addVertex(vertexIt->getPosition() + m_Epsilon, index);
                        m_SelectedVertexTable.push_back(vertexTable.data() + index);
                    }

                    pointMesh->updateShape();
                    pointMesh->updateColor();

                    // After extrusion a Line becomes a Chain
                    if(pointMesh->getMeshShape() == PointMesh::Shape::Line)
                        pointMesh->setMeshShape(PointMesh::Shape::Chain);

                    m_SelectedMesh = pointMesh;

                    // Complete operation
                    return true;
                }
            }
        }

        return false;
    }

    bool MeshEditor::handleLeftClickPressOnLine()
    {
        // Click on line
        // Move line : Line, Chain, Polygon
        // Extrude line : Line, Chain, Polygon
        /*for(auto line_it = (*meshIt)->getLineTable().begin();
            line_it != (*meshIt)->getLineTable().end();
            line_it++)
        {
            // when we found a match
            if((*meshIt)->getMeshShape() != PointMesh::Shape::Circle)
            {
                bool is_polygon_last_line =
                    ((*meshIt)->getMeshShape() == PointMesh::Shape::Polygon &&
                     line_it == ((*meshIt)->getLineTable().end() - 1));

                Vertex* v1 =
                    &(*meshIt)->getVertexTable()[line_it - (*meshIt)->getLineTable().begin()];
                Vertex* v2 =
                    &(*meshIt)->getVertexTable()[is_polygon_last_line
                                                     ? 0
                                                     : (line_it -
                                                        (*meshIt)->getLineTable().begin() + 1)];

                // select the line of a polygon
                if(math::distance(v1->getPosition(), v2->getPosition(), worldPos) < 4.f)
                {
                    // Normal selection
                    if(!keyboard::CTRL_SHIFT_ALT())
                    {
                        m_SelectedVertexTable.push_back(v1);
                        m_SelectedVertexTable.push_back(v2);
                    }

                    // Select entire mesh with line : Line, Chain
                    if(keyboard::CTRL())
                    {
                        unselectMesh(m_SelectedMesh);

                        if((*meshIt)->getMeshShape() == PointMesh::Shape::Line ||
                           (*meshIt)->getMeshShape() == PointMesh::Shape::Chain)
                        {
                            for(auto vertex_it = (*meshIt)->getVertexTable().begin();
                                vertex_it != (*meshIt)->getVertexTable().end();
                                vertex_it++)
                                m_SelectedVertexTable.push_back(
                                    &(*meshIt)

                                         ->getVertexTable()[vertex_it - (*meshIt)

                                                                            ->getVertexTable()
                                                                            .begin()]);

                            m_SelectedMesh = (*meshIt);
                            // m_SelectedMesh->setColor(
                            // EngineConstant.COLOR_SELECTED_MESH);

                            isDone         = true;
                            break;
                        }
                    }

                    else if(keyboard::SHIFT())
                    {
                        if(!is_polygon_last_line)
                        {
                            (*meshIt)->addVertex(v2->getPosition(),
                                                 line_it - (*meshIt)->getLineTable().begin() + 1);
                            (*meshIt)->addVertex(v1->getPosition(),
                                                 line_it - (*meshIt)->getLineTable().begin() + 1);

                            m_SelectedVertexTable.push_back(
                                &(*meshIt)
                                     ->getVertexTable()[line_it -
                                                        (*meshIt)->getLineTable().begin() + 1]);
                            m_SelectedVertexTable.push_back(
                                &(*meshIt)
                                     ->getVertexTable()[line_it -
                                                        (*meshIt)->getLineTable().begin() + 2]);
                        }
                        else
                        {
                            (*meshIt)->addVertex(v1->getPosition());
                            (*meshIt)->addVertex(v2->getPosition());

                            m_SelectedVertexTable.push_back(
                                &(*meshIt)->getVertexTable()[line_it -
                                                             (*meshIt)->getLineTable().end() - 1]);
                            m_SelectedVertexTable.push_back(
                                &(*meshIt)->getVertexTable()[line_it -
                                                             (*meshIt)->getLineTable().end() - 2]);
                        }

                        if((*meshIt)->getMeshShape() == PointMesh::Shape::Line)
                            (*meshIt)->setMeshShape(PointMesh::Shape::Chain);
                    }

                    m_SelectedMesh = (*meshIt);

                    v1             = nullptr;
                    v2             = nullptr;
                    delete v1;
                    delete v2;

                    isDone = true;
                    break;
                }
            }
        }
        */
    }

    bool MeshEditor::handleLeftClickPressOnPolygon()
    {
        /* auto polygonMesh = PolygonMesh::Cast((*meshIt));
         for(auto polygon_it = polygonMesh->getPolygonTable().begin();
             polygon_it != polygonMesh->getPolygonTable().end();
             polygon_it++)
         {
             // when we found a match
             if(polygon_it->getGlobalBounds().contains(worldPos))
             {
                 if(!keyboard::CTRL_SHIFT_ALT())
                 {
                     for(auto vertex_it = (*meshIt)->getVertexTable().begin();
                         vertex_it != (*meshIt)->getVertexTable().end();
                         vertex_it++)
                         m_SelectedVertexTable.push_back(
                             &(*meshIt)->getVertexTable()[vertex_it -
                                                          (*meshIt)->getVertexTable().begin()]);

                     m_SelectedMesh = (*meshIt);

                     isDone         = true;
                     break;
                 }
                 else if(keyboard::CTRL())
                 {
                     unselectMesh(m_SelectedMesh);

                     for(auto vertex_it = (*meshIt)->getVertexTable().begin();
                         vertex_it != (*meshIt)->getVertexTable().end();
                         vertex_it++)
                         m_SelectedVertexTable.push_back(
                             &(*meshIt)->getVertexTable()[vertex_it -
                                                          (*meshIt)->getVertexTable().begin()]);

                     m_SelectedMesh = (*meshIt);
                     // m_SelectedMesh->setColor(EngineConstant.COLOR_SELECTED_MESH);

                     isDone         = true;
                     break;
                 }
             }
         }*/
    }

    bool MeshEditor::handleLeftClickPressOnCircle()
    {
        /* auto circleMesh = CircleMesh::Cast((*meshIt));

        if(circleMesh->getCircleShape().getGlobalBounds().contains(worldPos))
        {
            if(!keyboard::CTRL_SHIFT_ALT())
            {
                m_SelectedVertexTable.push_back(
                    (*meshIt)->getVertexTable().data() +
                    ((*meshIt)->getVertexTable().begin() - (*meshIt)->getVertexTable().begin()));
                m_SelectedVertexTable.push_back((*meshIt)->getVertexTable().data() +
                                                ((*meshIt)->getVertexTable().begin() + 1 -
                                                 (*meshIt)->getVertexTable().begin()));

                m_SelectedMesh = (*meshIt);

                isDone         = true;
                break;
            }
            else if(keyboard::CTRL())
            {
                unselectMesh(m_SelectedMesh);

                m_SelectedVertexTable.push_back(
                    (*meshIt)->getVertexTable().data() +
                    ((*meshIt)->getVertexTable().begin() - (*meshIt)->getVertexTable().begin()));
                m_SelectedVertexTable.push_back((*meshIt)->getVertexTable().data() +
                                                ((*meshIt)->getVertexTable().begin() + 1 -
                                                 (*meshIt)->getVertexTable().begin()));

                m_SelectedMesh = (*meshIt);
                // m_SelectedMesh->setColor(EngineConstant.COLOR_SELECTED_MESH);

                isDone         = true;
                break;
            }
        }*/
    }

    void MeshEditor::handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse,
                                             const bool&                        isPressed)
    {
        // Get the mouse position in the world
        sf::Vector2f worldPos = getMouseWorldPosition();

        // Handle vertex selection, whole mesh selection and extrusion
        if(mouse.button == sf::Mouse::Left && isPressed)
        {

            m_LastMousePosition = worldPos;

            for(auto meshIt = m_MeshTable.rbegin(); meshIt != m_MeshTable.rend(); ++meshIt)
            {
                auto pointMesh = (*meshIt);

                if(handleLeftClickPressOnVertex(*meshIt))
                    break;

                // if(handleLeftClickPressOnLine())
                // break;

                // Polygon only
                if(pointMesh->getMeshShape() == PointMesh::Shape::Polygon)
                {
                    // if(handleLeftClickPressOnPolygon())
                    // break;
                }

                // Circle only
                if(pointMesh->getMeshShape() == PointMesh::Shape::Circle)
                {
                    // if(handleLeftClickPressOnCircle())
                    // break;
                }
            }
        }

        if(mouse.button == sf::Mouse::Left && !isPressed && m_SelectedMesh)
        {
            if(!keyboard::CTRL())
            {
                unselectMesh(m_SelectedMesh);
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
                for(auto meshIt = m_MeshTable.begin(); meshIt != m_MeshTable.end(); meshIt++)
                {
                    // Click on vertex
                    // Remove vertex : Chain, Polygon
                    for(auto vertex_it = (*meshIt)->getVertexTable().begin();
                        vertex_it != (*meshIt)->getVertexTable().end();
                        vertex_it++)
                    {
                        // When we get a match
                        if(vertex_it->getGlobalBounds().contains(worldPos))
                        {
                            // Chain
                            if(!keyboard::CTRL_SHIFT_ALT() &&
                               (*meshIt)->getMeshShape() == PointMesh::Shape::Chain)
                            {
                                (*meshIt)->deleteVertex(vertex_it -
                                                        (*meshIt)->getVertexTable().begin());

                                if((*meshIt)->getVertexTable().size() == 2)
                                    (*meshIt)->setMeshShape(PointMesh::Shape::Line);

                                (*meshIt)->updateShape();
                                (*meshIt)->updateColor();

                                m_SelectedMesh = (*meshIt);

                                isDone         = true;
                                break;
                            }

                            // Polygon
                            if(!keyboard::CTRL_SHIFT_ALT() &&
                               (*meshIt)->getMeshShape() == PointMesh::Shape::Polygon)
                            {
                                if((*meshIt)->getVertexTable().size() > 3)
                                {
                                    (*meshIt)->deleteVertex(vertex_it -
                                                            (*meshIt)->getVertexTable().begin());
                                    (*meshIt)->updateShape();
                                    (*meshIt)->updateColor();

                                    m_SelectedMesh = (*meshIt);

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
                    for(auto line_it = (*meshIt)->getLineTable().begin();
                        line_it != (*meshIt)->getLineTable().end();
                        line_it++)
                    {
                        bool is_polygon_last_line =
                            ((*meshIt)->getMeshShape() == PointMesh::Shape::Polygon &&
                             line_it == ((*meshIt)->getLineTable().end() - 1));

                        Vertex* v1 =
                            &(*meshIt)

                                 ->getVertexTable()[line_it - (*meshIt)->getLineTable().begin()];
                        Vertex* v2 =
                            &(*meshIt)
                                 ->getVertexTable()[is_polygon_last_line
                                                        ? 0
                                                        : (line_it -
                                                           (*meshIt)->getLineTable().begin() + 1)];

                        // select the line of a polygon
                        if(math::distance(v1->getPosition(), v2->getPosition(), worldPos) < 4.f)
                        {
                            // act only on line
                            if(!keyboard::CTRL_SHIFT_ALT() &&
                               (*meshIt)->getMeshShape() == PointMesh::Shape::Line)
                            {
                                (*meshIt)->addVertex(worldPos,
                                                     line_it - (*meshIt)->getLineTable().begin() +
                                                         1);
                                (*meshIt)->setMeshShape(PointMesh::Shape::Chain);
                                (*meshIt)->updateShape();
                                (*meshIt)->updateColor();

                                m_SelectedMesh = (*meshIt);

                                isDone         = true;
                                break;
                            }

                            // act on chain
                            if(!keyboard::CTRL_SHIFT_ALT() &&
                               (*meshIt)->getMeshShape() == PointMesh::Shape::Chain)
                            {
                                (*meshIt)->addVertex(worldPos,
                                                     line_it - (*meshIt)->getLineTable().begin() +
                                                         1);
                                (*meshIt)->updateShape();
                                (*meshIt)->updateColor();

                                isDone = true;
                                (*meshIt)->updateShape();
                                (*meshIt)->updateColor();

                                m_SelectedMesh = (*meshIt);

                                break;
                            }

                            // act on polygon
                            if(!keyboard::CTRL_SHIFT_ALT() &&
                               (*meshIt)->getMeshShape() == PointMesh::Shape::Polygon)
                            {
                                (*meshIt)->addVertex(worldPos,
                                                     line_it - (*meshIt)->getLineTable().begin() +
                                                         1);

                                (*meshIt)->updateShape();
                                (*meshIt)->updateColor();

                                m_SelectedMesh = (*meshIt);

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
        sf::Vector2f worldPos = getMouseWorldPosition();

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2f diff = worldPos - m_LastMousePosition;

            if(!m_SelectedVertexTable.empty() && m_SelectedMesh)
            {
                for(auto vertex : m_SelectedVertexTable)
                    vertex->move(diff);

                m_SelectedMesh->updateShape();
                m_SelectedMesh->updateColor();

                m_LastMousePosition = worldPos;
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
