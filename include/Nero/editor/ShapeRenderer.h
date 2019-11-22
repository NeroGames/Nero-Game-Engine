////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SHAPERENDERER_H
#define SHAPERENDERER_H
///////////////////////////HEADERS//////////////////////////
//SFGUI
#include <SFML/Graphics/RenderWindow.hpp>
//SFML
#include <SFML/Graphics/RenderTexture.hpp>
//Box2D
#include <Box2D/Common/b2Draw.h>
//STD
#include <memory>
//////////////////// FORWARD DECLARATION ////////////////////
struct b2AABB;
////////////////////////////////////////////////////////////
namespace nero
{
    ///\brief This class implements drawing callbacks that are invoked inside b2World::Step.
    class ShapeRenderer : public b2Draw
    {
		public:
			typedef std::shared_ptr<sf::RenderTexture> RenderTexturePtr;

        public :
									ShapeRenderer(const float& thickness = -2.f, const int& tranparency = 50);

            void                    DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
            void                    DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
            void                    DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
            void                    DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
            void                    DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
            void                    DrawTransform(const b2Transform& xf);
            void                    DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
            void                    DrawAABB(b2AABB* aabb, const b2Color& color);
			void					setRenderTexture(const RenderTexturePtr& renderTexture);

        private:
			RenderTexturePtr        m_RenderTexture;
            float                   m_Thickness;
            int                     m_Tranparency;
    };
}
#endif // SHAPERENDERER_H
