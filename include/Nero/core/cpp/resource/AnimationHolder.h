////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef ANIMATIONHOLDER_H
#define ANIMATIONHOLDER_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/cpp/resource/ResourceHolder.h>
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
	class AnimationHolder : public ResourceHolder
    {
		public: //utility
			typedef std::shared_ptr<AnimationHolder>    Ptr;
			using	ResourceHolder::loadDirectory;
			using	ResourceHolder::loadFile;

        public:
                                                AnimationHolder();
												AnimationHolder(const Setting& setting);
			virtual							   ~AnimationHolder()					override;

			virtual void						loadDirectory()						override;
			virtual void						loadFile(const std::string& file)	override;
			virtual void						destroy()							override;
			virtual void						clear() override;


            sf::Texture&				        getTexture(std::string animationName);
            const sf::Texture&			        getTexture(std::string animationName) const;
            sf::IntRect                         getAnimationBound(std::string animationName) const;
            std::string                         getDefaultSequence(std::string animationName) const;
            const std::vector<std::string>&     getAnimationTable() const;

            std::map<std::string, std::vector<sf::IntRect>>            getSequenceMap(std::string animationName) const;



        private:
            void        addTexture(std::string name, std::unique_ptr<sf::Texture> texture);


        private:
            std::map<std::string, std::unique_ptr<sf::Texture>>	                        m_TextureMap;
            std::map<std::string, std::map<std::string, std::vector<sf::IntRect>>>      m_SequenceMap;
            std::vector<std::string>                                                    m_AnimationTable;
    };

}
#endif // ANIMATIONHOLDER_H
