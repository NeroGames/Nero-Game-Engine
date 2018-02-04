#ifndef SHAPERENDER_H
#define SHAPERENDER_H

//SFGUI
#include <SFGUI/Canvas.hpp>
//Box2D
#include <Box2D/Common/b2Draw.h>
//Forward Declaration
struct b2AABB;

namespace nero
{
   class ShapeRender : public b2Draw
    {
        // This class implements drawing callbacks that are invoked inside b2World::Step.
        public :
            //Constructor
                    ShapeRender(sfg::Canvas::Ptr& renderCanvas, const float& thickness = -2.f, const int& tranparency = 50);

            //Drawing foncitons
            void    DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

            void    DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

            void    DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

            void    DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

            void    DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

            void    DrawTransform(const b2Transform& xf);

            void    DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

            void    DrawAABB(b2AABB* aabb, const b2Color& color);

        private:
            sfg::Canvas::Ptr&       m_RenderCanvas;

            float                   m_Thickness;
            int                     m_Tranparency;

    };
}


#endif // SHAPERENDER_H
