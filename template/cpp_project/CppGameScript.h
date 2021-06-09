////////////////////////////////////////////////////////////
// ::Project_Name::
// Copyright (c) ::Coypright_Date:: ::Project_Lead::
/////////////////////////////////////////////////////////////
#ifndef ::Header_Gard::
#define ::Header_Gard::
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/scene/GameScript.h>
//Boost
#include <boost/dll/alias.hpp>
/////////////////////////////////////////////////////////////
namespace ::Namespace::
{
    class ::Class_Name:: : public nero::GameScript
    {
        public: //Utility
            typedef std::shared_ptr<::Class_Name::> Ptr;

        public: //Factory
                static nero::GameScript::Ptr create::Class_Name::(nero::GameScript::Context context) noexcept;

        public: //Scene core
                                        ::Class_Name::(nero::ScriptObject::Context context);
            virtual                    ~::Class_Name::() override;

            virtual void                init();
            virtual void                handleEvent(const sf::Event& event) override;
            virtual void                update(const sf::Time& timeStep) override;
            virtual void                render() override;
    };


    //Class export
    BOOST_DLL_ALIAS(::Namespace::::::Scene_Class::::create::Class_Name::, create::Class_Name::)
}

#endif // ::Header_Gard::
