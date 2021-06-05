////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//SFML
#include <SFML/Window/Keyboard.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
	namespace keyboard
	{
		bool CTRL()
		{
			return   sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)	&&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool SHIFT()
		{
			return  !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					 sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)	&&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool ALT()
		{
			return  !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)	&&
					 sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool CTRL_SHIFT()
		{
			return   sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					 sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)	&&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool CTRL_ALT()
		{
			return   sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)	&&
					 sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool SHIFT_ALT()
		{
			return  !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					 sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)	&&
					 sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool CTRL_SHIFT_ALT()
		{
			return  sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)	||
					sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)	||
					sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}
	}
}
