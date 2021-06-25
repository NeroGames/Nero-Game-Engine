////////////////////////////////////////////////////////////
// ::ProjectName::
// Copyright (c) ::CoyprightDate:: ::ProjectLead::
/////////////////////////////////////////////////////////////
#ifndef ::HeaderGard::
#define ::HeaderGard::
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/engine/StartupScreen.h>
////////////////////////////////////////////////////////////
namespace ::Namespace::
{
	class ::StartupScreenClass:: : public nero::StartupScreen
    {
		public:
			typedef std::unique_ptr<::StartupScreenClass::> Ptr;

        public:
											::StartupScreenClass::();

			virtual void                    init()								override;
			virtual void                    handleEvent(sf::Event& event)		override;
			virtual void                    update(const sf::Time& timeStep)	override;
			virtual void                    render()							override;
			virtual const sf::Color         getCanvasColor()	const			override;
			virtual float					getDuration()		const			override;
    };
}
#endif // ::HeaderGard::
