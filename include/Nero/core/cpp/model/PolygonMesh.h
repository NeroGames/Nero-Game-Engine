////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef POLYGONMESH_H
#define POLYGONMESH_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/model/PointMesh.h>
// SFML
#include <SFML/Graphics/ConvexShape.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    using PolygonTable = std::vector<sf::ConvexShape>;

    class PolygonMesh : public PointMesh
    {
      public:
        using Ptr = std::shared_ptr<PolygonMesh>;

      public:
        PolygonMesh();
        virtual void            updateShape() override;
        virtual void            updateColor() override;
        PolygonTable&           getPolygonTable();
        static PolygonMesh::Ptr Cast(PointMesh::Ptr pointMesh);

      private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        // TODO move in nero::math lib
        PointTable   generateRegularPolygon(const sf::Vector2f& position,
                                            const int&          pointCount,
                                            const float&        radius);
        void         addPolygon(const PointTable& pointTable);

      private:
        PolygonTable m_PolygonTable;
    };
} // namespace nero
#endif // POLYGONMESH_H
