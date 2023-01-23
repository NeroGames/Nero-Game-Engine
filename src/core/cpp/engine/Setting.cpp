////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/engine/Setting.h>
#include <Nero/core/cpp/utility/String.h>
#include <Nero/core/cpp/utility/File.h>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/algorithm/string.hpp>
/////////////////////////////////////////////////////////////
namespace nero
{
    Setting::Setting()
        : m_SettingHolder(std::make_shared<nlohmann::json>(nlohmann::json::object()))
        , m_SettingTable()
    {
        // empty
    }

    // load setting from file
    void Setting::loadSetting(const std::string& file, const bool& hasExtention, const bool& root)
    {
        // can load a json object or a json array
        std::string    filePath = file::getPath({file});
        std::string    setting  = file::getFileName(filePath);
        nlohmann::json loaded   = file::loadJson(filePath, hasExtention);

        if(!loaded.is_null())
        {
            if(root)
            {
                m_SettingHolder.reset(new nlohmann::json(loaded));
            }
            else
            {
                (*m_SettingHolder)[setting] = loaded;
            }
        }
    }

    // load setting from json
    void Setting::loadJson(const std::string& setting, const nlohmann::json& json)
    {
        // can load a json object or a json array
        if(!json.is_null())
        {
            (*m_SettingHolder)[setting] = json;
        }
    }

    void Setting::loadJson(const nlohmann::json& json)
    {
        // can load a json object or a json array
        if(!json.is_null())
        {
            m_SettingHolder.reset(new nlohmann::json(json));
        }
    }

    Setting Setting::getSetting(const std::string& setting) const
    {
        // a child setting is only determine by the content of m_SettingTable;
        // each a child setting is retrieve, only is setting table differ from other settings
        Setting childSetting;

        childSetting.m_SettingHolder = m_SettingHolder;
        childSetting.m_SettingTable  = m_SettingTable;
        childSetting.m_SettingTable.push_back(setting);

        return childSetting;
    }

    nlohmann::json Setting::getCurrentSetting() const
    {
        nlohmann::json currentSetting = (*m_SettingHolder);

        for(const std::string& setting : m_SettingTable)
        {
            currentSetting = currentSetting[setting];
        }

        return currentSetting;
    }

    void Setting::updateSetting(nlohmann::json local)
    {
        std::string root = "{::value::}";

        for(const std::string& s : m_SettingTable)
        {
            std::string current = "\"" + s + "\"" + ":" + "{::value::}";

            boost::algorithm::replace_all(root, "::value::", current);
        }

        boost::algorithm::replace_all(root, "{::value::}", local.dump());

        m_SettingHolder->merge_patch(nlohmann::json::parse(root));
    }

    const std::string& Setting::getName() const
    {
        if(m_SettingTable.empty())
        {
            return "root";
        }
        else
        {
            return m_SettingTable.back();
        }
    }

    bool Setting::empty() const
    {
        return getCurrentSetting().empty();
    }

    std::string Setting::toString() const
    {
        return getCurrentSetting().dump(3);
    }

    nlohmann::json Setting::toJson() const
    {
        return getCurrentSetting();
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

    std::vector<std::string> Setting::getStringTable(const std::string& setting) const
    {
        return getCurrentSetting()[setting];
    }

    bool Setting::getBool(const std::string& setting) const
    {
        return getCurrentSetting()[setting].get<bool>();
    }

    void Setting::setString(const std::string& setting, const std::string& value)
    {
        nlohmann::json local = nlohmann::json::object();
        local[setting]       = value;

        updateSetting(local);
    }

    void Setting::setFloat(const std::string& setting, const float value)
    {
        nlohmann::json local = nlohmann::json::object();
        local[setting]       = value;

        updateSetting(local);
    }
    void Setting::setInt(const std::string& setting, const int value)
    {
        nlohmann::json local = nlohmann::json::object();
        local[setting]       = value;

        updateSetting(local);
    }

    void Setting::setBool(const std::string& setting, const bool value)
    {
        nlohmann::json local = nlohmann::json::object();
        local[setting]       = value;

        updateSetting(local);
    }

    void Setting::setUInt(const std::string& setting, const unsigned int value)
    {
        nlohmann::json local = nlohmann::json::object();
        local[setting]       = value;

        updateSetting(local);
    }

    void Setting::setSetting(const std::string& name, const Setting& setting)
    {
        nlohmann::json local = nlohmann::json::object();
        local[name]          = setting.getCurrentSetting();

        updateSetting(local);
    }

    void Setting::setStringTable(const std::string& setting, const std::vector<std::string>& value)
    {
        nlohmann::json local = nlohmann::json::object();
        local[setting]       = value;

        updateSetting(local);
    }

} // namespace nero
