////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2018 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ANIMATIONHOLDER_H
#define ANIMATIONHOLDER_H
///////////////////////////HEADERS//////////////////////////
//SFML
#include <SFML/Graphics/Texture.hpp>
//JSON
#include <json/json.hpp>
//STD
#include <memory>
#include <vector>
#include <map>
////////////////////////////////////////////////////////////
namespace nero
{
    class AnimationHolder
    {
        public:
                                                AnimationHolder();
            void                                load();

            sf::Texture&				        getTexture(std::string animationName);
            const sf::Texture&			        getTexture(std::string animationName) const;
            sf::IntRect                         getAnimationBound(std::string animationName) const;
            const std::vector<std::string>&     getAnimationTable() const;

            std::map<std::string, std::vector<sf::IntRect>>            getSequenceMap(std::string animationName) const;



        private:
            void        addTexture(std::string name, std::unique_ptr<sf::Texture> texture);


        private:
            std::map<std::string, std::unique_ptr<sf::Texture>>	                        m_TextureMap;
            std::map<std::string, std::map<std::string, std::vector<sf::IntRect>>>      m_SequenceMap;
            std::vector<std::string>                                                    m_AnimationTable;
            nlohmann::json                                                              m_Configuration;
    };

}

#endif // ANIMATIONHOLDER_H
