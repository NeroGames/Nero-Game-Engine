////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef MATH_H
#define MATH_H
///////////////////////////HEADERS//////////////////////////
// SFML
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
// Box2d
#include <Box2D/Common/b2Math.h>
// Cpp
#include <vector>
////////////////////////////////////////////////////////////
namespace nero
{
    namespace math
    {
        // constant
        const int   MAX_VALUE  = 2147483647;
        const int   RAND_LIMIT = 32767;
        const float PI         = 3.141592653589793238462643383f;
        // point
        struct Point
        {
            float x;
            float y;
        };
        // typedef
        typedef std::vector<sf::Vector2f> VectorTable;
        typedef std::vector<Point>        PointTable;

        // random
        int                               randomInt(int exclusiveMax);
        float                             randomFloat();                   // Random number in range [-1,1]
        float                             randomFloat(float lo, float hi); // Random floating point number in range [lo, hi]

        // angle
        float                             toDegree(float radian);
        float                             toRadian(float degree);

        // geometry
        // vector
        float                             vectorLength(sf::Vector2f vector);
        float                             vectorLength(b2Vec2 vector);
        sf::Vector2f                      unitVector(sf::Vector2f vector);
        float                             dotProduct(const sf::Vector2f& vector1, const sf::Vector2f& vector2);
        float                             distance(sf::Vector2f vector1, sf::Vector2f vector2);
        float                             distance(b2Vec2 vector1, b2Vec2 vector2);
        float                             distance(sf::Vector2f lineVector1, sf::Vector2f lineVector2, sf::Vector2f vector3);
        // polygon
        bool                              isPolygonConvex(std::vector<Point> polygon);
        bool                              isPolygonConvex(std::vector<sf::RectangleShape> polygon);
        std::vector<VectorTable>          concaveToConvex(VectorTable vectorTable);
        int                               validatePolygon(const VectorTable& vectorTable);
        float                             det(float x1, float y1, float x2, float y2, float x3, float y3);
        sf::Vector2f*                     hitRay(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
        bool                              isOnSegment(float px, float py, float x1, float y1, float x2, float y2);
        bool                              isOnLine(float px, float py, float x1, float y1, float x2, float y2);
        bool                              pointsMatch(float x1, float y1, float x2, float y2);
        sf::Vector2f*                     hitSegment(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
        sf::Vector2f                      getPolygonCenter(VectorTable vertices);
        sf::Vector2f                      getLineCenter(const sf::Vector2f& a, const sf::Vector2f& b);
        sf::Vector2f                      rotateVertex(sf::Vector2f c, float angle, sf::Vector2f p);
    } // namespace math
} // namespace nero
#endif // MATH_H
