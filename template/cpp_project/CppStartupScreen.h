////////////////////////////////////////////////////////////
// Project ::ProjectName::
// Copyright (c) ::Date:: ::ProjectLead::
////////////////////////////////////////////////////////////
#ifndef ::HeaderGard::_H
#define ::HeaderGard::_H
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/engine/StartupScreen.h>
////////////////////////////////////////////////////////////
namespace ::NameSpace::
{
	class ::StartupScreenClass:: : public nero::StartupScreen
    {
        public:
									::StartupScreenClass::();

			void                    init();

            void                    handleEvent(sf::Event& event);
            void                    update(const sf::Time& timeStep);
            void                    render();
            const sf::Color         getBackgroundColor()    const;
			const float             getDuration()           const;
    };
}

#endif // :HeaderGard:_H
