////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
#include <Nero/core/cpp/engine/BackgroundTask.h>
#include <Nero/core/cpp/utility/String.h>
////////////////////////////////////////////////////////////
namespace nero
{
    BackgroundTask::BackgroundTask(const std::string& name, const std::string& category)
        : m_Name(name)
        , m_Category(category)
        , m_Step(0)
        , m_ErrorCode()
        , m_Completed(false)
        , m_Failed(false)
        , m_Message(StringPool.BLANK)
    {
        // Empty
    }

    std::string BackgroundTask::getName() const
    {
        return m_Name;
    }

    std::string BackgroundTask::getCategory() const
    {
        return m_Category;
    }

    void BackgroundTask::nextStep()
    {
        if(!m_Completed && !m_Failed)
        {
            m_Step++;
        }
    }

    int BackgroundTask::getStep() const
    {
        return m_Step;
    }

    void BackgroundTask::setErrorCode(const int error)
    {
        m_ErrorCode = error;
    }

    int BackgroundTask::getErrorCode() const
    {
        return m_ErrorCode;
    }

    void BackgroundTask::setCompleted(const bool completed)
    {
        m_Completed = completed;
    }

    bool BackgroundTask::completed() const
    {
        return m_Completed;
    }

    void BackgroundTask::setFailed(const bool failed)
    {
        m_Failed = failed;
    }

    bool BackgroundTask::failed() const
    {
        return m_Failed;
    }

    void BackgroundTask::addMessage(const std::string& message)
    {
        m_Message += message + StringPool.NEW_LINE;
    }

    std::string BackgroundTask::getMessage() const
    {
        return m_Message;
    }
} // namespace nero
