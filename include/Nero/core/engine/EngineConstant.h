////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ENGINECONSTANT_H
#define ENGINECONSTANT_H
///////////////////////////HEADERS//////////////////////////
//STD
#include <string>
//SFML
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    struct _EngineConstant
    {
        //engine version
		const int				ENGINE_VERSION_MAJOR	= 2;
		const int				ENGINE_VERSION_MINOR	= 0;
		const int				ENGINE_VERSION_PATCH	= 0;
        //engine window
		const std::string		ENGINE_WINDOW_TITLE		= "Nero Game Engine";
		const unsigned int		ENGINE_WINDOW_WIDTH		= 800;
		const unsigned int		ENGINE_WINDOW_HEIGHT	= 600;
        //time step
		const sf::Time			TIME_PER_FRAME			= sf::seconds(1.f/60.f);
		//canvas color
		const sf::Color			CANVAS_COLOR			= sf::Color::Black;
		//render window
    };

    const _EngineConstant EngineConstant;
}
#endif // ENGINECONSTANT_H
