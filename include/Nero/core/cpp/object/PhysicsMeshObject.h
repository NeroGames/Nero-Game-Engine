////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef PHYSICALMESHOBJECT_H
#define PHYSICALMESHOBJECT_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/object/Object.h>
#include <Nero/core/cpp/model/PointMesh.h>
#include <Nero/core/cpp/scene/PhysicsUtility.h>
// SFML
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    class PhysicsMeshObject : public Object
    {
      public:
        using Ptr = std::shared_ptr<PhysicsMeshObject>;
        static Ptr Cast(Object::Ptr object);

      public:
        PhysicsMeshObject();
        virtual ~PhysicsMeshObject() override;

        void                   setMesh(const PointMesh::Ptr& pointMesh);
        PointMesh::Ptr         getMesh();

        virtual sf::FloatRect  getGlobalBounds() const override;

        virtual Object::Ptr    clone(sf::Vector2f& position) const override;
        virtual Object::Ptr    clone() const override;
        virtual nlohmann::json toJson() const override;

        void                   setProperty(const std::string& name, const bool value);
        void                   setProperty(const std::string& name, const float& value);
        bool                   getBoolProperty(const std::string& name);
        float                  getFloatProperty(const std::string& name);
        const PhysicsPoperty   getPhysicsPoperty() const;

      private:
        void drawObject(sf::RenderTarget& target, sf::RenderStates states) const override;
        void updateObject(sf::Time timeStep) override;

      private:
        PointMesh::Ptr m_PointMesh;
        PhysicsPoperty m_PhysicsPoperty;
    };
} // namespace nero
#endif // PHYSICALMESHOBJECT_H
