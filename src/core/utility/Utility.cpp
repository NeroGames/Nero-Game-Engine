#include <Nero/core/utility/Utility.h>

#include <SFML/Window/Keyboard.hpp>

namespace nero
{
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

	bool CTRL()
		{
			return  sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) &&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool SHIFT()
		{
			return  !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) &&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool ALT()
		{
			return  !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) &&
					sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool CTRL_SHIFT()
		{
			return  sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) &&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool CTRL_ALT()
		{
			return  sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) &&
					sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool SHIFT_ALT()
		{
			return  !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) &&
					sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool CTRL_SHIFT_ALT()
		{
			return  sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}
}
