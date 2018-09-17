////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2018 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef SHADERHOLDER_H
#define SHADERHOLDER_H
///////////////////////////HEADERS//////////////////////////
//SFML
#include <SFML/Graphics/Shader.hpp>
//JSON
#include <json/json.hpp>
//STD
#include <memory>
#include <vector>
#include <map>
////////////////////////////////////////////////////////////
namespace nero
{
    class ShaderHolder
    {
        public:
                        ShaderHolder();
            void        load();

        private:
            void        addShader(std::string name, std::unique_ptr<sf::Shader> shader);

        private:
            std::map<std::string, std::unique_ptr<sf::Shader>>	            m_ShaderMap;
            std::vector<std::string>                                        m_ShaderTable;
            nlohmann::json                                                  m_Configuration;
            bool                                                            m_ShaderAvailable;
    };
}


#endif // SHADERHOLDER_H
