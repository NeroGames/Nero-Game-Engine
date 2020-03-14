////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//NERO
#include <Nero/core/cpp/resource/ResourceHolder.h>
#include <Nero/core/cpp/utility/StringUtil.h>
/////////////////////////////////////////////////////////////
namespace nero
{
	ResourceHolder::ResourceHolder():
		 m_Setting()
		,m_DirectoryTable()
		,m_SelectedDirectory(StringPool.BLANK)
	{
		//empty
	}

	ResourceHolder::ResourceHolder(const Setting& setting):
		m_Setting(setting)
	   ,m_DirectoryTable()
	   ,m_SelectedDirectory(StringPool.BLANK)
	{

	}


	ResourceHolder::~ResourceHolder()
	{
		//empty
	}

	void ResourceHolder::setSetting(const Setting& setting)
	{
		m_Setting = setting;
	}

	void ResourceHolder::addDirectory(const std::string& directory, bool autoSelect)
	{
		m_DirectoryTable.insert(directory);

		if(autoSelect)
		{
			selectDirectory(directory);
		}
	}

	void ResourceHolder::selectDirectory(const std::string& directory)
	{
		m_SelectedDirectory = directory;
	}

	void ResourceHolder::loadFile(const std::vector<std::string> fileTable)
	{
		for(const std::string& file : fileTable)
		{
			loadFile(file);
		}
	}

	void ResourceHolder::loadDirectory(const std::string& directory)
	{
		addDirectory(directory, true);

		loadDirectory();
	}

	void ResourceHolder::reloadDirectory(const std::string& directory)
	{
		selectDirectory(directory);

		loadDirectory();
	}

	void ResourceHolder::clear()
	{
		//setting does not nee to be cleared
		m_DirectoryTable.clear();
		m_SelectedDirectory = StringPool.BLANK;
	}
}
