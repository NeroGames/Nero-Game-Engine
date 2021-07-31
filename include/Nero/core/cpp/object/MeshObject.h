////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef MESHEDOBJECT_H
#define MESHEDOBJECT_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/cpp/object/Object.h>
#include <Nero/core/cpp/model/Mesh.h>
//SFML
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    class MeshObject : public Object
    {
        public:
            typedef std::shared_ptr<MeshObject>     Ptr;
            static Ptr                              Cast(Object::Ptr object);


        public:
                                        MeshObject();
            virtual                    ~MeshObject();

            void                        setMesh(const Mesh& mesh);
            Mesh*                       getMesh();

            virtual sf::FloatRect       getGlobalBounds()               const;

            void                        setParentLastPosition(const sf::Vector2f& position);
            void                        setParentLastScale(const sf::Vector2f& factor);
            void                        setParentLastRotation(const float& angle);

            void                        setMeshType(Mesh::Type type);

            void                        setMeshFixedRotation(bool flag);
            void                        setMeshSensor(bool flag);
            void                        setMeshAllowSleep(bool flag);

            void                        setMeshDensity(const float& density);
            void                        setMeshFriction(const float& friction);
            void                        setMeshRestitution(const float& restitution);
            void                        setMeshGravityScale(const float& gravityScale);

            bool                        getMeshFixedRotation()          const;
            bool                        getMeshSensor()                 const;
            bool                        getMeshAllowSleep()             const;

            float                       getMeshDensity()                const;
            float                       getMeshFriction()               const;
            float                       getMeshRestitution()            const;
            float                       getMeshGravityScale()           const;

            Object::Ptr                 clone(sf::Vector2f& position)   const;
            Object::Ptr                 clone()                         const;
            virtual nlohmann::json      toJson()                        const;

        private:
            void	                    drawObject(sf::RenderTarget& target, sf::RenderStates states) const;
            void                        updateObject(sf::Time time_step);

        private:
			mutable Mesh                m_Mesh;
            sf::Vector2f                m_ParentLastPosition;
            float                       m_ParentLastRotation;
			sf::Vector2f                m_ParentLastScale;
	};
}
#endif // MESHEDOBJECT_H
