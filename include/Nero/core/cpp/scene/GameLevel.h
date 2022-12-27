////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GAMELEVEL_H
#define GAMELEVEL_H
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/resource/ResourceManager.h>
#include <Nero/core/cpp/utility/Keyboard.h>
#include <Nero/core/cpp/model/Collision.h>
#include <Nero/core/cpp/scene/PhysicalWorld.h>
#include <Nero/core/cpp/scene/LightEngine.h>
//SFML
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Window/Event.hpp>
//STD
#include <memory>
/////////////////////////////////////////////////////////////
namespace nero
{
	class GameLevel
	{
        public:
            //utility
			typedef std::shared_ptr<GameLevel> Ptr;

			class Context
			{

			};

		public:
			//ctr & dtr
														GameLevel(GameLevel::Context context);
			virtual					   				   ~GameLevel();
			//main
			virtual void								init();
			virtual void								destroy();
			virtual void								handleEvent(const sf::Event& event);
			virtual void								update(const sf::Time& timeStep);
			virtual void								render();
			virtual void								renderShape();
			//builder
			void										loadLevel();
			void										loadChunk();
			void										loadLayer();
			void										loadObject();
			//input
				//window
			virtual void								onWindowClosed();
			virtual void								onWindowResized(const sf::Vector2u& size);
			virtual void								onWindowFocusChanged(const bool& gainedFocus);
				//keyboard
			virtual void								onKeyboardButton(const sf::Keyboard::Key& key, const bool& isPressed, const ModifierKey& modifier);
			virtual void								onTextEntered(const std::string& c);
				//mouse
			virtual void								onMouseMoved(const sf::Vector2f& position);
			virtual void								onMouseButton(const sf::Mouse::Button& button, const bool& isPressed, const sf::Vector2f& position);
			virtual void								onMouseWheel(const sf::Mouse::Wheel& wheel, const float& delta, const sf::Vector2f& position);
			virtual void								onMouseWindowSurface(const bool& mouseEntered);
				//joystick
			virtual void								onJoystickConnection(const unsigned int& joystickId, const bool& connected);
			virtual void								onJoystickButton(const unsigned int& joystickId, const unsigned int& button, const bool& isPressed);
			virtual void								onJoystickAxis(const unsigned int& joystickId, const sf::Joystick::Axis& axis, const float& position);
			//collision
			virtual void								onCollisionContactBegin(Collision collision);
			virtual void								onCollisionContactEnd(Collision collision);
			virtual void								onCollisionPreSolveContact(Collision collision);
			virtual void								onCollisionPostSolveContact(Collision collision);

		private:
			Setting::Ptr								m_LevelSetting;
			std::shared_ptr<sf::RenderTexture>			m_RenderTexture;
			ResourceManager								m_ResourceManager;
			PhysicalWorld::Ptr							m_PhysicalWorld;
			LightEngine::Ptr							m_LightEngine;
	};
}



#endif // GAMELEVEL_H
