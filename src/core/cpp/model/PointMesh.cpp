////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
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
    {
        // Create the default Line/Chain with two vertices
        if(m_MeshShape == Shape::Line || m_MeshShape == Shape::Chain)
        {
            const auto center = getPointCenter();
            const auto radius = 75.f;
            addVertex(sf::Vector2f(center.x + radius, center.y));
            addVertex(sf::Vector2f(center.x - radius, center.y));

            updateShape();
            updateColor();
        }
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

    sf::FloatRect PointMesh::getGlobalBounds() const
    {
        sf::FloatRect globalBound;

        if(m_MeshShape == Shape::None)
            return globalBound;

        // TODO move to circleMesh class
        if(m_MeshShape == Shape::Circle)
        {
            sf::Vector2f pointOne = m_VertexTable[0].getPosition();
            sf::Vector2f pointTwo = m_VertexTable[1].getPosition();
            float        radius   = math::distance(pointOne, pointTwo);

            globalBound.left      = pointOne.x - radius - 5.f;
            globalBound.top       = pointTwo.y - radius - 5.f;
            globalBound.height    = radius * 2.f + 10.f;
            globalBound.width     = radius * 2.f + 10.f;

            return globalBound;
        }

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

    sf::Vector2f PointMesh::getPointCenter() const
    {
        sf::Vector2f  pointCenter;
        sf::FloatRect globalBound = getGlobalBounds();

        pointCenter.x             = globalBound.left + globalBound.width / 2.f;
        pointCenter.y             = globalBound.top + globalBound.height / 2.f;

        return pointCenter;
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
        line.setFillColor(getColor());

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
        sf::RectangleShape vertex;

        vertex.setOrigin(sf::Vector2f(m_VertexSize / 2.f, m_VertexSize / 2.f));
        vertex.setSize(sf::Vector2f(m_VertexSize, m_VertexSize));
        vertex.setPosition(position);

        if(index < 0)
        {
            m_VertexTable.push_back(vertex);
        }
        else
        {
            m_VertexTable.insert(m_VertexTable.begin() + index, vertex);
        }
    }

    void PointMesh::deleteVertex(const int& index)
    {
        m_VertexTable.erase(m_VertexTable.begin() + index);
    }

    void PointMesh::addLine(const sf::Vector2f& point1, const sf::Vector2f& point2)
    {
        sf::RectangleShape line;

        float              length = math::distance(point1, point2);
        line.setOrigin(sf::Vector2f(line.getOrigin().x, m_VertexSize / 4.f));
        line.setSize(sf::Vector2f(length, m_VertexSize / 2.f));
        line.setPosition(point1);

        float delta_x = point2.x - point1.x;
        float delta_y = point2.y - point1.y;
        float angle   = atan2(delta_y, delta_x);
        line.setRotation(math::toDegree(angle));

        m_LineTable.push_back(line);
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
            vectorTable.push_back(vertex.getPosition());
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
        else if(m_MeshShape == Shape::Circle)
        {
            sf::Vector2f massCenter = m_VertexTable.front().getPosition();
            sf::Vector2f pointTwo   = m_VertexTable.back().getPosition();
            sf::Vector2f newPointTwo =
                sf::Vector2f((pointTwo.x - massCenter.x) * scaleFactor.x + massCenter.x,
                             (pointTwo.y - massCenter.y) * scaleFactor.y + massCenter.y);
            m_VertexTable.back().setPosition(newPointTwo);
        }
        else
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
        else if(m_MeshShape == Shape::Circle)
        {
            m_VertexTable.back().setPosition(
                math::rotateVertex(m_VertexTable.front().getPosition(),
                                   angle,
                                   m_VertexTable.back().getPosition()));
        }
        else
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
        if(m_MeshShape == Shape::Polygon)
            updateLine(true);
        else
            updateLine(false);
    }

    nlohmann::json PointMesh::toJson() const
    {
        nlohmann::json mesh;

        mesh["shape"]    = "TODO"; // String_Shape[getShape()];
        mesh["type"]     = "TODO"; // String_Type[getType()];
        mesh["is_valid"] = m_MeshValid;

        std::vector<nlohmann::json> vertex_table;

        int                         i = 0;
        for(auto v : m_VertexTable)
        {
            nlohmann::json vertex;

            vertex = {
                {"x", v.getPosition().x},
                {"y", v.getPosition().y}
            };
            vertex["order"] = i++;

            vertex_table.push_back(vertex);
        }

        mesh["vertex_table"] = vertex_table;
        mesh["vertex_count"] = i;

        return mesh;
    }

    sf::String PointMesh::toSting() const
    {
        return toJson().dump(4);
    }
} // namespace nero
