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
#include <Nero/core/utility/StringUtil.h>
////////////////////////////////////////////////////////////
namespace nero
{
    struct _EngineConstant
    {
        //engine version
		const int				ENGINE_VERSION_MAJOR	= 2;
		const int				ENGINE_VERSION_MINOR	= 0;
		const int				ENGINE_VERSION_PATCH	= 0;
		const std::string		ENGINE_VERSION			= toString(ENGINE_VERSION_MAJOR) + "." + toString(ENGINE_VERSION_MINOR)  + "." + toString(ENGINE_VERSION_PATCH);
        //engine window
		const std::string		ENGINE_WINDOW_TITLE		= "Nero Game Engine";
		const unsigned int		ENGINE_WINDOW_WIDTH		= 800;
		const unsigned int		ENGINE_WINDOW_HEIGHT	= 600;
        //time step
		const sf::Time			TIME_PER_FRAME			= sf::seconds(1.f/60.f);
		//canvas color
		const sf::Color			CANVAS_COLOR			= sf::Color::Black;
		//document type
		const std::string		DOCUMENT_TYPE_WORKSPACE	= "nero_game_workspace";
		const std::string		DOCUMENT_TYPE_PROJECT	= "nero_game_project";

    };

    const _EngineConstant EngineConstant;
}
#endif // ENGINECONSTANT_H
