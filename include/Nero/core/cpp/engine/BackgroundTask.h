////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef BACKGROUNDTASK_H
#define BACKGROUNDTASK_H

#include <string>
#include <vector>
#include <memory>

namespace nero
{
	class BackgroundTask
	{
		public:
			typedef std::shared_ptr<BackgroundTask>    Ptr;

		public:
										BackgroundTask(const std::string& name, const std::string& category);

			std::string					getName();
			std::string					getCategory();
			int							getStatus();
			void						setStatus(int status);
			void						setErrorCode(int error);
			void						addMessage(const std::string& message);
			bool						isCompleted();
			bool						isFailed();
			void						setCompleted(bool completed);
			std::string					printMessage();

		private:
			std::string					m_Name;
			std::string					m_Category;
			int							m_Status;
			int							m_ErrorCode;
			bool						m_Completed;
			bool						m_Failed;
			std::vector<std::string>	m_MessageTable;
			std::string					m_Message;
	};
}



#endif // BACKGROUNDTASK_H
