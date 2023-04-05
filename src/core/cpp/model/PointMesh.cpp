////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/model/PointMesh.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
#include <Nero/core/cpp/utility/Math.h>
////////////////////////////////////////////////////////////
namespace nero
{
    PointMesh::PointMesh(const Shape& shape)
        : m_MeshId(-1)
        , m_MeshShape(shape)
        , m_MeshType(Type::Static)
        , m_MeshValid(false)
        , m_Position(sf::Vector2f(0.f, 0.f))
        , m_Scale(sf::Vector2f(1.f, 1.f))
        , m_Rotation(0.f)
        , m_MeshSelected(false)
    {
    }

    void PointMesh::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for(auto& vertex : m_VertexTable)
        {
            states.transform = sf::Transform::Identity;
            states.transform.translate(vertex.getPosition());
            target.draw(vertex);
        }

        for(auto& line : m_LineTable)
        {
            states.transform = sf::Transform::Identity;
            states.transform.translate(line.getPosition());
            target.draw(line);
        }
    }

    sf::Color PointMesh::getColor() const
    {
        if(!m_MeshValid)
            return EngineConstant.COLOR_INVALIDE_MESH;

        if(m_MeshSelected)
            return EngineConstant.COLOR_SELECTED_MESH;

        switch(m_MeshType)
        {
            case Type::Static:
                return EngineConstant.COLOR_STATIC_MESH;
            case Type::Dynamic:
                return EngineConstant.COLOR_DYNAMIC_MESH;
            case Type::Kinematic:
                return EngineConstant.COLOR_KINEMATIC_MESH;
        }
    }

    void PointMesh::updateColor()
    {
        const auto color = getColor();

        for(auto it = m_VertexTable.begin(); it != m_VertexTable.end(); it++)
            it->setFillColor(color);

        for(auto it = m_LineTable.begin(); it != m_LineTable.end(); it++)
            it->setFillColor(graphics::getTransparentColor(color, m_ColorAlpha));
    }

    sf::FloatRect PointMesh::getRealGlobalBounds() const
    {
        sf::FloatRect globalBound;

        if(m_MeshShape == Shape::None)
            return globalBound;

        const auto   pointTable = getPointTable();

        auto         xExtremes  = std::minmax_element(pointTable.begin(),
                                             pointTable.end(),
                                             [](const sf::Vector2f& left, const sf::Vector2f& right)
                                             {
                                                 return left.x < right.x;
                                             });

        auto         yExtremes  = std::minmax_element(pointTable.begin(),
                                             pointTable.end(),
                                             [](const sf::Vector2f& left, const sf::Vector2f& right)
                                             {
                                                 return left.y < right.y;
                                             });

        sf::Vector2f upperLeft(xExtremes.first->x, yExtremes.first->y);
        sf::Vector2f lowerRight(xExtremes.second->x, yExtremes.second->y);

        globalBound.left   = upperLeft.x - 5.f;
        globalBound.top    = upperLeft.y - 5.f;
        globalBound.width  = lowerRight.x - upperLeft.x + 10.f;
        globalBound.height = lowerRight.y - upperLeft.y + 10.f;

        return globalBound;
    }

    sf::FloatRect PointMesh::getGlobalBounds() const
    {
        if(m_MeshShape == Shape::None)
            return sf::FloatRect();

        sf::FloatRect globalBound = getRealGlobalBounds();
        const float   adjustment  = 5.f;

        globalBound.left          -= adjustment;
        globalBound.top           -= adjustment;
        globalBound.height        += adjustment * 2.f;
        globalBound.width         += adjustment * 2.f;

        return globalBound;
    }

    sf::Vector2f PointMesh::getMeshSize() const
    {
        const auto globalBound = getRealGlobalBounds();

        return sf::Vector2f(globalBound.width, globalBound.height);
    }

    sf::Vector2f PointMesh::getMassCenter() const
    {
        const auto   globalBound = getRealGlobalBounds();

        sf::Vector2f massCenter;
        massCenter.x = globalBound.left + globalBound.width / 2.f;
        massCenter.y = globalBound.top + globalBound.height / 2.f;

        return massCenter;
    }

    sf::RectangleShape PointMesh::createVertex(const sf::Vector2f& point) const
    {
        sf::RectangleShape vertex;

        vertex.setOrigin(sf::Vector2f(m_VertexSize / 2.f, m_VertexSize / 2.f));
        vertex.setSize(sf::Vector2f(m_VertexSize, m_VertexSize));
        vertex.setPosition(point);
        vertex.setFillColor(getColor());

        return vertex;
    }

    sf::RectangleShape PointMesh::createLine(const sf::Vector2f& pointOne,
                                             const sf::Vector2f& pointTwo) const
    {
        sf::RectangleShape line;

        float              lineLength = math::distance(pointOne, pointTwo);
        line.setOrigin(sf::Vector2f(line.getOrigin().x, m_VertexSize / 4.f));
        line.setSize(sf::Vector2f(lineLength, m_VertexSize / 2.f));
        line.setPosition(pointOne);
        line.setFillColor(graphics::getTransparentColor(getColor(), m_ColorAlpha));

        float xDelta = pointTwo.x - pointOne.x;
        float yDelta = pointTwo.y - pointOne.y;
        float angle  = atan2(yDelta, xDelta);
        line.setRotation(math::toDegree(angle));

        return line;
    }

    void PointMesh::addVertex(const sf::Vector2f& point)
    {
        m_VertexTable.push_back(createVertex(point));

        for(unsigned int i = 0; i < m_VertexTable.size() - 1; i++)
        {
            sf::Vector2f pointOne = m_VertexTable[i].getPosition();
            sf::Vector2f pointTwo = m_VertexTable[i + 1].getPosition();

            validateMesh(pointOne, pointTwo);

            if(!m_MeshValid)
                break;
        }
    }

    void PointMesh::addVertex(const sf::Vector2f& position, const int& index)
    {
        if(index < 0)
        {
            // should not happen
            m_VertexTable.emplace_back(createVertex(position));
        }
        else
        {
            m_VertexTable.insert(m_VertexTable.begin() + index, createVertex(position));
        }
    }

    void PointMesh::deleteVertex(const int& index)
    {
        m_VertexTable.erase(m_VertexTable.begin() + index);
    }

    void PointMesh::validateMesh(const sf::Vector2f& pointOne, const sf::Vector2f& pointTwo)
    {
        if(math::distance(pointOne, pointTwo) < m_MinVertexDistance)
        {
            m_MeshValid = false;
        }
    }

    void PointMesh::setMeshId(const int& meshId)
    {
        m_MeshId = meshId;
    }

    int PointMesh::getMeshId() const
    {
        return m_MeshId;
    }

    void PointMesh::setMeshType(const Type type)
    {
        m_MeshType = type;

        updateColor();
    }

    void PointMesh::setMeshShape(const Shape shape)
    {
        m_MeshShape = shape;
    }

    PointMesh::Type PointMesh::getMeshType() const
    {
        return m_MeshType;
    }

    PointMesh::Shape PointMesh::getMeshShape() const
    {
        return m_MeshShape;
    }

    bool PointMesh::meshValid() const
    {
        return m_MeshValid;
    }

    PointTable PointMesh::getPointTable() const
    {
        PointTable vectorTable;

        for(auto vertex : m_VertexTable)
        {
            vectorTable.emplace_back(vertex.getPosition());
        }

        return vectorTable;
    }

    unsigned int PointMesh::getPointCount() const
    {
        return m_VertexTable.size();
    }

    VertexTable& PointMesh::getVertexTable()
    {
        return m_VertexTable;
    }

    LineTable& PointMesh::getLineTable()
    {
        return m_LineTable;
    }

    void PointMesh::moveMesh(const sf::Vector2f& offset)
    {
        for(auto& vertex : m_VertexTable)
        {
            vertex.move(offset);
        }

        updateShape();
    }

    void PointMesh::scaleMesh(const sf::Vector2f& scaleFactor)
    {
        if(m_MeshShape == Shape::Line)
        {
            sf::Vector2f pointOne   = m_VertexTable.front().getPosition();
            sf::Vector2f pointTwo   = m_VertexTable.back().getPosition();

            sf::Vector2f massCenter = math::getLineCenter(pointOne, pointTwo);

            sf::Vector2f newPointOne =
                sf::Vector2f((pointOne.x - massCenter.x) * scaleFactor.x + massCenter.x,
                             (pointOne.y - massCenter.y) * scaleFactor.y + massCenter.y);
            sf::Vector2f newPointTwo =
                sf::Vector2f((pointTwo.x - massCenter.x) * scaleFactor.x + massCenter.x,
                             (pointTwo.y - massCenter.y) * scaleFactor.y + massCenter.y);

            m_VertexTable.front().setPosition(newPointOne);
            m_VertexTable.back().setPosition(newPointTwo);
        }
        else // Chain and Polygon
        {
            PointTable   pointTable = getPointTable();
            sf::Vector2f massCenter = math::getPolygonCenter(pointTable);

            for(auto it = m_VertexTable.begin(); it != m_VertexTable.end(); it++)
            {
                sf::Vector2f point = it->getPosition();
                sf::Vector2f newPoint =
                    sf::Vector2f((point.x - massCenter.x) * scaleFactor.x + massCenter.x,
                                 (point.y - massCenter.y) * scaleFactor.y + massCenter.y);

                it->setPosition(newPoint);
            }
        }

        updateShape();
    }

    void PointMesh::rotateMesh(const float& angle)
    {
        if(m_MeshShape == Shape::Line)
        {
            sf::Vector2f center = math::getLineCenter(m_VertexTable.front().getPosition(),
                                                      m_VertexTable.back().getPosition());
            m_VertexTable.front().setPosition(
                math::rotateVertex(center, angle, m_VertexTable.front().getPosition()));
            m_VertexTable.back().setPosition(
                math::rotateVertex(center, angle, m_VertexTable.back().getPosition()));
        }
        else // Chain and Polygon
        {
            PointTable   pointTable = getPointTable();
            sf::Vector2f massCenter = math::getPolygonCenter(pointTable);

            for(auto it = m_VertexTable.begin(); it != m_VertexTable.end(); it++)
            {
                it->setPosition(math::rotateVertex(massCenter, angle, it->getPosition()));
            }
        }

        updateShape();
    }

    void PointMesh::updateLine(const bool loop)
    {
        m_LineTable.clear();

        m_MeshValid = true;

        for(unsigned int i = 0; i < m_VertexTable.size() - 1; i++)
        {
            sf::Vector2f pointOne = m_VertexTable[i].getPosition();
            sf::Vector2f pointTwo = m_VertexTable[i + 1].getPosition();
            m_LineTable.push_back(createLine(pointOne, pointTwo));

            validateMesh(pointOne, pointTwo);
        }

        if(loop)
        {
            sf::Vector2f pointOne = m_VertexTable[0].getPosition();
            sf::Vector2f pointTwo = m_VertexTable[m_VertexTable.size() - 1].getPosition();
            m_LineTable.push_back(createLine(pointOne, pointTwo));

            validateMesh(pointOne, pointTwo);
        }
    }

    void PointMesh::updateShape()
    {
        updateLine(false);
    }

    nlohmann::json PointMesh::toJson() const
    {
        nlohmann::json meshJson;

        switch(m_MeshShape)
        {
            case Shape::Line:
                meshJson["shape"] = "line_mesh";
                break;
            case Polygon:
                meshJson["shape"] = "polygon_mesh";
                break;
            case Circle:
                meshJson["shape"] = "circle_mesh";
                break;
            case Chain:
                meshJson["shape"] = "chain_mesh";
                break;
            case None:
                meshJson["shape"] = "none";
                break;
        }

        switch(m_MeshType)
        {
            case Static:
                meshJson["type"] = "static_mesh";
                break;
            case Dynamic:
                meshJson["type"] = "dynamic_mesh";
                break;
            case Kinematic:
                meshJson["type"] = "kinematic_mesh";
                break;
        }

        std::vector<nlohmann::json> vertexTableJson;
        int                         i = 0;
        for(auto v : m_VertexTable)
        {
            nlohmann::json vertex;

            vertex = {
                {"x", v.getPosition().x},
                {"y", v.getPosition().y}
            };
            vertex["order"] = i++;

            vertexTableJson.emplace_back(vertex);
        }

        meshJson["vertex_table"] = vertexTableJson;
        meshJson["vertex_count"] = m_VertexTable.size();

        return meshJson;
    }

    sf::String PointMesh::toSting() const
    {
        return toJson().dump(4);
    }

    void PointMesh::transform(const sf::Vector2f& position,
                              const sf::Vector2f& scale,
                              const float&        rotation)
    {
        if(scale != m_Scale)
        {
            scaleMesh(sf::Vector2f(scale.x / m_Scale.x, scale.y / m_Scale.y));
            m_Scale = scale;
        }
        else if(rotation > m_Rotation || rotation < m_Rotation)
        {
            rotateMesh(rotation - m_Rotation);
            m_Rotation = rotation;
        }
        else if(position != m_Position)
        {
            moveMesh(position - m_Position);
            m_Position = position;
        }
    }

    void PointMesh::loadTransform(const sf::Vector2f& position,
                                  const sf::Vector2f& scale,
                                  const float&        rotation)
    {
        m_Position = position;
        m_Scale    = scale;
        m_Rotation = rotation;
    }

    void PointMesh::generateDefaultShape()
    {
        // Create the default Line/Chain with two vertices
        if(m_MeshShape != Shape::Line && m_MeshShape != Shape::Chain)
            return;

        const auto center = sf::Vector2f(0.f, 0.f);
        const auto radius = 75.f;
        addVertex(sf::Vector2f(center.x + radius, center.y));
        addVertex(sf::Vector2f(center.x - radius, center.y));

        updateShape();
        updateColor();
    }

    PointMesh::Ptr PointMesh::clone() const
    {
        // Create the default Line/Chain with two vertices
        if(m_MeshShape != Shape::Line && m_MeshShape != Shape::Chain)
            return nullptr;

        PointMesh::Ptr pointMesh = std::make_shared<PointMesh>(m_MeshShape);

        for(const auto& point : getPointTable())
            pointMesh->addVertex(point);

        pointMesh->setMeshType(m_MeshType);
        pointMesh->m_Scale    = m_Scale;
        pointMesh->m_Rotation = m_Rotation;
        pointMesh->m_Position = m_Position;
        pointMesh->updateShape();
        pointMesh->updateColor();

        return pointMesh;
    }

    void PointMesh::setMeshSelected(const bool selected)
    {
        m_MeshSelected = selected;
    }

} // namespace nero
