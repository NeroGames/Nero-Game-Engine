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
}

#endif // UTILITY_H
