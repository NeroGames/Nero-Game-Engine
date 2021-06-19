////////////////////////////////////////////////////////////
// ::ProjectName::
// Copyright (c) ::CoyprightDate:: ::ProjectLead::
/////////////////////////////////////////////////////////////
#ifndef ::HeaderGard::
#define ::HeaderGard::
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/engine/StartupScreen.h>
////////////////////////////////////////////////////////////
namespace ::Namespace::
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

#endif // ::HeaderGard::
