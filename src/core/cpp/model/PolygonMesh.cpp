////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/model/PolygonMesh.h>
////////////////////////////////////////////////////////////
namespace nero
{
    PolygonMesh::PolygonMesh()
        : PointMesh(PointMesh::Shape::Polygon)
    {

        const auto                massCenter = sf::Vector2f(0.f, 0.f);
        const auto                pointCount = 4;
        const auto                radius     = 50.f;
        std::vector<sf::Vector2f> pointTable =
            generateRegularPolygon(massCenter, pointCount, radius);

        for(const sf::Vector2f& point : pointTable)
        {
            addVertex(point);
        }

        updateShape();
        updateColor();
    }

    void PolygonMesh::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        PointMesh::draw(target, states);

        for(auto& polygon : m_PolygonTable)
        {
            states.transform = sf::Transform::Identity;
            states.transform.translate(polygon.getPosition());
            target.draw(polygon);
        }
    }

    void PolygonMesh::updateShape()
    {
        updateLine(true);

        m_PolygonTable.clear();

        PointTable pointTable = getPointTable();
        m_MeshValid           = math::validatePolygon(pointTable) == 0;

        if(m_MeshValid)
        {
            std::vector<PointTable> polygonTable = math::concaveToConvex(pointTable);

            for(PointTable polygonPointTable : polygonTable)
            {
                addPolygon(polygonPointTable);
            }
        }
    }

    void PolygonMesh::addPolygon(const PointTable& pointTable)
    {
        sf::ConvexShape polygon;

        polygon.setPointCount(pointTable.size());
        polygon.setFillColor(graphics::getTransparentColor(getColor(), m_ColorAlpha));

        for(int i = 0; i < pointTable.size(); i++)
        {
            polygon.setPoint(i, pointTable[i]);
        }

        m_PolygonTable.push_back(polygon);
    }

    void PolygonMesh::updateColor()
    {
        PointMesh::updateColor();
        for(auto& polygon : m_PolygonTable)
        {
            polygon.setFillColor(graphics::getTransparentColor(getColor(), m_ColorAlpha));
        }
    }

    PointTable PolygonMesh::generateRegularPolygon(const sf::Vector2f& position,
                                                   const int&          pointCount,
                                                   const float&        radius)
    {
        PointTable pointTable;

        float      theta = math::PI / 4.f;
        for(int i = 0; i < pointCount; i++)
        {
            sf::Vector2f point;
            point.x = radius * cos(2 * math::PI * i / pointCount + theta) + position.x;
            point.y = radius * sin(2 * math::PI * i / pointCount + theta) + position.y;

            pointTable.push_back(point);
        }

        return pointTable;
    }

} // namespace nero
