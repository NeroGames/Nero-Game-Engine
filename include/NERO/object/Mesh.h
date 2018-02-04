////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
// All rights reserved
//
////////////////////////////////////////////////////////////

#ifndef MESH_H
#define MESH_H

///////////////////////////HEADERS//////////////////////////
//NERO
#include <NERO/Utility.h>
//SFML
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
//JSON
#include <json/json.hpp>
////////////////////////////////////////////////////////////

namespace nero
{
    struct Mesh : public sf::Transformable, public sf::Drawable
    {
        public: //Utility
            std::vector<sf::String> String_Type{"static_mesh", "dynamic_mesh", "kinematic_mesh"};
            std::vector<sf::String> String_Shape{"polygon_mesh", "circle_mesh", "chain_mesh", "line_mesh", "none"};
            enum Type {Static_Mesh, Dynamic_Mesh, Kinematic_Mesh};
            enum Shape {Polygon_Mesh, Circle_Mesh, Chain_Mesh, Line_Mesh, None};

            typedef std::vector<sf::RectangleShape>     VertexTab;
            typedef std::vector<sf::RectangleShape>     LineTab;
            typedef std::vector<sf::ConvexShape>        PolygonTab;
            typedef std::vector<Mesh>                   Tab;

            float                                       Vertex_Size             = 8.f;
            int                                         Color_Alpha             = 50.f;
            float                                       Min_Vertex_Distance     = 1.f;

        public: //Method
                                        Mesh(Shape shape = None, sf::Vector2f position = sf::Vector2f(0.f, 0.f), float size = 100.f, Type type = Static_Mesh);

            void                        updateShape();
            void                        updateCircleShape();
            void                        updateLineShape();
            void                        updateLineShapeLoop();
            void                        updatePolygonShape();
            void                        updateColor();


            void                        addVertex(sf::Vector2f point, const int& index);
            void                        addVertex(sf::Vector2f point);
            void                        deleteVertex(const int& index);

            void                        move(const sf::Vector2f& offset);
            void                        rotate(const float& angle);
            void                        scale(const sf::Vector2f& factor);

            float                       getRadius()             const;
            sf::Vector2f                getPosition()           const;
            float                       getRotation()           const;
            int                         getPointCount()         const;
            PointTab                    getAllVertexPoint()     const;
            sf::FloatRect               getGlobalBounds()       const;

            nlohmann::json              toJson()                const;
            sf::String                  toSting()               const;


            //
            int                         getId()                 const;
            Type                        getType()               const;
            Shape                       getShape()              const;
            bool                        isValid()               const;

            bool                        getFixedRotation()      const;
            bool                        getIsSensor()           const;
            bool                        getAllowSleep()         const;
            float                       getDensity()            const;
            float                       getFriction()           const;
            float                       getRestitution()        const;
            float                       getGravityScale()       const;


            void                        setId(int id);
            void                        setType(Type type);
            void                        setShape(Shape shape);
            void                        setIsValid(bool);

            void                        setFixedRotation(bool flag);
            void                        setIsSensor(bool flag);
            void                        setAllowSleep(bool flag);
            void                        setDensity(float density);
            void                        setFriction(float friction);
            void                        setRestitution(float restitution);
            void                        setGravityScale(float gravity_scale);

            VertexTab*                  getAllVertex();


        private: //Method
            void                        updateLine(bool loop);
            void                        setColor(sf::Color color);
            void                        validate(const sf::Vector2f& point1, const sf::Vector2f& point2);

            sf::RectangleShape          createVertex(const sf::Vector2f& point, const sf::Color& color);
            sf::RectangleShape          createLine(const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Color& color);
            sf::ConvexShape             createRegularPolygon(const sf::Vector2f& position, const sf::Color& color, const int& pointCount);
            sf::ConvexShape             createPolygon(const PointTab& pointTab, const sf::Color& color);

            virtual void			    draw(sf::RenderTarget& target, sf::RenderStates states) const;

        private: //Attribute
            friend class                MeshEditor;
            //
            int                         m_Id;
            Type                        m_Type;
            Shape                       m_Shape;
            bool                        m_IsValid;
            //
            sf::Vector2f                m_Position;
            sf::Color                   m_Color;
            float                       m_Size;
            //
            VertexTab                   m_VertexTab;
            LineTab                     m_LineTab;
            PolygonTab                  m_PolygonTab;
            sf::CircleShape             m_CircleShape;
            //
            bool                        m_FixedRotation;
            bool                        m_IsSensor;
            bool                        m_AllowSleep;
            float                       m_Density;
            float                       m_Friction;
            float                       m_Restitution;
            float                       m_GravityScale;
    };

}
#endif // MESH_H
