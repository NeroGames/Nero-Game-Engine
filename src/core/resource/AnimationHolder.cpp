////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/resource/AnimationHolder.h>
#include <Nero/core/utility/Utility.h>
#include <experimental/filesystem>
////////////////////////////////////////////////////////////
namespace nero
{
    AnimationHolder::AnimationHolder()
    {

    }

	AnimationHolder::AnimationHolder(const Setting& setting) : ResourceHolder (setting)
	{

	}

	AnimationHolder::~AnimationHolder()
	{
		destroy();
	}

	void AnimationHolder::destroy()
	{

	}

	void AnimationHolder::loadFile(const std::string& file)
	{
		std::experimental::filesystem::path filePath(file);

		//Load the Texture
		std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();

		const std::string textureName = filePath.filename().stem().string();

		if (!texture->loadFromFile(filePath.string()))
		{
			nero_log("failed to load texture : " + textureName);

			return;
		}

		//Get the JSON helper file
		std::string jsonHelper  = replaceFileExtension(filePath.string(), "json");
		if (!fileExist(jsonHelper))
		{
			nero_log("failed to load animation resource");
			nero_log("file not found : " + jsonHelper);
			return;
		}

		nlohmann::json helper = loadJson(jsonHelper, true);

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


	void AnimationHolder::loadDirectory()
    {
		if(m_SelectedDirectory == StringPool.BLANK)
		{
			nero_log("failed to load directory");
			return;
		}

		nero_log("Resource path : " + m_SelectedDirectory);

		std::experimental::filesystem::path folderPath(m_SelectedDirectory);

		if(!directoryExist(m_SelectedDirectory))
        {
            nero_log("failed to load animation resource");
			nero_log("folder not found : " + m_SelectedDirectory);
            return;
        }

		std::experimental::filesystem::directory_iterator it{folderPath};
		while(it != std::experimental::filesystem::directory_iterator{})
		{
			nero_log(it->path().string());

			if(checkExtention(it->path().extension().string(), m_Setting.getStringTable("extension")))
            {
				loadFile(it->path().string());
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

        return found->second.find(getDefaultSequence(animationName))->second[0];
    }

    std::map<std::string, std::vector<sf::IntRect>> AnimationHolder::getSequenceMap(std::string animationName) const
    {
        auto found = m_SequenceMap.find(animationName);

        return found->second;
    }

    std::string AnimationHolder::getDefaultSequence(std::string animationName) const
    {
        auto foundAnimation = m_SequenceMap.find(animationName);
        auto foundSequence =  foundAnimation->second.find("idle");

        if(foundSequence != foundAnimation->second.end())
        {
            return "idle";
        }
        else
        {
            return foundAnimation->second.begin()->first;
        }
    }
}
