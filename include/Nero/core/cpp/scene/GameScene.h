////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GAMESCENE_H
#define GAMESCENE_H
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/camera/Camera.h>
#include <Nero/core/cpp/engine/Setting.h>
//SFML
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
//STD
#include <memory>
#include <vector>
/////////////////////////////////////////////////////////////
namespace nero
{
	class GameScene
	{
		public:
			//type definiton
			typedef std::shared_ptr	<GameScene> Ptr;

			//utility
			enum class EngineType
			{
				RENDER,
				EDITOR,
				PREVIEW
			};

			enum class PlatformType
			{
				WINDOWS,
				LINUX,
				MACOS,
				ANDROID,
				IOS
			};

			//scene context
			class Context
			{
				public:
					Context(std::string sceneName,
							std::shared_ptr<sf::RenderTexture> renderTexture,
							Camera::Ptr camera,
							Setting::Ptr setting,
							EngineType engineType,
							PlatformType platformType);

					Context();

				private:
					friend class						GameScene;
					friend class						GameProject;
					std::shared_ptr<sf::RenderTexture>	renderTexture;
					Camera::Ptr							camera;
					Setting::Ptr						setting;
					EngineType							engineType;
					PlatformType						platformType;
					std::string							sceneName;
			};

		public:
			//ctr & dtr
											GameScene(Context context);
			virtual                        ~GameScene();
			//main method
			virtual void					init();
			virtual void                    destroy();
			virtual void					handleEvent(const sf::Event& event);
			virtual void					update(const sf::Time& timeStep);
			virtual void					render();
			virtual void					renderShape();
			virtual void					renderFrontScreen();
	};
}

#endif // GAMESCENE_H
