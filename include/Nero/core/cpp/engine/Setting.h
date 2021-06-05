////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SETTING_H
#define SETTING_H
///////////////////////////HEADERS///////////////////////////
//Json
#include <json/json.hpp>
//Cpp
#include <string>
#include <vector>
#include <memory>
/////////////////////////////////////////////////////////////

namespace nero
{
	class Setting
	{
		public:
			typedef std::shared_ptr<Setting>		Ptr;
			typedef std::shared_ptr<nlohmann::json> JSONPtr;

		public: //constructor
											Setting();

		public:
			//load setting
			void							loadSetting(const std::string& file, const bool& hasExtention = false);
			void							loadJson(const std::string& setting, const nlohmann::json& json);
			//check states
			const std::string&				getName()	const;
			bool							empty()		const;
			std::string						toString()	const;
			nlohmann::json					toJson()	const;

			//get
			Setting							getSetting(const std::string& setting)		const;
			unsigned int					getUInt(const std::string& setting)			const;
			int								getInt(const std::string& setting)			const;
			float							getFloat(const std::string& setting)		const;
			bool							getBool(const std::string& setting)			const;
			std::string						getString(const std::string& setting)		const;
			std::vector<std::string>		getStringTable(const std::string& setting)  const;
			//set
			void							setSetting(const std::string& name, const Setting& setting);
			void							setFloat(const std::string& setting, const float value);
			void							setInt(const std::string& setting, const int value);
			void							setUInt(const std::string& setting, const unsigned int value);
			void							setBool(const std::string& setting, const bool value);
			void							setString(const std::string& setting, const std::string& value);
			void							setStringTable(const std::string& setting, const std::vector<std::string>& value);

		private:
			nlohmann::json					getCurrentSetting() const;
			void							updateSetting(nlohmann::json local);

		private:
			//core setting holder
			JSONPtr							m_SettingHolder;
			//setting detph [setting_1][setting_2] ... [setting_n]
			std::vector<std::string>		m_SettingTable;
	};
}

#endif // SETTING_H
