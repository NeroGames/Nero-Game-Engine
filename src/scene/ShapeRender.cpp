////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
// All rights reserved
//
////////////////////////////////////////////////////////////

#include <NERO/scene/ShapeRender.h>
#include <NERO/Utility.h>
//SFML
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
//Box2D
#include <Box2D/Collision/b2Collision.h>    //b2AABB

namespace nero
{
    ShapeRender::ShapeRender(sfg::Canvas::Ptr& renderCanvas, const float& thickness, const int& tranparency):
        m_RenderCanvas(renderCanvas),
        m_Thickness(thickness),
        m_Tranparency(tranparency)
    {
        //ctor
    }

    void ShapeRender::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
        //Convex shape
        sf::ConvexShape polygon;
            polygon.setOutlineThickness(m_Thickness);
            polygon.setOutlineColor(b2_to_sf(color));
            polygon.setFillColor(sf::Color::Transparent);

        polygon.setPointCount(vertexCount);
        for(int32 i = 0; i < vertexCount; i++)
        {
            polygon.setPoint(i, b2_to_sf(vertices[i], SCALE));
        }

        m_RenderCanvas->Draw(polygon);
    }

    void ShapeRender::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
        //Solid convex shape
        sf::ConvexShape solidPolygon;
            solidPolygon.setOutlineThickness(m_Thickness);
            solidPolygon.setOutlineColor(b2_to_sf(color));
            solidPolygon.setFillColor(b2_to_sf(color, m_Tranparency));

        solidPolygon.setPointCount(vertexCount);
        for(int32 i = 0; i < vertexCount; i++)
        {
            solidPolygon.setPoint(i, b2_to_sf(vertices[i], SCALE));
        }


        m_RenderCanvas->Draw(solidPolygon);
    }

    void ShapeRender::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
    {
        //Circle shape
        sf::CircleShape circle;
            circle.setOutlineThickness(m_Thickness);
            circle.setOutlineColor(b2_to_sf(color));
            circle.setFillColor(sf::Color::Transparent);

            float rad = radius * SCALE;
            circle.setPosition(b2_to_sf(center, SCALE));
            circle.setRadius(rad);
            circle.setOrigin(sf::Vector2f(rad, rad));

        m_RenderCanvas->Draw(circle);
    }

    void ShapeRender::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
    {
        //Solid circle shape
        sf::CircleShape solidCircle;
            solidCircle.setOutlineThickness(m_Thickness);
            solidCircle.setOutlineColor(b2_to_sf(color));
            solidCircle.setFillColor(b2_to_sf(color, m_Tranparency));

            float rad = radius * SCALE;
            solidCircle.setPosition(b2_to_sf(center, SCALE));
            solidCircle.setRadius(rad);
            solidCircle.setOrigin(sf::Vector2f(rad, rad));

        m_RenderCanvas->Draw(solidCircle);

        b2Vec2 p = center + radius * axis;
        DrawSegment(center, p, color);
    }

    void ShapeRender::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
    {
        sf::RectangleShape line;
        float length = distance(b2_to_sf(p1, SCALE), b2_to_sf(p2, SCALE));
        line.setSize(sf::Vector2f(length, m_Thickness));
        line.setPosition(b2_to_sf(p1, SCALE));
        line.setFillColor(b2_to_sf(color));
        line.setOrigin(sf::Vector2f(line.getOrigin().x, m_Thickness/2.f));


        float delta_x = p2.x - p1.x;
        float delta_y = p2.y - p1.y;
        float angle = atan2(delta_y, delta_x);
        line.setRotation(toDegree(angle));

        m_RenderCanvas->Draw(line);
    }


    void ShapeRender::DrawTransform(const b2Transform& xf)
    {
        const float32 k_axisScale = 0.4f;

        b2Vec2 p1, p2;
        p1 = xf.p;
        p2 = p1 + k_axisScale * xf.q.GetXAxis();
        DrawSegment(p1, p2, b2Color(1.0f, 0.0f, 0.0f));

        p2 = p1 + k_axisScale * xf.q.GetYAxis();
        DrawSegment(p1, p2, b2Color(0.0f, 1.0f, 0.0f));
    }

    void ShapeRender::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
    {
        float s = size*1.5f;
        sf::RectangleShape point;
        point.setFillColor(b2_to_sf(color));
        point.setPosition(b2_to_sf(p, SCALE));
        point.setSize(sf::Vector2f(s, s));
        point.setOrigin(sf::Vector2f(s/2, s/2));

        m_RenderCanvas->Draw(point);
    }

    void ShapeRender::DrawAABB(b2AABB* aabb, const b2Color& c)
    {
        sf::ConvexShape aabb_shape;
        aabb_shape.setOutlineColor(b2_to_sf(c));
        aabb_shape.setPointCount(4);

        aabb_shape.setPoint(0, b2_to_sf(aabb->lowerBound, SCALE));
        aabb_shape.setPoint(1, sf::Vector2f(aabb->upperBound.x * SCALE, aabb->lowerBound.y * SCALE));
        aabb_shape.setPoint(2, b2_to_sf(aabb->upperBound, SCALE));
        aabb_shape.setPoint(3, sf::Vector2f(aabb->lowerBound.x * SCALE, aabb->upperBound.y * SCALE));

        m_RenderCanvas->Draw(aabb_shape);
    }
}
