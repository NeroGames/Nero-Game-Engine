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
        , m_LeftSelection(false)
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

    void MeshEditor::selectMesh(PointMesh::Ptr pointMesh)
    {
        if(!pointMesh)
            return;

        pointMesh->setMeshSelected(true);
        pointMesh->updateColor();
        m_SelectedMesh = pointMesh;
    }

    void MeshEditor::unselectMesh(PointMesh::Ptr pointMesh)
    {
        if(!pointMesh)
            return;

        pointMesh->setMeshSelected(false);
        pointMesh->updateColor();

        for(unsigned int i = 0; i < m_SelectedVertexTable.size(); ++i)
            m_SelectedVertexTable[i] = nullptr;

        m_SelectedVertexTable.clear();
        m_SelectedMesh = nullptr;
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
            sf::Vector2i(static_cast<int>(m_RenderContext->mousePosition.x),
                         static_cast<int>(m_RenderContext->mousePosition.y)),
            m_RenderTexture->getView());
    }

    bool MeshEditor::handleLeftClickPressOnVertex(const PointMesh::Ptr& pointMesh)
    {
        // No operation on CircleMesh vertex
        if(pointMesh->getMeshShape() == PointMesh::Shape::Circle)
            return false;

        const auto mousePosition = getMouseWorldPosition();
        auto&      vertexTable   = pointMesh->getVertexTable();

        // Move vertex : Line, Chain, Polygon
        // Extrude vertex : Line, Chain, Polygon
        for(auto vertexIt = vertexTable.begin(); vertexIt != vertexTable.end(); ++vertexIt)
        {
            if(vertexIt->getGlobalBounds().contains(mousePosition))
            {
                // When no modifier is pressed,
                // Select vertex for move operation
                // Cannot select CircleMesh vertex
                if(!keyboard::CTRL_SHIFT_ALT())
                {
                    const auto index = vertexIt - vertexTable.begin();
                    m_SelectedVertexTable.push_back(vertexTable.data() + index);

                    m_SelectedMesh  = pointMesh;
                    m_LeftSelection = true;
                    // Complete action
                    return true;
                }

                // When Shift modifier is pressed
                // Select vertex for extrusion operation
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

                    m_SelectedMesh  = pointMesh;
                    m_LeftSelection = true;

                    // Complete operation
                    return true;
                }
            }
        }

        return false;
    }

    bool MeshEditor::handleLeftClickPressOnLine(const PointMesh::Ptr& pointMesh)
    {
        // No operation on CircleMesh line
        if(pointMesh->getMeshShape() == PointMesh::Shape::Circle)
            return false;

        const auto mousePosition = getMouseWorldPosition();
        auto&      vertexTable   = pointMesh->getVertexTable();

        // Move line : Line, Chain, Polygon
        // Extrude line : Line, Chain, Polygon
        for(auto lineIt = vertexTable.begin(); lineIt != vertexTable.end(); ++lineIt)
        {

            const bool polygonLastLine = (pointMesh->getMeshShape() == PointMesh::Shape::Polygon &&
                                          lineIt == (vertexTable.end() - 1));

            const auto index1          = lineIt - vertexTable.begin();
            const auto index2          = polygonLastLine ? 0 : (lineIt - vertexTable.begin() + 1);
            Vertex*    v1              = vertexTable.data() + index1;
            Vertex*    v2              = vertexTable.data() + index2;

            // Select if mouse close to the line
            if(math::distance(v1->getPosition(), v2->getPosition(), mousePosition) < 4.f)
            {
                // When Ctrl modifier is pressed
                // Select entire mesh with line : Line, Chain
                if(keyboard::CTRL() && (pointMesh->getMeshShape() == PointMesh::Shape::Line ||
                                        pointMesh->getMeshShape() == PointMesh::Shape::Chain))
                {
                    unselectMesh(m_SelectedMesh);

                    for(auto& vertex : vertexTable)
                        m_SelectedVertexTable.push_back(&vertex);

                    selectMesh(pointMesh);

                    return true;
                }

                // Select line for move operation
                if(!keyboard::CTRL_SHIFT_ALT())
                {
                    m_SelectedVertexTable.push_back(v1);
                    m_SelectedVertexTable.push_back(v2);
                }

                // Select line for extrusion operation
                else if(keyboard::SHIFT())
                {
                    if(polygonLastLine)
                    {
                        /*pointMesh->addVertex(v1->getPosition() + m_Epsilon);
                        pointMesh->addVertex(v2->getPosition() + m_Epsilon);

                        m_SelectedVertexTable.push_back(
                            &vertexTable[lineIt - vertexTable.end() - 1]);
                        m_SelectedVertexTable.push_back(
                            &vertexTable[lineIt - vertexTable.end() - 2]);*/
                    }
                    else
                    {
                        /*pointMesh->addVertex(v2->getPosition() + m_Epsilon,
                                             lineIt - vertexTable.begin() + 1);
                        pointMesh->addVertex(v1->getPosition() + m_Epsilon,
                                             lineIt - vertexTable.begin() + 1);

                        m_SelectedVertexTable.push_back(
                            &vertexTable[lineIt - vertexTable.begin() + 1]);
                        m_SelectedVertexTable.push_back(
                            &vertexTable[lineIt - vertexTable.begin() + 2]);*/
                    }

                    pointMesh->updateShape();
                    pointMesh->updateColor();
                }

                m_SelectedMesh = pointMesh;

                v1             = nullptr;
                v2             = nullptr;

                return true;
            }
        }

        return false;
    }

    bool MeshEditor::handleLeftClickPressOnPolygon(const PolygonMesh::Ptr& polygonMesh)
    {
        const auto mousePosition = getMouseWorldPosition();
        auto&      vertexTable   = polygonMesh->getVertexTable();
        auto&      polygonTable  = polygonMesh->getPolygonTable();

        for(auto polygonIt = polygonTable.begin(); polygonIt != polygonTable.end(); ++polygonIt)
        {
            // when we found a match
            if(polygonIt->getGlobalBounds().contains(mousePosition))
            {
                // Move polygon mesh
                if(!keyboard::CTRL_SHIFT_ALT())
                {
                    for(auto& vertex : vertexTable)
                        m_SelectedVertexTable.push_back(&vertex);

                    m_SelectedMesh  = polygonMesh;
                    m_LeftSelection = true;

                    return true;
                }
                else if(keyboard::CTRL())
                {
                    unselectMesh(m_SelectedMesh);

                    for(auto& vertex : vertexTable)
                        m_SelectedVertexTable.push_back(&vertex);

                    selectMesh(polygonMesh);
                    m_LeftSelection = true;

                    return true;
                }
            }
        }

        return false;
    }

    bool MeshEditor::handleLeftClickPressOnCircle(const CircleMesh::Ptr& circleMesh)
    {
        const auto mousePosition = getMouseWorldPosition();
        auto&      vertexTable   = circleMesh->getVertexTable();

        if(circleMesh->getCircleShape().getGlobalBounds().contains(mousePosition))
        {
            if(!keyboard::CTRL_SHIFT_ALT())
            {
                for(auto& vertex : vertexTable)
                    m_SelectedVertexTable.push_back(&vertex);

                m_SelectedMesh  = circleMesh;
                m_LeftSelection = true;

                return true;
            }
            else if(keyboard::CTRL())
            {
                unselectMesh(m_SelectedMesh);

                for(auto& vertex : vertexTable)
                    m_SelectedVertexTable.push_back(&vertex);

                selectMesh(circleMesh);
                m_LeftSelection = true;

                return true;
            }
        }

        return false;
    }

    void MeshEditor::handleLeftClickRelease()
    {
        if(!m_SelectedMesh)
            return;

        // Normal release, perform a unselect
        // When CTRL is pressed there will be no unselect
        if(!keyboard::CTRL_SHIFT_ALT())
        {
            // clear selected vertices
            for(unsigned int i = 0; i < m_SelectedVertexTable.size(); ++i)
                m_SelectedVertexTable[i] = nullptr;

            m_SelectedVertexTable.clear();
            m_SelectedMesh->setMeshSelected(false);
            m_SelectedMesh = nullptr;
        }

        m_UpdateUndo();
    }

    void MeshEditor::handleRightClickRelease()
    {
        if(m_SelectedMesh)
        {
            m_SelectedMesh = nullptr;
            m_UpdateUndo();
        }
    }

    bool MeshEditor::handleRightClickPressOnVertex(const PointMesh::Ptr& pointMesh)
    {
        const auto mousePosition = getMouseWorldPosition();
        auto&      vertexTable   = pointMesh->getVertexTable();

        // Click on vertex
        // Remove vertex : Chain, Polygon
        for(auto vertexIt = vertexTable.begin(); vertexIt != vertexTable.end(); ++vertexIt)
        {
            // When we get a match
            if(vertexIt->getGlobalBounds().contains(mousePosition))
            {
                // Chain
                if(!keyboard::CTRL_SHIFT_ALT() &&
                   pointMesh->getMeshShape() == PointMesh::Shape::Chain)
                {
                    pointMesh->deleteVertex(vertexIt - vertexTable.begin());

                    pointMesh->updateShape();
                    pointMesh->updateColor();

                    m_SelectedMesh = pointMesh;

                    return true;
                }

                // Polygon
                if(!keyboard::CTRL_SHIFT_ALT() &&
                   pointMesh->getMeshShape() == PointMesh::Shape::Polygon)
                {
                    // Cannot remove vertex when only three left
                    if(vertexTable.size() > 3)
                    {
                        pointMesh->deleteVertex(vertexIt - vertexTable.begin());

                        pointMesh->updateShape();
                        pointMesh->updateColor();

                        m_SelectedMesh = pointMesh;

                        return true;
                    }
                }
            }
        }

        return false;
    }

    bool MeshEditor::handleRightClickPressOnLine(const PointMesh::Ptr& pointMesh)
    {
        const auto mousePosition = getMouseWorldPosition();
        auto&      vertexTable   = pointMesh->getVertexTable();
        auto&      lineTable     = pointMesh->getLineTable();

        // Click on line
        // Add vertex : Line, Chain, Polygon
        for(auto lineIt = lineTable.begin(); lineIt != lineTable.end(); ++lineIt)
        {
            const bool polygonLastLine = (pointMesh->getMeshShape() == PointMesh::Shape::Polygon &&
                                          lineIt == (vertexTable.end() - 1));

            const auto index1          = lineIt - lineTable.begin();
            const auto index2          = polygonLastLine ? 0 : (lineIt - lineTable.begin() + 1);
            Vertex*    v1              = vertexTable.data() + index1;
            Vertex*    v2              = vertexTable.data() + index2;

            // Select if mouse close to the line
            if(math::distance(v1->getPosition(), v2->getPosition(), mousePosition) < 4.f)
            {
                if(!keyboard::CTRL_SHIFT_ALT())
                {
                    pointMesh->addVertex(mousePosition, lineIt - lineTable.begin() + 1);

                    pointMesh->updateShape();
                    pointMesh->updateColor();

                    m_SelectedMesh = pointMesh;

                    return true;
                }
            }
        }

        return false;
    }

    void MeshEditor::handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse,
                                             const bool&                        isPressed)
    {
        // Get the mouse position in the world
        const sf::Vector2f mousePosition = getMouseWorldPosition();

        // Handle vertex selection, whole mesh selection and extrusion
        if(mouse.button == sf::Mouse::Left && isPressed)
        {
            if(m_SelectedMesh)
                unselectMesh(m_SelectedMesh);

            m_LastMousePosition = mousePosition;

            for(auto meshIt = m_MeshTable.rbegin(); meshIt != m_MeshTable.rend(); ++meshIt)
            {
                auto pointMesh = (*meshIt);

                if(handleLeftClickPressOnVertex(pointMesh))
                    break;

                // if(handleLeftClickPressOnLine(pointMesh))
                // break;

                // Polygon only
                if(pointMesh->getMeshShape() == PointMesh::Shape::Polygon)
                {
                    if(handleLeftClickPressOnPolygon(PolygonMesh::Cast(pointMesh)))
                        break;
                }

                // Circle only
                if(pointMesh->getMeshShape() == PointMesh::Shape::Circle)
                {
                    if(handleLeftClickPressOnCircle(CircleMesh::Cast(pointMesh)))
                        break;
                }
            }
        }
        else if(mouse.button == sf::Mouse::Left && !isPressed)
        {
            m_LeftSelection = false;

            handleLeftClickRelease();
        }
        else if(mouse.button == sf::Mouse::Right && isPressed)
        {
            for(auto meshIt = m_MeshTable.rbegin(); meshIt != m_MeshTable.rend(); ++meshIt)
            {
                auto pointMesh = (*meshIt);

                if(handleRightClickPressOnVertex(pointMesh))
                    break;

                if(handleRightClickPressOnLine(pointMesh))
                    break;
            }
        }

        else if(mouse.button == sf::Mouse::Right && !isPressed)
        {
            handleRightClickRelease();
        }
    }

    void MeshEditor::handleMouseMoveInput(const sf::Event::MouseMoveEvent&)
    {
        if(m_LeftSelection)
        {
            sf::Vector2f mousePosition = getMouseWorldPosition();
            sf::Vector2f moveDistance  = mousePosition - m_LastMousePosition;

            if(!m_SelectedVertexTable.empty() && m_SelectedMesh)
            {
                for(auto vertex : m_SelectedVertexTable)
                    vertex->move(moveDistance);

                m_SelectedMesh->updateShape();
                m_SelectedMesh->updateColor();

                m_LastMousePosition = mousePosition;
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
