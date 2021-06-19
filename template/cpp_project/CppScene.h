////////////////////////////////////////////////////////////
// ::ProjectName::
// Copyright (c) ::CoyprightDate:: ::ProjectLead::
/////////////////////////////////////////////////////////////
#ifndef ::HeaderGard::
#define ::HeaderGard::
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/scene/GameScene.h>
//Boost
#include <boost/dll/alias.hpp>
/////////////////////////////////////////////////////////////

namespace ::Namespace::
{
	class ::SceneClass:: : public nero::GameScene
    {
		public: //Utility
			typedef std::shared_ptr<::SceneClass::> Ptr;

        public: //Scene Factory
			static nero::GameScene::Ptr createScene(nero::GameScene::Context context) noexcept;

        public: //Scene core
										::Scene_Class::(nero::GameScene::Context context);
			virtual                     ~::Scene_Class::() override;

			virtual void				init()								override;
			virtual void				destroy()							override;
			virtual void                handleEvent(const sf::Event& event) override;
			virtual void                update(const sf::Time& timeStep)	override;
			virtual void                render()							override;
    };

	//Class export (DOT NOT MODIFY)
    BOOST_DLL_ALIAS(::Namespace::::::Scene_Class::::createScene, createScene)
}

#endif // ::Header_Gard::
