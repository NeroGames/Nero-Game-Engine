#ifndef UTILITY_H
#define UTILITY_H

#include <Nero/core/cpp/utility/LogUtil.h>
#include <Nero/core/cpp/utility/FileUtil.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Time.hpp>
//Box2D
#include <Box2D/Common/b2Math.h>
#include <Box2D/Common/b2Draw.h>
//
#include <queue>

namespace nero
{

	sf::Color b2_to_sf(const b2Color& color, int transparency = 255);

	const sf::Time  TIME_PER_FRAME              = sf::seconds(1.f/60.f);
	const sf::Time  AUTO_SAVE_TIME              = sf::seconds(30.f);
	const float SCALE                           = 50.f;
	const float GRAVITY                         = 9.8f;
	const int RAND_LIMIT	                    = 32767;
	const int SPRITE_LAYER_MAX                  = 1000;
	const float PI                              = 3.141592653589793238462643383f;
	const int MAX_VALUE                         = 2147483647;
	const sf::Color STATIC_MESH_COLOR           = b2_to_sf(b2Color(0.5f, 0.9f, 0.5f)); // sf::Color(0, 255, 0);
	const sf::Color DYNAMIC_MESH_COLOR          = b2_to_sf(b2Color(0.9f, 0.7f, 0.7f)); //sf::Color(219, 112, 147);
	const sf::Color KINEMATIC_MESH_COLOR        = b2_to_sf(b2Color(0.5f, 0.5f, 0.9f)); //sf::Color(0, 0, 255);
	const sf::Color SELECTED_MESH_COLOR         = sf::Color(255, 140, 0);
	const sf::Color INVALIDE_MESH_COLOR         = sf::Color(255, 0, 0);

    void                centerOrigin(sf::Sprite& sprite);
    void                centerOrigin(sf::Text& text);

	bool CTRL();
   bool SHIFT();
   bool ALT();
   bool CTRL_SHIFT();
   bool CTRL_ALT();
   bool SHIFT_ALT();
   bool CTRL_SHIFT_ALT();

  sf::Vector2f        b2_to_sf(b2Vec2 vect, float scale = 1.f);
  sf::Vector2f        b2_to_sf(float x, float y, float scale = 1.f);
  b2Color             sf_to_b2(const sf::Color& color);
  b2Vec2              sf_to_b2(const sf::Vector2f& vect, float scale = 1.f);
  b2Vec2              sf_to_b2(float x, float y, float scale = 1.f);

  sf::Color           transparent_color(sf::Color color, int transparency = 255);

  typedef std::vector<sf::Vector2f>               PointTab;
  struct Point
	  {
		  float x;
		  float y;
	  };
	  bool                isConvexPolygon(std::vector<Point> polygon);
	  bool                isConvex(std::vector<sf::RectangleShape> polygon);

  std::vector<PointTab> concave_to_convex(PointTab vectorTab);
	  float det(float x1, float y1, float x2, float y2, float x3, float y3);
	  sf::Vector2f* hitRay(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
	  bool isOnSegment(float px, float py, float x1, float y1, float x2, float y2);
	  bool isOnLine(float px, float py, float x1, float y1, float x2, float y2);
	  void err();
	  bool pointsMatch(float x1, float y1, float x2, float y2);
	  int validate_polygon(const PointTab &verticesVec);
	  sf::Vector2f* hitSegment(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

	  sf::Vector2f getPolygonCenter(PointTab vertices);
	  sf::Vector2f getLineCenter(const sf::Vector2f& a, const sf::Vector2f& b);
	  sf::Vector2f rotateVertex(sf::Vector2f c, float angle, sf::Vector2f p);

	  ///Color to JSON
		  nlohmann::json toJson(sf::Color color);

		  ///Vector to JSON
		  template<typename T>
		  nlohmann::json toJson(T point)
		  {
			  return  {{"x", point.x}, {"y", point.y}};
		  }

		  ///Color from JSON
		  sf::Color colorFromJson(nlohmann::json json);

		  ///Vector from JSON
		  template<typename T>
		  T vectorFromJson(nlohmann::json json)
		  {
			 T point;

			  point.x = json["x"];
			  point.y = json["y"];

			  return point;
		  }

		  float               toDegree(float radian);
			float               toRadian(float degree);

			float               vectLength(sf::Vector2f vect);
				float               vectLength(b2Vec2 vect);
				sf::Vector2f        unitVector(sf::Vector2f vect);
				float               distance(sf::Vector2f vect1, sf::Vector2f vect2);
				float               distance(b2Vec2 vect1, b2Vec2 vect2);
				float               distance(sf::Vector2f line_vect1, sf::Vector2f line_vect2, sf::Vector2f vect3);
				float               dot_product(const sf::Vector2f& vect1, const sf::Vector2f& vect2);


}

#endif // UTILITY_H
