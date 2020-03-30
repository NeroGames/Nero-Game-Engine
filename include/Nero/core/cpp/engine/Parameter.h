////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef PARAMETER_H
#define PARAMETER_H
///////////////////////////HEADERS///////////////////////////
//Json
#include <json/json.hpp>
//Cpp
#include <string>
#include <vector>
/////////////////////////////////////////////////////////////
namespace nero
{
	class Parameter
	{
		public:
			typedef std::shared_ptr<Parameter>		Ptr;

		public:
											Parameter();

			//load
			void							loadJson(const nlohmann::json& json, const bool& override = false);
			//check states
			bool							empty()		const;
			std::string						toString()	const;
			nlohmann::json					toJson()	const;

			//get
			unsigned int					getUInt(const std::string& setting)			const;
			int								getInt(const std::string& setting)			const;
			float							getFloat(const std::string& setting)		const;
			bool							getBool(const std::string& setting)			const;
			std::string						getString(const std::string& setting)		const;
			std::vector<std::string>		getStringTable(const std::string& setting)  const;
			//set
			void							setFloat(const std::string& setting, const float value);
			void							setInt(const std::string& setting, const int value);
			void							setUInt(const std::string& setting, const unsigned int value);
			void							setBool(const std::string& setting, const bool value);
			void							setString(const std::string& setting, const std::string& value);
			void							setStringTable(const std::string& setting, const std::vector<std::string>& value);

		private:
			//parameter holder
			nlohmann::json					m_ParameterHolder;
	};
}

#endif // PARAMETER_H
