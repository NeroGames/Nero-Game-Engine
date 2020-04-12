////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/renderer/NoGameFound.h>
/////////////////////////////////////////////////////////////
namespace nero
{
	NoGameFound::NoGameFound(Scene::Context context): Scene(context)
	{

	}

	NoGameFound::~NoGameFound()
	{

	}

	void NoGameFound::handleEvent(const sf::Event& event)
	{
		switch(event.type)
		{
			case sf::Event::Closed:
				quitScene();
				break;

			case sf::Event::MouseButtonPressed:
				handleMouseButtonInput(event.mouseButton, true);
				break;
			case sf::Event::MouseButtonReleased:
				handleMouseButtonInput(event.mouseButton, false);
				break;
		}
	}

	void NoGameFound::update(const sf::Time& timeStep)
	{

	}

	void NoGameFound::render()
	{

	}
}

