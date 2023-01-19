////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/engine/Parameter.h>
#include <Nero/core/cpp/utility/Logging.h>
/////////////////////////////////////////////////////////////
namespace nero
{
    Parameter::Parameter()
        : m_ParameterHolder(nlohmann::json::object())
    {
        // empty
    }

    void Parameter::loadJson(const nlohmann::json& json, const bool& override)
    {
        if(!json.is_null() && json.is_object())
        {
            if(override)
            {
                m_ParameterHolder = json;
            }
            else
            {
                m_ParameterHolder.merge_patch(json);
            }
        }
    }

    bool Parameter::empty() const
    {
        return m_ParameterHolder.empty();
    }

    std::string Parameter::toString() const
    {
        return m_ParameterHolder.dump(3);
    }

    nlohmann::json Parameter::toJson() const
    {
        return m_ParameterHolder;
    }

    unsigned int Parameter::getUInt(const std::string& setting) const
    {
        return m_ParameterHolder[setting].get<unsigned int>();
    }

    int Parameter::getInt(const std::string& setting) const
    {
        return m_ParameterHolder[setting].get<int>();
    }

    float Parameter::getFloat(const std::string& setting) const
    {
        return m_ParameterHolder[setting].get<float>();
    }

    std::string Parameter::getString(const std::string& setting) const
    {
        return m_ParameterHolder[setting].get<std::string>();
    }

    std::vector<std::string> Parameter::getStringTable(const std::string& setting) const
    {
        return m_ParameterHolder[setting];
    }

    bool Parameter::getBool(const std::string& setting) const
    {
        return m_ParameterHolder[setting].get<bool>();
    }

    void Parameter::setString(const std::string& setting, const std::string& value)
    {
        m_ParameterHolder[setting] = value;
    }

    void Parameter::setFloat(const std::string& setting, const float value)
    {
        m_ParameterHolder[setting] = value;
    }

    void Parameter::setInt(const std::string& setting, const int value)
    {
        m_ParameterHolder[setting] = value;
    }

    void Parameter::setBool(const std::string& setting, const bool value)
    {
        m_ParameterHolder[setting] = value;
    }

    void Parameter::setUInt(const std::string& setting, const unsigned int value)
    {
        m_ParameterHolder[setting] = value;
    }

    void Parameter::setStringTable(const std::string&              setting,
                                   const std::vector<std::string>& value)
    {
        m_ParameterHolder[setting] = value;
    }
} // namespace nero
