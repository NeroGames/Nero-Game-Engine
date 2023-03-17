////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef POINTMESH_H
#define POINTMESH_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/utility/Utility.h>
// SFML
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
// JSON
#include <json/json.hpp>
// Std
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    using VertexTable = std::vector<sf::RectangleShape>;
    using LineTable   = std::vector<sf::RectangleShape>;
    using PointTable  = std::vector<sf::Vector2f>;

    class PointMesh : public sf::Transformable, public sf::Drawable
    {
      public: // utility
        enum Type
        {
            Static,
            Dynamic,
            Kinematic
        };
        enum Shape
        {
            Polygon,
            Circle,
            Chain,
            Line,
            None
        };

        using Ptr   = std::shared_ptr<PointMesh>;
        using Table = std::vector<PointMesh::Ptr>;

      public:
        // ctr & dtr
        PointMesh(const Shape& shape = Shape::None);
        ~PointMesh() override = default;

        void                   setMeshId(const int& meshId);
        int                    getMeshId() const;
        void                   setMeshType(const Type type);
        Type                   getMeshType() const;
        void                   setMeshShape(const Shape shape);
        Shape                  getMeshShape() const;
        bool                   meshValid() const;

        void                   addVertex(const sf::Vector2f& point, const int& index);
        void                   addVertex(const sf::Vector2f& point);
        void                   deleteVertex(const int& index);
        void                   addLine(const sf::Vector2f& pointOne, const sf::Vector2f& pointTwo);
        PointTable             getPointTable() const;
        VertexTable&           getVertexTable();
        LineTable&             getLineTable();
        unsigned int           getPointCount() const;

        sf::Color              getColor() const;
        sf::String             toSting() const;

        virtual sf::FloatRect  getGlobalBounds() const;
        virtual sf::Vector2f   getPointCenter() const;
        virtual void           updateShape();
        virtual void           updateColor();
        virtual void           moveMesh(const sf::Vector2f& offset);
        virtual void           scaleMesh(const sf::Vector2f& scaleFactor);
        virtual void           rotateMesh(const float& angle);
        void                   transform(const sf::Vector2f& position,
                                         const sf::Vector2f& scale,
                                         const float&        rotation);

        virtual nlohmann::json toJson() const;

      protected:
        virtual void       draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        // Create & Generate
        sf::RectangleShape createVertex(const sf::Vector2f& point) const;
        sf::RectangleShape createLine(const sf::Vector2f& pointOne,
                                      const sf::Vector2f& pointTwo) const;
        // Edit mesh
        void               validateMesh(const sf::Vector2f& pointOne, const sf::Vector2f& pointTwo);
        void               updateLine(const bool loop);

      protected:
        // Mesh constants
        const float     m_VertexSize        = 8.f;
        const sf::Uint8 m_ColorAlpha        = 50.f;
        const float     m_MinVertexDistance = 1.f;
        // Mesh properties
        int             m_MeshId;
        Shape           m_MeshShape;
        Type            m_MeshType;
        bool            m_MeshValid;
        VertexTable     m_VertexTable;
        LineTable       m_LineTable;
        // Transform
        sf::Vector2f    m_Position;
        sf::Vector2f    m_Scale;
        float           m_Rotation;
    };
} // namespace nero
#endif // POINTMESH_H
