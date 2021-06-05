////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/cpp/utility/Graphics.h>
////////////////////////////////////////////////////////////
namespace nero
{
	namespace graphics
	{
		b2Color sf_to_b2(const sf::Color& color)
		{
			return b2Color(color.r / 255, color.g / 255, color.b / 255);
		}

		b2Vec2 sf_to_b2(const sf::Vector2f& vect, float scale)
		{
			return b2Vec2(vect.x / scale, vect.y / scale);
		}

		b2Vec2 sf_to_b2(float x, float y, float scale)
		{
			return b2Vec2(x / scale, y / scale);
		}

		sf::Color b2_to_sf(const b2Color& color, int transparency)
		{
			return sf::Color(color.r * 255, color.g * 255, color.b * 255, transparency);
		}

		sf::Vector2f b2_to_sf(b2Vec2 vect, float scale)
		{
			return sf::Vector2f(vect.x * scale, vect.y * scale);
		}

		sf::Vector2f b2_to_sf(float x, float y, float scale)
		{
			return sf::Vector2f(x * scale, y * scale);
		}

		void centerOrigin(sf::Sprite& sprite)
		{
			sf::FloatRect bounds = sprite.getLocalBounds();
			sprite.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
		}

		void centerOrigin(sf::Text& text)
		{
			sf::FloatRect bounds = text.getLocalBounds();
			text.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
		}

		sf::Color getTransparentColor(const sf::Color& color, int transparency)
		{
			return sf::Color(color.r, color.g, color.b, transparency);
		}

		nlohmann::json colorToJson(const sf::Color& color)
		{
			return  {{"r", color.r}, {"g", color.g}, {"b", color.b}, {"a", color.a}};
		}

		sf::Color colorFromJson(nlohmann::json json)
		{
			return sf::Color(json["r"], json["g"], json["b"], json["a"]);
		}
	}
}
