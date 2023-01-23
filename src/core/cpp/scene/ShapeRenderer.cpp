////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/scene/ShapeRenderer.h>
#include <Nero/core/cpp/utility/Utility.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
// SFML
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
// Box2D
#include <Box2D/Collision/b2Collision.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ShapeRenderer::ShapeRenderer(RenderTexturePtr renderTexture,
                                 const float&     thickness,
                                 const int&       transparency)
        : m_RenderTexture(renderTexture)
        , m_Thickness(thickness)
        , m_Transparency(transparency)
    {
        // Empty
    }

    void ShapeRenderer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
        // Convex shape
        sf::ConvexShape polygon;
        polygon.setOutlineThickness(m_Thickness);
        polygon.setOutlineColor(graphics::b2_to_sf(color));
        polygon.setFillColor(sf::Color::Transparent);

        polygon.setPointCount(vertexCount);
        for(int32 i = 0; i < vertexCount; i++)
        {
            polygon.setPoint(i, graphics::b2_to_sf(vertices[i], EngineConstant.SCALE));
        }

        m_RenderTexture->draw(polygon);
    }

    void ShapeRenderer::DrawSolidPolygon(const b2Vec2*  vertices,
                                         int32          vertexCount,
                                         const b2Color& color)
    {
        // Solid convex shape
        sf::ConvexShape solidPolygon;
        solidPolygon.setOutlineThickness(m_Thickness);
        solidPolygon.setOutlineColor(graphics::b2_to_sf(color));
        solidPolygon.setFillColor(graphics::b2_to_sf(color, m_Transparency));

        solidPolygon.setPointCount(vertexCount);
        for(int32 i = 0; i < vertexCount; i++)
        {
            solidPolygon.setPoint(i, graphics::b2_to_sf(vertices[i], EngineConstant.SCALE));
        }

        m_RenderTexture->draw(solidPolygon);
    }

    void ShapeRenderer::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
    {
        // Circle shape
        sf::CircleShape circle;
        circle.setOutlineThickness(m_Thickness);
        circle.setOutlineColor(graphics::b2_to_sf(color));
        circle.setFillColor(sf::Color::Transparent);

        float rad = radius * EngineConstant.SCALE;
        circle.setPosition(graphics::b2_to_sf(center, EngineConstant.SCALE));
        circle.setRadius(rad);
        circle.setOrigin(sf::Vector2f(rad, rad));

        m_RenderTexture->draw(circle);
    }

    void ShapeRenderer::DrawSolidCircle(const b2Vec2&  center,
                                        float32        radius,
                                        const b2Vec2&  axis,
                                        const b2Color& color)
    {
        // Solid circle shape
        sf::CircleShape solidCircle;
        solidCircle.setOutlineThickness(m_Thickness);
        solidCircle.setOutlineColor(graphics::b2_to_sf(color));
        solidCircle.setFillColor(graphics::b2_to_sf(color, m_Transparency));

        float rad = radius * EngineConstant.SCALE;
        solidCircle.setPosition(graphics::b2_to_sf(center, EngineConstant.SCALE));
        solidCircle.setRadius(rad);
        solidCircle.setOrigin(sf::Vector2f(rad, rad));

        m_RenderTexture->draw(solidCircle);

        b2Vec2 p = center + radius * axis;
        DrawSegment(center, p, color);
    }

    void ShapeRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
    {
        sf::RectangleShape line;
        float              length = math::distance(graphics::b2_to_sf(p1, EngineConstant.SCALE),
                                      graphics::b2_to_sf(p2, EngineConstant.SCALE));
        line.setSize(sf::Vector2f(length, m_Thickness));
        line.setPosition(graphics::b2_to_sf(p1, EngineConstant.SCALE));
        line.setFillColor(graphics::b2_to_sf(color));
        line.setOrigin(sf::Vector2f(line.getOrigin().x, m_Thickness / 2.f));

        float delta_x = p2.x - p1.x;
        float delta_y = p2.y - p1.y;
        float angle   = atan2(delta_y, delta_x);
        line.setRotation(math::toDegree(angle));

        m_RenderTexture->draw(line);
    }

    void ShapeRenderer::DrawTransform(const b2Transform& xf)
    {
        const float32 k_axisScale = 0.4f;

        b2Vec2        p1, p2;
        p1 = xf.p;
        p2 = p1 + k_axisScale * xf.q.GetXAxis();
        DrawSegment(p1, p2, b2Color(1.0f, 0.0f, 0.0f));

        p2 = p1 + k_axisScale * xf.q.GetYAxis();
        DrawSegment(p1, p2, b2Color(0.0f, 1.0f, 0.0f));
    }

    void ShapeRenderer::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
    {
        float              s = size * 1.5f;
        sf::RectangleShape point;
        point.setFillColor(graphics::b2_to_sf(color));
        point.setPosition(graphics::b2_to_sf(p, EngineConstant.SCALE));
        point.setSize(sf::Vector2f(s, s));
        point.setOrigin(sf::Vector2f(s / 2, s / 2));

        m_RenderTexture->draw(point);
    }

    void ShapeRenderer::DrawAABB(b2AABB* aabb, const b2Color& c)
    {
        sf::ConvexShape aabb_shape;
        aabb_shape.setOutlineColor(graphics::b2_to_sf(c));
        aabb_shape.setPointCount(4);

        aabb_shape.setPoint(0, graphics::b2_to_sf(aabb->lowerBound, EngineConstant.SCALE));
        aabb_shape.setPoint(1,
                            sf::Vector2f(aabb->upperBound.x * EngineConstant.SCALE,
                                         aabb->lowerBound.y * EngineConstant.SCALE));
        aabb_shape.setPoint(2, graphics::b2_to_sf(aabb->upperBound, EngineConstant.SCALE));
        aabb_shape.setPoint(3,
                            sf::Vector2f(aabb->lowerBound.x * EngineConstant.SCALE,
                                         aabb->upperBound.y * EngineConstant.SCALE));

        m_RenderTexture->draw(aabb_shape);
    }

    void ShapeRenderer::setRenderTexture(const RenderTexturePtr& renderTexture)
    {
        m_RenderTexture = renderTexture;
    }
} // namespace nero
