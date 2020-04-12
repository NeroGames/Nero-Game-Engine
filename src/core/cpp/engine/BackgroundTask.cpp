////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#include <Nero/core/cpp/engine/BackgroundTask.h>
#include <Nero/core/cpp/utility/String.h>
#include <Nero/core/cpp/utility/Logging.h>

namespace nero
{
	BackgroundTask::BackgroundTask(const std::string& name, const std::string& category):
		m_Name(name)
	   ,m_Category(category)
	   ,m_Status(0)
	   ,m_ErrorCode()
	   ,m_Completed(false)
	   ,m_Failed(false)
	   ,m_MessageTable()
	   ,m_Message(string::StringPool.BLANK)
	{

	}

	std::string BackgroundTask::getName()
	{
		return m_Name;
	}

	std::string BackgroundTask::getCategory()
	{
		return m_Category;
	}

	int BackgroundTask::getStatus()
	{
		return m_Status;
	}

	void BackgroundTask::setStatus(int status)
	{
		m_Status = status;
	}

	void BackgroundTask::addMessage(const std::string& message)
	{
		nero_log(message);
		m_Message += message + string::StringPool.NEW_LINE;
		//m_MessageTable.push_back(message);
	}

	bool BackgroundTask::isCompleted()
	{
		return m_Completed;
	}

	bool BackgroundTask::isFailed()
	{
		return m_Failed;
	}

	std::string BackgroundTask::printMessage()
	{
		/*std::string result = string::StringPool.BLANK;

		for (const std::string& message : m_MessageTable)
		{
			result += message + string::StringPool.NEW_LINE;
		}*/

		return  m_Message;
	}

	void BackgroundTask::setErrorCode(int error)
	{
		m_ErrorCode = error;
	}

	int BackgroundTask::getErrorCode()
	{
		return m_ErrorCode;
	}


	void BackgroundTask::setCompleted(bool completed)
	{
		m_Completed = completed;
	}

}

