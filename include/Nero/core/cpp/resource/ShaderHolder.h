////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SHADERHOLDER_H
#define SHADERHOLDER_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/resource/ResourceHolder.h>
// SFML
#include <SFML/Graphics/Shader.hpp>
// JSON
#include <json/json.hpp>
// STD
#include <memory>
#include <vector>
#include <map>
////////////////////////////////////////////////////////////
namespace nero
{
    class ShaderHolder : public ResourceHolder
    {
      public: // utility
        typedef std::shared_ptr<ShaderHolder> Ptr;
        using ResourceHolder::loadDirectory;
        using ResourceHolder::loadFile;

      public:
        ShaderHolder();
        void         load();
        virtual void clear() override;

      private:
        void addShader(std::string name, std::unique_ptr<sf::Shader> shader);

      private:
        std::map<std::string, std::unique_ptr<sf::Shader>> m_ShaderMap;
        std::vector<std::string>                           m_ShaderTable;
        nlohmann::json                                     m_Configuration;
        bool                                               m_ShaderAvailable;
    };
} // namespace nero
#endif // SHADERHOLDER_H
