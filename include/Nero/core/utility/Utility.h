#ifndef UTILITY_H
#define UTILITY_H

#include <Nero/core/utility/LogUtil.h>
#include <Nero/core/utility/FileUtil.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace nero
{
    void                centerOrigin(sf::Sprite& sprite);
    void                centerOrigin(sf::Text& text);

	bool CTRL();
	   bool SHIFT();
	   bool ALT();
	   bool CTRL_SHIFT();
	   bool CTRL_ALT();
	   bool SHIFT_ALT();
	   bool CTRL_SHIFT_ALT();
}

#endif // UTILITY_H
