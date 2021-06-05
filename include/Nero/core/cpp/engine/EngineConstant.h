////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ENGINECONSTANT_H
#define ENGINECONSTANT_H
///////////////////////////HEADERS//////////////////////////
//STD
#include <string>
//SFML
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <Nero/core/cpp/utility/String.h>
////////////////////////////////////////////////////////////
namespace nero
{
	const struct
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
		const sf::Color			COLOR_CANVAS			= sf::Color::Black;
		//document type
		const std::string		DOCUMENT_TYPE_WORKSPACE	= "nero_game_workspace";
		const std::string		DOCUMENT_TYPE_PROJECT	= "nero_game_project";
		//
		const std::string		NO_GAME_FOUND			= "No Game Found";


		const unsigned int		NUMBER_ZERO				= 0;
		const std::string		DLL_CREATE_STARTUP_SCREEN	 = "create_startup_screen";
		const std::string		DLL_CREATE_SCENE			 = "create_scene";

		const std::string		DEFAULT_TASK_CATEGORY	= "Nero";

		const float SCALE                           = 50.f;
		const float GRAVITY                         = 9.8f;
		const int SPRITE_LAYER_MAX                  = 1000;

		const sf::Color COLOR_STATIC_MESH		= sf::Color(0.5f * 255.f, 0.9f * 255.f, 0.5f * 255.f);
		const sf::Color COLOR_DYNAMIC_MESH		= sf::Color(0.9f * 255.f, 0.7f * 255.f, 0.7f * 255.f);
		const sf::Color COLOR_KINEMATIC_MESH	= sf::Color(0.5f * 255.f, 0.5f * 255.f, 0.9f * 255.f);
		const sf::Color COLOR_SELECTED_MESH		= sf::Color(255, 140, 0);
		const sf::Color COLOR_INVALIDE_MESH		= sf::Color(255, 0, 0);
	} EngineConstant;
}
#endif // ENGINECONSTANT_H
