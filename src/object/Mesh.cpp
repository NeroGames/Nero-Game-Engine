////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
// All rights reserved
//
////////////////////////////////////////////////////////////

#include <NERO/object/Mesh.h>

namespace nero
{
    Mesh::Mesh(Shape shape,  sf::Vector2f position, float size, Type type):
        m_Id(-1),
        m_Shape(shape),
        m_Type(type),
        m_IsValid(true),
        //
        m_Position(position),
        m_Size(size),
        m_Color(STATIC_MESH_COLOR),
        //
        m_FixedRotation(false),
        m_IsSensor(false),
        m_AllowSleep(true),
        m_Density(1.f),
        m_Friction(0.1f),
        m_Restitution(0.1f),
        m_GravityScale(3.f),
        //
        m_VertexTab(),
        m_LineTab(),
        m_PolygonTab(),
        m_CircleShape()
    {
        switch(shape)
        {
            case Line_Mesh:
            {
                //A line is created with the position of its center end its size
                //Representation : two vertex, one line

                m_Color = STATIC_MESH_COLOR;
                m_Type  = Static_Mesh;

                //Create vertex
                sf::Vector2f point1 = sf::Vector2f(position.x - size, position.y);
                sf::Vector2f point2 = sf::Vector2f(position.x + size, position.y);
                m_VertexTab.push_back(createVertex(point1, m_Color));
                m_VertexTab.push_back(createVertex(point2, m_Color));

                //Create line
                updateLineShape();

            }break;

            case Circle_Mesh:
            {
                //A circle is created with the position of its center and the size of its radius
                //Representation: two vertex, one line, one circle shape

                m_Color = DYNAMIC_MESH_COLOR;
                m_Type  = Dynamic_Mesh;

                //Create vertex
                sf::Vector2f center = sf::Vector2f(position);
                sf::Vector2f point = sf::Vector2f(position.x + size, position.y);
                m_VertexTab.push_back(createVertex(center, m_Color));
                m_VertexTab.push_back(createVertex(point, m_Color));

                //Create line
                updateLineShape();

                //Create circle
                updateCircleShape();

            }break;

            case Polygon_Mesh:
            {
                //The default polygon is a square
                //Representation : a set of vertex, a set of lines and a set of polygon

                m_Color = DYNAMIC_MESH_COLOR;
                m_Type  = Dynamic_Mesh;

                //Create vertex and polygon
                m_PolygonTab.push_back(createRegularPolygon(position, m_Color, 4));

                //Create line
                updateLineShapeLoop();

            }break;
        }
    }

    void Mesh::validate(const sf::Vector2f& point1, const sf::Vector2f& point2)
    {
        if(distance(point1, point2) < Min_Vertex_Distance)
            m_IsValid = false;
    }


    sf::RectangleShape Mesh::createVertex(const sf::Vector2f& point, const sf::Color& color)
    {
        sf::RectangleShape vertex;

        vertex.setPosition(point);
        vertex.setFillColor(color);
        vertex.setSize(sf::Vector2f(Vertex_Size, Vertex_Size));
        vertex.setOrigin(sf::Vector2f(Vertex_Size/2, Vertex_Size/2));

        return vertex;
    }

    void Mesh::updateLine(bool loop)
    {
        m_LineTab.clear();

        m_IsValid = true;

        for(int i = 0; i < m_VertexTab.size()-1; i++)
        {
            sf::Vector2f point1 = m_VertexTab[i].getPosition();
            sf::Vector2f point2 = m_VertexTab[i+1].getPosition();
            m_LineTab.push_back(createLine(point1, point2, transparent_color(m_Color, Color_Alpha)));

            validate(point1, point2);
        }

        if(loop)
        {
            sf::Vector2f point1 = m_VertexTab[0].getPosition();
            sf::Vector2f point2 = m_VertexTab[m_VertexTab.size()-1].getPosition();
            m_LineTab.push_back(createLine(point1, point2, transparent_color(m_Color, Color_Alpha)));

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

    sf::RectangleShape Mesh::createLine(const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Color& color)
    {
        sf::RectangleShape line;

        float length = distance(point1, point2);
        line.setSize(sf::Vector2f(length, Vertex_Size/2.f));
        line.setPosition(point1);
        line.setFillColor(color);
        line.setOrigin(sf::Vector2f(line.getOrigin().x, Vertex_Size/4.f));

        float delta_x = point2.x - point1.x;
        float delta_y = point2.y - point1.y;
        float angle = atan2(delta_y, delta_x);
        line.setRotation(toDegree(angle));

        return line;
    }

    void Mesh::updateCircleShape()
    {
        m_Size = distance(m_VertexTab.front().getPosition(), m_VertexTab.back().getPosition());
        m_CircleShape.setOutlineThickness(Vertex_Size/8.f);
        m_CircleShape.setOutlineColor(m_Color);
        m_CircleShape.setFillColor(transparent_color(m_Color, 50));
        m_CircleShape.setPosition(m_VertexTab.front().getPosition());
        m_CircleShape.setRadius(m_Size);
        m_CircleShape.setOrigin(sf::Vector2f(m_Size, m_Size));
    }

    sf::ConvexShape Mesh::createRegularPolygon(const sf::Vector2f& position, const sf::Color& color, const int& pointCount)
    {
        float theta = PI/4.f;

        sf::ConvexShape poligon;

        poligon.setPointCount(pointCount);

        for(int i = 0; i < pointCount; i++)
        {
            sf::Vector2f point;
            point.x = m_Size * cos(2*PI*i/pointCount + theta) + position.x;
            point.y = m_Size * sin(2*PI*i/pointCount + theta) + position.y;

            poligon.setPoint(i, point);

            m_VertexTab.push_back(createVertex(point, color));
        }

        poligon.setOutlineColor(color);
        poligon.setFillColor(transparent_color(color, Color_Alpha));

        return poligon;
    }

    PointTab Mesh::getAllVertexPoint() const
    {
        PointTab vertexPointTab;

        for(auto vertex : m_VertexTab)
        {
            sf::Vector2f point;
            point.x = vertex.getPosition().x;
            point.y = vertex.getPosition().y;
            vertexPointTab.push_back(point);
        }

        return vertexPointTab;
    }

    void Mesh::updatePolygonShape()
    {
        m_PolygonTab.clear();

        PointTab vertexPointTab = getAllVertexPoint();

        if(validate_polygon(vertexPointTab) == 0)
        {
            m_IsValid = true;

            std::vector<PointTab> convexPolygonTab = concave_to_convex(vertexPointTab);

            for(PointTab polygon : convexPolygonTab)
            {
                m_PolygonTab.push_back(createPolygon(polygon, m_Color));
            }
        }
        else
        {
            m_IsValid = false;
        }
    }

    sf::ConvexShape Mesh::createPolygon(const PointTab& pointTab, const sf::Color& color)
    {
        sf::ConvexShape polygon;
        polygon.setPointCount(pointTab.size());

        for(int i = 0; i < pointTab.size(); i++)
        {
            polygon.setPoint(i, pointTab[i]);
        }

        polygon.setOutlineColor(color);
        polygon.setFillColor(transparent_color(color, Color_Alpha));

        return polygon;
    }


    void Mesh::addVertex(sf::Vector2f point, const int& index)
    {
        point.x += 1.f;
        point.y += 1.f;

        m_VertexTab.insert(m_VertexTab.begin() + index, createVertex(point, m_Color));

        for(int i = 0; i < m_VertexTab.size()-1; i++)
        {
            sf::Vector2f point1 = m_VertexTab[i].getPosition();
            sf::Vector2f point2 = m_VertexTab[i+1].getPosition();

            validate(point1, point2);

            if(!m_IsValid)
                break;
        }
    }

    void Mesh::addVertex(sf::Vector2f point)
    {
        point.x += 0.000000000000000001f;
        point.y += 0.000000000000000001f;

        m_VertexTab.push_back(createVertex(point, m_Color));

        for(int i = 0; i < m_VertexTab.size()-1; i++)
        {
            sf::Vector2f point1 = m_VertexTab[i].getPosition();
            sf::Vector2f point2 = m_VertexTab[i+1].getPosition();

            validate(point1, point2);

            if(!m_IsValid)
                break;
        }

    }

    void Mesh::deleteVertex(const int& index)
    {
        m_VertexTab.erase(m_VertexTab.begin() + index);
    }

    void Mesh::setColor(sf::Color color)
    {
        m_Color = color;

        for(auto it = m_VertexTab.begin(); it != m_VertexTab.end(); it++)
            it->setFillColor(color);

        for(auto it = m_LineTab.begin(); it != m_LineTab.end(); it++)
            it->setFillColor(transparent_color(color, Color_Alpha));

        if(m_Shape == Polygon_Mesh)
        {
            for(auto it = m_PolygonTab.begin(); it != m_PolygonTab.end(); it++)
                it->setFillColor(transparent_color(color, Color_Alpha));
        }

        if(m_Shape == Circle_Mesh)
        {
            m_CircleShape.setFillColor(transparent_color(color, Color_Alpha));
            m_CircleShape.setOutlineColor(color);
        }
    }

    void Mesh::updateColor()
    {
        if(m_IsValid)
        {
            switch(m_Type)
            {
                case Static_Mesh:
                    m_Color = STATIC_MESH_COLOR;
                    break;
                case Dynamic_Mesh:
                    m_Color = DYNAMIC_MESH_COLOR;
                    break;
                case Kinematic_Mesh:
                    m_Color = KINEMATIC_MESH_COLOR;
                    break;
            }

            setColor(m_Color);
        }
        else
        {
            setColor(INVALIDE_MESH_COLOR);
        }
    }

    sf::Vector2f Mesh::getPosition() const
    {
        if(m_Shape == Circle_Mesh)
            return m_CircleShape.getPosition();
        else
            return sf::Vector2f(0.f, 0.f);
    }

    float Mesh::getRotation() const
    {
        return 0.f;
    }

    float Mesh::getRadius() const
    {
        if(m_Shape == Circle_Mesh)
            return m_CircleShape.getRadius();
        else
            return m_Size;
    }

    int Mesh::getPointCount() const
    {
        return m_VertexTab.size();
    }

    sf::FloatRect Mesh::getGlobalBounds() const
    {
        sf::FloatRect boundRect;

        switch(m_Shape)
        {
            case Mesh::Line_Mesh:
            {
                boundRect = m_LineTab[0].getGlobalBounds();
                boundRect.left = boundRect.left - 10.f;
                boundRect.top = boundRect.top  -10.f;
                boundRect.height = boundRect.height + 20.f;
                boundRect.width = boundRect.width + 20.f;
            }
                break;

            case Mesh::Circle_Mesh:
            {
                //boundRect = circleShape.getLocalBounds();
                boundRect = getTransform().transformRect(m_CircleShape.getGlobalBounds());
                boundRect.left = boundRect.left - 7.f;
                boundRect.top = boundRect.top  -7.f;
                boundRect.height = boundRect.height + 14.f;
                boundRect.width = boundRect.width + 14.f;

            }break;

            default:
            {
                auto v = getAllVertexPoint();

                auto xExtremes = std::minmax_element(v.begin(), v.end(),
                                     [](const sf::Vector2f& lhs, const sf::Vector2f& rhs) {
                                        return lhs.x < rhs.x;
                                     });

                auto yExtremes = std::minmax_element(v.begin(), v.end(),
                                                     [](const sf::Vector2f& lhs, const sf::Vector2f& rhs) {
                                                        return lhs.y < rhs.y;
                                                     });

                sf::Vector2f  upperLeft(xExtremes.first->x, yExtremes.first->y);
                sf::Vector2f  lowerRight(xExtremes.second->x, yExtremes.second->y);

                boundRect.left = upperLeft.x -10.f;
                boundRect.top = upperLeft.y -10.f;
                boundRect.width = lowerRight.x - upperLeft.x + 20.f;
                boundRect.height = lowerRight.y - upperLeft.y  + 20.f;

            }break;
        }

        return boundRect;
    }

     void Mesh::updateShape()
    {
        if(m_Shape == Polygon_Mesh)
        {
            updateLineShapeLoop();
            updatePolygonShape();
        }
        else
            updateLineShape();

        if(m_Shape == Circle_Mesh)
            updateCircleShape();
    }

    void Mesh::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        if(m_Shape == Mesh::Polygon_Mesh)
            for(auto polygon : m_PolygonTab)
                target.draw(polygon, states);

        if(m_Shape == Mesh::Circle_Mesh)
            target.draw(m_CircleShape, states);

        for(auto line : m_LineTab)
            target.draw(line, states);

        for(auto vertex : m_VertexTab)
            target.draw(vertex, states);
    }

    void Mesh::move(const sf::Vector2f& offset)
    {
        for(auto& vertex : m_VertexTab)
                vertex.move(offset);

        updateShape();
    }

    void Mesh::rotate(const float& angle)
    {
        if(m_Shape == Mesh::Line_Mesh)
        {
            sf::Vector2f center = getLineCenter(m_VertexTab.front().getPosition(), m_VertexTab.back().getPosition());
            m_VertexTab.front().setPosition(rotateVertex(center, angle, m_VertexTab.front().getPosition()));
            m_VertexTab.back().setPosition(rotateVertex(center, angle, m_VertexTab.back().getPosition()));

        }
        else if (m_Shape == Mesh::Circle_Mesh)
        {
            m_VertexTab.back().setPosition(rotateVertex(m_VertexTab.front().getPosition(), angle, m_VertexTab.back().getPosition()));
        }
        else
        {
            PointTab pointTab = getAllVertexPoint();
            sf::Vector2f center = getPolygonCenter(pointTab);

            for(auto it = m_VertexTab.begin(); it != m_VertexTab.end(); it++)
            {
                it->setPosition(rotateVertex(center, angle, it->getPosition()));
            }
        }

        updateShape();
    }

    void Mesh::scale(const sf::Vector2f& factor)
    {
        float scale = factor.x;

        if(m_Shape == Mesh::Line_Mesh)
        {
            sf::Vector2f pos1 = m_VertexTab.front().getPosition();
            sf::Vector2f pos2 = m_VertexTab.back().getPosition();

            sf::Vector2f center = getLineCenter(pos1, pos2);

            sf::Vector2f new_pos1 = sf::Vector2f((pos1.x-center.x)*scale + center.x, (pos1.y-center.y)*scale + center.y);
            sf::Vector2f new_pos2 = sf::Vector2f((pos2.x-center.x)*scale + center.x, (pos2.y-center.y)*scale + center.y);

            m_VertexTab.front().setPosition(new_pos1);
            m_VertexTab.back().setPosition(new_pos2);
        }
        else if (m_Shape == Mesh::Circle_Mesh)
        {
            sf::Vector2f center = m_VertexTab.front().getPosition();
            sf::Vector2f pos = m_VertexTab.back().getPosition();
            sf::Vector2f new_pos = sf::Vector2f((pos.x-center.x)*scale + center.x, (pos.y-center.y)*scale + center.y);
            m_VertexTab.back().setPosition(new_pos);
        }
        else
        {
            PointTab vectorTab = getAllVertexPoint();
            sf::Vector2f center = getPolygonCenter(vectorTab);

            for(auto it = m_VertexTab.begin(); it != m_VertexTab.end(); it++)
            {
                sf::Vector2f pos = it->getPosition();
                sf::Vector2f new_pos = sf::Vector2f((pos.x-center.x)*scale + center.x, (pos.y-center.y)*scale + center.y);

                it->setPosition(new_pos);
            }
        }

        updateShape();
    }

    int Mesh::getId() const
    {
        return m_Id;
    }

    Mesh::Type Mesh::getType() const
    {
        return m_Type;
    }

    Mesh::Shape Mesh::getShape() const
    {
        return m_Shape;
    }

    bool Mesh::isValid() const
    {
        return m_IsValid;
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


    void Mesh::setId(int id)
    {
        m_Id = id;
    }

    void Mesh::setType(Type type)
    {
        m_Type = type;
    }

    void Mesh::setShape(Shape shape)
    {
        m_Shape = shape;
    }

    void Mesh::setIsValid(bool flag)
    {
        m_IsValid = flag;
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

        mesh["shape"]           = String_Shape[getShape()];
        mesh["type"]            = String_Type[getType()];
        mesh["density"]         = getDensity();
        mesh["friction"]        = getFriction();
        mesh["restitution"]     = getRestitution();
        mesh["fixed_rotation"]  = getFixedRotation();
        mesh["is_sensor"]       = getIsSensor();
        mesh["allow_sleep"]     = getAllowSleep();
        mesh["gravity_scale"]   = getGravityScale();
        mesh["is_valid"]        = isValid();

        std::vector<nlohmann::json> vertex_table;

        int i = 0;
        for(auto v : m_VertexTab)
        {
            nlohmann::json vertex;

            vertex = {{"x", v.getPosition().x}, {"y", v.getPosition().y}};
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
        return &m_VertexTab;
    }

}
