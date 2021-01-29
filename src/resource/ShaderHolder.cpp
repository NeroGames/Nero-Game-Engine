////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/resource/ShaderHolder.h>
#include <Nero/utility/Utility.h>
//BOOST
#include <boost/filesystem.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    ShaderHolder::ShaderHolder()
    {
        m_Configuration = loadConfiguration(RESOURCE_CONFIGURATION)["shader"];
        m_ShaderAvailable = sf::Shader::isAvailable();
    }

    void ShaderHolder::load()
    {
        nero_log("///////////////////////// LOADING SHADER //////////////////////");

        if(!m_ShaderAvailable)
        {
            nero_log("The current computer or device does not support Shader");
            nero_log("////////////////////////////////////////////////////////////");
            return;
        }

        const std::string folder_name = m_Configuration["folder"].get<std::string>();

        nero_log("Shader resource path : " + folder_name);

        using namespace boost::filesystem;

        if(!exists(folder_name))
        {
            nero_log("failed to load texture resource");
            nero_log("folder not found : " + folder_name);
            return;
        }

        //load the shader json
        nlohmann::json shader_table = loadJson(m_Configuration["descriptor"].get<std::string>());

        shader_table = shader_table["shader_list"];

        for (auto& shader_json : shader_table)
        {
            sf::Shader* shader = new sf::Shader();

            //std::unique_ptr<sf::Shader> shader = make_unique<sf::Shader>();
            sf::String shaderName;

            if (shader_json.find("fragment") != shader_json.end() && shader_json.find("vertex") != shader_json.end())
            {
                std::string file_1 = folder_name + "/" +  shader_json["vertex"].get<std::string>();
                std::string file_2 = folder_name + "/" +  shader_json["fragment"].get<std::string>();

                if(!exists(file_1))
                {
                    nero_log("failed to load shader resource");
                    nero_log("file not found : " + file_1);
                    continue;
                }

                if(!exists(file_2))
                {
                    nero_log("failed to load shader resource");
                    nero_log("file not found : " + file_2);
                    continue;
                }

                if(!shader->loadFromFile(file_1, file_2))
                {
                    nero_log("failed to load shader : " + file_1 + " | " + file_2);
                }

                if(shader_json.find("name") != shader_json.end())
                   shaderName = shader_json["name"].get<std::string>();
                else
                    shaderName = removeFileExtension(file_1) + "_" + removeFileExtension(file_2);

            }
            else if (shader_json.find("fragment") != shader_json.end() && shader_json.find("vertex") == shader_json.end())
            {
                std::string file = folder_name + "/" +  shader_json["fragment"].get<std::string>();

                if(!exists(file))
                {
                    nero_log("failed to load shader resource");
                    nero_log("file not found : " + file);
                    continue;
                }

                if(!shader->loadFromFile(file, sf::Shader::Fragment))
                {
                    nero_log("failed to load shader : " + file);
                }

                if(shader_json.find("name") != shader_json.end())
                   shaderName = shader_json["name"].get<std::string>();
                else
                    shaderName = removeFileExtension(file);
            }
            else if (shader_json.find("fragment") == shader_json.end() && shader_json.find("vertex") != shader_json.end())
            {
                std::string file = folder_name + "/" +  shader_json["vertex"].get<std::string>();

                if(!exists(file))
                {
                    nero_log("failed to load shader resource");
                    nero_log("file not found : " + file);
                    continue;
                }

                if(!shader->loadFromFile(file, sf::Shader::Vertex))
                {
                    nero_log("failed to load shader : " + file);
                }

                if(shader_json.find("name") != shader_json.end())
                   shaderName = shader_json["name"].get<std::string>();
                else
                    shaderName = removeFileExtension(file);
            }

            addShader(shaderName, std::move(std::unique_ptr<sf::Shader>(shader)));
            m_ShaderTable.push_back(shaderName);
        }
    }

    void ShaderHolder::addShader(std::string shaderName, std::unique_ptr<sf::Shader> shader)
    {
        auto inserted = m_ShaderMap.insert(std::make_pair(shaderName, std::move(shader)));

        if(!inserted.second)
        {
            nero_log("failed to store shader " + shaderName);
        }
    }

}

