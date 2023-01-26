////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Poco
// Nero
#include <Nero/editor/NotificationManager.h>
////////////////////////////////////////////////////////////
namespace nero
{
    NotificationManager::NotificationManager()
    {
    }

    void NotificationManager::update(const sf::Time& timeStep)
    {
        for(auto it = m_NotificationTable.begin(); it != m_NotificationTable.end();)
        {
            it->timer += timeStep.asSeconds();

            if(it->timer >= it->timeOut)
            {
                it = m_NotificationTable.erase(it);
            }
            else
            {
                ++it;
            }
        }

        if(m_MesssageTable.size() != m_NotificationTable.size())
        {
            m_MesssageTable.clear();
            for(auto& notification : m_NotificationTable)
                m_MesssageTable.push_back(notification.message);
        }
    }

    void NotificationManager::notify(const std::string& message, const float& timeOut)
    {
        m_NotificationTable.push_back(Notification{message, timeOut, 0.f});
    }

    const std::vector<std::string>& NotificationManager::getMessageTable() const
    {
        return m_MesssageTable;
    }
} // namespace nero
