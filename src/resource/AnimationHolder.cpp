////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2018 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/resource/AnimationHolder.h>
#include <Nero/utility/Utility.h>
//BOOST
#include <boost/filesystem.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    AnimationHolder::AnimationHolder()
    {
        m_Configuration = loadConfiguration(RESOURCE_CONFIGURATION)["animation"];
    }

    void AnimationHolder::load()
    {
        nero_log("///////////////////////// LOADING ANIMATION //////////////////////");

        const std::string folder_name = m_Configuration["folder"].get<std::string>();

        nero_log("Folder : " + folder_name);
        nero_log("");

        using namespace boost::filesystem;
        path folder_path(folder_name);

        if(!exists(folder_name))
        {
            nero_log("failed to load animation resource");
            nero_log("folder not found : " + folder_name);
            return;
        }

        directory_iterator it{folder_path};
        while(it != directory_iterator{})
        {
            if(checkExtention(it->path().extension().string(), m_Configuration["extension"]))
            {
                //Load the Texture
                std::unique_ptr<sf::Texture> texture = make_unique<sf::Texture>();

                const std::string textureName = it->path().filename().stem().string();

                if (!texture->loadFromFile(it->path().string()))
                {
                    nero_log("failed to load texture : " + textureName);
                    it++;
                    continue;
                }

                //Get the JSON helper file
                std::string jsonHelper  = replaceFileExtension(it->path().string(), "json");
                if (!exists(jsonHelper))
                {
                    nero_log("failed to load animation resource");
                    nero_log("file not found : " + jsonHelper);
                    it++;
                    continue;
                }

                nlohmann::json helper = loadJson(jsonHelper);

                if (helper.find("type") == helper.end()) //Texture packer animation
                {
                    nlohmann::json frame_table = helper["frames"];

                    for (auto& frame : frame_table)
                    {
                        std::string frameName                   = removeFileExtension(frame["filename"].get<std::string>());
                        std::vector<std::string> splitResult    = splitString(frameName, '/');

                        std::string sequenceName    = splitResult[0];
                        frameName                   = splitResult[1];

                        int rectLeft                = frame["frame"]["x"];
                        int rectTop                 = frame["frame"]["y"];
                        int rectWidth               = frame["frame"]["w"];
                        int rectHeight              = frame["frame"]["h"];

                        sf::IntRect frameBound = sf::IntRect(rectLeft, rectTop, rectWidth, rectHeight);

                        if(m_SequenceMap.find(textureName) == m_SequenceMap.end())
                        {
                            m_SequenceMap[textureName][sequenceName] = std::vector<sf::IntRect>();
                        }

                        //nero_log("loaded : " + textureName + "-" + sequenceName + " " + _s(frameBound.width) + " " + _s(frameBound.height));
                        m_SequenceMap[textureName][sequenceName].push_back(frameBound);
                    }
                }

                m_AnimationTable.push_back(textureName);
                addTexture(textureName, std::move(texture));
            }

            it++;
        }

    }

    void AnimationHolder::addTexture(std::string textureName, std::unique_ptr<sf::Texture> texture)
    {
        auto inserted = m_TextureMap.insert(std::make_pair(textureName, std::move(texture)));

        if(!inserted.second)
        {
            nero_log("failed to store texture " + textureName);
            return;
        }
    }

    const std::vector<std::string>& AnimationHolder::getAnimationTable() const
    {
        return m_AnimationTable;
    }

    sf::Texture& AnimationHolder::getTexture(std::string animationName)
    {
        auto found = m_TextureMap.find(animationName);
        assert(found != m_TextureMap.end());

        return *found->second;
    }

    const sf::Texture& AnimationHolder::getTexture(std::string animationName) const
    {
        auto found = m_TextureMap.find(animationName);
        assert(found != m_TextureMap.end());

        return *found->second;
    }

    sf::IntRect AnimationHolder::getAnimationBound(std::string animationName) const
    {
        auto found = m_SequenceMap.find(animationName);

        return found->second.find(animationName)->second[0];
    }

    std::map<std::string, std::vector<sf::IntRect>> AnimationHolder::getSequenceMap(std::string animationName) const
    {
        auto found = m_SequenceMap.find(animationName);

        return found->second;
    }

}
