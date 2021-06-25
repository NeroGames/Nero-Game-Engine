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
	class ::ClassName:: : public nero::GameLevel
    {
        public: //Utility
			typedef std::shared_ptr<::ClassName::> Ptr;

		public: //Factory
			static nero::GameLevel::Ptr create::ClassName::(nero::GameLevel::Context context) noexcept;

        public: //Scene core
										::ClassName::(nero::GameLevel::Context context);
			virtual                    ~::ClassName::() override;

			virtual	void				init();
			virtual void                handleEvent(const sf::Event& event) override;
			virtual void                update(const sf::Time& timeStep) override;
			virtual void                render() override;
    };


    //Class export
	BOOST_DLL_ALIAS(::Namespace::::::ClassName::::create::ClassName::, create::ClassName::)
}

#endif // ::HeaderGard::
