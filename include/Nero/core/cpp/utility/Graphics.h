////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef GRAPHICS_H
#define GRAPHICS_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/cpp/utility/Logging.h>
#include <Nero/core/cpp/utility/File.h>
//SFML
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Time.hpp>
//Box2D
#include <Box2D/Common/b2Math.h>
#include <Box2D/Common/b2Draw.h>
//Cpp
#include <queue>
////////////////////////////////////////////////////////////
namespace nero
{
	namespace graphics
	{
		b2Vec2									sf_to_b2(const sf::Vector2f& vect, float scale = 1.f);
		b2Vec2									sf_to_b2(float x, float y, float scale = 1.f);
		b2Color									sf_to_b2(const sf::Color& color);
		sf::Vector2f							b2_to_sf(b2Vec2 vect, float scale = 1.f);
		sf::Vector2f							b2_to_sf(float x, float y, float scale = 1.f);
		sf::Color								b2_to_sf(const b2Color& color, int transparency = 255);

		void									centerOrigin(sf::Sprite& sprite);
		void									centerOrigin(sf::Text& text);
		sf::Color								getTransparentColor(const sf::Color& color, int transparency = 255);
		nlohmann::json							colorToJson(const sf::Color& color);
		sf::Color								colorFromJson(nlohmann::json json);
		template<typename T>
		nlohmann::json							vectorToJson(T point);
		template<typename T>
		nlohmann::json							vectorFromJson(T point);		

		//template functions
		template<typename T>
		nlohmann::json vectorToJson(T point)
		{
			return  {{"x", point.x}, {"y", point.y}};
		}

		template<typename T>
		T vectorFromJson(nlohmann::json json)
		{
			T point;
			point.x = json["x"];
			point.y = json["y"];

			return point;
		}
	}
}
#endif // GRAPHICS_H
