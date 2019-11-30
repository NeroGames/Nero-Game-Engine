////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/engine/Setting.h>
#include <Nero/core/cpp/utility/StringUtil.h>
#include <Nero/core/cpp/utility/FileUtil.h>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/algorithm/string.hpp>
/////////////////////////////////////////////////////////////
namespace nero
{
	Setting::Setting():
		 m_CurrentSetting()
		,m_Directory(StringPool.BLANK)
	{
		m_SettingHolder = std::make_shared<nlohmann::json>(nlohmann::json::object());
	}

	void Setting::setDirectory(const std::string& directory)
	{
		m_Directory = directory;
	}

	void Setting::loadJson(const nlohmann::json& setting)
	{
		nlohmann::json& holder = (*m_SettingHolder);
		holder = setting;
	}

	void Setting::loadSetting(const std::string& setting, const bool& path_has_extention)
	{
		std::string file = StringPool.BLANK;

		if(m_Directory != StringPool.BLANK)
		{
			file = getPath({m_Directory, setting});
		}
		else
		{
			file = getPath({setting});
		}

		auto loaded = nero::loadJson(file, path_has_extention);

		(*m_SettingHolder)[setting] = loaded;
	}

	void Setting::load()
	{
		//TODO
	}

	nlohmann::json Setting::getCurrentSetting() const
	{
		nlohmann::json currentSetting = (*m_SettingHolder);

		for(const std::string& setting : m_CurrentSetting)
		{
			currentSetting = currentSetting[setting];
		}

		return  currentSetting;
	}

	void Setting::updateSetting(nlohmann::json local)
	{
		std::string root = "{::value::}";

		for(const std::string& s : m_CurrentSetting)
		{
			std::string current = "\"" + s + "\"" + ":" + "{::value::}";

			boost::algorithm::replace_all(root, "::value::", current);
		}

		boost::algorithm::replace_all(root, "{::value::}", local.dump());


		m_SettingHolder->merge_patch(nlohmann::json::parse(root));
	}

	std::string Setting::toString() const
	{

		return getCurrentSetting().dump(3);
	}

	unsigned int Setting::getUInt(const std::string& setting) const
	{
		return getCurrentSetting()[setting].get<unsigned int>();
	}

	int Setting::getInt(const std::string& setting) const
	{
		return getCurrentSetting()[setting].get<int>();
	}

	float Setting::getFloat(const std::string& setting) const
	{
		return getCurrentSetting()[setting].get<float>();
	}

	std::string Setting::getString(const std::string& setting) const
	{
		return getCurrentSetting()[setting].get<std::string>();
	}

	Setting Setting::getSetting(const std::string& setting) const
	{
		Setting subSetting;

		subSetting.m_SettingHolder	= m_SettingHolder;
		subSetting.m_CurrentSetting = m_CurrentSetting;
		subSetting.m_CurrentSetting.push_back(setting);

		return subSetting;
	}

	bool Setting::getBool(const std::string& setting) const
	{
		return getCurrentSetting()[setting].get<bool>();
	}

	void Setting::setString(const std::string& setting, const std::string& value)
	{
		nlohmann::json local = nlohmann::json::object();
		local[setting] = value;

		updateSetting(local);
	}

	void Setting::setFloat(const std::string& setting, const float value)
	{
		nlohmann::json local = nlohmann::json::object();
		local[setting] = value;

		updateSetting(local);
	}
	void Setting::setInt(const std::string& setting, const int value)
	{
		nlohmann::json local = nlohmann::json::object();
		local[setting] = value;

		updateSetting(local);
	}

	void Setting::setBool(const std::string& setting, const bool value)
	{
		nlohmann::json local = nlohmann::json::object();
		local[setting] = value;

		updateSetting(local);
	}

	void Setting::setUInt(const std::string& setting, const unsigned int value)
	{
		nlohmann::json local = nlohmann::json::object();
		local[setting] = value;

		updateSetting(local);
	}

	std::vector<std::string> Setting::getStringTable(const std::string& setting)  const
	{
		return getCurrentSetting()[setting];
	}

	void Setting::setSetting(const std::string& name, const Setting& setting)
	{
		nlohmann::json local = nlohmann::json::object();
		local[name] = *(setting.m_SettingHolder);

		updateSetting(local);
	}

	nlohmann::json Setting::toJson() const
	{
		return getCurrentSetting();
	}

}

