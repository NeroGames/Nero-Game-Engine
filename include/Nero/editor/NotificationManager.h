////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H
///////////////////////////HEADERS//////////////////////////
// SFML
#include <SFML/System/Time.hpp>
// Std
#include <memory>
#include <vector>
#include <string>
////////////////////////////////////////////////////////////
namespace nero
{
    class NotificationManager
    {
      public:
        using Ptr = std::shared_ptr<NotificationManager>;

      public:
        NotificationManager();

        void                            update(const sf::Time& timeStep);
        void                            notify(const std::string& message);
        const std::vector<std::string>& getMessageTable() const;

      private:
        struct Notification
        {
            std::string message;
            float       timer;
        };

      private:
        std::vector<Notification> m_NotificationTable;
        std::vector<std::string>  m_MesssageTable;
        float                     m_NotificationTime;
    };
} // namespace nero
#endif // NOTIFICATIONMANAGER_H
