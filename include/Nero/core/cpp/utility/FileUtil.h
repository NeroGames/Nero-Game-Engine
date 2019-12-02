////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef FILEUTIL_H
#define FILEUTIL_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/cpp/utility/StringUtil.h>
#include <Nero/core/cpp/utility/LogUtil.h>
#include <Nero/core/cpp/utility/PlatformUtil.h>
#include <experimental/filesystem>

//JSON
#include <json/json.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    //get file path : OS dependent
	std::string getPath(const std::vector<std::string>& list, const std::string& extension = StringPool.BLANK);
	std::string getWindowsPath(const std::vector<std::string>& list, const std::string& extension = StringPool.BLANK);
	std::string getLinuxPath(const std::vector<std::string>& list, const std::string& extension = StringPool.BLANK);
	std::string getWindowsPath(const std::string& path);
	std::string escapeAntiSlash(const std::string& word);

    //get file content
    std::string loadText(const std::string& file);

    //load Json file
    nlohmann::json loadJson(const std::string& path, const bool& path_has_extantion = false);

    //load setting file
    nlohmann::json loadSetting(const std::string& name);

    //Create a directory if not exist
    void createDirectory(const std::string& name);

    //Check if a file exist
    bool fileExist(const std::string& name);

    //check if a directory exist
    bool directoryExist(const std::string& name);

    //remove file
    bool removeFile(const std::string& name);

    //remove directory
    bool removeDirectory(const std::string& name, bool recursive = true);

    //Save a text to a file
    void  saveFile(const std::string& name, const std::string& content, bool overwrite = false);

    //Save binary date to a file
    void  saveFile(const std::string& name, const unsigned char* content, const unsigned int length, bool overwrite = false);

    //Current path
    std::string getCurrentPath();

    //
    std::string removeFileExtension(const std::string& filename);

	bool checkExtention(const std::string& extention, const std::vector<std::string>& extentionTable);

	std::string replaceFileExtension(const std::string& filename, const std::string& extension);

	std::string getParentDirectory(const std::string& directory, const int& level = 1);


}


#endif // FILEUTIL_H
