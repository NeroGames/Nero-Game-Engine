////////////////////////////////////////////////////////////
// ::Project_Name::
// Copyright (c) ::Coypright_Date:: ::Project_Lead::
/////////////////////////////////////////////////////////////
#ifndef ::Header_Gard::
#define ::Header_Gard::
///////////////////////////HEADERS///////////////////////////
//NERO
#include <Nero/core/scene/Scene.h>
//BOOST
#include <boost/dll/alias.hpp>
/////////////////////////////////////////////////////////////

namespace ::Namespace::
{
    class ::Scene_Class:: : public nero::Scene
    {
        public: //Utility
            typedef std::shared_ptr<::Scene_Class::> Ptr;

        public: //Scene Factory
            static ::Scene_Class::::Ptr createScene(nero::Scene::Context context) noexcept;

        public: //Scene core
                                        ::Scene_Class::(nero::Scene::Context context);
           virtual                     ~::Scene_Class::() override;

           virtual void                 handleEvent(const sf::Event& event) override;
           virtual void                 update(const sf::Time& timeStep) override;
           virtual void                 render() override;
    };



    //Class export
    BOOST_DLL_ALIAS(::Namespace::::::Scene_Class::::createScene, createScene)
}

#endif // ::Header_Gard::
