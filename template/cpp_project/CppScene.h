////////////////////////////////////////////////////////////
// ::Project_Name::
// Copyright (c) ::Coypright_Date:: ::Project_Lead::
/////////////////////////////////////////////////////////////
#ifndef ::Header_Gard::
#define ::Header_Gard::
///////////////////////////HEADERS///////////////////////////
//NERO
#include <Nero/core/cpp/scene/GameScene.h>
//BOOST
#include <boost/dll/alias.hpp>
/////////////////////////////////////////////////////////////

namespace ::Namespace::
{
	class ::Scene_Class:: : public nero::GameScene
    {
		public: //Utility
			typedef std::shared_ptr<::Scene_Class::> Ptr;

        public: //Scene Factory
			static nero::GameScene::Ptr createScene(nero::GameScene::Context context) noexcept;

        public: //Scene core
										::Scene_Class::(nero::GameScene::Context context);
			virtual                     ~::Scene_Class::() override;

			virtual void				init();
			virtual void				destroy();
			virtual void                handleEvent(const sf::Event& event) override;
			virtual void                update(const sf::Time& timeStep) override;
			virtual void                render() override;
    };

	//Class export (DOT NOT MODIFY)
    BOOST_DLL_ALIAS(::Namespace::::::Scene_Class::::createScene, createScene)
}

#endif // ::Header_Gard::
