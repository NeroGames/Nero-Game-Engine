////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/model/Mesh.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
#include <Nero/core/cpp/utility/Math.h>
////////////////////////////////////////////////////////////
namespace nero
{
    Mesh::Mesh(const Shape& shape)
        : m_MeshId(-1)
        , m_MeshShape(shape)
        , m_MeshType(Type::Dynamic)
        , m_GravityCenter(sf::Vector2f(0.f, 0.f))
        , m_Valid(true)
        , m_CircleShape(nullptr)
        , m_Position(sf::Vector2f(0.f, 0.f))
        , m_Scale(sf::Vector2f(1.f, 1.f))
        , m_Rotation(0.f)
    {
        switch(shape)
        {
            case Shape::Polygon:
                createDefaultPolygon();
                break;
            case Shape::Circle:
                createDefaultCircle();
                break;
            case Shape::Line:
                createDefaultLine();
                break;
        }

        if(m_MeshShape != Shape::None)
        {
            setOrigin(getGlobalBounds().width / 2.f, getGlobalBounds().height / 2.f);
        }

        // TODO
        m_CircleShape = new sf::CircleShape();
    }

    Mesh::~Mesh()
    {
        m_CircleShape = nullptr;
        delete m_CircleShape;
    }

    void Mesh::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for(auto vertex : m_VertexTable)
            target.draw(vertex);

        for(auto line : m_LineTable)
            target.draw(line);

        if(m_CircleShape)
            target.draw(*m_CircleShape);

        if(m_MeshShape == Shape::Polygon)
        {
            for(auto polygon : m_PolygonTable)
                target.draw(polygon);
        }
    }

    void Mesh::createDefaultPolygon()
    {
        m_MeshType                           = Type::Dynamic;

        std::vector<sf::Vector2f> pointTable = generateRegularPolygon(m_GravityCenter, 4, 50.f);

        for(const sf::Vector2f& point : pointTable)
        {
            addVertex(point);
        }

        updateMesh();
    }

    void Mesh::createDefaultCircle()
    {
        m_MeshType = Type::Dynamic;

        addVertex(m_GravityCenter);
        addVertex(sf::Vector2f(m_GravityCenter.x + 50.f, m_GravityCenter.y));

        updateMesh();
    }

    void Mesh::createDefaultLine()
    {
        m_MeshType = Type::Static;

        addVertex(sf::Vector2f(m_GravityCenter.x + 75.f, m_GravityCenter.y));
        addVertex(sf::Vector2f(m_GravityCenter.x - 75.f, m_GravityCenter.y));

        updateMesh();
    }

    void Mesh::addVertex(const sf::Vector2f& position, const int& index)
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

    void Mesh::addLine(const sf::Vector2f& point1, const sf::Vector2f& point2)
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

    void Mesh::addPolygon(const std::vector<sf::Vector2f>& pointTable)
    {
        sf::ConvexShape polygon;

        polygon.setPointCount(pointTable.size());

        for(int i = 0; i < pointTable.size(); i++)
        {
            polygon.setPoint(i, pointTable[i]);
        }

        m_PolygonTable.push_back(polygon);
    }

    void Mesh::validate(const sf::Vector2f& point1, const sf::Vector2f& point2)
    {
        if(math::distance(point1, point2) < m_MinVertexDistance)
        {
            m_Valid = false;
        }
    }

    void Mesh::updateMesh(const bool& shape, const bool& color)
    {
        if(shape)
        {
            // update line
            m_LineTable.clear();
            m_Valid = true;

            for(int i = 0; i < m_VertexTable.size() - 1; i++)
            {
                sf::Vector2f point1 = m_VertexTable[i].getPosition();
                sf::Vector2f point2 = m_VertexTable[i + 1].getPosition();
                addLine(point1, point2);

                validate(point1, point2);
            }

            if(m_MeshShape == Shape::Polygon)
            {
                sf::Vector2f point1 = m_VertexTable[0].getPosition();
                sf::Vector2f point2 = m_VertexTable[m_VertexTable.size() - 1].getPosition();
                addLine(point1, point2);

                validate(point1, point2);
            }

            // update circle shape
            if(m_MeshShape == Shape::Circle)
            {
                float radius = math::distance(m_VertexTable.front().getPosition(),
                                              m_VertexTable.back().getPosition());

                if(!m_CircleShape)
                {
                    m_CircleShape = new sf::CircleShape(radius);
                    m_CircleShape->setOutlineThickness(m_VertexSize / 8.f);
                }

                m_CircleShape->setRadius(radius);
                m_CircleShape->setOrigin(sf::Vector2f(radius, radius));
                m_CircleShape->setPosition(m_VertexTable.front().getPosition());
            }

            // update polygon shape
            if(m_MeshShape == Shape::Polygon)
            {
                m_PolygonTable.clear();

                std::vector<sf::Vector2f> posTable = getVertexPosition();
                m_Valid                            = (math::validatePolygon(posTable) == 0);

                if(m_Valid)
                {
                    std::vector<std::vector<sf::Vector2f>> polygonTable =
                        math::concaveToConvex(posTable);

                    for(std::vector<sf::Vector2f> pointTable : polygonTable)
                    {
                        addPolygon(pointTable);
                    }
                }
            }
        }

        if(color)
        {
            sf::Color meshColor = getColor();

            for(auto it = m_VertexTable.begin(); it != m_VertexTable.end(); it++)
                it->setFillColor(meshColor);

            for(auto it = m_LineTable.begin(); it != m_LineTable.end(); it++)
                it->setFillColor(sf::Color(meshColor.r, meshColor.g, meshColor.b, m_ColorAlpha));

            if(m_MeshShape == Shape::Polygon)
            {
                for(auto it = m_PolygonTable.begin(); it != m_PolygonTable.end(); it++)
                    it->setFillColor(
                        sf::Color(meshColor.r, meshColor.g, meshColor.b, m_ColorAlpha));
            }
            else if(m_MeshShape == Shape::Circle)
            {
                m_CircleShape->setFillColor(
                    sf::Color(meshColor.r, meshColor.g, meshColor.b, m_ColorAlpha));
                m_CircleShape->setOutlineColor(meshColor);
            }
        }
    }

    std::vector<sf::Vector2f> Mesh::generateRegularPolygon(const sf::Vector2f& position,
                                                           const int&          pointCount,
                                                           const float&        radius)
    {
        std::vector<sf::Vector2f> pointTable;

        float                     theta = math::PI / 4.f;
        for(int i = 0; i < pointCount; i++)
        {
            sf::Vector2f point;
            point.x = radius * cos(2 * math::PI * i / pointCount + theta) + position.x;
            point.y = radius * sin(2 * math::PI * i / pointCount + theta) + position.y;

            pointTable.push_back(point);
        }

        return pointTable;
    }

    void Mesh::setMeshId(const int& meshId)
    {
        m_MeshId = meshId;
    }

    int Mesh::getMeshId() const
    {
        return m_MeshId;
    }

    sf::Color Mesh::getColor()
    {
        if(m_Valid)
        {
            switch(m_MeshType)
            {
                case Type::Static:
                    return EngineConstant.COLOR_STATIC_MESH;
                    break;
                case Type::Dynamic:
                    return EngineConstant.COLOR_DYNAMIC_MESH;
                    break;
                case Type::Kinematic:
                    return EngineConstant.COLOR_KINEMATIC_MESH;
                    break;
            }
        }
        else
        {
            return EngineConstant.COLOR_INVALIDE_MESH;
        }
    }

    std::vector<sf::Vector2f> Mesh::getVertexPosition() const
    {
        std::vector<sf::Vector2f> posTable;

        for(auto vertex : m_VertexTable)
        {
            posTable.push_back(vertex.getPosition());
        }

        return posTable;
    }

    sf::FloatRect Mesh::getGlobalBounds() const
    {
        sf::FloatRect boundRect;

        switch(m_MeshShape)
        {
            case Shape::Circle:
            {
                sf::Vector2f point1 = m_VertexTable[0].getPosition();
                sf::Vector2f point2 = m_VertexTable[1].getPosition();
                float        radius = math::distance(point1, point2);

                boundRect.left      = point1.x - radius - 5.f;
                boundRect.top       = point1.y - radius - 5.f;
                boundRect.height    = radius * 2.f + 10.f;
                boundRect.width     = radius * 2.f + 10.f;
            }
            break;

            case Shape::Polygon:
            case Shape::Chain:
            case Shape::Line:
            {
                std::vector<sf::Vector2f> posTable = getVertexPosition();

                auto                      xExtremes =
                    std::minmax_element(posTable.begin(),
                                        posTable.end(),
                                        [](const sf::Vector2f& lhs, const sf::Vector2f& rhs)
                                        {
                                            return lhs.x < rhs.x;
                                        });

                auto yExtremes =
                    std::minmax_element(posTable.begin(),
                                        posTable.end(),
                                        [](const sf::Vector2f& lhs, const sf::Vector2f& rhs)
                                        {
                                            return lhs.y < rhs.y;
                                        });

                sf::Vector2f upperLeft(xExtremes.first->x, yExtremes.first->y);
                sf::Vector2f lowerRight(xExtremes.second->x, yExtremes.second->y);

                boundRect.left   = upperLeft.x - 5.f;
                boundRect.top    = upperLeft.y - 5.f;
                boundRect.width  = lowerRight.x - upperLeft.x + 10.f;
                boundRect.height = lowerRight.y - upperLeft.y + 10.f;
            }
            break;
        }

        return boundRect;
    }

    void Mesh::updateMesh(const sf::Vector2f& position,
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

    void Mesh::moveMesh(const sf::Vector2f& offset)
    {
        for(auto& vertex : m_VertexTable)
        {
            vertex.move(offset);
        }

        updateMesh();
    }

    void Mesh::scaleMesh(const sf::Vector2f& factor)
    {
        if(m_MeshShape == Shape::Line)
        {
            sf::Vector2f pos1     = m_VertexTable.front().getPosition();
            sf::Vector2f pos2     = m_VertexTable.back().getPosition();

            sf::Vector2f center   = math::getLineCenter(pos1, pos2);

            sf::Vector2f new_pos1 = sf::Vector2f((pos1.x - center.x) * factor.x + center.x,
                                                 (pos1.y - center.y) * factor.y + center.y);
            sf::Vector2f new_pos2 = sf::Vector2f((pos2.x - center.x) * factor.x + center.x,
                                                 (pos2.y - center.y) * factor.y + center.y);

            m_VertexTable.front().setPosition(new_pos1);
            m_VertexTable.back().setPosition(new_pos2);
        }
        else if(m_MeshShape == Shape::Circle)
        {
            sf::Vector2f center  = m_VertexTable.front().getPosition();
            sf::Vector2f pos     = m_VertexTable.back().getPosition();
            sf::Vector2f new_pos = sf::Vector2f((pos.x - center.x) * factor.x + center.x,
                                                (pos.y - center.y) * factor.y + center.y);
            m_VertexTable.back().setPosition(new_pos);
        }
        else
        {
            std::vector<sf::Vector2f> posTable = getVertexPosition();
            sf::Vector2f              center   = math::getPolygonCenter(posTable);

            for(auto it = m_VertexTable.begin(); it != m_VertexTable.end(); it++)
            {
                sf::Vector2f pos     = it->getPosition();
                sf::Vector2f new_pos = sf::Vector2f((pos.x - center.x) * factor.x + center.x,
                                                    (pos.y - center.y) * factor.y + center.y);

                it->setPosition(new_pos);
            }
        }

        updateMesh();
    }

    void Mesh::rotateMesh(const float& angle)
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
            std::vector<sf::Vector2f> posTable = getVertexPosition();
            sf::Vector2f              center   = math::getPolygonCenter(posTable);

            for(auto it = m_VertexTable.begin(); it != m_VertexTable.end(); it++)
            {
                it->setPosition(math::rotateVertex(center, angle, it->getPosition()));
            }
        }

        updateMesh();
    }

    void Mesh::deleteVertex(const int& index)
    {
        m_VertexTable.erase(m_VertexTable.begin() + index);
    }

    sf::RectangleShape Mesh::createVertex(const sf::Vector2f& point, const sf::Color& color)
    {
        sf::RectangleShape vertex;

        vertex.setOrigin(sf::Vector2f(Vertex_Size / 2.f, Vertex_Size / 2.f));
        vertex.setSize(sf::Vector2f(Vertex_Size, Vertex_Size));
        vertex.setPosition(point);
        vertex.setFillColor(color);

        return vertex;
    }

    void Mesh::updateLine(bool loop)
    {
        m_LineTable.clear();

        m_Valid = true;

        for(int i = 0; i < m_VertexTable.size() - 1; i++)
        {
            sf::Vector2f point1 = m_VertexTable[i].getPosition();
            sf::Vector2f point2 = m_VertexTable[i + 1].getPosition();
            m_LineTable.push_back(
                createLine(point1, point2, graphics::getTransparentColor(m_Color, Color_Alpha)));

            validate(point1, point2);
        }

        if(loop)
        {
            sf::Vector2f point1 = m_VertexTable[0].getPosition();
            sf::Vector2f point2 = m_VertexTable[m_VertexTable.size() - 1].getPosition();
            m_LineTable.push_back(
                createLine(point1, point2, graphics::getTransparentColor(m_Color, Color_Alpha)));

            validate(point1, point2);
        }
    }

    void Mesh::updateLineShape()
    {
        updateLine(false);
    }

    void Mesh::updateLineShapeLoop()
    {
        updateLine(true);
    }

    sf::RectangleShape Mesh::createLine(const sf::Vector2f& point1,
                                        const sf::Vector2f& point2,
                                        const sf::Color&    color)
    {
        sf::RectangleShape line;

        float              length = math::distance(point1, point2);
        line.setOrigin(sf::Vector2f(line.getOrigin().x, Vertex_Size / 4.f));
        line.setSize(sf::Vector2f(length, Vertex_Size / 2.f));
        line.setPosition(point1);
        line.setFillColor(color);

        float delta_x = point2.x - point1.x;
        float delta_y = point2.y - point1.y;
        float angle   = atan2(delta_y, delta_x);
        line.setRotation(math::toDegree(angle));

        return line;
    }

    void Mesh::updateCircleShape()
    {
        m_Size =
            math::distance(m_VertexTable.front().getPosition(), m_VertexTable.back().getPosition());
        m_CircleShape->setOrigin(sf::Vector2f(m_Size, m_Size));
        m_CircleShape->setOutlineThickness(Vertex_Size / 8.f);
        m_CircleShape->setOutlineColor(m_Color);
        m_CircleShape->setFillColor(graphics::getTransparentColor(m_Color, 50));
        m_CircleShape->setPosition(m_VertexTable.front().getPosition());
        m_CircleShape->setRadius(m_Size);
    }

    sf::ConvexShape Mesh::createRegularPolygon(const sf::Vector2f& position,
                                               const sf::Color&    color,
                                               const int&          pointCount)
    {
        float           theta = math::PI / 4.f;

        sf::ConvexShape poligon;

        poligon.setPointCount(pointCount);

        for(int i = 0; i < pointCount; i++)
        {
            sf::Vector2f point;
            point.x = m_Size * cos(2 * math::PI * i / pointCount + theta) + position.x;
            point.y = m_Size * sin(2 * math::PI * i / pointCount + theta) + position.y;

            poligon.setPoint(i, point);

            m_VertexTable.push_back(createVertex(point, color));
        }

        poligon.setOutlineColor(color);
        poligon.setFillColor(graphics::getTransparentColor(color, Color_Alpha));

        return poligon;
    }

    Mesh::VectorTable Mesh::getAllVertexPoint() const
    {
        VectorTable vectorTable;

        for(auto vertex : m_VertexTable)
        {
            sf::Vector2f point;
            point.x = vertex.getPosition().x;
            point.y = vertex.getPosition().y;
            vectorTable.push_back(point);
        }

        return vectorTable;
    }

    void Mesh::updatePolygonShape()
    {
        m_PolygonTable.clear();

        VectorTable vertexVectorTable = getAllVertexPoint();

        if(math::validatePolygon(vertexVectorTable) == 0)
        {
            m_Valid                                   = true;

            std::vector<VectorTable> convexPolygonTab = math::concaveToConvex(vertexVectorTable);

            for(VectorTable polygon : convexPolygonTab)
            {
                m_PolygonTable.push_back(createPolygon(polygon, m_Color));
            }
        }
        else
        {
            m_Valid = false;
        }
    }

    sf::ConvexShape Mesh::createPolygon(const VectorTable& VectorTable, const sf::Color& color)
    {
        sf::ConvexShape polygon;
        polygon.setPointCount(VectorTable.size());

        for(int i = 0; i < VectorTable.size(); i++)
        {
            polygon.setPoint(i, VectorTable[i]);
        }

        polygon.setOutlineColor(color);
        polygon.setFillColor(graphics::getTransparentColor(color, Color_Alpha));

        return polygon;
    }

    void Mesh::addVertex(sf::Vector2f point)
    {
        point.x += 0.000000000000000001f;
        point.y += 0.000000000000000001f;

        m_VertexTable.push_back(createVertex(point, m_Color));

        for(int i = 0; i < m_VertexTable.size() - 1; i++)
        {
            sf::Vector2f point1 = m_VertexTable[i].getPosition();
            sf::Vector2f point2 = m_VertexTable[i + 1].getPosition();

            validate(point1, point2);

            if(!m_Valid)
                break;
        }
    }

    void Mesh::setColor(const sf::Color& color)
    {
        m_Color = color;

        for(auto it = m_VertexTable.begin(); it != m_VertexTable.end(); it++)
            it->setFillColor(color);

        for(auto it = m_LineTable.begin(); it != m_LineTable.end(); it++)
            it->setFillColor(graphics::getTransparentColor(color, Color_Alpha));

        if(m_MeshShape == Shape::Polygon)
        {
            for(auto it = m_PolygonTable.begin(); it != m_PolygonTable.end(); it++)
                it->setFillColor(graphics::getTransparentColor(color, Color_Alpha));
        }

        if(m_MeshShape == Shape::Chain)
        {
            m_CircleShape->setFillColor(graphics::getTransparentColor(color, Color_Alpha));
            m_CircleShape->setOutlineColor(color);
        }
    }

    void Mesh::updateColor()
    {
        if(m_Valid)
        {
            switch(m_MeshType)
            {
                case Type::Static:
                    m_Color = EngineConstant.COLOR_STATIC_MESH;
                    break;
                case Type::Dynamic:
                    m_Color = EngineConstant.COLOR_DYNAMIC_MESH;
                    break;
                case Type::Kinematic:
                    m_Color = EngineConstant.COLOR_KINEMATIC_MESH;
                    break;
            }

            setColor(m_Color);
        }
        else
        {
            setColor(EngineConstant.COLOR_INVALIDE_MESH);
        }
    }

    sf::Vector2f Mesh::getPosition() const
    {
        if(m_MeshShape == Shape::Circle)
            return m_CircleShape->getPosition();
        else
            return sf::Vector2f(0.f, 0.f);
    }

    float Mesh::getRotation() const
    {
        return 0.f;
    }

    float Mesh::getRadius() const
    {
        if(m_MeshShape == Shape::Circle)
            return m_CircleShape->getRadius();
        else
            return m_Size;
    }

    int Mesh::getPointCount() const
    {
        return m_VertexTable.size();
    }

    sf::Vector2f Mesh::getSize()
    {
        sf::FloatRect bound = getGlobalBounds();

        switch(m_MeshShape)
        {
            case Mesh::Shape::Circle:
            {
                bound.height = bound.height - 14.f;
                bound.width  = bound.width - 14.f;
            }
            break;

            default:
            {
                bound.height = bound.height - 20.f;
                bound.width  = bound.width - 20.f;
            }
            break;
        }

        return sf::Vector2f(bound.width, bound.height);
    }

    void Mesh::updateShape()
    {
        if(m_MeshShape == Shape::Polygon)
        {
            updateLineShapeLoop();
            updatePolygonShape();
        }
        else
            updateLineShape();

        if(m_MeshShape == Shape::Circle)
            updateCircleShape();
    }

    void Mesh::move(const sf::Vector2f& offset)
    {
        for(auto& vertex : m_VertexTable)
            vertex.move(offset);

        updateShape();
    }

    void Mesh::rotate(const float& angle)
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
        else if(m_MeshShape == Mesh::Shape::Circle)
        {
            m_VertexTable.back().setPosition(
                math::rotateVertex(m_VertexTable.front().getPosition(),
                                   angle,
                                   m_VertexTable.back().getPosition()));
        }
        else
        {
            VectorTable  VectorTable = getAllVertexPoint();
            sf::Vector2f center      = math::getPolygonCenter(VectorTable);

            for(auto it = m_VertexTable.begin(); it != m_VertexTable.end(); it++)
            {
                it->setPosition(math::rotateVertex(center, angle, it->getPosition()));
            }
        }

        updateShape();
    }

    void Mesh::scale(const sf::Vector2f& factor)
    {
        float scale = factor.x;

        if(m_MeshShape == Shape::Line)
        {
            sf::Vector2f pos1     = m_VertexTable.front().getPosition();
            sf::Vector2f pos2     = m_VertexTable.back().getPosition();

            sf::Vector2f center   = math::getLineCenter(pos1, pos2);

            sf::Vector2f new_pos1 = sf::Vector2f((pos1.x - center.x) * scale + center.x,
                                                 (pos1.y - center.y) * scale + center.y);
            sf::Vector2f new_pos2 = sf::Vector2f((pos2.x - center.x) * scale + center.x,
                                                 (pos2.y - center.y) * scale + center.y);

            m_VertexTable.front().setPosition(new_pos1);
            m_VertexTable.back().setPosition(new_pos2);
        }
        else if(m_MeshShape == Mesh::Shape::Circle)
        {
            sf::Vector2f center  = m_VertexTable.front().getPosition();
            sf::Vector2f pos     = m_VertexTable.back().getPosition();
            sf::Vector2f new_pos = sf::Vector2f((pos.x - center.x) * scale + center.x,
                                                (pos.y - center.y) * scale + center.y);
            m_VertexTable.back().setPosition(new_pos);
        }
        else
        {
            VectorTable  vectorTab = getAllVertexPoint();
            sf::Vector2f center    = math::getPolygonCenter(vectorTab);

            for(auto it = m_VertexTable.begin(); it != m_VertexTable.end(); it++)
            {
                sf::Vector2f pos     = it->getPosition();
                sf::Vector2f new_pos = sf::Vector2f((pos.x - center.x) * scale + center.x,
                                                    (pos.y - center.y) * scale + center.y);

                it->setPosition(new_pos);
            }
        }

        updateShape();
    }

    Mesh::Type Mesh::getType() const
    {
        return m_MeshType;
    }

    Mesh::Shape Mesh::getShape() const
    {
        return m_MeshShape;
    }

    bool Mesh::isValid() const
    {
        return m_Valid;
    }

    bool Mesh::getFixedRotation() const
    {
        return m_FixedRotation;
    }

    bool Mesh::getIsSensor() const
    {
        return m_IsSensor;
    }

    bool Mesh::getAllowSleep() const
    {
        return m_AllowSleep;
    }

    float Mesh::getDensity() const
    {
        return m_Density;
    }

    float Mesh::getFriction() const
    {
        return m_Friction;
    }

    float Mesh::getRestitution() const
    {
        return m_Restitution;
    }

    float Mesh::getGravityScale() const
    {
        return m_GravityScale;
    }

    void Mesh::setType(Type type)
    {
        m_MeshType = type;
    }

    void Mesh::setShape(Shape shape)
    {
        m_MeshShape = shape;
    }

    void Mesh::setIsValid(bool flag)
    {
        m_Valid = flag;
    }

    void Mesh::setFixedRotation(bool flag)
    {
        m_FixedRotation = flag;
    }

    void Mesh::setIsSensor(bool flag)
    {
        m_IsSensor = flag;
    }

    void Mesh::setAllowSleep(bool flag)
    {
        m_AllowSleep = flag;
    }

    void Mesh::setDensity(float density)
    {
        m_Density = density;
    }

    void Mesh::setFriction(float friction)
    {
        m_Friction = friction;
    }

    void Mesh::setRestitution(float restitution)
    {
        m_Restitution = restitution;
    }

    void Mesh::setGravityScale(float gravity_scale)
    {
        m_GravityScale = gravity_scale;
    }

    nlohmann::json Mesh::toJson() const
    {
        nlohmann::json mesh;

        mesh["shape"]          = String_Shape[getShape()];
        mesh["type"]           = String_Type[getType()];
        mesh["density"]        = getDensity();
        mesh["friction"]       = getFriction();
        mesh["restitution"]    = getRestitution();
        mesh["fixed_rotation"] = getFixedRotation();
        mesh["is_sensor"]      = getIsSensor();
        mesh["allow_sleep"]    = getAllowSleep();
        mesh["gravity_scale"]  = getGravityScale();
        mesh["is_valid"]       = isValid();

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

    sf::String Mesh::toSting() const
    {
        return toJson().dump(4);
    }

    Mesh::VertexTab* Mesh::getAllVertex()
    {
        return &m_VertexTable;
    }

    sf::Vector2f Mesh::getCenter() const
    {
        sf::Vector2f  center;
        sf::FloatRect bound = getGlobalBounds();

        center.x            = bound.left + bound.width / 2.f;
        center.y            = bound.top + bound.height / 2.f;

        return center;
    }

    std::vector<sf::RectangleShape> Mesh::getLineTable()
    {
        return m_LineTable;
    }

    std::vector<sf::ConvexShape> Mesh::getPolygonTable()
    {
        return m_PolygonTable;
    }

    sf::CircleShape* Mesh::getCircleShape()
    {
        return m_CircleShape;
    }

    void Mesh::update(const sf::Transform& transform)
    {
    }
} // namespace nero
