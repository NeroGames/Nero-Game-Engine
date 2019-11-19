////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SCRIPTHOLDER_H
#define SCRIPTHOLDER_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/resource/ResourceHolder.h>
//JSON
#include <json/json.hpp>
//STD
#include <vector>
#include <map>
////////////////////////////////////////////////////////////
namespace nero
{
	class ScriptHolder : public ResourceHolder
    {
		public: //utility
			typedef std::shared_ptr<ScriptHolder>    Ptr;
			using ResourceHolder::loadDirectory;

        public:
                        ScriptHolder();
            void        load();

        private:
            std::map<std::string, std::string>          m_ScriptMap;
            std::vector<std::string>                    m_ScriptTable;
            nlohmann::json                              m_Configuration;
    };
}
#endif // SCRIPTHOLDER_H
