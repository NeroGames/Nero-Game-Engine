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

    void MeshEditor::addMesh(const PhysicsMeshObject::Ptr& meshObject)
    {
        m_MeshTable.push_back(meshObject);
    }

    void MeshEditor::deleteMesh(const int& id)
    {
        for(auto meshIt = m_MeshTable.begin(); meshIt != m_MeshTable.end(); ++meshIt)
        {
            if((*meshIt)->getMesh()->getMeshId() == id)
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

    void MeshEditor::selectMesh(const PhysicsMeshObject::Ptr& meshObject)
    {
        if(!meshObject)
            return;

        auto pointMesh = meshObject->getMesh();
        pointMesh->setMeshSelected(true);
        pointMesh->updateColor();
        m_SelectedMesh = meshObject;
    }

    void MeshEditor::unselectMesh(const PhysicsMeshObject::Ptr& meshObject)
    {
        if(!meshObject)
            return;

        auto pointMesh = meshObject->getMesh();
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
            const float offset = 0.1f;
            const float scale  = 0.05f;

            if(key == sf::Keyboard::Numpad8 && keyboard::CTRL())
            {
                m_SelectedMesh->move(sf::Vector2f(0.f, -offset));
                m_SelectedMesh->update(sf::Time());
            }

            if(key == sf::Keyboard::Numpad2 && keyboard::CTRL())
            {
                m_SelectedMesh->move(sf::Vector2f(0.f, offset));
                m_SelectedMesh->update(sf::Time());
            }

            if(key == sf::Keyboard::Numpad4 && keyboard::CTRL())
            {
                m_SelectedMesh->move(sf::Vector2f(-offset, 0.f));
                m_SelectedMesh->update(sf::Time());
            }

            if(key == sf::Keyboard::Numpad6 && keyboard::CTRL())
            {
                m_SelectedMesh->move(sf::Vector2f(offset, 0.f));
                m_SelectedMesh->update(sf::Time());
            }

            // Rotation
            if(key == sf::Keyboard::Numpad9 && keyboard::CTRL())
            {
                m_SelectedMesh->rotate(offset);
                m_SelectedMesh->update(sf::Time());
            }

            else if(key == sf::Keyboard::Numpad7 && keyboard::CTRL())
            {
                m_SelectedMesh->rotate(-offset);
                m_SelectedMesh->update(sf::Time());
            }

            // Zoom
            else if(key == sf::Keyboard::Add && keyboard::CTRL())
            {
                m_SelectedMesh->scale(sf::Vector2f(1.f + scale, 1.f + scale));
                m_SelectedMesh->update(sf::Time());
            }

            else if(key == sf::Keyboard::Subtract && keyboard::CTRL())
            {
                m_SelectedMesh->scale(sf::Vector2f(1.f - scale, 1.f - scale));
                m_SelectedMesh->update(sf::Time());
            }

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

    sf::Vector2f MeshEditor::getMouseWorldPosition() const
    {
        return m_RenderTexture->mapPixelToCoords(
            sf::Vector2i(static_cast<int>(m_RenderContext->mousePosition.x),
                         static_cast<int>(m_RenderContext->mousePosition.y)),
            m_RenderTexture->getView());
    }

    bool MeshEditor::handleLeftClickPressOnVertex(const PhysicsMeshObject::Ptr& meshObject)
    {
        auto pointMesh = meshObject->getMesh();

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

                    m_SelectedMesh  = meshObject;
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
                        const auto position = vertexIt->getPosition();
                        pointMesh->addVertex(position + m_Epsilon, 0);
                        m_SelectedVertexTable.push_back(vertexTable.data());
                    }
                    // Last vertex
                    else if(vertexIt == vertexTable.end() - 1)
                    {
                        const auto position = vertexIt->getPosition();
                        const auto index    = vertexIt - vertexTable.begin() + 1;
                        pointMesh->addVertex(position + m_Epsilon);
                        m_SelectedVertexTable.push_back(vertexTable.data() + index);
                    }
                    // Middle vertex
                    else
                    {
                        const auto position = vertexIt->getPosition();
                        const auto index    = vertexIt - vertexTable.begin() + 1;
                        pointMesh->addVertex(position + m_Epsilon, index);
                        m_SelectedVertexTable.push_back(vertexTable.data() + index);
                    }

                    pointMesh->updateShape();
                    pointMesh->updateColor();

                    m_SelectedMesh  = meshObject;
                    m_LeftSelection = true;

                    // Complete operation
                    return true;
                }
            }
        }

        return false;
    }

    bool MeshEditor::handleLeftClickPressOnLine(const PhysicsMeshObject::Ptr& meshObject)
    {
        auto pointMesh = meshObject->getMesh();

        // No operation on CircleMesh line
        if(pointMesh->getMeshShape() == PointMesh::Shape::Circle)
            return false;

        const auto mousePosition = getMouseWorldPosition();
        auto&      vertexTable   = pointMesh->getVertexTable();

        // Move line : Line, Chain, Polygon
        // Extrude line : Line, Chain, Polygon
        for(auto vertexIt = vertexTable.begin(); vertexIt != vertexTable.end(); ++vertexIt)
        {
            // Last vertex
            const bool lastVertex = (vertexIt == (vertexTable.end() - 1));

            // Line or Chain + Last vertex
            if(pointMesh->getMeshShape() != PointMesh::Shape::Polygon && lastVertex)
                continue;

            const bool polygonLastLine =
                (pointMesh->getMeshShape() == PointMesh::Shape::Polygon && lastVertex);

            const auto index1 = vertexIt - vertexTable.begin();
            const auto index2 = polygonLastLine ? 0 : (vertexIt - vertexTable.begin() + 1);
            Vertex*    v1     = vertexTable.data() + index1;
            Vertex*    v2     = vertexTable.data() + index2;

            // Select if mouse close to the line
            if(math::distance(v1->getPosition(), v2->getPosition(), mousePosition) < 4.f)
            {
                // Select line for move operation
                if(!keyboard::CTRL_SHIFT_ALT())
                {
                    m_SelectedVertexTable.push_back(v1);
                    m_SelectedVertexTable.push_back(v2);

                    m_SelectedMesh  = meshObject;
                    m_LeftSelection = true;

                    v1              = nullptr;
                    v2              = nullptr;

                    return true;
                }
                // Selected entire mesh
                // Only for Line and Chain
                else if(keyboard::CTRL() && pointMesh->getMeshShape() != PointMesh::Shape::Polygon)
                {
                    unselectMesh(m_SelectedMesh);

                    for(auto& vertex : vertexTable)
                        m_SelectedVertexTable.push_back(&vertex);

                    selectMesh(meshObject);
                    m_LeftSelection = true;

                    v1              = nullptr;
                    v2              = nullptr;

                    return true;
                }
                else if(keyboard::SHIFT())
                {
                    const auto position1 = v1->getPosition();
                    const auto position2 = v2->getPosition();

                    if(polygonLastLine)
                    {
                        pointMesh->addVertex(position1 + m_Epsilon);
                        pointMesh->addVertex(position2 + m_Epsilon);

                        const auto index = vertexTable.size() - 1;
                        m_SelectedVertexTable.push_back(vertexTable.data() + index);
                        m_SelectedVertexTable.push_back(vertexTable.data() + index - 1);
                    }
                    else
                    {
                        const auto index = vertexIt - vertexTable.begin() + 1;
                        pointMesh->addVertex(position1 + m_Epsilon, index);
                        pointMesh->addVertex(position2 + m_Epsilon, index + 1);

                        m_SelectedVertexTable.push_back(vertexTable.data() + index);
                        m_SelectedVertexTable.push_back(vertexTable.data() + index + 1);
                    }

                    pointMesh->updateShape();
                    pointMesh->updateColor();

                    m_SelectedMesh  = meshObject;
                    m_LeftSelection = true;

                    v1              = nullptr;
                    v2              = nullptr;

                    return true;
                }
            }

            v1 = nullptr;
            v2 = nullptr;
        }

        return false;
    }

    bool MeshEditor::handleLeftClickPressOnPolygon(const PhysicsMeshObject::Ptr& meshObject)
    {
        auto       polygonMesh   = PolygonMesh::Cast(meshObject->getMesh());
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

                    m_SelectedMesh  = meshObject;
                    m_LeftSelection = true;

                    return true;
                }
                else if(keyboard::CTRL())
                {
                    unselectMesh(m_SelectedMesh);

                    for(auto& vertex : vertexTable)
                        m_SelectedVertexTable.push_back(&vertex);

                    selectMesh(meshObject);
                    m_LeftSelection = true;

                    return true;
                }
            }
        }

        return false;
    }

    bool MeshEditor::handleLeftClickPressOnCircle(const PhysicsMeshObject::Ptr& meshObject)
    {
        auto       circleMesh    = CircleMesh::Cast(meshObject->getMesh());
        const auto mousePosition = getMouseWorldPosition();
        auto&      vertexTable   = circleMesh->getVertexTable();

        if(circleMesh->getCircleShape().getGlobalBounds().contains(mousePosition))
        {
            if(!keyboard::CTRL_SHIFT_ALT())
            {
                for(auto& vertex : vertexTable)
                    m_SelectedVertexTable.push_back(&vertex);

                m_SelectedMesh  = meshObject;
                m_LeftSelection = true;

                return true;
            }
            else if(keyboard::CTRL())
            {
                unselectMesh(m_SelectedMesh);

                for(auto& vertex : vertexTable)
                    m_SelectedVertexTable.push_back(&vertex);

                selectMesh(meshObject);
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
            m_SelectedMesh->getMesh()->setMeshSelected(false);
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

    bool MeshEditor::handleRightClickPressOnVertex(const PhysicsMeshObject::Ptr& meshObject)
    {
        auto pointMesh = meshObject->getMesh();

        // No operation on CircleMesh line
        if(pointMesh->getMeshShape() == PointMesh::Shape::Circle)
            return false;

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

                    m_SelectedMesh = meshObject;
                }

                // Polygon
                else if(!keyboard::CTRL_SHIFT_ALT() &&
                        pointMesh->getMeshShape() == PointMesh::Shape::Polygon)
                {
                    // Cannot remove vertex when only three left
                    if(vertexTable.size() > 3)
                    {
                        pointMesh->deleteVertex(vertexIt - vertexTable.begin());

                        pointMesh->updateShape();
                        pointMesh->updateColor();

                        m_SelectedMesh = meshObject;
                    }
                }

                return true;
            }
        }

        return false;
    }

    bool MeshEditor::handleRightClickPressOnLine(const PhysicsMeshObject::Ptr& meshObject)
    {
        auto pointMesh = meshObject->getMesh();

        // No operation on CircleMesh line
        if(pointMesh->getMeshShape() == PointMesh::Shape::Circle)
            return false;

        const auto mousePosition = getMouseWorldPosition();
        auto&      vertexTable   = pointMesh->getVertexTable();

        // Click on line
        // Add vertex : Line, Chain, Polygon
        for(auto vertexIt = vertexTable.begin(); vertexIt != vertexTable.end(); ++vertexIt)
        {
            // Last vertex
            const bool lastVertex = (vertexIt == (vertexTable.end() - 1));

            // Line or Chain + Last vertex
            if(pointMesh->getMeshShape() != PointMesh::Shape::Polygon && lastVertex)
                continue;

            const bool polygonLastLine =
                (pointMesh->getMeshShape() == PointMesh::Shape::Polygon && lastVertex);

            const auto index1 = vertexIt - vertexTable.begin();
            const auto index2 = polygonLastLine ? 0 : (vertexIt - vertexTable.begin() + 1);
            Vertex*    v1     = vertexTable.data() + index1;
            Vertex*    v2     = vertexTable.data() + index2;

            // Select if mouse close to the line
            if(math::distance(v1->getPosition(), v2->getPosition(), mousePosition) < 4.f)
            {
                if(!keyboard::CTRL_SHIFT_ALT())
                {
                    polygonLastLine
                        ? pointMesh->addVertex(mousePosition)
                        : pointMesh->addVertex(mousePosition, vertexIt - vertexTable.begin() + 1);

                    pointMesh->updateShape();
                    pointMesh->updateColor();

                    m_SelectedMesh = meshObject;

                    v1             = nullptr;
                    v2             = nullptr;

                    return true;
                }
            }

            v1 = nullptr;
            v2 = nullptr;
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
                auto meshObject = (*meshIt);

                if(handleLeftClickPressOnVertex(meshObject))
                    break;

                if(handleLeftClickPressOnLine(meshObject))
                    break;

                // Polygon only
                if(meshObject->getMesh()->getMeshShape() == PointMesh::Shape::Polygon)
                {
                    if(handleLeftClickPressOnPolygon(meshObject))
                        break;
                }

                // Circle only
                if(meshObject->getMesh()->getMeshShape() == PointMesh::Shape::Circle)
                {
                    if(handleLeftClickPressOnCircle(meshObject))
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
            if(m_SelectedMesh)
                unselectMesh(m_SelectedMesh);

            for(auto meshIt = m_MeshTable.rbegin(); meshIt != m_MeshTable.rend(); ++meshIt)
            {
                auto meshObject = (*meshIt);

                if(handleRightClickPressOnVertex(meshObject))
                    break;

                if(handleRightClickPressOnLine(meshObject))
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

                auto pointMesh = m_SelectedMesh->getMesh();
                pointMesh->updateShape();
                pointMesh->updateColor();

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
