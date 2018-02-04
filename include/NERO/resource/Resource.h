////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
////////////////////////////////////////////////////////////
#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED

///////////////////////////HEADERS//////////////////////////
//NERO
#include <NERO/resource/ResourceHolder.h>
#include <NERO/resource/TextureHolder.h>
//SFML
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shader.hpp>
////////////////////////////////////////////////////////////

namespace nero
{
    typedef ResourceHolder<sf::Font>    FontHolder;
    typedef ResourceHolder<sf::Sound>   SoundHolder;
    typedef ResourceHolder<sf::Music>   MusicHolder;
    typedef ResourceHolder<sf::Shader>  ShaderHolder;

    struct ResourceManager
    {
        TextureHolder   Texture;
        FontHolder      Font;
        SoundHolder     Sound;
        MusicHolder     Music;
        ShaderHolder    Shader;
    };
}

#endif // RESOURCE_H_INCLUDED
