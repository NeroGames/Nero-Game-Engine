////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef MESHEDOBJECT_H
#define MESHEDOBJECT_H
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/object/Object.h>
#include <Nero/core/cpp/model/PointMesh.h>
// SFML
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    class MeshObject : public Object
    {
      public:
        typedef std::shared_ptr<MeshObject> Ptr;
        static Ptr                          Cast(Object::Ptr object);

      public:
        MeshObject();
        virtual ~MeshObject() override;

        void                   setMesh(const PointMesh::Ptr& mesh);
        PointMesh::Ptr         getMesh();

        virtual sf::FloatRect  getGlobalBounds() const;

        void                   setMeshType(PointMesh::Type type);

        Object::Ptr            clone(sf::Vector2f& position) const;
        Object::Ptr            clone() const;
        virtual nlohmann::json toJson() const;

      private:
        void drawObject(sf::RenderTarget& target, sf::RenderStates states) const;

      private:
        PointMesh::Ptr m_Mesh;
    };
} // namespace nero
#endif // MESHEDOBJECT_H
