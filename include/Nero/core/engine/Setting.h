////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SETTING_H
#define SETTING_H
///////////////////////////HEADERS///////////////////////////
//JSON
#include <json/json.hpp>
//STD
#include <string>
/////////////////////////////////////////////////////////////

namespace nero
{
	class Setting
	{
		public: //constructor
									Setting();

		public:
			void					setDirectory(const std::string& directory);
			void					loadSetting(const std::string& setting);
			void					load();

			unsigned int			getUInt(const std::string& setting);
			int						getInt(const std::string& setting);
			float					getFloat(const std::string& setting);
			bool					getBool(const std::string& setting);
			std::string				getString(const std::string& setting);
			Setting					getSetting(const std::string& setting);

			std::string				toString() const;

			void					setString(const std::string& setting, const std::string& value);
			void					setFloat(const std::string& setting, const float value);
			void					setInt(const std::string& setting, const int value);
			void					setBool(const std::string& setting, const int value);

		private:
			nlohmann::json			m_Setting;
			std::string				m_Directory;
	};
}

#endif // SETTING_H
