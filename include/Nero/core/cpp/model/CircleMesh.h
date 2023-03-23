////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef CIRCLEMESH_H
#define CIRCLEMESH_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/model/PointMesh.h>
// SFML
#include <SFML/Graphics/CircleShape.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    class CircleMesh : public PointMesh
    {
      public:
        using Ptr = std::shared_ptr<CircleMesh>;

      public:
        CircleMesh();
        virtual void           updateShape() override;
        virtual void           updateColor() override;
        virtual void           scaleMesh(const sf::Vector2f& scaleFactor) override;
        virtual void           rotateMesh(const float& angle) override;
        virtual sf::FloatRect  getRealGlobalBounds() const override;
        virtual void           generateDefaultShape() override;
        virtual PointMesh::Ptr clone() const override;
        sf::CircleShape&       getCircleShape();
        static CircleMesh::Ptr Cast(PointMesh::Ptr pointMesh);

      private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

      private:
        sf::CircleShape m_CircleShape;
    };
} // namespace nero
#endif // CIRCLEMESH_H
