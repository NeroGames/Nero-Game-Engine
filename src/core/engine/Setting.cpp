////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/engine/Setting.h>
#include <Nero/core/utility/StringUtil.h>
#include <Nero/core/utility/FileUtil.h>
/////////////////////////////////////////////////////////////
namespace nero
{
	Setting::Setting():
		 m_Setting(nlohmann::json::object())
		,m_Directory(StringPool.BLANK)
	{

	}

	void Setting::setDirectory(const std::string& directory)
	{
		m_Directory = directory;
	}

	void Setting::loadSetting(const std::string& setting)
	{
		std::string file = StringPool.BLANK;

		if(m_Directory != StringPool.BLANK)
		{
			file = getPath({m_Directory, setting + StringPool.SUFFIX_SETTING});
		}
		else
		{
			file = getPath({setting  + StringPool.SUFFIX_SETTING});
		}

		auto loaded = loadJson(file);
		m_Setting[setting] = loaded;
	}

	void Setting::load()
	{
		//TODO
	}

	std::string Setting::toString() const
	{
		return m_Setting.dump(3);
	}

	unsigned int Setting::getUInt(const std::string& setting)
	{
		return m_Setting[setting].get<unsigned int>();
	}

	int Setting::getInt(const std::string& setting)
	{
		return m_Setting[setting].get<int>();
	}

	float Setting::getFloat(const std::string& setting)
	{
		return m_Setting[setting].get<float>();
	}

	std::string Setting::getString(const std::string& setting)
	{
		return m_Setting[setting].get<std::string>();
	}

	Setting Setting::getSetting(const std::string& setting)
	{
		Setting subSetting;

		subSetting.m_Setting = m_Setting[setting];

		return subSetting;
	}

	bool Setting::getBool(const std::string& setting)
	{
		return m_Setting[setting].get<bool>();
	}

	void Setting::setString(const std::string& setting, const std::string& value)
	{
		m_Setting[setting] = value;
	}

	void Setting::setFloat(const std::string& setting, const float value)
	{
		m_Setting[setting] = value;
	}
	void Setting::setInt(const std::string& setting, const int value)
	{
		m_Setting[setting] = value;
	}

	void Setting::setBool(const std::string& setting, const int value)
	{
		m_Setting[setting] = value;
	}
}

