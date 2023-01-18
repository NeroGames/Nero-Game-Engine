////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef BACKGROUNDTASK_H
#define BACKGROUNDTASK_H
///////////////////////////HEADERS//////////////////////////
// CPP
#include <string>
#include <vector>
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class BackgroundTask {
      public:
        using Ptr = std::shared_ptr<BackgroundTask>;

      public:
        BackgroundTask(const std::string& name, const std::string& category);

        // Setter
        void        nextStep();
        void        setErrorCode(const int error);
        void        setCompleted(const bool completed);
        void        setFailed(const bool failed);
        void        addMessage(const std::string& message);
        // Getter
        std::string getName() const;
        std::string getCategory() const;
        int         getStep() const;
        int         getErrorCode() const;
        bool        completed() const;
        bool        failed() const;
        std::string getMessage() const;

      private:
        friend class BackgroundTaskManager;
        std::string m_Name;
        std::string m_Category;
        int         m_Step;
        int         m_ErrorCode;
        bool        m_Completed;
        bool        m_Failed;
        std::string m_Message;
    };
} // namespace nero

#endif // BACKGROUNDTASK_H
