////////////////////////////////////////////////////////////
// ::ProjectName::
// Copyright (c) ::CoyprightDate:: ::ProjectLead::
/////////////////////////////////////////////////////////////
#ifndef ::HeaderGard::
#define ::HeaderGard::
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/scene/GameLevel.h>
//Boost
#include <boost/dll/alias.hpp>
/////////////////////////////////////////////////////////////
namespace ::Namespace::
{
	class ::GameLevelClass:: : public nero::GameLevel
    {
        public: //Utility
			typedef std::shared_ptr<::GameLevelClass::> Ptr;

		public: //Factory
			static nero::GameLevel::Ptr create::GameLevelClass::(nero::GameLevel::Context context) noexcept;

        public: //Scene core
										::GameLevelClass::(nero::GameLevel::Context context);
			virtual                    ~::GameLevelClass::() override;

			virtual	void				init()								override;
			virtual void                handleEvent(const sf::Event& event) override;
			virtual void                update(const sf::Time& timeStep)	override;
			virtual void                render()							override;
    };


    //Class export
	BOOST_DLL_ALIAS(::Namespace::::::GameLevelClass::::create::GameLevelClass::, create::GameLevelClass::)
}

#endif // ::HeaderGard::
